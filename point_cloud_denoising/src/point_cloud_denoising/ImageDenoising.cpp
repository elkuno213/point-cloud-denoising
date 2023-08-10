#include "ImageDenoising.hpp"

ImageDenoising::ImageDenoising() {}

ImageDenoising::~ImageDenoising() {}

void ImageDenoising::set_parameters(
  float _h,
  int _templateWindowSize,
  int _searchWindowSize,
  double _nonNoiseLevel
) {
  h_                    = _h;
  template_window_size_ = _templateWindowSize;
  search_window_size_   = _searchWindowSize;
  non_noise_level_      = _nonNoiseLevel;
}

void ImageDenoising::read_inputs(cv::Mat& _noisy_image) {
  noisy_image_ = _noisy_image;
}

void ImageDenoising::process_data() {
  // Fast NL Means Denoising for CV_8U image.
  if (noisy_image_.depth() != 0) {
    // If the image type is not CV_8U, convert it.
    noisy_image_.convertTo(noisy_image_, CV_8U);
  }
  denoised_image_
    = cv::Mat(noisy_image_.size(), noisy_image_.depth(), cv::Scalar::all(0));
  cv::fastNlMeansDenoising(
    noisy_image_,
    denoised_image_,
    h_,
    template_window_size_,
    search_window_size_
  );

  // Noise extraction
  noise_ = noisy_image_ - denoised_image_;

  // Noise filtering
  double min_limit, max_limit;
  get_limits(noisy_image_, min_limit, max_limit);
  cv::threshold(
    noise_,
    noise_,
    non_noise_level_ * (max_limit - min_limit) + min_limit,
    max_limit,
    cv::THRESH_TOZERO
  );

  noise_ratio_ = static_cast<double>(cv::countNonZero(noise_))
               / static_cast<double>(noise_.rows * noise_.cols); // Noise ratio
}

void ImageDenoising::write_outputs(
  cv::Mat& _denoised_image, cv::Mat& _noise, double& _noise_ratio
) {
  // Pass the values of scores to the MAPS wrapper
  _denoised_image = denoised_image_.clone();
  _noise          = noise_.clone();
  _noise_ratio    = noise_ratio_;
}

template <typename T>
void ImageDenoising::get_limits(
  const cv::Mat image, T& min_limit, T& max_limit
) {
  switch (image.depth()) {
    case 0:
      min_limit = 0;
      max_limit = 255;
      break;
    case 1:
      min_limit = -128;
      max_limit = 127;
      break;
    case 2:
      min_limit = 0;
      max_limit = 65535;
      break;
    case 3:
      min_limit = -32768;
      max_limit = 32767;
      break;
    case 4:
      min_limit = -2147483648;
      max_limit = 2147483647;
      break;
    case 5:
    case 6:
      cv::Point minLoc, maxLoc;
      cv::minMaxLoc(image, &min_limit, &max_limit, &minLoc, &maxLoc);
      break;
  }
}
