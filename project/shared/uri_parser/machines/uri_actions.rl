
#include "uri_func.h"
#include "uri_parser.h"
#include <iostream>
#include  <string>
/*
 * capitalizes all lower-case ASCII characters,
 * converts dashes to underscores.
 */
static void snake_upcase_char(char *c)
{
    if (*c >= 'a' && *c <= 'z') {
        *c &= ~0x20;
    } else if (*c == '-') {
        *c = '_';
    }
}


%%{
    machine actions;

    action mark_start {
        mark = fpc-buffer;
    }

    action fetch_scheme{
       std::string value = std::string(buffer+mark,(size_t)(fpc-buffer-mark));
       named_values.insert({"schema",value});
    }

    action fetch_host{
      std::string value = std::string(buffer+mark,(size_t)(fpc-buffer-mark));
      named_values.insert({"host",value});
    }

    action fetch_userinfo{
        std::string value = std::string(buffer+mark,(size_t)(fpc-buffer-mark));
        named_values.insert({"userinfo",value});
    }

    action fetch_port{
      std::string value = std::string(buffer+mark,(size_t)(fpc-buffer-mark));
      named_values.insert({"port",value});
    }

    action fetch_path{
      std::string value = std::string(buffer+mark,(size_t)(fpc-buffer-mark));
      named_values.insert({"path",value});
    }

    action fetch_query{
        std::string value = std::string(buffer+mark,(size_t)(fpc-buffer-mark));
        named_values.insert({"query",value});
    }

    action fetch_fragment{
      std::string value = std::string(buffer+mark,(size_t)(fpc-buffer-mark));
      named_values.insert({"frag",value});
    }

    action fetch_sip_uri_params{
      std::string value = std::string(buffer+mark,(size_t)(fpc-buffer-mark));
      named_values.insert({"params",value});
    }
    action fetch_sip_uri_header{
      std::string value = std::string(buffer+mark,(size_t)(fpc-buffer-mark));
      named_values.insert({"sip_header",value});
    }
}%%
