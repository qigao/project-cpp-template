#ifndef __HTTP_FILE_TRANSFER_H__
#define __HTTP_FILE_TRANSFER_H__


#if defined(_MSC_VER)
    #if defined(MYLIB_EXPORTS)
        #define MVIEW_API __declspec(dllexport)
    #else
        #define MVIEW_API __declspec(dllimport)
    #endif
#else
    #define MVIEW_API __attribute__((visibility("default")))
#endif
#ifdef __cplusplus
extern "C" {
#endif
    typedef struct http_handle http_client_handle;

    MVIEW_API http_client_handle* new_http_client(char const* host, int port);

    MVIEW_API void free_client_handle(http_client_handle* handle);
    MVIEW_API void setup_http_header(http_client_handle* handle,
                                     char const* header_name,
                                     char const* header_value);
    MVIEW_API
    MVIEW_API void read_config_from_yml(char const* file_path);
    MVIEW_API void http_request_initialize(http_client_handle* handle);
    MVIEW_API void set_auth_token(http_client_handle* handle, char const* token);
 /**
 * Calculates the sum of two integers.
 *
 * @param pLhs The first integer to be summed.
 * @param pRhs The second integer to be summed.
 *
 * @return The sum of the two integers.
 *
 * @throws None
 */
 MVIEW_API int Sum(const int pLhs, const int pRhs);
   /**
 * Downloads a file synchronously from the specified URL and saves it locally.
 *
 * @param url The URL of the file to download.
 * @param local_filename The local filename to save the downloaded file.
 */
 MVIEW_API void sync_file_download(const char* url,const char* local_filename);
   /**
 * Downloads a file asynchronously from the specified URL and saves it locally.
 *
 * @param url The URL of the file to download.
 * @param local_filename The local filename to save the downloaded file.
 *
 * @throws None
 */
 MVIEW_API void async_file_download(const char* url,const char* local_filename);

#ifdef __cplusplus
}
#endif //__cplusplus


#endif // __HTTP_FILE_TRANSFER_H__
