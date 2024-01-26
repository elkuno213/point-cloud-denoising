#pragma once

// Standard
#include <iostream>
#include <vector>

// Read a CSV file and output 2D data vector.
class CSVReader {
public:
  CSVReader(
    const double _unit,
    const int _column_name_index,
    const int _row_name_index
  );
  ~CSVReader();

public:
  std::vector<std::vector<double>> read(const std::string& csv_file) const;

private:
  // Data unit coefficient
  double unit_ = 1.;
  // Zero-based row index of the column labels (-1 gives access to all rows as
  // document data)
  int column_name_index_ = -1;
  // Zero-based column index of the row labels (-1 gives access to all columns
  // as document data)
  int row_name_index_ = 0;
};
