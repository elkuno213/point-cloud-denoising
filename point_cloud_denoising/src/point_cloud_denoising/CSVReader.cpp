/**
 * @name TEMPLATE FOR AUTONOMOUS DRIVING COMPONENTS
 * @copyright Gaussin Manugistique S.A. (c)
 * @author Vu-Hoi HUYNH
 * @brief CSV reader class
 * @version 1.0
 * @date 02/06/2021
 * @comment
 */

#include "CSVReader.hpp"

/******************************************************************************************************************************************************
CONSTRUCTORS & DESTRUCTORS
******************************************************************************************************************************************************/
// Constructor
CSVReader::CSVReader() {
	this->unit 				= 1.0;
	this->columnNameIdx 	= -1;
	this->rowNameIdx 		= 0;
}

// Destructor
CSVReader::~CSVReader() {
}

/******************************************************************************************************************************************************
PUBLIC METHODS
******************************************************************************************************************************************************/
/** @brief Set parameters for object.
	@param _unit Unit coefficient to scale data
	@param _columnNameIdx Zero-based row index of the column labels (-1 gives access to all rows as document data)
	@param _rowNameIdx Zero-based column index of the row labels (-1 gives access to all columns as document data)
**/
void CSVReader::setParameters(double _unit, int _columnNameIdx, int _rowNameIdx) {
	this->unit 				= _unit;
	this->columnNameIdx 	= _columnNameIdx;
	this->rowNameIdx 		= _rowNameIdx;
}

/** @brief Read input for object.
	@param _iPath Input path to data file
**/
void CSVReader::readInputs(const std::string _iPath) {
	// Receives the inputs from the MAPS wrapper and saves them on the class
	this->iPath 		= _iPath;
}

/** @brief Process data for object.
**/
void CSVReader::processData() {
	rapidcsv::Document CSVDoc(this->iPath, rapidcsv::LabelParams(this->columnNameIdx, this->rowNameIdx), rapidcsv::SeparatorParams(';'));
	for (std::size_t i {0}; i < CSVDoc.GetRowCount(); ++i) {
		std::vector<double> row = CSVDoc.GetRow<double>(i);
		if (this->unit != 1.0)
			std::transform(row.begin(), row.end(), row.begin(), [this](double& element){ return element * unit; });
		this->oData.push_back(row);
    }
}

/** @brief Write out the processed data.
	@param _oData Input processed data
**/
void CSVReader::writeOutputs(std::vector<std::vector<double>>& _oData) {
	// Pass the values of scores to the MAPS wrapper
	_oData = this->oData;
}

/******************************************************************************************************************************************************
PRIVATE METHODS
******************************************************************************************************************************************************/
