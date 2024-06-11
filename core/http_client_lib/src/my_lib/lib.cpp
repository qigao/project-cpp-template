
#include <fmt/core.h>
#include <httplib.h>

#include "my_lib/lib.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#ifdef OS_WINDOWS
#include <Windows.h>
#include <wincrypt.h>
#endif
int some_fun()
{
    fmt::print("Hello {} !", "world");
    return 0;
}

void upload_file_by_http(const std::string& url, const std::string& file_path,
                         const std::string& chat_id)
{

    fmt::print("Upload file {} by http: {} !", file_path, url);
    std::ifstream input(file_path, std::ios::binary);
    std::stringstream buffer;
    buffer << input.rdbuf();
    httplib::Client cli(url);
    cli.set_follow_location(true);
    cli.set_read_timeout(30, 0);
    cli.set_write_timeout(30, 0);
    cli.set_basic_auth("admin", "admin");
    httplib::MultipartFormDataItems items{
        {"document", buffer.str(), "my.dll", "application/octet-stream"},
        {"chat_id", chat_id, "", ""},
    };
    auto res = cli.Post(url, items);
    if (res && res->status == 200)
    {
        std::cout << "Success" << std::endl;
    }
}

int http_get_demo()
{
#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
    httplib::SSLClient cli("jsonplaceholder.typicode.com");
    cli.enable_server_certificate_verification(true);
    std::cout << "HTTPS" << std::endl;
#else
    httplib::Client cli("jsonplaceholder.typicode.com");
    std::cout << "HTTP" << std::endl;
#endif

    if (auto res = cli.Get("/todos/1"))
    {
        if (res->status == 200)
        {
            std::cout << res->body << std::endl;
        }
        return res->status;
    }
    return 500;
}
