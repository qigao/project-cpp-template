#ifndef __HTTP_FILE_TRANSFER_H__
#define __HTTP_FILE_TRANSFER_H__
#include "http_client_export.h"

 #ifdef __cplusplus
extern "C" {
#endif
typedef struct http_handle_t* http_client_handle;

HTTP_CLIENT_EXPORT http_client_handle new_http_client(char const* filename);
HTTP_CLIENT_EXPORT void free_client_handle(http_client_handle handle);
HTTP_CLIENT_EXPORT void setup_http_header(http_client_handle handle,
                                          char const* header_name,
                                          char const* header_value);
HTTP_CLIENT_EXPORT void http_request_initialize(http_client_handle handle);
HTTP_CLIENT_EXPORT void set_auth_token(http_client_handle handle,
                                       char const* token);

HTTP_CLIENT_EXPORT int Sum(int const pLhs, int const pRhs);

HTTP_CLIENT_EXPORT int file_download(http_client_handle handle,
                                      char const* url, char const* path,
                                      char const* local_file);
HTTP_CLIENT_EXPORT int post_json_request(http_client_handle handle,
                                         char const* url, char const* json);
HTTP_CLIENT_EXPORT int post_json_with_resp(http_client_handle handle,
                                         char const* url, char const* json,
                                         char* resp_body);
HTTP_CLIENT_EXPORT int post_file_request(http_client_handle handle,
                                         char const* url, char const* filename);

HTTP_CLIENT_EXPORT int post_file_stream(http_client_handle handle,
                                        char const* url,
                                        char const* data,long data_len);
#ifdef __cplusplus
}
#endif
#endif // __HTTP_FILE_TRANSFER_H__
