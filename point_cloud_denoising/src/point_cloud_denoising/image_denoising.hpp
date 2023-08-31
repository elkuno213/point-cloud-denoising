#pragma once

// Standard
#include <iostream>
// 3rd-parties
#include <opencv2/opencv.hpp>

class ImageDenoising {
public:
  ImageDenoising();
  ~ImageDenoising();

public:
  void set_parameters(
    const float _h,
    const int _template_window_size,
    const int _search_window_size,
    const double _non_noise_level
  );
  void read_inputs(const cv::Mat& _noisy_image);
  void process_data();
  void write_outputs(
    cv::Mat& _denoised_image,
    cv::Mat& _noise,
    double& _noise_ratio
  );

private:
  template <typename T>
  void get_limits(const cv::Mat image, T& min_limit, T& max_limit);

private:
  // Inputs
  cv::Mat noisy_image_; // Input noisy image

  // Outputs
  cv::Mat denoised_image_; // Output denoised image
  cv::Mat noise_;          // Output noise
  double noise_ratio_;     // Percentage of noise of output noise

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
