#include <iostream>
#include <vector>
#include "rapidcsv.h"
#include "CSVReader.hpp"
#include "SphericalProjection.hpp"
#include "ImageDenoising.hpp"
#include <opencv2/opencv.hpp>

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

int main(int argc, char* argv[])
{
    // ------------------------------------------------------------------------------------------------------------
    // Parameters
	int height 			    = 32;		// pixel
	int width 			    = 1000;		// pixel
	double elevation_max 	= 15.0;		// degree
	double elevation_min 	= -16.0;	// degree
	double delta_elevation 	= 1.0;		// degree
	double azimuth_max 		= 360.0;	// degree
	double azimuth_min 		= 0.0;		// degree
	double delta_azimuth 	= 0.18;		// degree
    std::string nameLIDAR = argv[1];
    std::string nameCase = std::to_string(delta_azimuth).substr(0, 4) + "_" + std::to_string(height) + "x" + std::to_string(width);

    // Azimuth reading
    CSVReader azimuthReader;
    std::vector<std::vector<double>> azimuthData;
    azimuthReader.setParameters(0.01, -1, 0);
    azimuthReader.readInputs("/home/hvh/MyGit/PointCloud_Denoising/data/Logs/Log_2min/" + nameLIDAR + "Azimuths.csv");
    azimuthReader.processData();
    azimuthReader.writeOutputs(azimuthData);
    std::cout << "azimuthData:\t(" << azimuthData.size() << " rows, " << azimuthData[0].size() << " cols)" << std::endl;

    // Distance reading
    CSVReader distanceReader;
    std::vector<std::vector<double>> distanceData;
    distanceReader.setParameters(0.004, -1, 0);
    distanceReader.readInputs("/home/hvh/MyGit/PointCloud_Denoising/data/Logs/Log_2min/" + nameLIDAR + "Distances.csv");
    distanceReader.processData();
    distanceReader.writeOutputs(distanceData);
    std::cout << "distanceData:\t(" << distanceData.size() << " rows, " << distanceData[0].size() << " cols)" << std::endl;

    // Intensity reading
    CSVReader intensityReader;
    std::vector<std::vector<double>> intensityData;
    intensityReader.setParameters(1, -1, 0);
    intensityReader.readInputs("/home/hvh/MyGit/PointCloud_Denoising/data/Logs/Log_2min/" + nameLIDAR + "Intensities.csv");
    intensityReader.processData();
    intensityReader.writeOutputs(intensityData);
    std::cout << "intensityData:\t(" << intensityData.size() << " rows, " << intensityData[0].size() << " cols)" << std::endl;

    // Video objects
    cv::VideoWriter videoDistance(  "/home/hvh/MyGit/PointCloud_Denoising/data/" + nameLIDAR + "/" + nameCase + "/Distance.avi", 
                                    cv::VideoWriter::fourcc('M','J','P','G'), 10, cv::Size(width, height), false);
    cv::VideoWriter videoDenoisedDistance(  "/home/hvh/MyGit/PointCloud_Denoising/data/" + nameLIDAR + "/" + nameCase + "/DenoisedDistance.avi", 
                                    cv::VideoWriter::fourcc('M','J','P','G'), 10, cv::Size(width, height), false);
    cv::VideoWriter videoIntensity(  "/home/hvh/MyGit/PointCloud_Denoising/data/" + nameLIDAR + "/" + nameCase + "/Intensity.avi", 
                                    cv::VideoWriter::fourcc('M','J','P','G'), 10, cv::Size(width, height), false);
    cv::VideoWriter videoDenoisedIntensity(  "/home/hvh/MyGit/PointCloud_Denoising/data/" + nameLIDAR + "/" + nameCase + "/DenoisedIntensity.avi", 
                                    cv::VideoWriter::fourcc('M','J','P','G'), 10, cv::Size(width, height), false);
                                
    // Spherical projection 
    for (int i {0}; i < azimuthData.size(); ++i) {
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
        cv::imwrite("/home/hvh/MyGit/PointCloud_Denoising/data/" + nameLIDAR + "/" + nameCase + "/distanceImages/distanceImage" + std::to_string(i) + ".png", distanceImage);

        // Denoise distance image
        cv::Mat denoisedDistanceImage, distanceNoise;
        double distanceNoiseRatio;
        ImageDenoising idDistance;
        idDistance.setParameters(3, 7, 21, 0.03);
        idDistance.readInputs(distanceImage);
        idDistance.processData();
        idDistance.writeOutputs(denoisedDistanceImage, distanceNoise, distanceNoiseRatio);
        // std::cout << "distanceNoiseRatio:\t" << distanceNoiseRatio << std::endl;
        videoDenoisedDistance.write(denoisedDistanceImage);
        cv::imwrite("/home/hvh/MyGit/PointCloud_Denoising/data/" + nameLIDAR + "/" + nameCase + "/denoisedDistanceImages/denoisedDistanceImage" + std::to_string(i) + ".png", denoisedDistanceImage);
        cv::imwrite("/home/hvh/MyGit/PointCloud_Denoising/data/" + nameLIDAR + "/" + nameCase + "/distanceNoises/distanceNoise" + std::to_string(i) + ".png", distanceNoise);

        // Intensity image
        cv::Mat intensityImage = channels[3];
        intensityImage.convertTo(intensityImage, CV_8UC1);
        videoIntensity.write(intensityImage);
        cv::imwrite("/home/hvh/MyGit/PointCloud_Denoising/data/" + nameLIDAR + "/" + nameCase + "/intensityImages/intensityImage" + std::to_string(i) + ".png", intensityImage);
    
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
        cv::imwrite("/home/hvh/MyGit/PointCloud_Denoising/data/" + nameLIDAR + "/" + nameCase + "/denoisedIntensityImages/denoisedIntensityImage" + std::to_string(i) + ".png", denoisedIntensityImage);
        cv::imwrite("/home/hvh/MyGit/PointCloud_Denoising/data/" + nameLIDAR + "/" + nameCase + "/intensityNoises/intensityNoise" + std::to_string(i) + ".png", intensityNoise);
    }

    videoDistance.release();
    videoDenoisedDistance.release();
    videoIntensity.release();
    videoDenoisedIntensity.release();
                         
    return 0;
}