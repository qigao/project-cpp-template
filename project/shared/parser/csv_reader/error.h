#ifndef __ERROR_H__
#define __ERROR_H__

#include <sstream>
#include <utility>
namespace Parser {

  struct base : std::exception
  {
    virtual void formatErrorMessage() const = 0;

    const char *what() const throw()
    {
      this->formatErrorMessage();
      return error_message.c_str();
    }

    mutable std::string error_message;
  };

  struct with_field_num
  {
    with_field_num(size_t field_num) : field_num(field_num) {}
    size_t field_num;
  };

  struct with_record_num
  {
    with_record_num(size_t record_num) : record_num(record_num) {}
    size_t record_num;
  };

  struct with_file_name
  {
    with_file_name(const std::string &file_name) : file_name(std::move(file_name)) {}
    std::string file_name;
  };

  // ERRORS

  struct record_parsing_error
    : base
    , with_field_num
  {

    record_parsing_error(size_t field_num) : with_field_num(field_num) {}

    virtual void formatErrorMessage() const
    {
      std::ostringstream stream;
      stream << "Parsing error in field " << this->field_num;
      this->error_message = stream.str();
    }
  };

  struct file_parsing_error
    : base
    , with_field_num
    , with_record_num
    , with_file_name
  {

    file_parsing_error(const std::string &file_name, size_t record_num, size_t field_num)
      : with_field_num(field_num), with_record_num(record_num), with_file_name(file_name)
    {}

    virtual void formatErrorMessage() const
    {
      std::ostringstream stream;
      stream << "Parsing error in file '" << this->file_name << "', record " << this->record_num
             << ", field " << this->field_num;
      this->error_message = stream.str();
    }
  };

}// namespace Parser
#endif// __ERROR_H__
