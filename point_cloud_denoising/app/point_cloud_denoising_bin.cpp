// TODO: refactor this bin
// TODO: convert data to rosbag
// TODO: convert data to point-cloud format and algorihtms to compute this format
// TODO: rename files : CSVReader -> csv_reader
// TODO: implement ros node.
// TODO: add more data from kitti, etc.
// TODO: remove .csv data
// TODO: remove useless code
// TODO: consider using matrix transformation
// TODO: update README

// Standard
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>
// 3rd-parties
#include <opencv2/opencv.hpp>
#include <pugixml.hpp>
// Local
#include "point_cloud_denoising/CSVReader.hpp"
#include "point_cloud_denoising/ImageDenoising.hpp"
#include "point_cloud_denoising/SphericalProjection.hpp"
#include "point_cloud_denoising/rapidcsv.h"

void create_directory(const std::string& directory) {
  if (std::filesystem::exists(directory)) {
    return;
  }
  std::filesystem::create_directories(directory);
}

bool parse_parameters(
  const std::string& params_file,
  std::string& data_folder,
  int& height,
  int& width,
  double& elevation_max,
  double& elevation_min,
  double& elevation_resolution,
  double& azimuth_max,
  double& azimuth_min,
  double& azimuth_resolution,
  float& distance_h,
  int& distance_template_window_size,
  int& distance_search_window_size,
  double& distance_non_noise_level,
  float& intensity_h,
  int& intensity_template_window_size,
  int& intensity_search_window_size,
  double& intensity_non_noise_level
) {
  pugi::xml_document doc;

  const pugi::xml_parse_result result = doc.load_file(params_file.c_str());
  if (!result) {
    std::cerr << "Failed to load XML file: " << result.description() << '\n';
    return false;
  }

  const pugi::xml_node root_node = doc.child("root");
  if (!root_node) {
    std::cerr << "Invalid XML format: missing root node <root>\n";
    return false;
  }

  {
    const pugi::xml_node node = root_node.child("data_folder");
    if (!node) {
      std::cerr << "Invalid XML format: missing node <data_folder>\n";
      return false;
    }
    data_folder = node.attribute("value").as_string();
    std::cout << "input_cloud_file: " << data_folder << '\n';
  }

  {
    const pugi::xml_node node = root_node.child("height");
    if (!node) {
      std::cerr << "Invalid XML format: missing node <height>\n";
      return false;
    }
    height = node.attribute("value").as_int();
    std::cout << "height: " << height << '\n';
  }

  {
    const pugi::xml_node node = root_node.child("width");
    if (!node) {
      std::cerr << "Invalid XML format: missing node <width>\n";
      return false;
    }
    width = node.attribute("value").as_int();
    std::cout << "width: " << width << '\n';
  }

  {
    const pugi::xml_node node = root_node.child("elevation_max");
    if (!node) {
      std::cerr << "Invalid XML format: missing node <elevation_max>\n";
      return false;
    }
    elevation_max = node.attribute("value").as_double();
    std::cout << "elevation_max: " << elevation_max << '\n';
  }

  {
    const pugi::xml_node node = root_node.child("elevation_min");
    if (!node) {
      std::cerr << "Invalid XML format: missing node <elevation_min>\n";
      return false;
    }
    elevation_min = node.attribute("value").as_double();
    std::cout << "elevation_min: " << elevation_min << '\n';
  }

  {
    const pugi::xml_node node = root_node.child("elevation_resolution");
    if (!node) {
      std::cerr << "Invalid XML format: missing node <elevation_resolution>\n";
      return false;
    }
    elevation_resolution = node.attribute("value").as_double();
    std::cout << "elevation_resolution: " << elevation_resolution << '\n';
  }

  {
    const pugi::xml_node node = root_node.child("azimuth_max");
    if (!node) {
      std::cerr << "Invalid XML format: missing node <azimuth_max>\n";
      return false;
    }
    azimuth_max = node.attribute("value").as_double();
    std::cout << "azimuth_max: " << azimuth_max << '\n';
  }

  {
    const pugi::xml_node node = root_node.child("azimuth_min");
    if (!node) {
      std::cerr << "Invalid XML format: missing node <azimuth_min>\n";
      return false;
    }
    azimuth_min = node.attribute("value").as_double();
    std::cout << "azimuth_min: " << azimuth_min << '\n';
  }

  {
    const pugi::xml_node node = root_node.child("azimuth_resolution");
    if (!node) {
      std::cerr << "Invalid XML format: missing node <azimuth_resolution>\n";
      return false;
    }
    azimuth_resolution = node.attribute("value").as_double();
    std::cout << "azimuth_resolution: " << azimuth_resolution << '\n';
  }

  const pugi::xml_node distance_node = root_node.child("distance");
  if (!distance_node) {
    std::cerr << "Invalid XML format: missing node <distance>\n";
    return false;
  }

  {
    const pugi::xml_node node = distance_node.child("h");
    if (!node) {
      std::cerr << "Invalid XML format: missing node <h> under <distance>\n";
      return false;
    }
    distance_h = node.attribute("value").as_float();
    std::cout << "distance_h: " << distance_h << '\n';
  }

  {
    const pugi::xml_node node = distance_node.child("template_window_size");
    if (!node) {
      std::cerr << "Invalid XML format: missing node <template_window_size> "
                   "under <distance>\n";
      return false;
    }
    distance_template_window_size = node.attribute("value").as_int();
    std::cout << "distance_template_window_size: "
              << distance_template_window_size << '\n';
  }

  {
    const pugi::xml_node node = distance_node.child("search_window_size");
    if (!node) {
      std::cerr
        << "Invalid XML format: missing node <search_window_size> under "
           "<distance>\n";
      return false;
    }
    distance_search_window_size = node.attribute("value").as_int();
    std::cout << "distance_search_window_size: " << distance_search_window_size
              << '\n';
  }

  {
    const pugi::xml_node node = distance_node.child("non_noise_level");
    if (!node) {
      std::cerr << "Invalid XML format: missing node <non_noise_level> under "
                   "<distance>\n";
      return false;
    }
    distance_non_noise_level = node.attribute("value").as_double();
    std::cout << "distance_non_noise_level: " << distance_non_noise_level
              << '\n';
  }

  const pugi::xml_node intensity_node = root_node.child("intensity");
  if (!intensity_node) {
    std::cerr << "Invalid XML format: missing node <intensity>\n";
    return false;
  }

  {
    const pugi::xml_node node = intensity_node.child("h");
    if (!node) {
      std::cerr << "Invalid XML format: missing node <h> under <intensity>\n";
      return false;
    }
    intensity_h = node.attribute("value").as_float();
    std::cout << "intensity_h: " << intensity_h << '\n';
  }

  {
    const pugi::xml_node node = intensity_node.child("template_window_size");
    if (!node) {
      std::cerr << "Invalid XML format: missing node <template_window_size> "
                   "under <intensity>\n";
      return false;
    }
    intensity_template_window_size = node.attribute("value").as_int();
    std::cout << "intensity_template_window_size: "
              << intensity_template_window_size << '\n';
  }

  {
    const pugi::xml_node node = intensity_node.child("search_window_size");
    if (!node) {
      std::cerr
        << "Invalid XML format: missing node <search_window_size> under "
           "<intensity>\n";
      return false;
    }
    intensity_search_window_size = node.attribute("value").as_int();
    std::cout << "intensity_search_window_size: "
              << intensity_search_window_size << '\n';
  }

  {
    const pugi::xml_node node = intensity_node.child("non_noise_level");
    if (!node) {
      std::cerr << "Invalid XML format: missing node <non_noise_level> under "
                   "<intensity>\n";
      return false;
    }
    intensity_non_noise_level = node.attribute("value").as_double();
    std::cout << "intensity_non_noise_level: " << intensity_non_noise_level
              << '\n';
  }

  return true;
}

