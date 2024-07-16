#include "http_client_lib.h"

#include "client_yaml.hpp"
#include "constants.hpp"
#include "helpers.hpp"
#include "http_file_download.hpp"

#include <BS_thread_pool.hpp>
#include <algorithm>
#include <filesystem>
#include <thread>
namespace fs = std::filesystem;
static constexpr size_t BUF_SIZE = 16384L;

struct http_handle
{
#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
    std::shared_ptr<httplib::SSLClient> mClient;
#else
    std::shared_ptr<httplib::Client> mClient;
#endif
    std::multimap<std::string, std::string> mHeaders;
    client_config config;
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
            auto resp = handle->mClient->Post("/hello/100", json, APP_JSON);
            return resp;
        });
    auto resp = resp_result.get();
    if (resp == nullptr || resp->status != 200)
    {
        spdlog::error("failed to post json request");
        return;
    }
}

void post_image_info_request(http_client_handle* handle, char const* url,
                             char const* json)
{
    if (!handle->mClient->is_valid())
    {
        spdlog::error("invalid http connection");
        return;
    }
    auto resp_result = handle->pool->submit_task(
        [&] { return handle->mClient->Post(url, json, APP_JSON); });
    auto resp = resp_result.get();
    if (resp == nullptr || resp->status != 200)
    {
        spdlog::error("failed to post json request");
        return;
    }
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
    if (!file.is_open())
    {
        spdlog::error("failed to open file {}", filename);
        return;
    }

    auto resp = handle->mClient->Post(
        url, headers,
        [&](size_t offset, httplib::DataSink& sink)
        {
            const size_t remaining = file_size - offset;
            size_t chunk_size = std::min(BUF_SIZE, remaining);
            file.seekg(offset);
            char buffer[BUF_SIZE];
            file.read(buffer, chunk_size);
            sink.write(buffer, chunk_size);
            return remaining <= BUF_SIZE;
        },
        OCTET_STREAM);
    if (resp == nullptr || resp->status != 200)
    {
        spdlog::error("failed to post file request");
        return;
    }
}

void post_file_stream(http_client_handle* handle, char const* url,
                      char const* file_id, char const* data, unsigned long size)
{

    auto file_attach = fmt::format("attachment; filename=\"{}\"", file_id);
    httplib::Headers headers = {{CONTENT_DISPOSITION, file_attach.c_str()}};
    auto resp = handle->mClient->Post(
        url, headers,
        [&](size_t offset, httplib::DataSink& sink)
        {
            const size_t remaining = size - offset;
            size_t chunk_size = std::min(BUF_SIZE, remaining);
            sink.write(data + offset, chunk_size);
            return remaining <= BUF_SIZE;
        },
        OCTET_STREAM);
    if (resp == nullptr || resp->status != 200)
    {
        spdlog::error("failed to post file request");
        return;
    }
}
http_client_handle* new_http_client(char const* filename)
{
    auto yaml = std::make_shared<ClientYml>(std::string(filename));
    yaml->parse();
    auto config = yaml->getConfig();
#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
    auto cli = std::make_shared<httplib::SSLClient>(config.host, config.port);
    cli->set_ca_cert_path(config.auth_token);
#else
    auto cli = std::make_shared<httplib::Client>(config.host, config.port);
#endif
    if (!cli->is_valid())
    {
        return nullptr;
    }
    auto handle = new http_client_handle();
    handle->mClient = cli;
    handle->pool = std::make_shared<BS::thread_pool>(2);
    handle->config = config;
    return handle;
}
void free_client_handle(http_client_handle* handle)
{
    if (handle == nullptr)
    {
        return;
    }
    handle->mHeaders.clear();
    free(handle);
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
    handle->mHeaders.emplace(API_KEY, token);
}

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

    for (auto const& [key, value] : handle->mHeaders)
    {
        spdlog::debug("Header: {}: {}", key, value);
        headers.emplace(key, value);
    }

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
