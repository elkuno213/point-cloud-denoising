#pragma once

// Standard
#include <iostream>
#include <vector>
// 3rd-parties
#include "rapidcsv.h"

class CSVReader {
public:
  CSVReader();
  ~CSVReader();

public:
  void set_parameters(double _unit, int _column_name_index, int _row_name_index);
  void read_inputs(const std::string _data_file);
  void process_data();
  void write_outputs(std::vector<std::vector<double>>& _data);

private:
  // Inputs
  std::string data_file_; // Path to data file

  // Outputs
  std::vector<std::vector<double>> data_; // Output data 2D vector

  // Data unit coefficient
  double unit_ = 1.;
  // Zero-based row index of the column labels (-1 gives access to all rows as
  // document data)
  int column_name_index_ = -1;
  // Zero-based column index of the row labels (-1 gives access to all columns
  // as document data)
  int row_name_index_ = 0;
};
