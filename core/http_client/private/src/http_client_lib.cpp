#include "http_client_lib.h"

#include "client_yaml.hpp"
#include "pch_headers.hpp"

#include <ostream>

static constexpr size_t BUF_SIZE = 16384L;
template <typename T>
T custom_min(T a, T b)
{
    return (a < b) ? a : b;
}
struct http_handle_t
{
#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
    std::unique_ptr<httplib::SSLClient> mClient;
#else
    std::unique_ptr<httplib::Client> mClient;
#endif
    std::multimap<std::string, std::string> mHeaders;
    client_config config;
    std::unique_ptr<BS::thread_pool> pool;
};

int Sum(int const pLhs, int const pRhs) { return pLhs + pRhs; }

int file_download(http_client_handle handle, char const* url, char const* path,
                  char const* local_file)
{
    if (!handle->mClient->is_valid())
    {
        std::cerr << "invalid http connection" << std::endl;
        return -1;
    }
    ensureDirectoryExists(path);
    auto local_file_path = fs::path(path) / local_file;
    // Open output file
    std::ofstream outfile(local_file_path, std::ios::binary);
    if (!outfile)
    {
        std::cerr << "Failed to open output file: " << local_file << std::endl;
    }
    // Define content receiver callback
    size_t total_bytes = 0;
    auto content_receiver = [&](char const* data, size_t data_length)
    {
        outfile.write(data, data_length);
        total_bytes += data_length;
        return true; // Return false to cancel the download
    };

    auto resp = handle->mClient->Get(url, content_receiver);
    outfile.close();
    return resp->status;
}

int post_json_with_resp(http_client_handle handle, char const* url,
                        char const* json, char* resp_body)
{
    if (!handle->mClient->is_valid())
    {
        std::cerr << "invalid http connection" << std::endl;
        return -1;
    }

    auto resp_result = handle->pool->submit_task(
        [&]
        {
            auto resp = handle->mClient->Post(url, json, APP_JSON);
            return resp;
        });
    auto resp = resp_result.get();
    if (resp == nullptr || resp->status != 200)
    {
        std::cerr << "failed to post json request" << std::endl;
        return resp->status;
    }
    std::memcpy(resp_body, resp->body.c_str(), resp->body.length());
    return resp->status;
}

int post_json_request(http_client_handle handle, char const* url,
                      char const* json)
{
    if (!handle->mClient->is_valid())
    {
        std::cerr << "invalid http connection" << std::endl;
        return -1;
    }
    auto resp_result = handle->pool->submit_task(
        [&]
        {
            auto resp = handle->mClient->Post(url, json, APP_JSON);
            return resp;
        });
    auto resp = resp_result.get();
    if (resp == nullptr || resp->status != 200)
    {
        std::cerr << "failed to post json request" << std::endl;
    }
    return resp->status;
}
int post_file_request(http_client_handle handle, char const* url,
                      char const* filename)
{
    std::ifstream file(filename, std::ios::binary);
    if (!file)
    {
        return false;
    }

    // Get file size
    file.seekg(0, std::ios::end);
    size_t file_size = file.tellg();
    file.seekg(0, std::ios::beg);

    // Read file content into a vector
    std::vector<char> file_content(file_size);
    file.read(file_content.data(), file_size);

    httplib::MultipartFormDataItems items = {
        {"file", file_content.data(), filename, OCTET_STREAM}};

    // Send POST request
    auto resp = handle->mClient->Post(url, items);
    if (resp == nullptr || resp->status != 200)
    {
        std::cerr << "failed to post json request" << std::endl;
    }
    return resp->status;
}

int post_file_stream(http_client_handle handle, char const* url,
                     char const* data, long data_len)
{
    if (!handle->mClient->is_valid())
    {
        std::cerr << "invalid http connection" << std::endl;
        return -1;
    }
    auto resp = handle->mClient->Post(
        url, data_len,
        [&](size_t offset, size_t length, httplib::DataSink& sink)
        {
            sink.write(data + offset, length);
            return true; // return 'false' if you want to cancel the request.
        },
        OCTET_STREAM);
    return resp->status;
}

http_client_handle new_http_client(char const* filename)
{
    auto yaml = std::make_unique<ClientYml>(std::string(filename));
    yaml->parse();
    auto config = yaml->getConfig();
#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
    auto cli = std::make_unique<httplib::SSLClient>(config.host, config.port);
    cli->set_ca_cert_path(config.auth_token);
#else
    auto cli = std::make_unique<httplib::Client>(config.host, config.port);
#endif
    if (!cli->is_valid())
    {
        return nullptr;
    }
    auto handle = new http_handle_t();
    handle->mClient = std::move(cli);
    handle->pool = std::make_unique<BS::thread_pool>(2);
    handle->config = config;
    return handle;
}
void free_client_handle(http_client_handle handle)
{
    if (handle == nullptr)
    {
        return;
    }
    handle->mHeaders.clear();
    free(handle);
}
void setup_http_header(http_client_handle handle, char const* header_name,
                       char const* header_value)
{
    handle->mHeaders.emplace(header_name, header_value);
}

void http_request_initialize(http_client_handle handle)
{
    if (!handle->mClient->is_valid())
    {
        return;
    }
    handle->mClient->set_keep_alive(true);
    handle->mClient->set_read_timeout(30, 0);
    handle->mClient->set_write_timeout(10, 0);

    httplib::Headers headers = {{"User-Agent", "MicroView Http library"}};
    headers.emplace(API_KEY, handle->config.auth_token);
    for (auto const& [key, value] : handle->mHeaders)
    {
        headers.emplace(key, value);
    }

    handle->mClient->set_default_headers(headers);
    if (handle->config.debug)
    {
        handle->mClient->set_logger(
            [](httplib::Request const& req, httplib::Response const& res)
            {
                dump_http_request(req);
                dump_http_response(res);
            });
    }
}
