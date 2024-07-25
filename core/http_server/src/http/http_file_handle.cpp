#include "http/http_file_handle.hpp"

#include "config/pch_headers.hpp"
#include "fs.hpp"
template <typename T>
T custom_min(T a, T b)
{
    return (a < b) ? a : b;
}
HttpFileHandle::HttpFileHandle(std::string const& shared_folder,
                               bool delete_after_download)
    : shared_folder_(shared_folder),
      delete_after_download_(delete_after_download)
{
    logger = Logger::GetInstance()->get();
    ensureDirectoryExists(shared_folder_);
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
    std::string req_file = req.matches[1];
    auto file_path = fmt::format("{}/{}", shared_folder_, req_file);
    logger->info("trying to download: {} from: {}", req_file, shared_folder_);
    std::ifstream file(file_path, std::ios::binary);
    if (!file)
    {
        res.status = 404;
        res.set_content("File not found", "text/plain");
        return;
    }

    res.set_header("Content-Type", "application/octet-stream");
    res.set_header("Content-Disposition",
                   "attachment; filename=\"" + file_path + "\"");

    res.set_content_provider(
        file.seekg(0, std::ios::end).tellg(), "text/plain",
        [&, file_path](size_t offset, size_t length, httplib::DataSink& sink)
        {
            std::ifstream file(file_path, std::ios::binary);
            file.seekg(offset);
            char buffer[8192];
            size_t read_length;

            while (length > 0 &&
                   (read_length =
                        file.read(buffer, custom_min(length, sizeof(buffer)))
                            .gcount()) > 0)
            {
                sink.write(buffer, read_length);
                length -= read_length;
            }

            return true;
        });

    res.status = 200;
}

bool HttpFileHandle::parse_range(std::string& range, int64_t& start,
                                 int64_t& len)
{
    std::size_t pos1 = range.find('=');
    std::size_t pos2 = range.find('-');
    if (pos1 == std::string::npos || pos2 == std::string::npos)
    {
        logger->error("range format error");
        return false;
    }

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

void HttpFileHandle::handle_file_lists(httplib::Request const& /* req */,
                                       httplib::Response& res)
{

    std::vector<std::string> files;
    for (auto const& entry : fs::directory_iterator(shared_folder_))
    {
        if (fs::is_directory(entry))
        {
            continue;
        }
        auto pathname = entry.path();
        files.push_back(pathname.string());
    }
    std::string result = fmt::format("{}", fmt::join(files, ""));

    res.set_content(result, "text/plain");
    res.status = 200;
}

void HttpFileHandle::handle_multipart_file(
    httplib::ContentReader const& content_reader)
{
    httplib::MultipartFormDataItems files;
    content_reader(
        [&](httplib::MultipartFormData const& file)
        {
            files.push_back(file);
            logger->info("upload file: {},type:{},name: {}", file.filename,
                         file.content_type, file.filename);
            return true;
        },
        [&](char const* data, size_t data_length)
        {
            auto last_file = files.back();
            auto file_path =
                fmt::format("{}/{}", shared_folder_, last_file.filename);

            auto contentType = last_file.content_type;
            fs_write(file_path, data, data_length);
            logger->info("upload file: {},type:{},size: {}", last_file.filename,
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
        logger->info("upload file by multipart");
        handle_multipart_file(content_reader);
        res.set_content(UP_LOAD_MESSAGE, APP_JSON);
    }
    else
    {
        logger->info("upload file by stream");
        std::string body;
        content_reader(
            [&](char const* data, size_t data_length)
            {
                body.append(data, data_length);
                return true;
            });
        std::string filename = req.matches[1];
        auto file_path = fmt::format("{}/{}", shared_folder_, filename);
        std::ofstream file(file_path, std::ios::binary);

        if (!file)
        {
            res.status = 500;
            res.set_content("Failed to create file", "text/plain");
            return;
        }

        file.write(body.c_str(), body.length());
        file.close();

        if (file)
        {
            res.set_content("File uploaded successfully", "text/plain");
        }
        else
        {
            res.status = 500;
            res.set_content("Failed to save file", "text/plain");
        }
    }
}