int main(int argc, char* argv[]) {
  // Read parameters
  if (argc < 2) {
    std::cerr << "Please give the path to the XML file of parameters.\n";
    std::cout << argv[0] << " <path-to-xml-file>.\n";
    return -1;
  }

  const std::string params_file = argv[1];
  std::string data_folder;
  std::string lidar_name;
  int height;
  int width;
  double elevation_max;
  double elevation_min;
  double elevation_resolution;
  double azimuth_max;
  double azimuth_min;
  double azimuth_resolution;
  float distance_h;
  int distance_template_window_size;
  int distance_search_window_size;
  double distance_non_noise_level;
  float intensity_h;
  int intensity_template_window_size;
  int intensity_search_window_size;
  double intensity_non_noise_level;
  const bool are_parameters_valid = parse_parameters(
    params_file,
    data_folder,
    height,
    width,
    elevation_max,
    elevation_min,
    elevation_resolution,
    azimuth_max,
    azimuth_min,
    azimuth_resolution,
    distance_h,
    distance_template_window_size,
    distance_search_window_size,
    distance_non_noise_level,
    intensity_h,
    intensity_template_window_size,
    intensity_search_window_size,
    intensity_non_noise_level
  );
  if (!are_parameters_valid) {
    return -1;
  }

  // Create paths
  const auto output_folder_path
    = std::filesystem::path(data_folder) / std::string("denoising");
  create_directory(output_folder_path.string());

  const auto distance_folder_path
    = output_folder_path / std::string("distance");
  create_directory(distance_folder_path.string());

  const auto denoised_distance_folder_path
    = output_folder_path / std::string("denoised_distance");
  create_directory(denoised_distance_folder_path.string());

  const auto distance_noise_folder_path
    = output_folder_path / std::string("distance_noise");
  create_directory(distance_noise_folder_path);

  const auto intensity_folder_path
    = output_folder_path / std::string("intensity");
  create_directory(intensity_folder_path);

  const auto denoised_intensity_folder_path
    = output_folder_path / std::string("denoised_intensity");
  create_directory(denoised_intensity_folder_path);

  const auto intensity_noise_folder_path
    = output_folder_path / std::string("intensity_noise");
  create_directory(intensity_noise_folder_path);

  // Azimuth reading
  CSVReader azimuth_reader;
  std::vector<std::vector<double>> azimuth_data;
  azimuth_reader.set_parameters(0.01, -1, 0);
  azimuth_reader.read_inputs(
    (std::filesystem::path(data_folder) / std::string("azimuths.csv")).string()
  );
  azimuth_reader.process_data();
  azimuth_reader.write_outputs(azimuth_data);
  std::cout << "azimuth_data:\t(" << azimuth_data.size() << " rows, "
            << azimuth_data[0].size() << " cols)" << '\n';

  // Distance reading
  CSVReader distance_reader;
  std::vector<std::vector<double>> distance_data;
  distance_reader.set_parameters(0.004, -1, 0);
  distance_reader.read_inputs(
    (std::filesystem::path(data_folder) / std::string("distances.csv")).string()
  );
  distance_reader.process_data();
  distance_reader.write_outputs(distance_data);
  std::cout << "distance_data:\t(" << distance_data.size() << " rows, "
            << distance_data[0].size() << " cols)" << '\n';

  // Intensity reading
  CSVReader intensity_reader;
  std::vector<std::vector<double>> intensity_data;
  intensity_reader.set_parameters(1, -1, 0);
  intensity_reader.read_inputs((std::filesystem::path(data_folder)
                                / std::string("intensities.csv"))
                                 .string());
  intensity_reader.process_data();
  intensity_reader.write_outputs(intensity_data);
  std::cout << "intensity_data:\t(" << intensity_data.size() << " rows, "
            << intensity_data[0].size() << " cols)" << '\n';

  // Video objects
  cv::VideoWriter video_distance(
    (output_folder_path / std::string("distance.avi")).string(),
    cv::VideoWriter::fourcc('M', 'J', 'P', 'G'),
    10,
    cv::Size(width, height),
    false
  );
  cv::VideoWriter video_denoised_distance(
    (output_folder_path / std::string("denoised_distance.avi")).string(),
    cv::VideoWriter::fourcc('M', 'J', 'P', 'G'),
    10,
    cv::Size(width, height),
    false
  );
  cv::VideoWriter video_intensity(
    (output_folder_path / std::string("intensity.avi")).string(),
    cv::VideoWriter::fourcc('M', 'J', 'P', 'G'),
    10,
    cv::Size(width, height),
    false
  );
  cv::VideoWriter video_denoised_intensity(
    (output_folder_path / std::string("denoised_intensity.avi")).string(),
    cv::VideoWriter::fourcc('M', 'J', 'P', 'G'),
    10,
    cv::Size(width, height),
    false
  );

  // PointCloud Denosing
  for (std::size_t i{0}; i < azimuth_data.size(); ++i) {
    // Spherical projection
    cv::Mat image;
    SphericalProjection spherical_projection;
    spherical_projection.set_parameters(
      height,
      width,
      elevation_max,
      elevation_min,
      elevation_resolution,
      azimuth_max,
      azimuth_min,
      azimuth_resolution
    );
    spherical_projection.read_inputs(
      azimuth_data[i], distance_data[i], intensity_data[i]
    );
    spherical_projection.process_data();
    spherical_projection.write_outputs(image);

    // Split channels
    std::vector<cv::Mat> channels(4);
    cv::split(image, channels);

    // Elevation image
    cv::Mat elevation_image = channels[0];
    cv::normalize(
      elevation_image, elevation_image, 0.0, 255.0, cv::NORM_MINMAX
    );
    elevation_image.convertTo(elevation_image, CV_8UC1);

    // Azimuth image
    cv::Mat azimuth_image = channels[1];
    cv::normalize(azimuth_image, azimuth_image, 0.0, 255.0, cv::NORM_MINMAX);
    azimuth_image.convertTo(azimuth_image, CV_8UC1);

    // Distance image
    cv::Mat distance_image = channels[2];
    cv::normalize(distance_image, distance_image, 0.0, 255.0, cv::NORM_MINMAX);
    // cv::subtract(cv::Scalar::all(255.0), distanceImage, distanceImage);
    distance_image.convertTo(distance_image, CV_8UC1);
    video_distance.write(distance_image);
    {
      const std::string filename = "distance" + std::to_string(i) + ".png";
      const std::string file     = (distance_folder_path / filename).string();
      cv::imwrite(file, distance_image);
    }

    // Denoise distance image
    cv::Mat denoised_distance_image, distance_noise;
    double distance_noise_ratio;
    ImageDenoising image_denoising_distance;
    image_denoising_distance.set_parameters(
      distance_h,
      distance_template_window_size,
      distance_search_window_size,
      distance_non_noise_level
    );
    image_denoising_distance.read_inputs(distance_image);
    image_denoising_distance.process_data();
    image_denoising_distance.write_outputs(
      denoised_distance_image, distance_noise, distance_noise_ratio
    );
    video_denoised_distance.write(denoised_distance_image);
    {
      const std::string filename
        = "denoised_distance" + std::to_string(i) + ".png";
      const std::string file
        = (denoised_distance_folder_path / filename).string();
      cv::imwrite(file, denoised_distance_image);
    }
    {
      const std::string filename = "distanceNoise" + std::to_string(i) + ".png";
      const std::string file = (distance_noise_folder_path / filename).string();
      cv::imwrite(file, distance_noise);
    }

    // Intensity image
    cv::Mat intensity_image = channels[3];
    intensity_image.convertTo(intensity_image, CV_8UC1);
    video_intensity.write(intensity_image);
    {
      const std::string filename
        = "intensity_image" + std::to_string(i) + ".png";
      const std::string file = (intensity_folder_path / filename).string();
      cv::imwrite(file, intensity_image);
    }

    // Denoise intensity image
    cv::Mat denoised_intensity_image, intensity_noise;
    double intensity_noise_ratio;
    ImageDenoising image_denoising_intensity;
    image_denoising_intensity.set_parameters(
      intensity_h,
      intensity_template_window_size,
      intensity_search_window_size,
      intensity_non_noise_level
    );
    image_denoising_intensity.read_inputs(intensity_image);
    image_denoising_intensity.process_data();
    image_denoising_intensity.write_outputs(
      denoised_intensity_image, intensity_noise, intensity_noise_ratio
    );
    std::cout << "intensity_noise_ratio:\t" << intensity_noise_ratio << '\n';
    video_denoised_intensity.write(denoised_intensity_image);
    {
      const std::string filename
        = "denoised_intensity_image" + std::to_string(i) + ".png";
      const std::string file
        = (denoised_intensity_folder_path / filename).string();
      cv::imwrite(file, denoised_intensity_image);
    }
    {
      const std::string filename
        = "intensity_noise" + std::to_string(i) + ".png";
      const std::string file
        = (intensity_noise_folder_path / filename).string();
      cv::imwrite(file, intensity_noise);
    }
  }

  video_distance.release();
  video_denoised_distance.release();
  video_intensity.release();
  video_denoised_intensity.release();

  return 0;
}
