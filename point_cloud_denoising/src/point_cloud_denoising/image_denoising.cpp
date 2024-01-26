#include "image_denoising.hpp"

ImageDenoising::ImageDenoising(
  const float _h,
  const int _template_window_size,
  const int _search_window_size,
  const double _non_noise_level
) : h_{_h}
  , template_window_size_{_template_window_size}
  , search_window_size_{_search_window_size}
  , non_noise_level_{_non_noise_level}
{}

void ImageDenoising::compute(
  const cv::Mat& noisy_image,
  cv::Mat& denoised_image,
  cv::Mat& noise,
  double& noise_ratio
) const {
  // Fast NL Means Denoising for CV_8U image.
  if (noisy_image.depth() != 0) {
    // If the image type is not CV_8U, convert it.
    noisy_image.convertTo(noisy_image, CV_8U);
  }
  denoised_image = cv::Mat(
    noisy_image.size(),
    noisy_image.depth(),
    cv::Scalar::all(0)
  );
  cv::fastNlMeansDenoising(
    noisy_image,
    denoised_image,
    h_,
    template_window_size_,
    search_window_size_
  );

  // Noise extraction
  noise = noisy_image - denoised_image;

  // Noise filtering
  double min_limit, max_limit;
  get_limits(noisy_image, min_limit, max_limit);
  cv::threshold(
    noise,
    noise,
    non_noise_level_ * (max_limit - min_limit) + min_limit,
    max_limit,
    cv::THRESH_TOZERO
  );

  noise_ratio = static_cast<double>(cv::countNonZero(noise))
               / static_cast<double>(noise.rows * noise.cols); // Noise ratio
}

template <typename T>
void ImageDenoising::get_limits(
  const cv::Mat image,
  T& min_limit,
  T& max_limit
) const {
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
