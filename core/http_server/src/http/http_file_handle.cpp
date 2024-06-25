#include "http/http_file_handle.hpp"
#include "constants.hpp"
#include "cpp_yyjson.hpp"
#include "fs.hpp"
#include "spdlog/spdlog.h"
#include <algorithm>
#include <cstdio>
#include <filesystem>
#include <fmt/core.h>
#include <fstream>
#include <httplib.h>
#include <map>
#include <sstream>
#include <vector>
namespace fs = std::filesystem;

HttpFileHandle::HttpFileHandle(std::string const& shared_folder,
                               bool delete_after_download)
    : shared_folder_(shared_folder),
      delete_after_download_(delete_after_download)
{
}
/**
 * Handles the upload of a file through an HTML form.
 *
 * @param req the HTTP request object
 * @param res the HTTP response object
 *
 * @throws None
 */
void HttpFileHandle::list_upload_form(httplib::Request const& /* req */,
                                      httplib::Response& res)
{
    auto html = R"(
                    <!DOCTYPE html>
                    <body>
                    <form id="formElem">
                         <input type="file" name="image_file" accept="image/*"><br>
                         <input type="submit">
                    </form>
                        <script>
                            formElem.onsubmit = async (e) => {
                            e.preventDefault();
                            let res = await fetch('/upload', {
                                method: 'POST',
                                body: new FormData(formElem)
                            });
                            console.log(await res.text());
                        };
                        </script>
                        </body>
                        </html>
                   )";
    res.set_content(html, "text/html");
}

void HttpFileHandle::handle_file_download(httplib::Request const& req,
                                          httplib::Response& res)
{

    // req: /list/a.txt ---> Download/a.txt
    fs::path path(req.path);
    auto file_name =
        fmt::format("{}/{}", shared_folder_, path.filename().string());
    spdlog::info("trying to download: {} from: {}", path.filename().string(),
                 shared_folder_);
    // 没有请求的文件,返回404
    if (!fs::exists(file_name))
    {
        spdlog::error("file not found: {}", file_name);
        res.status = 404;
        return;
    }
    // 请求的是目录,返回403
    if (fs::is_directory(file_name))
    {
        spdlog::error("file is directory: {}", file_name);
        res.status = 403;
        return;
    }
    auto range_header = req.get_header_value("Range");
    if (range_header.empty())
    {
        spdlog::error("Range header not found");
        download_file_by_order(req, res);
        return;
    }
    int64_t start, len;
    auto range = parse_range(range_header, start, len);
    if (!range)
    {
        spdlog::error("Range parse error");
        res.status = 416;
        return;
    }
    // TODO:: read file by multithread
    //  file读入文件
    std::ifstream file(file_name, std::ios::binary);
    if (!file.is_open())
    {
        spdlog::error("file open error: {}", file_name);
        res.status = 500;
        return;
    }
    // 跳转到下载位置,写入rsp
    file.seekg(start, std::ios::beg);
    res.body.resize(len);
    file.read(res.body.data(), len);
    // 文件出错
    if (!file.good())
    {
        spdlog::error("file read error: {}", file_name);
        res.status = 500;
        return;
    }
    auto file_size = fs::file_size(file_name);
    file.close();

    res.status = 206;

    res.set_header(CACHE_CONTROL, "no-cache");
    res.set_header(CONTENT_TYPE, OCTET_STREAM);
    res.set_header(CONTENT_RANGE, std::to_string(start) + "-" +
                                      std::to_string(start + len) + "/" +
                                      std::to_string(file_size));
    res.set_header(CONTENT_LENGTH, std::to_string(len));
}

bool HttpFileHandle::parse_range(std::string& range, int64_t& start,
                                 int64_t& len)
{
    // 检查格式
    auto pos1 = range.find('=');
    auto pos2 = range.find('-');
    if (pos1 == std::string::npos || pos2 == std::string::npos)
    {
        spdlog::error("range format error");
        return false;
    }

    // 解析start,end
    int64_t end;
    auto sstart = range.substr(pos1 + 1, pos2 - pos1 - 1);
    auto send = range.substr(pos2 + 1);

    std::istringstream(sstart) >> start;
    std::istringstream(send) >> end;
    len = end - start + 1;

    return true;
}

/**
 * download file by byte order in binary form
 * @param file_name
 * @param res
 */
