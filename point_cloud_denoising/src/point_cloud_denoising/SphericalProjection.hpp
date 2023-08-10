#pragma once

// Standard
#include <algorithm>
#include <iostream>
#include <vector>
// 3rd-parties
#include <opencv2/opencv.hpp>

class SphericalProjection {
public:
  SphericalProjection();
  ~SphericalProjection();

  void set_parameters(
    int _height,
    int _width,
    double _elevation_max,
    double _elevation_min,
    double _elevation_resolution,
    double _azimuth_max,
    double _azimuth_min,
    double _azimuth_resolution
  );
  void read_inputs(
    std::vector<double>& _azimuths,
    std::vector<double>& _iDistances,
    std::vector<double>& _intensities
  );
  void process_data();
  void write_outputs(cv::Mat& _image);

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
  );
  // Project a point into image plane.
  cv::Point2i project(const double elevation, const double azimuth);

private:
  // Inputs
  std::vector<double> azimuths_;
  std::vector<double> distances_;
  std::vector<double> intensities_;

  // Outputs
  cv::Mat image_; // Multi-channel Image (Channels: distance, elevation,
                  // azimuth, intensity)

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
