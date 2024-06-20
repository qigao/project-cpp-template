#include "http_file_transfer.h"
#include "http_file_download.hpp"
#include <range/v3/all.hpp>
#include <thread>
struct http_handle
{
#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
    std::shared_ptr<httplib::SSLClient> mClient;
#else
    std::shared_ptr<httplib::Client> mClient;
#endif
    std::multimap<std::string, std::string> mHeaders;
};
int Sum(int const pLhs, int const pRhs) { return pLhs + pRhs; }

void async_file_download(char const* url, char const* local_filename)
{
    HttpFileDownload downloader;
    std::thread thr(&HttpFileDownload::doDownload, downloader, url,
                    local_filename);
    thr.detach();
}

void sync_file_download(char const* url, char const* local_filename)
{
    HttpFileDownload downloader;
    downloader.doDownload(url, local_filename);
}

http_client_handle* new_http_client(char const* host, int port)
{

#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
    std::shared_ptr<httplib::SSlClient> mClient =
        std::make_shared<http::SSLClient>(host, port);
    mClient->enable_server_certificate_verification(true);
    mClient->set_ca_cert_path("ca-bundle.crt");
#else
    auto cli = std::make_shared<httplib::Client>(host, port);
#endif
    if (!cli->is_valid())
    {
        return nullptr;
    }
    auto handle = new http_client_handle();
    handle->mClient = cli;
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
void setup_default_header(http_client_handle* handle)
{
    handle->mHeaders.emplace("User-Agent", "MicroView Http library");
}
void set_auth_token(http_client_handle* handle, char const* token)
{
    handle->mClient->set_bearer_token_auth(token);
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
    httplib::Headers headers = {{}};

    ranges::for_each(handle->mHeaders,
                     [&headers](auto const& header)
                     {
                         spdlog::info("header[{}] = {}", header.first,
                                      header.second);
                         headers.emplace(header.first, header.second);
                     });
    handle->mClient->set_default_headers(headers);
}
