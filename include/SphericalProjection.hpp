/**
 * @name TEMPLATE FOR AUTONOMOUS DRIVING COMPONENTS
 * @copyright Gaussin Manugistique S.A. (c)
 * @author Vu-Hoi HUYNH
 * @brief Spherical projection of LIDAR PointCloud into 2D image.
 * @version 1.0
 * @date 02/06/2021
 * @comment 
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <opencv2/opencv.hpp>

#ifndef _SphericalProjection_H
#define _SphericalProjection_H

class SphericalProjection {

private:
	// Inputs
	std::vector<double> iAzimuths;		// Azimuth data
	std::vector<double> iDistances;		// Distance data
	std::vector<double> iIntensities;	// Intensity data

	// Outputs
	cv::Mat oImage;						// Multi-channel Image (Channels: distance, elevation, azimuth, intensity)

	// Parameters
	int height;							// Image height (pixel)
	int width;							// Image width (pixel)
	double elevation_max;				// Maximum elevation (degree)
	double elevation_min;				// Minimum elevation (degree)
	double delta_elevation;				// Elevation resolution (degree)
	double azimuth_max;					// Maximum azimuth (degree)
	double azimuth_min;					// Minimum azimuth (degree)
	double delta_azimuth;				// Azimuth resolution (degree)

// Constructors & Destructors
public:
	// Constructor
	SphericalProjection();

	// Destructor
	~SphericalProjection();

// Public methods
public:
	/** @brief Set parameters for object.
    	@param _height Input image height (pixel)
    	@param _width Input image width (pixel)
    	@param _elevation_max Input maximum elevation (degree)
    	@param _elevation_min Input minimum elevation (degree)
    	@param _delta_elevation Input elevation resolution (degree)
    	@param _azimuth_max Input maximum azimuth (degree)
    	@param _azimuth_min Input minimum azimuth (degree)
    	@param _delta_azimuth Input azimuth resolution (degree)
    **/
	void setParameters(	int _height, int _width,
						double _elevation_max, double _elevation_min, double _delta_elevation, 
						double _azimuth_max, double _azimuth_min, double _delta_azimuth);

	/** @brief Read input for object.
		@param _iAzimuths Input azimuth data
		@param _iDistances Input distance data
		@param _iIntensities Input intensity data
    **/
	void readInputs(std::vector<double>& _iAzimuths,
					std::vector<double>& _iDistances,
					std::vector<double>& _iIntensities);

	/** @brief Process data for object.
    **/
	void processData();

	/** @brief Write out the processed data.
    	@param _oImage Input image projected
    **/
	void writeOutputs(cv::Mat& _oImage);
	
// Private methods
private:
	/** @brief Angle checking
	 * 1. If maximum < minimum, then swap maximum and minimum values 
	 * 2. If maximum - minimum = 360°, then these two values are equal and maximum -= resolution.
		@param maximum InputOutput maximum value
		@param minimum InputOutput minimum value
		@param resolution Input resolution value
    **/
	void angleChecking(double& maximum, double& minimum, double resolution);

	/** @brief Correct height/width based on their maximum value calculated from angle specification
		@param size InputOutput size value (height/width)
		@param maximum Input maximum value
		@param minimum Input minimum value
		@param resolution Input resolution value
    **/
	void sizeCorrection(int& size, const double maximum, const double minimum, const double resolution);

	/** @brief Project a point into image plane.
		@param elevation Input elevation value
		@param azimuth Input azimuth value
		@param pixel_u Output pixel element u
		@param pixel_v Output pixel element v
    **/
	void normalize(const double elevation, const double azimuth, int* pixel_u, int* pixel_v);

};

#endif // _SphericalProjection_H
