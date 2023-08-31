// Standard
#include <algorithm>
// Local
#include "spherical_projection.hpp"

SphericalProjection::SphericalProjection() {}

SphericalProjection::~SphericalProjection() {}

void SphericalProjection::set_parameters(
  const int _height,
  const int _width,
  const double _elevation_max,
  const double _elevation_min,
  const double _elevation_resolution,
  const double _azimuth_max,
  const double _azimuth_min,
  const double _azimuth_resolution
) {
  height_               = _height;          // pixel
  width_                = _width;           // pixel
  elevation_max_        = _elevation_max;   // °
  elevation_min_        = _elevation_min;   // °
  elevation_resolution_ = _elevation_resolution; // °
  azimuth_max_          = _azimuth_max;     // °
  azimuth_min_          = _azimuth_min;     // °
  azimuth_resolution_   = _azimuth_resolution;   // °
}

void SphericalProjection::read_inputs(
  const std::vector<double>& _azimuths,
  const std::vector<double>& _distances,
  const std::vector<double>& _intensities
) {
  azimuths_    = _azimuths;
  distances_   = _distances;
  intensities_ = _intensities;
}

void SphericalProjection::process_data() {
  // Correct angle limits
  correct_angle_limits(elevation_max_, elevation_min_, elevation_resolution_);
  correct_angle_limits(azimuth_max_, azimuth_min_, azimuth_resolution_);

  // Correct image size
  height_ = correct_image_size(
    height_, elevation_max_, elevation_min_, elevation_resolution_
  );
  width_ = correct_image_size(
    width_, azimuth_max_, azimuth_min_, azimuth_resolution_
  );

  // Compute image
  image_ = cv::Mat(height_, width_, CV_64FC(4), cv::Scalar::all(0.));
  for (int i = 0; i < static_cast<int>(azimuths_.size()); ++i) {
    double elevation = elevation_max_; // elevation angle in °
    for (int j = 0; j < height_; ++j) {
      const auto pixel            = project(elevation, azimuths_[i]);
      image_.at<cv::Vec4d>(pixel) = cv::Vec4d{
        elevation,
        azimuths_[i],
        distances_[height_ * i + j],
        intensities_[height_ * i + j]};
      elevation -= 1;
    }
  }
}

void SphericalProjection::write_outputs(cv::Mat& _image) {
  _image = image_.clone();
}

void SphericalProjection::correct_angle_limits(
  double& angle_max, double& angle_min, const double angle_resolution
) {
  if (angle_max < angle_min) {
    std::swap(angle_max, angle_min);
  }
  angle_max
    = (angle_max - angle_min == 360.0 ? angle_max - angle_resolution : angle_max);
}

int SphericalProjection::correct_image_size(
  const int size,
  const double angle_max,
  const double angle_min,
  const double angle_resolution
) {
  const int max_size
    = static_cast<int>((angle_max - angle_min) / angle_resolution) + 1;
  return (0 <= size && size <= max_size) ? size : max_size;
}

cv::Point2i SphericalProjection::project(
  const double elevation, const double azimuth
) {
  cv::Point2i pixel;

  // Compute pixel x and make sure it is in [0, width - 1].
  const int x = std::round(
    (azimuth - azimuth_min_) / (azimuth_max_ - azimuth_min_) * (width_ - 1)
  );
  pixel.x = std::clamp(x, 0, width_ - 1);

  // Compute pixel y and make sure it is in [0, height - 1].
  const int y = std::round(
    (elevation - elevation_min_) / (elevation_max_ - elevation_min_) * (height_ - 1)
  );
  pixel.y = std::clamp(y, 0, height_ - 1);

  return pixel;
}
