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
extern "C"
{
#endif
    typedef struct http_handle http_client_handle;

    MVIEW_API http_client_handle* new_http_client(char const* filename);
    MVIEW_API void free_client_handle(http_client_handle* handle);
    MVIEW_API void setup_http_header(http_client_handle* handle,
                                     char const* header_name,
                                     char const* header_value);
    MVIEW_API void http_request_initialize(http_client_handle* handle);
    MVIEW_API void set_auth_token(http_client_handle* handle,
                                  char const* token);
    /**
     * Downloads a file synchronously from the relative URL and saves it
     * locally.
     *
     * @param url The relative URL of the file to download.
     * @param local_filename The local filename to save the downloaded file.
     */
    MVIEW_API void sync_file_download(http_client_handle* handle,
                                      char const* url,
                                      char const* local_filename);
    /**
     * Downloads a file asynchronously from the relative URL and saves it
     * locally.
     *
     * @param url The relative URL of the file to download.
     * @param local_filename The local filename to save the downloaded file.
     *
     * @throws None
     */
    MVIEW_API void async_file_download(http_client_handle* handle,
                                       char const* url,
                                       char const* local_filename);
    /**
     * Uploads a file to the relative URL.
     *
     * @param url The relative URL to upload the file to.
     * @param local_filename The local filename of the file to upload.
     *
     * @throws None
     */
    MVIEW_API void file_upload(http_client_handle* handle, char const* url,
                               char const* local_filename);
    /**
     * Sends a GET request to the relative URL.
     *
     * @param url The relative URL to send the GET request to.
     *
     * @throws None
     */
    MVIEW_API void get_request(http_client_handle* handle, char const* url);
    /**
     * Sends a POST request to the relative URL with the provided JSON data.
     *
     * @param url The relative URL to send the POST request to.
     * @param json The JSON data to send in the request body.
     *
     * @throws None
     */
    MVIEW_API void post_request(http_client_handle* handle, char const* url,
                                char const* json);
    /**
     * Sends a PUT request to the relative URL with the provided JSON data.
     *
     * @param url The relative URL to send the PUT request to.
     * @param json The JSON data to send in the request body.
     *
     * @throws None
     */
    MVIEW_API void put_request(http_client_handle* handle, char const* url,
                               char const* json);
    /**
     * Sends a DELETE request to the relative URL.
     *
     * @param url The relative URL to send the DELETE request to.
     *
     * @throws None
     */
    MVIEW_API void delete_request(http_client_handle* handle, char const* url);
    /**
     * Sends a POST request to the relative URL with the provided JSON data.
     *
     * @param url The relative URL to send the POST request to.
     * @param json The JSON data to send in the request body.
     *
     * @throws None
     */
    MVIEW_API void post_json_request(http_client_handle* handle,
                                     char const* url, char const* json);
    /**
     * Sends a POST request to the relative URL with the provided form data.
     *
     * @param url The relative URL to send the POST request to.
     * @param data The form data to send in the request body.
     *
     * @throws None
     */
    MVIEW_API void post_form_request(http_client_handle* handle,
                                     char const* url, char const* data);
    /**
     * Sends a POST request to the relative URL with the provided file data.
     *
     * @param url The relative URL to send the POST request to.
     * @param filename The name of the file to send in the request body.
     *
     * @throws None
     */
    MVIEW_API void post_file_request(http_client_handle* handle,
                                     char const* url, char const* filename);


    /** Sends a POST request to the relative URL with the provided file data.
     *
     * @param url The relative URL to send the POST request to.
     * @param file_id The file ID to send in the request body.
     * @param data The file data to send in the request body.
     * @param size The size of the file data.
     *
     * @throws None
     */
    MVIEW_API void post_file_stream(http_client_handle* handle,char const* url,
                                            char const* file_id, char const* data,
                                            unsigned long size);
    /**
     * Sends a POST request to the relative URL with the provided file data.
     *
     * @param url The relative URL to send the POST request to.
     * @param filename The name of the file to send in the request body.
     *
     * @throws None
     */
    MVIEW_API void post_image_info_request(http_client_handle* handle,
                                           char const* url, char const* json);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif // __HTTP_FILE_TRANSFER_H__