void HttpFileHandle::download_file_by_order(httplib::Request const& req,
                                            httplib::Response& res)
{
    res.set_content_provider(
        OCTET_STREAM, // Content type
        [&](size_t offset, httplib::DataSink& sink)
        {
            fs::path file_path(req.path);
            auto file_name = fmt::format("{}/{}", shared_folder_,
                                         file_path.filename().string());
            // open file
            std::ifstream file_reader(file_name, std::ios::in);
            // can't open file, cancel process
            if (!file_reader.good())
            {
                res.status = 404;
                res.set_content(R"({"message":"404 not found"})", APP_JSON);
                return false;
            }

            // get file size
            file_reader.seekg(0, file_reader.end);
            size_t file_size = static_cast<size_t>(file_reader.tellg());
            file_reader.seekg(0, file_reader.beg);

            // check offset and file size, cancel process if offset >=
            // file_size
            if (offset >= file_size)
            {
                return false;
            }

            // prepare read size of chunk
            size_t read_size = 0;
            if (file_size - offset > CHUNK_SIZE)
            {
                read_size = CHUNK_SIZE;
            }
            else
            {
                read_size = file_size - offset;
            }

            // allocate temp buffer, and read file chunk into buffer
            std::vector<char> buffer;
            buffer.reserve(CHUNK_SIZE);
            file_reader.seekg(static_cast<long>(offset), file_reader.beg);
            file_reader.read(&buffer[0], static_cast<long>(read_size));
            file_reader.close();

            // write buffer to sink
            sink.write(&buffer[0], read_size);
            return true; // return 'false' if you want to cancel the
                         // process.
        });
}
void HttpFileHandle::upload_file_by_multiform(httplib::Request const& req,
                                              httplib::Response& res)
{
    for (auto const& data : req.files)
    {
        auto file = data.second;
        spdlog::info("upload file: {},type:{},name: {},length: {}", file.name,
                     file.content_type, file.filename, file.content.length());
        auto file_path = fmt::format("{}/{}", shared_folder_, file.filename);
        if (file.content_type == TEXT_PLAIN)
        {
            std::ofstream ofs(file_path);

            ofs << file.content;
        }
        else
        {
            std::ofstream ofs(file_path, std::ios::binary);

            ofs << file.content;
        }
    }
    res.set_content("done", TEXT_PLAIN);
}

void HttpFileHandle::handle_file_lists(httplib::Request const& /* req */,
                                       httplib::Response& res)
{
    auto kv_list = std::vector<std::map<std::string, std::string>>();
    fs::directory_iterator item_begin(shared_folder_);
    fs::directory_iterator item_end;
    for (; item_begin != item_end; ++item_begin)
    {
        // 文件夹跳过
        if (fs::is_directory(*item_begin))
        {
            continue;
        }

        // 文件则放入body中,用\n分割标记
        auto pathname = item_begin->path();
        auto path = pathname.filename().string();
        auto result = std::map<std::string, std::string>();
        result.emplace("path", path);
        auto str_size = fmt::format("{}", get_file_size(pathname));
        result.emplace("size", str_size);
        kv_list.push_back(result);
    }
    // 设置body格式--文本文件 和状态码
    auto body = yyjson::array(kv_list).write();
    res.set_content(body.data(), APP_JSON);
    res.status = 200;
}

void HttpFileHandle::handle_stream_file(
    std::string const& file_name, httplib::ContentReader const& content_reader)
{
    auto file_path_str = fmt::format("{}/{}", shared_folder_, file_name);
    spdlog::info("upload file by stream: {}", file_path_str);
    std::string body;
    content_reader(
        [&](char const* data, size_t data_length)
        {
            body.append(data, data_length);
            write(file_path_str, data, data_length);
            return true;
        });
    spdlog::info("upload file by stream: {}", body);
}

void HttpFileHandle::handle_multipart_file(
    httplib::ContentReader const& content_reader)
{
    httplib::MultipartFormDataItems files;
    content_reader(
        [&](httplib::MultipartFormData const& file)
        {
            files.push_back(file);
            spdlog::info("upload file: {},type:{},name: {}", file.filename,
                         file.content_type, file.filename);
            return true;
        },
        [&](char const* data, size_t data_length)
        {
            auto last_file = files.back();
            auto file_path =
                fmt::format("{}/{}", shared_folder_, last_file.filename);

            auto contentType = last_file.content_type;
            write(file_path, data, data_length);
            spdlog::info("upload file: {},type:{},size: {}", last_file.filename,
                         contentType, last_file.content.size());
            return true;
        });
}

void HttpFileHandle::handle_file_upload(
    httplib::Request const& req, httplib::Response& res,
    httplib::ContentReader const& content_reader)
{
    if (req.is_multipart_form_data())
    {
        spdlog::info("upload file by multipart");
        handle_multipart_file(content_reader);
        res.set_content(R"({"message":"upload result"})", APP_JSON);
    }
    else
    {
        spdlog::info("upload file by stream");
        if (req.has_header(CONTENT_DISPOSITION))
        {
            std::string content_disposition =
                req.get_header_value(CONTENT_DISPOSITION);

            // Extract the filename from the Content-Disposition header
            std::string filename;
            size_t pos = content_disposition.find("filename=");
            if (pos != std::string::npos)
            {
                filename = content_disposition.substr(
                    pos + 9, content_disposition.length());
                // Remove any surrounding quotes
                if (filename.front() == '\"' && filename.back() == '\"')
                {
                    filename = filename.substr(1, filename.size() - 2);
                }
                spdlog::info("filename: {}", filename);
            }
            handle_stream_file(filename, content_reader);
            res.set_content(R"({"message":"upload result"})", APP_JSON);
        }
        else
        {
            spdlog::error("Content-Disposition header not found");
            res.status = 400;
            res.set_content(
                R"({"message":"Missing Content-Disposition header"})",
                APP_JSON);
        }
    }
}
