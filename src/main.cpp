#include <iostream>
#include <vector>
#include "rapidcsv.h"
#include "CSVReader.h"
#include "SphericalProjection.h"
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
	double delta_elevation  = 1.0;		// degree
	double azimuth_max 	    = 360.0;	// degree
	double azimuth_min 	    = 0.0;		// degree
	double delta_azimuth 	= 0.18;		// degree
    std::string nameLIDAR = "Back";
    std::string nameCase = "018_32x1000";

    // Azimuth reading
    CSVReader azimuthReader;
    std::vector<std::vector<double>> azimuthData;
    azimuthReader.setParameters(0.01, -1, 0);
    azimuthReader.readInputs("/home/hvh/MyGit/SphericalProjection/data/Logs/Log_05s/" + nameLIDAR + "Azimuths.csv");
    azimuthReader.processData();
    azimuthReader.writeOutputs(azimuthData);
    std::cout << "azimuthData:\t(" << azimuthData.size() << " rows, " << azimuthData[0].size() << " cols)" << std::endl;
    // display2DVector(azimuthData);

    // Distance reading
    CSVReader distanceReader;
    std::vector<std::vector<double>> distanceData;
    distanceReader.setParameters(0.004, -1, 0);
    distanceReader.readInputs("/home/hvh/MyGit/SphericalProjection/data/Logs/Log_05s/" + nameLIDAR + "Distances.csv");
    distanceReader.processData();
    distanceReader.writeOutputs(distanceData);
    std::cout << "distanceData:\t(" << distanceData.size() << " rows, " << distanceData[0].size() << " cols)" << std::endl;
    // display2DVector(distanceData);

    // Intensity reading
    CSVReader intensityReader;
    std::vector<std::vector<double>> intensityData;
    intensityReader.setParameters(1, -1, 0);
    intensityReader.readInputs("/home/hvh/MyGit/SphericalProjection/data/Logs/Log_05s/" + nameLIDAR + "Intensities.csv");
    intensityReader.processData();
    intensityReader.writeOutputs(intensityData);
    std::cout << "intensityData:\t(" << intensityData.size() << " rows, " << intensityData[0].size() << " cols)" << std::endl;
    // display2DVector(intensityData);

    // Video objects
    cv::VideoWriter videoDistance(  "/home/hvh/MyGit/SphericalProjection/data/" + nameLIDAR + "/" + nameCase + "/Distance.avi", 
                                    cv::VideoWriter::fourcc('M','J','P','G'), 10, cv::Size(width, height), false);
    cv::VideoWriter videoIntensity(  "/home/hvh/MyGit/SphericalProjection/data/" + nameLIDAR + "/" + nameCase + "/Intensity.avi", 
                                    cv::VideoWriter::fourcc('M','J','P','G'), 10, cv::Size(width, height), false);
    cv::VideoWriter videoEAD(  "/home/hvh/MyGit/SphericalProjection/data/" + nameLIDAR + "/" + nameCase + "/EAD.avi", 
                                cv::VideoWriter::fourcc('M','J','P','G'), 10, cv::Size(width, height), true);
    cv::VideoWriter videoEAI(  "/home/hvh/MyGit/SphericalProjection/data/" + nameLIDAR + "/" + nameCase + "/EAI.avi", 
                                cv::VideoWriter::fourcc('M','J','P','G'), 10, cv::Size(width, height), true);

    // Spherical projection 
    for (int i {0}; i < azimuthData.size(); ++i) {
        cv::Mat oImage;
        SphericalProjection sp;
        sp.setParameters(height, width, 15.0, -16.0, 1.0, 360.0, 0.0, 0.18);
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
        distanceImage.convertTo(distanceImage, CV_8UC1);
        videoDistance.write(distanceImage);
        cv::imwrite("/home/hvh/MyGit/SphericalProjection/data/" + nameLIDAR + "/" + nameCase + "/distanceImages/distanceImage" + std::to_string(i) + ".png", distanceImage);

        // Intensity image
        cv::Mat intensityImage = channels[3];
        intensityImage.convertTo(intensityImage, CV_8UC1);
        videoIntensity.write(intensityImage);
        cv::imwrite("/home/hvh/MyGit/SphericalProjection/data/" + nameLIDAR + "/" + nameCase + "/intensityImages/intensityImage" + std::to_string(i) + ".png", intensityImage);

        // Elevation, Azimuth, Distance
        cv::Mat EAD;
        std::vector<cv::Mat> channelEAD;
        channelEAD.push_back(elevationImage);
        channelEAD.push_back(azimuthImage);
        channelEAD.push_back(distanceImage);
        merge(channelEAD, EAD);
        videoEAD.write(EAD);
        cv::imwrite("/home/hvh/MyGit/SphericalProjection/data/" + nameLIDAR + "/" + nameCase + "/EADImages/EADImage" + std::to_string(i) + ".png", EAD);

        // Elevation, Azimuth, Intensity
        cv::Mat EAI;
        std::vector<cv::Mat> channelEAI;
        channelEAI.push_back(elevationImage);
        channelEAI.push_back(azimuthImage);
        channelEAI.push_back(intensityImage);
        merge(channelEAI, EAI);
        videoEAD.write(EAI);
        cv::imwrite("/home/hvh/MyGit/SphericalProjection/data/" + nameLIDAR + "/" + nameCase + "/EAIImages/EAIImage" + std::to_string(i) + ".png", EAI);
    }

    videoDistance.release();
    videoIntensity.release();
    videoEAD.release();
    videoEAI.release();
                         
    return 0;
}