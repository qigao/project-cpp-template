#ifndef __HTTP_FILE_TRANSFER_H__
#define __HTTP_FILE_TRANSFER_H__
#include "http_client_export.h"

 #ifdef __cplusplus
extern "C" {
#endif
typedef struct http_handle_t* http_client_handle;
/**
 * @brief Creates a new HTTP client handle using the specified filename.
 *
 * @param filename The name of the file to be used by the HTTP client.
 *
 **/
HTTP_CLIENT_API http_client_handle new_http_client(char const* filename);
/**
 * @brief Frees the client handle.
 *
 * @param handle The client handle to be freed.
 *
 **/
HTTP_CLIENT_API void free_client_handle(http_client_handle handle);
/**
 * Set up an HTTP header with the specified name and value for the given HTTP client handle.
 *
 * @param handle The HTTP client handle to set the header for
 * @param header_name The name of the header to set
 * @param header_value The value of the header to set
 */
HTTP_CLIENT_API void setup_http_header(http_client_handle handle,
                                          char const* header_name,
                                          char const* header_value);
 /**
 * Initializes an HTTP request using the provided HTTP client handle.
 *
 * @param handle The handle to the HTTP client for which the request is initialized.
 */
HTTP_CLIENT_API void http_request_initialize(http_client_handle handle);

/**
 * @brief Calculates the sum of two integers.
 *
 * @param pLhs The left-hand side integer.
 * @param pRhs The right-hand side integer.
 * @return The sum of pLhs and pRhs.
 */
HTTP_CLIENT_API int Sum(int const pLhs, int const pRhs);
/**
 * Downloads a file from the specified URL to the local file system.
 *
 * @param handle The handle to the HTTP client.
 * @param url The URL of the file to download.
 * @param path The path where the file will be saved locally.
 * @param local_file The name of the local file to be saved as.
 * @return An integer indicating the status of the file download operation.
 */
HTTP_CLIENT_API int file_download(http_client_handle handle,
                                      char const* url, char const* path,
                                      char const* local_file);
/**
 * Sends a POST request with JSON data to the specified URL using the HTTP client handle.
 *
 * @param handle The handle to the HTTP client.
 * @param url The URL to which the POST request will be sent.
 * @param json The JSON data to be included in the POST request.
 * @return An integer representing the status of the POST request.
 */
HTTP_CLIENT_API int post_json_request(http_client_handle handle,
                                         char const* url, char const* json);
/**
 * Sends a POST request with JSON data to the specified URL using the provided HTTP client handle.
 *
 * @param handle The HTTP client handle to use for the request.
 * @param url The URL to which the POST request will be sent.
 * @param json The JSON data to include in the request body.
 * @param resp_body A pointer to a buffer where the response body will be stored.
 * @return An integer representing the status of the POST request.
 */
HTTP_CLIENT_API int post_json_with_resp(http_client_handle handle,
                                         char const* url, char const* json,
                                         char* resp_body);
/**
 * @brief post a file request using the HTTP client.
 *
 * @param handle The handle to the HTTP client.
 * @param url The URL to which the file request will be posted.
 * @param filename The name of the file to be included in the request.
 * @return An integer indicating the status of the post request.
 */
HTTP_CLIENT_API int post_file_request(http_client_handle handle,
                                         char const* url, char const* filename);
/**
 * APIed function to post a file stream to a specified URL using the HTTP client handle.
 *
 * @param handle The HTTP client handle to use for the operation.
 * @param url The URL to which the file stream will be posted.
 * @param data Pointer to the data representing the file stream.
 * @param data_len The length of the data to be posted.
 * @return An integer indicating the status of the post operation.
 */
HTTP_CLIENT_API int post_file_stream(http_client_handle handle,
                                        char const* url,
                                        char const* data,long data_len);
#ifdef __cplusplus
}
#endif
#endif // __HTTP_FILE_TRANSFER_H__
