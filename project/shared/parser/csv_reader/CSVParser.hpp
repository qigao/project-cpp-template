#ifndef __CSVPARSER_H__
#define __CSVPARSER_H__

#include "CSVParser_export.h"
#include <string>
#include <vector>
namespace Parser {
  class CSVPARSER_EXPORT CSVParser
  {
  public:
    CSVParser();
    void parse(const char *csv_file_name,std::vector<std::string>);
    void parse_by_line(const char* csv_line,std::vector<std::string>);
  };

}// namespace Parser

#endif// __CSVPARSER_H__
