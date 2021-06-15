#include <iostream>
#include <vector>
#include <sys/stat.h>
#include "rapidcsv.h"
#include "CSVReader.hpp"
#include "SphericalProjection.hpp"
#include "ImageDenoising.hpp"
#include "tinyxml2.h"
#include <opencv2/opencv.hpp>

void createDir(std::string dirPath);

void denoiseImageExmaple(int argc, char* argv[]);

cv::Mat addNoise(cv::Mat image, double mean, double stdDev);

void displayMinMaxLoc(cv::Mat& image);

template<typename T>
void display2DVector(std::vector<std::vector<T>> vector);

int main(int argc, char* argv[])
{
    // Parameters
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLError result = doc.LoadFile(argv[1]);
    if (result != tinyxml2::XML_SUCCESS) 
        return false;

    std::string LogName = doc.FirstChildElement("EntryOfFile")->FirstChildElement("LogName")->GetText();
    std::string LIDARName = doc.FirstChildElement("EntryOfFile")->FirstChildElement("LIDARName")->GetText();

    tinyxml2::XMLElement* eHeight = doc.FirstChildElement("EntryOfFile")->FirstChildElement("height");
    int height;
    eHeight->QueryIntText(&height);

    tinyxml2::XMLElement* eWidth = doc.FirstChildElement("EntryOfFile")->FirstChildElement("width");
    int width;
    eWidth->QueryIntText(&width);

    tinyxml2::XMLElement* eElevation_max = doc.FirstChildElement("EntryOfFile")->FirstChildElement("elevation_max");
    double elevation_max;
    eElevation_max->QueryDoubleText(&elevation_max);

    tinyxml2::XMLElement* eElevation_min = doc.FirstChildElement("EntryOfFile")->FirstChildElement("elevation_min");
    double elevation_min;
    eElevation_min->QueryDoubleText(&elevation_min);

    tinyxml2::XMLElement* eDelta_elevation = doc.FirstChildElement("EntryOfFile")->FirstChildElement("delta_elevation");
    double delta_elevation;
    eDelta_elevation->QueryDoubleText(&delta_elevation);

    tinyxml2::XMLElement* eAzimuth_max = doc.FirstChildElement("EntryOfFile")->FirstChildElement("azimuth_max");
    double azimuth_max;
    eAzimuth_max->QueryDoubleText(&azimuth_max);

    tinyxml2::XMLElement* eAzimuth_min = doc.FirstChildElement("EntryOfFile")->FirstChildElement("azimuth_min");
    double azimuth_min;
    eAzimuth_min->QueryDoubleText(&azimuth_min);

    tinyxml2::XMLElement* eDelta_azimuth = doc.FirstChildElement("EntryOfFile")->FirstChildElement("delta_azimuth");
    double delta_azimuth;
    eDelta_azimuth->QueryDoubleText(&delta_azimuth);


    // Paths
    std::string dataPath = "/home/hvh/MyGit/PointCloud_Denoising/data/Logs/" + LogName + "/" + LIDARName;

    std::string outputDir = std::to_string(delta_azimuth).substr(0, 4) + "_" + std::to_string(height) + "x" + std::to_string(width);
    std::string outputPath = "/home/hvh/MyGit/PointCloud_Denoising/data/Output/" + LogName + "/" + LIDARName + "/" + outputDir + "/";
    createDir(outputPath);

    std::string distancePath = outputPath + "distance/";
    createDir(distancePath);

    std::string denoisedDistancePath = outputPath + "denoisedDistance/";
    createDir(denoisedDistancePath);

    std::string distanceNoisePath = outputPath + "distanceNoise/";
    createDir(distanceNoisePath);

    std::string intensityPath = outputPath + "intensity/";
    createDir(intensityPath);

    std::string denoisedIntensityPath = outputPath + "denoisedIntensity/";
    createDir(denoisedIntensityPath);

    std::string intensityNoisePath = outputPath + "intensityNoise/";
    createDir(intensityNoisePath);


    // Azimuth reading
    CSVReader azimuthReader;
    std::vector<std::vector<double>> azimuthData;
    azimuthReader.setParameters(0.01, -1, 0);
    azimuthReader.readInputs(dataPath + "Azimuths.csv");
    azimuthReader.processData();
    azimuthReader.writeOutputs(azimuthData);
    std::cout << "azimuthData:\t(" << azimuthData.size() << " rows, " << azimuthData[0].size() << " cols)" << std::endl;


    // Distance reading
    CSVReader distanceReader;
    std::vector<std::vector<double>> distanceData;
    distanceReader.setParameters(0.004, -1, 0);
    distanceReader.readInputs(dataPath + "Distances.csv");
    distanceReader.processData();
    distanceReader.writeOutputs(distanceData);
    std::cout << "distanceData:\t(" << distanceData.size() << " rows, " << distanceData[0].size() << " cols)" << std::endl;


    // Intensity reading
    CSVReader intensityReader;
    std::vector<std::vector<double>> intensityData;
    intensityReader.setParameters(1, -1, 0);
    intensityReader.readInputs(dataPath + "Intensities.csv");
    intensityReader.processData();
    intensityReader.writeOutputs(intensityData);
    std::cout << "intensityData:\t(" << intensityData.size() << " rows, " << intensityData[0].size() << " cols)" << std::endl;


    // Video objects
    cv::VideoWriter videoDistance(outputPath + "Distance.avi", 
                                    cv::VideoWriter::fourcc('M','J','P','G'), 10, cv::Size(width, height), false);
    cv::VideoWriter videoDenoisedDistance(outputPath + "DenoisedDistance.avi", 
                                    cv::VideoWriter::fourcc('M','J','P','G'), 10, cv::Size(width, height), false);
    cv::VideoWriter videoIntensity(outputPath + "Intensity.avi", 
                                    cv::VideoWriter::fourcc('M','J','P','G'), 10, cv::Size(width, height), false);
    cv::VideoWriter videoDenoisedIntensity(outputPath + "DenoisedIntensity.avi", 
                                    cv::VideoWriter::fourcc('M','J','P','G'), 10, cv::Size(width, height), false);


    // PointCloud Denosing
    for (int i {0}; i < azimuthData.size(); ++i) {
        // Spherical projection 
        cv::Mat oImage;
        SphericalProjection sp;
        sp.setParameters(height, width, elevation_max, elevation_min, delta_elevation, azimuth_max, azimuth_min, delta_azimuth);
        sp.readInputs(azimuthData[i], distanceData[i], intensityData[i]);
        sp.processData();
        sp.writeOutputs(oImage);

        // Split channels
        std::vector<cv::Mat> channels(4);
        cv::split(oImage, channels);
        
        // Elevation image
        cv::Mat elevationImage = channels[0];
        cv::normalize(elevationImage, elevationImage, 0.0, 255.0, cv::NORM_MINMAX);
        elevationImage.convertTo(elevationImage, CV_8UC1);
        
        // Azimuth image
        cv::Mat azimuthImage = channels[1];
        cv::normalize(azimuthImage, azimuthImage, 0.0, 255.0, cv::NORM_MINMAX);
        azimuthImage.convertTo(azimuthImage, CV_8UC1);
        
        // Distance image
        cv::Mat distanceImage = channels[2];
        cv::normalize(distanceImage, distanceImage, 0.0, 255.0, cv::NORM_MINMAX);
        // cv::subtract(cv::Scalar::all(255.0), distanceImage, distanceImage);
        distanceImage.convertTo(distanceImage, CV_8UC1);
        videoDistance.write(distanceImage);
        cv::imwrite(distancePath + "distance" + std::to_string(i) + ".png", distanceImage);

        // Denoise distance image
        cv::Mat denoisedDistanceImage, distanceNoise;
        double distanceNoiseRatio;
        ImageDenoising idDistance;
        idDistance.setParameters(3, 7, 21, 0.03);
        idDistance.readInputs(distanceImage);
        idDistance.processData();
        idDistance.writeOutputs(denoisedDistanceImage, distanceNoise, distanceNoiseRatio);
        videoDenoisedDistance.write(denoisedDistanceImage);
        cv::imwrite(denoisedDistancePath + "denoisedDistance" + std::to_string(i) + ".png", denoisedDistanceImage);
        cv::imwrite(distanceNoisePath + "distanceNoise" + std::to_string(i) + ".png", distanceNoise);

        // Intensity image
        cv::Mat intensityImage = channels[3];
        intensityImage.convertTo(intensityImage, CV_8UC1);
        videoIntensity.write(intensityImage);
        cv::imwrite(intensityPath + "intensityImage" + std::to_string(i) + ".png", intensityImage);
    
        // Denoise intensity image
        cv::Mat denoisedIntensityImage, intensityNoise;
        double intensityNoiseRatio;
        ImageDenoising idIntensity;
        idIntensity.setParameters(10, 7, 21, 0.03);
        idIntensity.readInputs(intensityImage);
        idIntensity.processData();
        idIntensity.writeOutputs(denoisedIntensityImage, intensityNoise, intensityNoiseRatio);
        // std::cout << "intensityNoiseRatio:\t" << intensityNoiseRatio << std::endl;
        videoDenoisedIntensity.write(denoisedIntensityImage);
        cv::imwrite(denoisedIntensityPath + "denoisedIntensityImage" + std::to_string(i) + ".png", denoisedIntensityImage);
        cv::imwrite(intensityNoisePath + "intensityNoise" + std::to_string(i) + ".png", intensityNoise);
    }

    videoDistance.release();
    videoDenoisedDistance.release();
    videoIntensity.release();
    videoDenoisedIntensity.release();
                         
    return 0;
}

