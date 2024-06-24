#ifndef __CSV_STREAM_PARSER_H__
#define __CSV_STREAM_PARSER_H__

#include "csv_record_parser.h"
#include "error.h"
#include "input_reader.h"
#include <iostream>
#include <memory>
#include <vector>
namespace Parser {
  template<size_t BLOCK_SIZE = 1 << 24>
  class CSVStreamParser
  {

  public:
    CSVStreamParser(std::unique_ptr<Reader> reader) : reader(std::move(reader))
    {
      this->_initializeBuffer();
    }

    void readLine(std::vector<std::string> &row)
    {
      if (!this->valid()) return;

      // move buffer and read more stuff
      this->_updateBuffer();
      this->current_record++;

      // parse line
      try {
        size_t read_chars = this->record_parser.parse(
          this->buffer.get() + this->current_pos, this->buffer.get() + this->buffer_size, row);
        this->current_pos += read_chars;
      } catch (const record_parsing_error &e) {
        this->invalid_due_to_parse_error = true;
        throw file_parsing_error(this->reader->name(), this->current_record, e.field_num);
      }
    }

    bool valid()
    {
      return !this->invalid_due_to_parse_error
             && (this->reader->valid() || (this->current_pos < this->buffer_size));
    }

    bool end() { return this->reader->end() && (this->current_pos >= this->buffer_size); }

  private:
    void _initializeBuffer()
    {
      this->buffer = std::unique_ptr<char[]>(new char[3 * BLOCK_SIZE]);

      this->current_pos = 0;
      this->buffer_size = this->reader->read(this->buffer.get(), 2 * BLOCK_SIZE);
      this->_fixLastLine();
    }

    void _updateBuffer()
    {
      if (this->current_pos >= BLOCK_SIZE) {
        // move buffer to the beginning
        std::memcpy(this->buffer.get(), this->buffer.get() + BLOCK_SIZE, BLOCK_SIZE);
        this->current_pos -= BLOCK_SIZE;
        this->buffer_size -= BLOCK_SIZE;

        // try to read another block
        if (this->reader->valid()) {
          this->buffer_size += this->reader->read(this->buffer.get() + BLOCK_SIZE, BLOCK_SIZE);
        }

        this->_fixLastLine();
      }
    }

    void _fixLastLine()
    {
      // force a \n in the last position, if it's not there...

      if (!this->reader->valid() && this->buffer_size) {

        // remove end character... not needed...
        this->buffer_size--;

        // add \n
        if (this->buffer_size < 2 * BLOCK_SIZE
            && *(this->buffer.get() + this->buffer_size - 1) != '\n') {
          *(this->buffer.get() + this->buffer_size) = '\n';
          this->buffer_size++;
        }
      }
    }

    std::unique_ptr<Reader> reader;
    RecordParser            record_parser;

    // buffer and buffer pointers
    std::unique_ptr<char[]> buffer;
    size_t                  current_pos;
    size_t                  buffer_size;
    size_t                  current_record = 0;

    bool invalid_due_to_parse_error = false;
  };
}// namespace Parser

#endif// __CSV_STREAM_PARSER_H__
