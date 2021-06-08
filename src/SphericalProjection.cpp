/**
 * @name TEMPLATE FOR AUTONOMOUS DRIVING COMPONENTS
 * @copyright Gaussin Manugistique S.A. (c)
 * @author Vu-Hoi HUYNH
 * @brief 
 * @version 1.0
 * @date 02/06/2021
 * @comment 
 */

#include "SphericalProjection.h"

/******************************************************************************************************************************************************
CONSTRUCTORS & DESTRUCTORS
******************************************************************************************************************************************************/
// Constructor
SphericalProjection::SphericalProjection() {
	this->height 			= 32;		// pixel
	this->width 			= 512;		// pixel
	this->elevation_max 	= 15.0;		// degree
	this->elevation_min 	= -16.0;	// degree
	this->delta_elevation 	= 1.0;		// degree
	this->azimuth_max 		= 360.0;	// degree
	this->azimuth_min 		= 0.0;		// degree
	this->delta_azimuth 	= 0.09;		// degree
}

// Destructor
SphericalProjection::~SphericalProjection() {
}

/******************************************************************************************************************************************************
PUBLIC METHODS
******************************************************************************************************************************************************/
/** @brief Set parameters for object.
	@param _nbParticles Input number of particles to be processed
	@param _sizePointCloud Input number of LIDAR points
**/
void SphericalProjection::setParameters(int _height, int _width,
										double _elevation_max, double _elevation_min,
										double _delta_elevation, double _azimuth_max,
										double _azimuth_min, double _delta_azimuth) {
	this->height 			= _height;			// pixel
	this->width 			= _width;			// pixel
	this->elevation_max 	= _elevation_max;	// degree
	this->elevation_min 	= _elevation_min;	// degree
	this->delta_elevation 	= _delta_elevation;	// degree
	this->azimuth_max 		= _azimuth_max;		// degree
	this->azimuth_min 		= _azimuth_min;		// degree
	this->delta_azimuth 	= _delta_azimuth;	// degree
}

/** @brief Read input for object.
	@param _iDistances Input distance data
	@param _iAzimuths Input azimuth data
	@param _iIntensities Input intensity data
**/
void SphericalProjection::readInputs(	std::vector<double>& _iAzimuths,
										std::vector<double>& _iDistances,
										std::vector<double>& _iIntensities) {
	// Receives the inputs from the MAPS wrapper and saves them on the class
	this->iAzimuths 	= _iAzimuths;
	this->iDistances 	= _iDistances;
	this->iIntensities 	= _iIntensities;
}

/** @brief Process data for object.
**/
void SphericalProjection::processData() {
	// Correct image size
	int max_height = int((this->elevation_max - this->elevation_min) / this->delta_elevation) + 1;
	if (this->height > max_height || this->height < 0)
		this->height = max_height;
	int max_width = int((this->azimuth_max - this->azimuth_min) / this->delta_azimuth);
	if (this->width > max_width || this->width < 0)
		this->width = max_width;

	// Make image
	this->oImage = cv::Mat(this->height, this->width, CV_64FC(4), cv::Scalar::all(0));
	for (int i {0}; i < this->iAzimuths.size(); ++i) {
		double elevation = this->elevation_max;		// elevation angle in degree
		for (int j {0}; j < this->height; ++j) {
			int pixel_u = 0, pixel_v = 0;
			pixelProjection(elevation, this->iAzimuths[i], &pixel_u, &pixel_v);
			this->oImage.at<cv::Vec4d>(pixel_u, pixel_v) = cv::Vec4d{elevation, this->iAzimuths[i], 
																	 this->iDistances[this->height * i + j], 
																	 this->iIntensities[this->height * i + j]};
			elevation -= 1;
		}
	}
}

/** @brief Process data for object.
	@param _oImage Input image projected
**/
void SphericalProjection::writeOutputs(cv::Mat& _oImage) {
	// Pass the values of scores to the MAPS wrapper
	_oImage = this->oImage.clone();
}

/** @brief Project a point into image plane.
	@param elevation Input elevation value
	@param azimuth Input azimuth value
	@param u Output pixel element u
	@param v Output pixel element v
**/
void SphericalProjection::pixelProjection(const double elevation, const double azimuth, int* pixel_u, int* pixel_v) {
	// Calculate pixel u and make sure that it is in the range [0, this->height - 1]
	int u = round((elevation - this->elevation_min) * (this->height - 1) / (this->elevation_max - this->elevation_min));
	u = std::min(this->height - 1, u);
	u = std::max(0, u);
	*pixel_u = int(u);

	// Calculate pixel v and make sure that it is in the range [0, this->width - 1]
	int v = round((azimuth - this->azimuth_min) * (this->width - 1) / (this->azimuth_max - this->azimuth_min));
	v = std::min(this->width - 1, v);
	v = std::max(0, v);
	*pixel_v = int(v);
}

/******************************************************************************************************************************************************
PRIVATE METHODS
******************************************************************************************************************************************************/
