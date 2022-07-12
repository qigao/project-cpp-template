#include "defines.h"
#include <gtest/gtest.h>
#include <string>
TEST(ParserPerformance, ragel_dump)
{
  http_parser_settings settings_dump = { on_message_begin_dump,
    on_url_dump,
    on_status_dump,
    on_header_field_dump,
    on_header_value_dump,
    on_headers_complete_dump,
    on_body_dump,
    on_message_complete_dump };

  http_parser parser;
  http_parser_init(&parser, HTTP_REQUEST);

  ASSERT_EQ(kMessage.size(),
    http_parser_execute(
      &parser, &settings_dump, kMessage.c_str(), kMessage.size()));
  printf("method: %s\n", http_method_str((http_method)parser.method));
}