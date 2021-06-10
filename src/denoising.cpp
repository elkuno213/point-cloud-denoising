#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include "ImageDenoising.hpp"

cv::Mat addNoise(cv::Mat image, double mean = 0.0, double stdDev = 1.0) {
    cv::Mat noise = cv::Mat(image.size(), image.depth());
    cv::randn(noise, cv::Scalar::all(mean), cv::Scalar::all(stdDev));
    cv::Mat noisyImage;
    cv::addWeighted(image, 0.5, noise, 0.5, 0.0, noisyImage);
    return noisyImage;
}

int main(int argc, char* argv[])
{
    // ----------------------------------------------------------------------------------------------------------
    // std::vector<cv::Mat> data;

    // // Create a VideoCapture object and open the input file
    // // If the input is the web camera, pass 0 instead of the video file name
    // cv::VideoCapture cap("/home/hvh/MyGit/PointCloud_Denoising/data/test.avi"); 

    // // Check if camera opened successfully
    // if (!cap.isOpened()) {
    //     std::cout << "Error opening video stream or file" << std::endl;
    //     return -1;
    // }

    // while (1) {
    //     cv::Mat frame;
    //     // Capture frame-by-frame
    //     cap >> frame;

    //     // If the frame is empty, break immediately
    //     if (frame.empty())
    //         break;

    //     // // Display the resulting frame
    //     // imshow("Frame", frame);

    //     // // Press  ESC on keyboard to exit
    //     // char c = (char)cv::waitKey(25);
    //     // if (c == 27)
    //     //     break;

    //     data.push_back(frame);
    // }
    // // std::cout << data.size() << std::endl;

    // // When everything done, release the video capture object
    // cap.release();

    // // Closes all the frames
    // // cv::destroyAllWindows();

    // // Denoise
    // cv::Mat denoisedImage;
    // cv::fastNlMeansDenoisingMulti(data, denoisedImage, atoi(argv[1]), atoi(argv[2]), strtod(argv[3], NULL), atoi(argv[4]), atoi(argv[5]));
    // cv::imwrite("/home/hvh/MyGit/PointCloud_Denoising/data/denoiseTest2.png", denoisedImage);
    // ----------------------------------------------------------------------------------------------------------



    // ----------------------------------------------------------------------------------------------------------
    cv::Mat image = cv::imread("/home/hvh/MyGit/PointCloud_Denoising/data/test/intensityImage" + std::to_string(atoi(argv[4])) + ".png", cv::IMREAD_GRAYSCALE);
    cv::Mat denoisedImage, noise;
    double noiseRatio;
    ImageDenoising id;
    id.setParameters(strtod(argv[1], NULL), atoi(argv[2]), atoi(argv[3]));
    id.readInputs(image);
    id.processData();
    id.writeOutputs(denoisedImage, noise, noiseRatio);
    cv::imwrite("/home/hvh/MyGit/PointCloud_Denoising/data/denoisedImage.png", denoisedImage);
    cv::imwrite("/home/hvh/MyGit/PointCloud_Denoising/data/noise.png", noise);
    std::cout << "noiseRatio:\t" << noiseRatio << std::endl;
    // ----------------------------------------------------------------------------------------------------------



    // ----------------------------------------------------------------------------------------------------------
    // cv::Mat image = cv::imread("/home/hvh/MyGit/PointCloud_Denoising/data/image.jpg", cv::IMREAD_GRAYSCALE);
    // cv::namedWindow("image", cv::WINDOW_GUI_EXPANDED);
    // cv::resizeWindow("image", cv::Size(500, 500));
    // cv::moveWindow("image", 100, 100);
    // cv::imshow("image", image);

    // cv::Mat noisyImage = addNoise(image, 0.0, 500.0);
    // cv::namedWindow("noisy image", cv::WINDOW_GUI_EXPANDED);
    // cv::resizeWindow("noisy image", cv::Size(500, 500));
    // cv::moveWindow("noisy image", 700, 100);
    // cv::imshow("noisy image", noisyImage);

    // cv::Mat denoisedImage;
    // cv::fastNlMeansDenoising(noisyImage, denoisedImage, 10, 7, 21);
    // cv::namedWindow("denoised image", cv::WINDOW_GUI_EXPANDED);
    // cv::resizeWindow("denoised image", cv::Size(500, 500));
    // cv::moveWindow("denoised image", 1300, 100);
    // cv::imshow("denoised image", denoisedImage);

    // cv::waitKey(0);
    // ----------------------------------------------------------------------------------------------------------

    return 0;
}