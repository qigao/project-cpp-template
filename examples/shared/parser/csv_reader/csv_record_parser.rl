#ifndef __CSV_RECORD_PARSER_H__
#define __CSV_RECORD_PARSER_H__

#include "error.h"
#include <algorithm>
#include <cstring>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace Parser {

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
          throw record_parsing_error(current_field);
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
}

#endif // __CSV_RECORD_PARSER_H__
