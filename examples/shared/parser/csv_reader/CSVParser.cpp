
#include "CSVParser.hpp"

#include "csv_stream_parser.h"
#include "input_reader.h"

#include <fstream>
#include <iostream>
#include <memory>
namespace Parser
{
CSVParser::CSVParser() {}
void CSVParser::parse(char const* csv_file_name,
                      std::vector<std::string> results)
{
    auto file_stream = std::make_unique<std::ifstream>(
        csv_file_name, std::ios_base::in | std::ios_base::binary);
    auto reader =
        CSVStreamParser<>(std::make_unique<StreamReader>(*file_stream));
    reader.readLine(results);
}
void CSVParser::parse_by_line(char const* csv_data,
                              std::vector<std::string> results)
{
    auto file_stream = std::make_unique<std::ifstream>(
        csv_data, std::ios_base::in | std::ios_base::binary);
    auto reader =
        CSVStreamParser<>(std::make_unique<StreamReader>(*file_stream));
    reader.readLine(results);
}
} // namespace Parser
