#ifndef __HTTP_FILE_H__
#define __HTTP_FILE_H__
#include "http_lib_header.hpp"
#include <fmt/core.h>

#include <string>

class HttpFileHandle
{
public:
    explicit HttpFileHandle(std::string const& shared_folder,
                            bool delete_after_download = false);
    /**
     *  the upload html form
     * @param req
     * @param res
     */
    void list_upload_form(httplib::Request const& req, httplib::Response& res);

    void upload_file_by_multiform(httplib::Request const& req,
                                  httplib::Response& res);
    /**
     * Handles request for downloading a file.
     *   1. If there is no range header, return the whole file.
     *   2. If there is a range header, return the part of the file.
     *
     * @param req The HTTP request object.
     * @param res The HTTP response object.
     *
     * @throws None.
     */
    void handle_file_download(httplib::Request const& req,
                              httplib::Response& res);
    /**
     * Get the list of files in the shared folder
     *    @param req The HTTP request object.
     *  @param res The HTTP response object.
     *
     * @throws None.
     * */
    void handle_file_lists(httplib::Request const& req, httplib::Response& res);
    /**
     * upload a file either by multipart form or stream
     *
     * @param req The HTTP request object.
     * @param res The HTTP response object.
     *
     * @throws None
     */
    void handle_file_upload(httplib::Request const& req, httplib::Response& res,
                            httplib::ContentReader const& content_reader);

private:
    HttpFileHandle(HttpFileHandle const&) = delete;
    HttpFileHandle& operator=(HttpFileHandle const&) = delete;
    bool parse_range(std::string& range, int64_t& start, int64_t& len);
    void download_file_by_order(httplib::Request const& req,
                                httplib::Response& res);

    void handle_multipart_file(httplib::ContentReader const& content_reader);
    void handle_stream_file(std::string const& file_name,
                            httplib::ContentReader const& content_reader);
    std::string shared_folder_;
    bool delete_after_download_;
};
#endif // __HTTP_FILE_H__
