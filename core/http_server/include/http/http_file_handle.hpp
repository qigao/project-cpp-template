#ifndef __HTTP_FILE_H__
#define __HTTP_FILE_H__
#include <fmt/core.h>
#include <httplib.h>
#include <string>

class HttpFileHandle
{
public:
    HttpFileHandle(const std::string& shared_folder);
    /**
     *  the upload html form
     * @param req
     * @param res
     */
    void uploadForm(const httplib::Request& req, httplib::Response& res);

    void uploadByMultiForm(const httplib::Request& req, httplib::Response& res);
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
    void handle_range_request(const httplib::Request& req,
                              httplib::Response& res);
    /**
     * Get the list of files in the shared folder
     *    @param req The HTTP request object.
     *  @param res The HTTP response object.
     *
     * @throws None.
     * */
    void GetFileList(const httplib::Request& req, httplib::Response& res);
    /**
     * upload a file either by multipart form or stream
     *
     * @param req The HTTP request object.
     * @param res The HTTP response object.
     *
     * @throws None
     */
    void handle_file_request(const httplib::Request& req,
                             httplib::Response& res,
                             const httplib::ContentReader& content_reader);

private:
    HttpFileHandle(HttpFileHandle const&) = delete;
    HttpFileHandle& operator=(HttpFileHandle const&) = delete;
    bool RangeParse(std::string& range, int64_t& start, int64_t& len);
    void getFileByOrder(const httplib::Request& req, httplib::Response& res);

    std::string shared_folder_;
    static constexpr const size_t CHUNK_SIZE = 32 * 1024;
    void
    handle_multipart_file(const httplib::ContentReader& content_reader) const;
    void handle_stream_file(const std::string& file_path,
                            const httplib::ContentReader& content_reader) const;
};
#endif // __HTTP_FILE_H__