void createDir(std::string dirPath)
{
    if (mkdir(dirPath.c_str(), 0777) == -1)
        std::cerr << "Error :  " << strerror(errno) << std::endl;
    else
        std::cout << "Directory created" << std::endl;
}

void denoiseImageExmaple(int argc, char* argv[]) 
{
    cv::Mat image = cv::imread("/home/hvh/MyGit/PointCloud_Denoising/data/Back/0.18_32x1000/intensityImages/intensityImage" + std::to_string(atoi(argv[5])) + ".png", cv::IMREAD_GRAYSCALE);
    // image.convertTo(image, CV_64F);
    cv::Mat denoisedImage, noise;
    double noiseRatio;
    ImageDenoising id;
    id.setParameters(strtod(argv[1], NULL), atoi(argv[2]), atoi(argv[3]), strtod(argv[4], NULL));
    id.readInputs(image);
    id.processData();
    id.writeOutputs(denoisedImage, noise, noiseRatio);
    cv::imwrite("/home/hvh/MyGit/PointCloud_Denoising/data/denoisedImage.png", denoisedImage);
    cv::imwrite("/home/hvh/MyGit/PointCloud_Denoising/data/noise.png", noise);
    std::cout << "noiseRatio:\t" << noiseRatio << std::endl;
}

cv::Mat addNoise(cv::Mat image, double mean = 0.0, double stdDev = 1.0) 
{
    cv::Mat noise = cv::Mat(image.size(), image.depth());
    cv::randn(noise, cv::Scalar::all(mean), cv::Scalar::all(stdDev));
    cv::Mat noisyImage;
    cv::addWeighted(image, 0.5, noise, 0.5, 0.0, noisyImage);
    return noisyImage;
}

void displayMinMaxLoc(cv::Mat& image)
{
    double minVal, maxVal; 
    cv::Point minLoc, maxLoc;
    cv::minMaxLoc(image, &minVal, &maxVal, &minLoc, &maxLoc);
    std::cout   << "(minVal, maxVal, minLoc, maxLoc):\t(" 
                << minVal << "; \t"
                << maxVal << "; \t"
                << minLoc << "; \t"
                << maxLoc << ")"
                << std::endl;
}

template<typename T>
void display2DVector(std::vector<std::vector<T>> vector)
{
    for (const auto &row : vector) {
        for (const auto &s : row) 
            std::cout << s << ' ';
        std::cout << std::endl;
    }
}
