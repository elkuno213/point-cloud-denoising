#include "CSVReader.hpp"

CSVReader::CSVReader() {}

CSVReader::~CSVReader() {}

void CSVReader::set_parameters(
  double _unit, int _column_name_index, int _row_name_index
) {
  unit_              = _unit;
  column_name_index_ = _column_name_index;
  row_name_index_    = _row_name_index;
}

void CSVReader::read_inputs(const std::string _data_file) {
  data_file_ = _data_file;
}

void CSVReader::process_data() {
  rapidcsv::Document CSVDoc(
    data_file_,
    rapidcsv::LabelParams(column_name_index_, row_name_index_),
    rapidcsv::SeparatorParams(';')
  );
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
    data_.push_back(row);
  }
}

void CSVReader::write_outputs(std::vector<std::vector<double>>& _data) {
  _data = data_;
}
