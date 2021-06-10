/**
 * @name TEMPLATE FOR AUTONOMOUS DRIVING COMPONENTS
 * @copyright Gaussin Manugistique S.A. (c)
 * @author Vu-Hoi HUYNH
 * @brief Image denoising class
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
	void setParameters(float _h, int _templateWindowSize, int _searchWindowSize);

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

};

#endif // _ImageDenoising_H
