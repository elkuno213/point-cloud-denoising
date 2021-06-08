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
    int height = 32, width = 1000;
    std::string nameLIDAR = "Back";
    std::string nameCase = "018_32x1000";

    // Azimuths
    CSVReader azimuthReader;
    std::vector<std::vector<double>> azimuthData;
    azimuthReader.setParameters(0.01, -1, 0);
    azimuthReader.readInputs("/home/hvh/MyGit/SphericalProjection/data/Logs/Log_05s/" + nameLIDAR + "Azimuths.csv");
    azimuthReader.processData();
    azimuthReader.writeOutputs(azimuthData);
    std::cout << "azimuthData:\t(" << azimuthData.size() << " rows, " << azimuthData[0].size() << " cols)" << std::endl;
    // display2DVector(azimuthData);

    CSVReader distanceReader;
    std::vector<std::vector<double>> distanceData;
    distanceReader.setParameters(0.004, -1, 0);
    distanceReader.readInputs("/home/hvh/MyGit/SphericalProjection/data/Logs/Log_05s/" + nameLIDAR + "Distances.csv");
    distanceReader.processData();
    distanceReader.writeOutputs(distanceData);
    std::cout << "distanceData:\t(" << distanceData.size() << " rows, " << distanceData[0].size() << " cols)" << std::endl;
    // display2DVector(distanceData);

    CSVReader intensityReader;
    std::vector<std::vector<double>> intensityData;
    intensityReader.setParameters(1, -1, 0);
    intensityReader.readInputs("/home/hvh/MyGit/SphericalProjection/data/Logs/Log_05s/" + nameLIDAR + "Intensities.csv");
    intensityReader.processData();
    intensityReader.writeOutputs(intensityData);
    std::cout << "intensityData:\t(" << intensityData.size() << " rows, " << intensityData[0].size() << " cols)" << std::endl;
    // display2DVector(intensityData);

    // Video objects
    // cv::VideoWriter videoElevation(  "/home/hvh/MyGit/SphericalProjection/data/" + nameLIDAR + "/" + nameCase + "/Elevation.avi", 
    //                                 cv::VideoWriter::fourcc('M','J','P','G'), 10, cv::Size(width, height), false);
    // cv::VideoWriter videoAzimuth(  "/home/hvh/MyGit/SphericalProjection/data/" + nameLIDAR + "/" + nameCase + "/Azimuth.avi", 
    //                                 cv::VideoWriter::fourcc('M','J','P','G'), 10, cv::Size(width, height), false);
    cv::VideoWriter videoDistance(  "/home/hvh/MyGit/SphericalProjection/data/" + nameLIDAR + "/" + nameCase + "/Distance.avi", 
                                    cv::VideoWriter::fourcc('M','J','P','G'), 10, cv::Size(width, height), false);
    cv::VideoWriter videoIntensity(  "/home/hvh/MyGit/SphericalProjection/data/" + nameLIDAR + "/" + nameCase + "/Intensity.avi", 
                                    cv::VideoWriter::fourcc('M','J','P','G'), 10, cv::Size(width, height), false);
    cv::VideoWriter videoEAD(  "/home/hvh/MyGit/SphericalProjection/data/" + nameLIDAR + "/" + nameCase + "/EAD.avi", 
                                cv::VideoWriter::fourcc('M','J','P','G'), 10, cv::Size(width, height), true);
    cv::VideoWriter videoEAI(  "/home/hvh/MyGit/SphericalProjection/data/" + nameLIDAR + "/" + nameCase + "/EAI.avi", 
                                cv::VideoWriter::fourcc('M','J','P','G'), 10, cv::Size(width, height), true);
    // cv::VideoWriter videoDI(  "/home/hvh/MyGit/SphericalProjection/data/" + nameLIDAR + "/" + nameCase + "/DI.avi", 
    //                             cv::VideoWriter::fourcc('M','J','P','G'), 10, cv::Size(width, height), true);

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
        // displayMinMaxLoc(elevationImage);
        cv::normalize(elevationImage, elevationImage, 0.0, 255.0, cv::NORM_MINMAX);
        // displayMinMaxLoc(elevationImage);
        elevationImage.convertTo(elevationImage, CV_8UC1);
        // videoElevation.write(elevationImage);
        // cv::imwrite("/home/hvh/MyGit/SphericalProjection/data/" + nameLIDAR + "/" + nameCase + "/elevationImages/elevationImage" + std::to_string(i) + ".png", elevationImage);
        
        // Azimuth image
        cv::Mat azimuthImage = channels[1];
        // displayMinMaxLoc(azimuthImage);
        cv::normalize(azimuthImage, azimuthImage, 0.0, 255.0, cv::NORM_MINMAX);
        // displayMinMaxLoc(azimuthImage);
        azimuthImage.convertTo(azimuthImage, CV_8UC1);
        // videoDistance.write(azimuthImage);
        // cv::imwrite("/home/hvh/MyGit/SphericalProjection/data/" + nameLIDAR + "/" + nameCase + "/azimuthImages/azimuthImage" + std::to_string(i) + ".png", azimuthImage);

        // Distance image
        cv::Mat distanceImage = channels[2];
        // displayMinMaxLoc(distanceImage);
        cv::normalize(distanceImage, distanceImage, 0.0, 255.0, cv::NORM_MINMAX);
        // displayMinMaxLoc(distanceImage);
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

        // // Distance, Intensity
        // cv::Mat DI;
        // std::vector<cv::Mat> channelDI;
        // channelDI.push_back(distanceImage);
        // channelDI.push_back(distanceImage);
        // merge(channelDI, DI);
        // cv::imwrite("/home/hvh/MyGit/SphericalProjection/data/" + nameLIDAR + "/" + nameCase + "/DIImages/DIImage" + std::to_string(i) + ".png", DI);
        // // videoEAD.write(DI);
    }

    // videoElevation.release();
    // videoAzimuth.release();
    videoDistance.release();
    videoIntensity.release();
    videoEAD.release();
    videoEAI.release();
    // videoDI.release();
                         
    return 0;
}