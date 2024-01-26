#pragma once

// Standard
#include <iostream>
// 3rd-parties
#include <opencv2/opencv.hpp>

class ImageDenoising {
public:
  ImageDenoising(
    const float _h,
    const int _template_window_size,
    const int _search_window_size,
    const double _non_noise_level
  );

public:
  void compute(
    // Input noisy image
    const cv::Mat& _noisy_image,
    // Output denoised image
    cv::Mat& _denoised_image,
    // Output noise
    cv::Mat& _noise,
    // Percentage of noise of output noise
    double& _noise_ratio
  ) const;

private:
  template <typename T>
  void get_limits(const cv::Mat image, T& min_limit, T& max_limit) const;

private:
  // Parameter regulating filter strength.
  // Big h value perfectly removes noise but also removes image
  // details. Smaller h value preserves details but also
  // preserves some noise.
  float h_ = 10.;
  // Size in pixels of the template patch that is
  // used to compute weights. Should be odd.
  // Recommended value 7 pixels.
  int template_window_size_ = 7;
  // Size in pixels of the window that is used to
  // compute weighted average for given pixel.
  // Should be odd. Recommended value 21 pixels.
  // Affect performance linearly: greater
  // searchWindowsSize - greater denoising time.
  int search_window_size_ = 21;
  // Percentage of non nosie pixel.
  // For example, with a noise image of [0.0; 255.0], a
  // nonNoiseLevel of 0.1 (10%) means that all pixels
  // less than 10% * 255.0 are thresholded to 0.0 and
  // considered as non noise ones.
  double non_noise_level_ = 0.;
};
