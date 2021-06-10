/**
 * @name TEMPLATE FOR AUTONOMOUS DRIVING COMPONENTS
 * @copyright Gaussin Manugistique S.A. (c)
 * @author Vu-Hoi HUYNH
 * @brief Image denoising class
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
}

// Destructor
ImageDenoising::~ImageDenoising() {
}

/******************************************************************************************************************************************************
PUBLIC METHODS
******************************************************************************************************************************************************/
/** @brief Set parameters for object.
	@param 
**/
void ImageDenoising::setParameters(float _h, int _templateWindowSize, int _searchWindowSize) {
	this->h 					= _h;
	this->templateWindowSize	= _templateWindowSize;
	this->searchWindowSize 		= _searchWindowSize;
}
/** @brief Read input for object.
	@param 
**/
void ImageDenoising::readInputs(cv::Mat& _iNoisyImage) {
	// Receives the inputs from the MAPS wrapper and saves them on the class
	this->iNoisyImage 		= _iNoisyImage;
}

/** @brief Process data for object.
**/
void ImageDenoising::processData() {
	// Denoising
	this->oDenoisedImage = cv::Mat(this->iNoisyImage.size(), this->iNoisyImage.depth(), cv::Scalar::all(0));
	cv::fastNlMeansDenoising(this->iNoisyImage, this->oDenoisedImage, this->h, this->templateWindowSize, this->searchWindowSize);

	// Noise extraction
	this->oNoise = this->iNoisyImage - this->oDenoisedImage;

	// Noise ratio
	this->oNoiseRatio = double(cv::countNonZero(this->oNoise)) / double(this->oNoise.rows * this->oNoise.cols);
}

/** @brief Write out the processed data.
	@param 
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
