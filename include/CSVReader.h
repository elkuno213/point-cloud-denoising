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
#include "rapidcsv.h"

#ifndef _CSVReader_H
#define _CSVReader_H

class CSVReader {

private:
	// Inputs
	std::string iPath;								// Path to data file

	// Outputs
	std::vector<std::vector<double>> oData;			// Output data 2D vector
	
	// Parameters
	double unit;									// Data unit coefficient
	int columnNameIdx;						// Zero-based row index of the column labels (-1 gives access to all rows as document data)
	int rowNameIdx;							// Zero-based column index of the row labels (-1 gives access to all columns as document data)

// Constructors & Destructors
public:
	// Constructor
	CSVReader();

	// Destructor
	~CSVReader();

// Public methods
public:
	/** @brief Set parameters for object.
    	@param _unit Unit coefficient to scale data
    	@param _columnNameIdx Zero-based row index of the column labels (-1 gives access to all rows as document data)
    	@param _rowNameIdx Zero-based column index of the row labels (-1 gives access to all columns as document data)
    **/
	void setParameters(double _unit, int _columnNameIdx, int _rowNameIdx);

	/** @brief Read input for object.
		@param _iPath Input path to data file
    **/
	void readInputs(const std::string _iPath);

	/** @brief Process data for object.
    **/
	void processData();

	/** @brief Write out the processed data.
    	@param _oData Input processed data
    **/
	void writeOutputs(std::vector<std::vector<double>>& _oData);

// Private methods
private:

};

#endif // _CSVReader_H
