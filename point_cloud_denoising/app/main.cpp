#include <iostream>
#include <string>
#include <vector>
#include <sys/stat.h>
#include "rapidcsv.h"
#include "CSVReader.hpp"
#include "SphericalProjection.hpp"
#include "ImageDenoising.hpp"
#include "para_finder2.h"
#include <opencv2/opencv.hpp>

void createDir(std::string dirPath);

cv::Mat addNoise(cv::Mat image, double mean, double stdDev);

void displayMinMaxLoc(cv::Mat& image);

template<typename T>
void display2DVector(std::vector<std::vector<T>> vector);

int main(int argc, char* argv[])
{
    // Parameters
    ReadParameter xmlReader;
	if (!xmlReader.loadXML(argv[1]))
        return -1;
    std::string LogName         = xmlReader.getString("PointCloud_Denoising", "LogName");
    std::string LIDARName       = xmlReader.getString("PointCloud_Denoising", "LIDARName");
    int height                  = xmlReader.getInt("PointCloud_Denoising", "height");
    int width                   = xmlReader.getInt("PointCloud_Denoising", "width");
    double elevation_max        = xmlReader.getDouble("PointCloud_Denoising", "elevation_max");
    double elevation_min        = xmlReader.getDouble("PointCloud_Denoising", "elevation_min");
    double delta_elevation      = xmlReader.getDouble("PointCloud_Denoising", "delta_elevation");
    double azimuth_max          = xmlReader.getDouble("PointCloud_Denoising", "azimuth_max");
    double azimuth_min          = xmlReader.getDouble("PointCloud_Denoising", "azimuth_min");
    double delta_azimuth        = xmlReader.getDouble("PointCloud_Denoising", "delta_azimuth");
    float h_D                   = xmlReader.getFloat("PointCloud_Denoising", "h_D");
    int templateWindowSize_D    = xmlReader.getInt("PointCloud_Denoising", "templateWindowSize_D");
    int searchWindowSize_D      = xmlReader.getInt("PointCloud_Denoising", "searchWindowSize_D");
    double nonNoiseLevel_D      = xmlReader.getDouble("PointCloud_Denoising", "nonNoiseLevel_D");
    float h_I                   = xmlReader.getFloat("PointCloud_Denoising", "h_I");
    int templateWindowSize_I    = xmlReader.getInt("PointCloud_Denoising", "templateWindowSize_I");
    int searchWindowSize_I      = xmlReader.getInt("PointCloud_Denoising", "searchWindowSize_I");
    double nonNoiseLevel_I      = xmlReader.getDouble("PointCloud_Denoising", "nonNoiseLevel_I");

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
        idDistance.setParameters(h_D, templateWindowSize_D, searchWindowSize_D, nonNoiseLevel_D);
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
        idIntensity.setParameters(h_I, templateWindowSize_I, searchWindowSize_I, nonNoiseLevel_I);
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
