#include "URIParser.h"
#include "uri_values.h"
#include <string>
namespace Parser {
%%{
  machine m_uri;
  include actions "machines/uri_actions.rl";
  include ip_addr "machines/ip_addr.rl";
  include uri "machines/uri.rl";
  include sip_uri "machines/sip_uri.rl";

  main :=  SIP_URI | URI_REF;

  write data;
}%%

  /* -------------------------------------------------------------------------- */
  /*                         class URIParser implement                         */
  /* -------------------------------------------------------------------------- */

URIParser::URIParser() : cs(0),nread(0), mark(0)
{
  %% write init;
}
bool URIParser::is_finished() { return cs == m_uri_first_final; }

bool URIParser::has_error() { return cs == m_uri_error; }

size_t URIParser::execute(char *buffer, size_t len, size_t off, NamedValues &named_values)
{
  assert(off <= len && "offset past end of buffer");

  char *p   = buffer + off;
  char *pe  = buffer + len;
  char *eof = pe;

  assert(pe - p == len - off && "pointers aren't same distance");

  %% write exec;
  assert(p <= pe && "buffer overflow after parsing.");

  nread += p - buffer + off;

  assert(nread <= len && "nread longer than length");
  assert(mark < len && "mark is after buffer end");
  return (nread);
}
}// namespace Parser
