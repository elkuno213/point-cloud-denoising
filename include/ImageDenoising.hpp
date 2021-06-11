/**
 * @name TEMPLATE FOR AUTONOMOUS DRIVING COMPONENTS
 * @copyright Gaussin Manugistique S.A. (c)
 * @author Vu-Hoi HUYNH
 * @brief Image denoising class for CV_8U type
 * @version 1.0
 * @date 02/06/2021
 * @comment 
 */

#include <iostream>
#include <opencv2/opencv.hpp>

#ifndef _ImageDenoising_H
#define _ImageDenoising_H

class ImageDenoising {

private:
	// Inputs
	cv::Mat iNoisyImage;						// Input noisy image

	// Outputs
	cv::Mat oDenoisedImage;						// Output denoised image
	cv::Mat oNoise;								// Output noise
	double oNoiseRatio;							// Percentage of noise of output noise
	
	// Parameters
	float h;									// Parameter regulating filter strength. 
												// Big h value perfectly removes noise but also removes image details.
												// Smaller h value preserves details but also preserves some noise.
	int templateWindowSize;						// Size in pixels of the template patch that is used to compute weights.
												// Should be odd. Recommended value 7 pixels.
	int searchWindowSize;						// Size in pixels of the window that is used to compute weighted average for given pixel. 
												// Should be odd. Recommended value 21 pixels.
												// Affect performance linearly: greater searchWindowsSize - greater denoising time.
	double nonNoiseLevel;						// Percentage of non nosie pixel.
												// For example, with a noise image of [0.0; 255.0], a nonNoiseLevel of 0.1 (10%) means that
												// all pixels less than 10% * 255.0 are thresholded to 0.0 and considered as non noise ones.

// Constructors & Destructors
public:
	// Constructor
	ImageDenoising();

	// Destructor
	~ImageDenoising();

// Public methods
public:
	/** @brief Set parameters for object.
    	@param 
    **/
	void setParameters(float _h, int _templateWindowSize, int _searchWindowSize, double _nonNoiseLevel);

	/** @brief Read input for object.
		@param 
    **/
	void readInputs(cv::Mat& _iNoisyImage);

	/** @brief Process data for object.
    **/
	void processData();

	/** @brief Write out the processed data.
    	@param 
    **/
	void writeOutputs(cv::Mat& _oDenoisedImage, cv::Mat& _oNoise, double& _oNoiseRatio);

// Private methods
private:
	/** @brief Get min and max limits of an image, depending its type.
    	@param 
    **/
	template<typename T>
	void getMinMaxLim(const cv::Mat image, T& minLimit, T& maxLimit);
};

#endif // _ImageDenoising_H
