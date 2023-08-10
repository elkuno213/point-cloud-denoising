/**
 * @name TEMPLATE FOR AUTONOMOUS DRIVING COMPONENTS
 * @copyright Gaussin Manugistique S.A. (c)
 * @author Vu-Hoi HUYNH
 * @brief Image denoising class for CV_8U type
 * @version 1.0
 * @date 02/06/2021
 * @comment 
 */

#include "ImageDenoising.hpp"

/******************************************************************************************************************************************************
CONSTRUCTORS & DESTRUCTORS
******************************************************************************************************************************************************/
// Constructor
ImageDenoising::ImageDenoising() {
	this->h 					= 10.0;
	this->templateWindowSize	= 7;
	this->searchWindowSize 		= 21;
	this->nonNoiseLevel			= 0.0;
}

// Destructor
ImageDenoising::~ImageDenoising() {
}

/******************************************************************************************************************************************************
PUBLIC METHODS
******************************************************************************************************************************************************/
/** @brief Set parameters for object.
	@param _h Parameter regulating filter strength.
	Big h value perfectly removes noise but also removes image details.
	Smaller h value preserves details but also preserves some noise.
	@param _templateWindowSize Size in pixels of the template patch that is used to compute weights.
	Should be odd. Recommended value 7 pixels.
	@param _searchWindowSize Size in pixels of the window that is used to compute weighted average for given pixel.
	Should be odd. Recommended value 21 pixels.
	Affect performance linearly: greater searchWindowsSize - greater denoising time.
	@param _nonNoiseLevel Percentage of non nosie pixel.
	For example, with a noise image of [0.0; 255.0], a nonNoiseLevel of 0.1 (10%) means that all pixels less than 
	10% * 255.0 are thresholded to 0.0 and considered as non noise ones.
**/
void ImageDenoising::setParameters(float _h, int _templateWindowSize, int _searchWindowSize, double _nonNoiseLevel) {
	this->h 					= _h;
	this->templateWindowSize	= _templateWindowSize;
	this->searchWindowSize 		= _searchWindowSize;
	this->nonNoiseLevel 		= _nonNoiseLevel;
}

/** @brief Read inputs for object.
	@param _iNoisyImage Noisy image
**/
void ImageDenoising::readInputs(cv::Mat& _iNoisyImage) {
	// Receives the inputs from the MAPS wrapper and saves them on the class
	this->iNoisyImage = _iNoisyImage;
}

/** @brief Process data for object.
**/
void ImageDenoising::processData() {
	// Fast NL Means Denoising for CV_8U image
	if (this->iNoisyImage.depth() != 0)		// If the image type is not CV_8U, convert it
		this->iNoisyImage.convertTo(this->iNoisyImage, CV_8U);
	this->oDenoisedImage = cv::Mat(this->iNoisyImage.size(), this->iNoisyImage.depth(), cv::Scalar::all(0));
	cv::fastNlMeansDenoising(this->iNoisyImage, this->oDenoisedImage, this->h, this->templateWindowSize, this->searchWindowSize);

	// Noise extraction
	this->oNoise = this->iNoisyImage - this->oDenoisedImage;

	// Noise filtering
	double minLim, maxLim;
	getMinMaxLim(this->iNoisyImage, minLim, maxLim);
	cv::threshold(this->oNoise, this->oNoise, this->nonNoiseLevel * (maxLim - minLim) + minLim, maxLim, cv::THRESH_TOZERO);

	this->oNoiseRatio = double(cv::countNonZero(this->oNoise)) / double(this->oNoise.rows * this->oNoise.cols); // Noise ratio
}

/** @brief Write out the processed data.
	@param _oDenoisedImage Image denoised
	@param _oNoise Noise
	@param _oNoiseRatio Noise ratio
**/
void ImageDenoising::writeOutputs(cv::Mat& _oDenoisedImage, cv::Mat& _oNoise, double& _oNoiseRatio) {
	// Pass the values of scores to the MAPS wrapper
	_oDenoisedImage = this->oDenoisedImage.clone();
	_oNoise = this->oNoise.clone();
	_oNoiseRatio = this->oNoiseRatio;
}

/******************************************************************************************************************************************************
PRIVATE METHODS
******************************************************************************************************************************************************/
/** @brief Get min and max limits of an image, based on its type.
	@param image Input image
	@param image Output minLimit
	@param image Output maxLimit
**/
template<typename T>
void ImageDenoising::getMinMaxLim(const cv::Mat image, T& minLim, T& maxLim) {
	switch (image.depth()) {
	case 0:
		minLim = 0;
		maxLim = 255;
		break;
	case 1:
		minLim = -128;
		maxLim = 127;
		break;
	case 2:
		minLim = 0;
		maxLim = 65535;
		break;
	case 3:
		minLim = -32768;
		maxLim = 32767;
		break;
	case 4:
		minLim = -2147483648;
		maxLim = 2147483647;
		break;
	case 5:
	case 6:
		cv::Point minLoc, maxLoc;
		cv::minMaxLoc(image, &minLim, &maxLim, &minLoc, &maxLoc);
		break;
	}
}