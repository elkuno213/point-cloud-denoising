// 3rd-parties
#include "rapidcsv.h"
// Local
#include "csv_reader.hpp"

CSVReader::CSVReader(
  const double _unit,
  const int _column_name_index,
  const int _row_name_index
) : unit_{_unit}
  , column_name_index_{_column_name_index}
  , row_name_index_{_row_name_index}
{}

std::vector<std::vector<double>> CSVReader::read(
  const std::string& csv_file
) const {
  // Initialize document.
  rapidcsv::Document CSVDoc(
    csv_file,
    rapidcsv::LabelParams(column_name_index_, row_name_index_),
    rapidcsv::SeparatorParams(';')
  );

  // Read 2D data.
  std::vector<std::vector<double>> data;
  for (std::size_t i = 0; i < CSVDoc.GetRowCount(); ++i) {
    std::vector<double> row = CSVDoc.GetRow<double>(i);
    if (unit_ != 1.0) {
      std::transform(
        row.begin(),
        row.end(),
        row.begin(),
        [this](double& element) { return element * unit_; }
      );
    }
    data.push_back(row);
  }

  return data;
}
