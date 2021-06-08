/**
 * @name TEMPLATE FOR AUTONOMOUS DRIVING COMPONENTS
 * @copyright Gaussin Manugistique S.A. (c)
 * @author Vu-Hoi HUYNH
 * @brief 
 * @version 1.0
 * @date 02/06/2021
 * @comment 
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <opencv2/opencv.hpp>

#ifndef _SphericalProjection_H
#define _SphericalProjection_H

class SphericalProjection {

private:
	// Inputs
	std::vector<double> iAzimuths;				// Azimuth data
	std::vector<double> iDistances;				// Distance data
	std::vector<double> iIntensities;			// Intensity data

	// Outputs
	cv::Mat oImage;											// Multi-channel Image (Channels: distance, elevation, azimuth, intensity)

	// Parameters
	int height;												// Image height
	int width;												// Image width
	double elevation_max;									// Maximum elevation
	double elevation_min;									// Minimum elevation
	double delta_elevation;									// Elevation resolution
	double azimuth_max;										// Maximum azimuth
	double azimuth_min;										// Minimum azimuth
	double delta_azimuth;									// Azimuth resolution

// Constructors & Destructors
public:
	// Constructor
	SphericalProjection();

	// Destructor
	~SphericalProjection();

// Public methods
public:
	/** @brief Set parameters for object.
    	@param _normalizeScores Input boolean variable to normalize score vector
    **/
	void setParameters(	int _height, int _width,
						double _elevation_max, double _elevation_min,
						double _delta_elevation, double _azimuth_max,
						double _azimuth_min, double _delta_azimuth);

	/** @brief Read input for object.
		@param _iDistances Input distance data
		@param _iAzimuths Input azimuth data
		@param _iIntensities Input intensity data
    **/
	void readInputs(std::vector<double>& _iAzimuths,
					std::vector<double>& _iDistances,
					std::vector<double>& _iIntensities);

	/** @brief Process data for object.
    **/
	void processData();

	/** @brief Process data for object.
    	@param _oImage Input image projected
    **/
	void writeOutputs(cv::Mat& _oImage);

	/** @brief Project a point into image plane.
		@param elevation Input elevation value
		@param azimuth Input azimuth value
		@param u Output pixel element u
		@param v Output pixel element v
    **/
	void pixelProjection(const double elevation, const double azimuth, int* pixel_u, int* pixel_v);
	
// Private methods
private:

};

#endif // _SphericalProjection_H
