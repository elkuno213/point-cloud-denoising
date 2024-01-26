#pragma once

// Standard
#include <vector>
// 3rd-parties
#include <opencv2/opencv.hpp>

class SphericalProjection {
public:
  SphericalProjection(
    const int _height,
    const int _width,
    const double _elevation_max,
    const double _elevation_min,
    const double _elevation_resolution,
    const double _azimuth_max,
    const double _azimuth_min,
    const double _azimuth_resolution
  );
  ~SphericalProjection();

  cv::Mat compute(
    const std::vector<double>& azimuths,
    const std::vector<double>& distances,
    const std::vector<double>& intensities
  );

private:
  // Check angle by:
  // - 1. If max < min, then swap max and min values.
  // - 2. If max - min = 360°, then they are equal and max -= resolution.
  void correct_angle_limits(
    double& angle_max, double& angle_min, double angle_resolution
  );
  // Correct height/width based on angle specification.
  int correct_image_size(
    const int size,
    const double angle_max,
    const double angle_min,
    const double angle_resolution
  ) const;
  // Project a point into image plane.
  cv::Point2i project(const double elevation, const double azimuth) const;

private:
  // Parameters
  int height_                  = 32;   // Image height (pixel)
  int width_                   = 512;  // Image width (pixel)
  double elevation_max_        = 15.;  // Maximum elevation (°)
  double elevation_min_        = -16.; // Minimum elevation (°)
  double elevation_resolution_ = 1.;   // Elevation resolution (°)
  double azimuth_max_          = 360.; // Maximum azimuth (°)
  double azimuth_min_          = 0.;   // Minimum azimuth (°)
  double azimuth_resolution_   = 0.09; // Azimuth resolution (°)
};
