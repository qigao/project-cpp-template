#ifndef _CSV_READER_H_
#define _CSV_READER_H_

#include <iostream>
#include <string>
#include <cstring>
#include <sstream>
#include <algorithm>
#include <memory>
#include <vector>

namespace csv {

  namespace error {

    struct base : std::exception {
      virtual void formatErrorMessage() const = 0;

      const char* what() const throw() {
        this->formatErrorMessage();
        return error_message.c_str();
      }

      mutable std::string error_message;
    };

    struct with_field_num {
      with_field_num(size_t field_num): field_num(field_num) {}
      size_t field_num;
    };

    struct with_record_num {
      with_record_num(size_t record_num): record_num(record_num) {}
      size_t record_num;
    };

    struct with_file_name {
      with_file_name(std::string file_name): file_name(file_name) {}
      std::string file_name;
    };

    // ERRORS

    struct record_parsing :
      base,
      with_field_num {

      record_parsing(size_t field_num): with_field_num(field_num) {}

      virtual void formatErrorMessage() const {
        std::ostringstream stream;
        stream << "Parsing error in field " << this->field_num;
        this->error_message = stream.str();
      }
    };

    struct file_parsing :
      base,
      with_field_num,
      with_record_num,
      with_file_name {

      file_parsing(const std::string &file_name, size_t record_num, size_t field_num):
        with_field_num(field_num),
        with_record_num(record_num),
        with_file_name(file_name) {}

      virtual void formatErrorMessage() const {
        std::ostringstream stream;
        stream << "Parsing error in file '" << this->file_name << "', record " << \
          this->record_num << ", field " << this->field_num;
        this->error_message = stream.str();
      }
    };

  }

  //
  // RECORD PARSER
  //

  %%{
    machine csv_record_parser;

    action reset_val {
      ts = p;
      te = p;
    }

    action inc_val {
      te++;
    }

    action finish_field {
      fields.push_back(std::string(ts, te-ts));
      current_field++;
    }

    action finish_field_with_la {
      // some little bit of lookahead
      if(*(fpc + 1) != '"') {
        fields.push_back(std::string(ts, te-ts));
        current_field++;
      }
    }

    action finish_record {
      finished = true;
    }

    action inc {
      chars_read++;
    }

    soft_space = space - '\n'; # all except "\n"

    quoted_value = ((any - '"') | '""')* >reset_val $inc_val %finish_field_with_la $inc;
    quoted_field = ('"' quoted_value '"') $inc;
    unquoted_value = (any - '\n' - '"' - ',')*  >reset_val $inc_val %finish_field $inc;
    unquoted_field = unquoted_value;
    field = (soft_space* <: (quoted_field | unquoted_field) :> soft_space*) $inc;
    main := (field (',' field)* '\n') @finish_record $inc;
  }%%

  %% write data;

  class RecordParser {
    public:
      RecordParser() {}

      // returns current position in buffer...
      size_t parse(const char *begin, const char *end, std::vector<std::string> &fields) const {
        // declare values
        const char *p, *pe;
        const char *ts, *te;
        int cs;

        // initialize all values
        %% write init;

        p = begin;
        pe = end;
        bool finished = false;
        size_t current_field = 0, chars_read = 0;

        %% write exec;

        if(!finished) {
          throw error::record_parsing(current_field);
        } else {
          this->_cleanFields(fields);
        }

        return chars_read;
      }

    private:

      void _cleanFields(std::vector<std::string> &fields) const {
        for(auto &field : fields) {
          // remove end " if exists
          if(field.back() == '"') {
            field.erase(field.end() - 1);

            // replace "" for " only in quoted fields
            // TODO: there is maybe a better way to do that...
            size_t index = 0;
            while(true) {
              index = field.find("\"\"", index);
              if(index == std::string::npos) break;
              field.replace(index, 2, "\"");
              index += 2;
            }
          }
        }
      }
  };


  //
  // FILE PARSER
  //

  namespace input {
    class Reader {
      public:
        Reader() {}
        virtual ~Reader() {}

        virtual size_t read(char *buffer, size_t size) = 0;
        virtual const std::string name() const = 0;
        virtual bool valid() const = 0;
        virtual bool end() const = 0;
    };

    class StreamReader : public Reader {
      public:
        StreamReader(std::istream &stream): stream(stream) {}
        virtual ~StreamReader() {}

        virtual size_t read(char *buffer, size_t size) {
          this->stream.read(buffer, size);
          return this->stream.gcount();
        }

        virtual bool valid() const {
          return this->stream.good();
        }

        virtual bool end() const {
          return this->stream.eof();
        }

        virtual const std::string name() const {
          return "stream";
        }

      private:
        std::istream &stream;
    };
  }

  template
  <
    size_t BLOCK_SIZE = 1 << 24
  >
  class Reader {

    public:
      Reader(std::unique_ptr<input::Reader> reader):
        reader(std::move(reader))
      {
        this->_initializeBuffer();
      }

      void readLine(std::vector<std::string> &row) {
        if(!this->valid()) return;

        // move buffer and read more stuff
        this->_updateBuffer();
        this->current_record++;

        // parse line
        try {
          size_t read_chars = this->record_parser.parse(this->buffer.get() + this->current_pos, \
            this->buffer.get() + this->buffer_size, row);
          this->current_pos += read_chars;
        } catch(const error::record_parsing& e) {
          this->invalid_due_to_parse_error = true;
          throw error::file_parsing(this->reader->name(), this->current_record, e.field_num);
        }
      }

      bool valid() {
        return !this->invalid_due_to_parse_error && (this->reader->valid() || (this->current_pos < this->buffer_size));
      }

      bool end() {
        return this->reader->end() && (this->current_pos >= this->buffer_size);
      }

    private:

      void _initializeBuffer() {
        this->buffer = std::unique_ptr<char[]>(new char[3*BLOCK_SIZE]);

        this->current_pos = 0;
        this->buffer_size = this->reader->read(this->buffer.get(), 2*BLOCK_SIZE);
        this->_fixLastLine();
      }

      void _updateBuffer() {
        if(this->current_pos >= BLOCK_SIZE) {
          // move buffer to the beginning
          std::memcpy(this->buffer.get(), this->buffer.get() + BLOCK_SIZE, BLOCK_SIZE);
          this->current_pos -= BLOCK_SIZE;
          this->buffer_size -= BLOCK_SIZE;

          // try to read another block
          if(this->reader->valid()) {
            this->buffer_size += this->reader->read(this->buffer.get() + BLOCK_SIZE, BLOCK_SIZE);
          }

          this->_fixLastLine();
        }
      }

      void _fixLastLine() {
        // force a \n in the last position, if it's not there...

        if(!this->reader->valid() && this->buffer_size) {

          // remove end character... not needed...
          this->buffer_size--;

          // add \n
          if(this->buffer_size < 2*BLOCK_SIZE && \
            *(this->buffer.get() + this->buffer_size - 1) != '\n') {
            *(this->buffer.get() + this->buffer_size) = '\n';
            this->buffer_size++;
          }

        }
      }

      std::unique_ptr<input::Reader> reader;
      RecordParser record_parser;

      // buffer and buffer pointers
      std::unique_ptr<char[]> buffer;
      size_t current_pos;
      size_t buffer_size;
      size_t current_record = 0;

      bool invalid_due_to_parse_error = false;
  };

}

#endif
