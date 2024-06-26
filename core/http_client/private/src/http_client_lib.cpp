#include "http_client_lib.h"
#include "constants.hpp"
#include "fs.hpp"
#include "helpers.hpp"
#include "http_file_download.hpp"
#include "http_lib_header.hpp"
#include <BS_thread_pool.hpp>
#include <cpp_yyjson.hpp>
#include <filesystem>
#include <range/v3/all.hpp>
#include <thread>

namespace fs = std::filesystem;

struct http_handle
{
#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
    std::shared_ptr<httplib::SSlClient> mClient;
#else
    std::shared_ptr<httplib::Client> mClient;
#endif
    std::multimap<std::string, std::string> mHeaders;
    bool debug = true;
    std::shared_ptr<BS::thread_pool> pool;
};

int Sum(int const pLhs, int const pRhs) { return pLhs + pRhs; }

void async_file_download(http_client_handle* handle, char const* url,
                         char const* local_filename)
{
    auto downloader = std::make_shared<HttpFileDownload>();
    std::thread thr(&HttpFileDownload::doDownload, downloader, handle->mClient,
                    url, local_filename);
    thr.detach();
}

void sync_file_download(http_client_handle* handle, char const* url,
                        char const* local_filename)
{
    if (!handle->mClient->is_valid())
    {
        spdlog::error("invalid http connection");
        return;
    }
    std::ofstream ofsl(local_filename, std::ofstream::binary);
    auto resp_result = handle->pool->submit_task(
        [&]
        {
            auto resp = handle->mClient->Get(
                url,
                [&](httplib::Response const& response)
                {
                    std::cerr << "Client read:" << response.status << std::endl;
                    return true;
                },
                [&](char const* data, size_t data_length)
                {
                    ofsl.write(data, data_length);
                    std::cerr << "Client write:" << data_length << std::endl;
                    return true;
                });
            return resp;
        });
    spdlog::info("download result: {}", resp_result.get()->status);
    ofsl.close();
}

void post_json_request(http_client_handle* handle, char const* url,
                       char const* json)
{
    if (!handle->mClient->is_valid())
    {
        spdlog::error("invalid http connection");
        return;
    }

    auto resp_result = handle->pool->submit_task(
        [&]
        {
            auto req = yyjson::object();
            req.emplace("id", 200);
            req.emplace("msg", "demo");
            auto body = req.write();
            auto resp =
                handle->mClient->Post("/hello/100", body.data(), APP_JSON);
            return resp;
        });
    auto resp = resp_result.get();
    if (resp == nullptr || resp->status != 200)
    {
        spdlog::error("failed to post json request");
        return;
    }
    auto val = yyjson::read(resp->body);
    auto obj = *val.as_object();
    auto id = *obj["id"].as_int();

    auto msg = *obj["msg"].as_string();
}

void post_image_info_request(http_client_handle* handle, char const* url,
                             char const* json)
{
    if (!handle->mClient->is_valid())
    {
        spdlog::error("invalid http connection");
        return;
    }
    handle->pool->detach_task(
        [&]
        {
            auto resp = handle->mClient->Post(url, json, APP_JSON);
            if (resp == nullptr || resp->status != 200)
            {
                spdlog::error("failed to post json request");
                return;
            }
            auto val = yyjson::read(resp->body);
            auto obj = *val.as_object();
            auto id = *obj["id"].as_int();
            auto msg = *obj["msg"].as_string();
            spdlog::info("id: {}, msg: {}", id, msg);
        });
}
void post_file_request(http_client_handle* handle, char const* url,
                       char const* filename)
{
    auto file_path = fs::path(filename);
    if (!fs::exists(file_path))
    {
        spdlog::error("file {} does not exist", filename);
        return;
    }
    auto file_name = file_path.filename().string();
    auto file_size = fs::file_size(file_path);
    std::ifstream file(filename, std::ifstream::binary);
    std::ostringstream oss;
    oss << file.rdbuf();
    httplib::MultipartFormDataItems items = {
        {"name", std::string(oss.str(), file_size), file_name, OCTET_STREAM},
    };

    auto resp = handle->mClient->Post(url, items);
    if (resp == nullptr || resp->status != 200)
    {
        spdlog::error("failed to post file request");
        return;
    }
}

void post_file_stream_request(http_client_handle* handle, char const* url,
                              char const* filename)
{
    auto file_path = fs::path(filename);
    if (!fs::exists(file_path))
    {
        spdlog::error("file {} does not exist", filename);
        return;
    }
    auto file_name = file_path.filename().string();
    auto file_size = fs::file_size(file_path);
    auto file_attach = fmt::format("attachment; filename=\"{}\"", file_name);
    httplib::Headers headers = {{CONTENT_DISPOSITION, file_attach.c_str()}};
    std::ifstream file(filename, std::ifstream::binary);
    auto resp = handle->mClient->Post(
        url, headers,
        [&](size_t offset, httplib::DataSink& sink)
        {
            // Read the file data in chunks and write to the sink
            char buffer[16384];
            file.seekg(offset);
            while (file.read(buffer, sizeof(buffer)))
            {
                sink.write(buffer, file.gcount());
            }
            sink.write(buffer, file.gcount());
            sink.done();
            return true;
        },
        OCTET_STREAM);
    if (resp == nullptr || resp->status != 200)
    {
        spdlog::error("failed to post file request");
        return;
    }
}

http_client_handle* new_http_client(char const* host, int port)
{
#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
    auto cli = std::make_shared<httplib::SSLClient>(host, port);
    cli->enable_server_certificate_verification(true);
    cli->set_ca_cert_path("ca-bundle.crt");
#else
    auto cli = std::make_shared<httplib::Client>(host, port);
#endif
    if (!cli->is_valid())
    {
        return nullptr;
    }
    auto handle = new http_client_handle();
    handle->mClient = cli;
    handle->pool = std::make_shared<BS::thread_pool>(2);
    return handle;
}

void free_client_handle(http_client_handle* handle)
{
    if (handle == nullptr)
    {
        return;
    }
}
/**
 *  headers will be applicable to all the request, such as http request of
 * token,agent etc.
 * @param handle
 * @param header_name
 * @param header_value
 */
void setup_http_header(http_client_handle* handle, char const* header_name,
                       char const* header_value)
{
    handle->mHeaders.emplace(header_name, header_value);
}

void set_auth_token(http_client_handle* handle, char const* token)
{
    handle->mHeaders.emplace("X-API-KEY", token);
}
void read_config_from_yml(char const* file_path) {}

void http_request_initialize(http_client_handle* handle)
{
    if (!handle->mClient->is_valid())
    {
        return;
    }
    handle->mClient->set_keep_alive(true);
    handle->mClient->set_read_timeout(5, 0);
    handle->mClient->set_write_timeout(5, 0);
    httplib::Headers headers = {{"User-Agent", "MicroView Http library"}};

    ranges::for_each(handle->mHeaders,
                     [&headers](auto const& header)
                     {
                         spdlog::info("header[{}] = {}", header.first,
                                      header.second);
                         headers.emplace(header.first, header.second);
                     });
    handle->mClient->set_default_headers(headers);
    if (handle->debug)
    {
        handle->mClient->set_logger(
            [](httplib::Request const& req, httplib::Response const& res)
            {
                dump_http_request(req);
                dump_http_response(res);
            });
    }
}
