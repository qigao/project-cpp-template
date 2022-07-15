#ifndef URI_PARSER_H
#define URI_PARSER_H
#include "URIParser_export.h"
#include "uri_values.h"
#include <cassert>
#include <map>
#include <memory>
#include <string>

namespace Parser {

class URIPARSER_EXPORT URIParser
{
private:
  int       cs;
  size_t    nread;
  ptrdiff_t mark;

public:
  URIParser();

  /**
   * @brief parse the given data
   *
   * @param buffer
   * @param len
   * @param off
   * @param values
   * @return size_t
   */
  size_t execute(char *buffer, size_t len, size_t off, NamedValues &named_values);

  /**
   * @brief parse error occured
   *
   * @return true got error
   * @return false no error found
   */
  bool has_error();

  /**
   * @brief check if parse finished
   *
   * @return true finished
   * @return false not finished
   */
  bool is_finished();

  /**
   * @brief parse finished
   *
   * @return int  -1: has error, 1: finished; 0: not finished
   */
  int finished()
  {
    if (has_error()) {
      return -1;
    } else if (is_finished()) {
      return 1;
    } else {
      return 0;
    }
  }
};
}// namespace Parser
#endif /* URI_PARSER_H */
