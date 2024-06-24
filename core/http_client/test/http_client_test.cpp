#include "../public/http_client_lib.h"
#include "fs.hpp"
#include <cpp_yyjson.hpp>
#include <cstdio>
#include <filesystem>
#include <functional>
#include <gtest/gtest.h>

#include "http/http_file_handle.hpp"
#include "http/http_json_handle.hpp"
#include "http/http_server.hpp"
#include <httplib.h>
#include <memory>

using namespace std::placeholders;
static char const* HOST = "127.0.0.1";
static int const PORT = 5060;
TEST(HttpClientTest, uploadFileByStream)
{
    HttpServer svr(PORT);
    svr.setSharedFolder("/tmp");

    auto handler = std::make_shared<HttpFileHandle>("/tmp");
    svr.PostWithReader(
        R"(/upload)", [&](httplib::Request const& req, httplib::Response& res,
                          httplib::ContentReader const& content_reader)
        { handler->handle_file_upload(req, res, content_reader); });
    svr.start();

    auto http_client_api = new_http_client(HOST, PORT);
    set_auth_token(http_client_api, "123456");
    http_request_initialize(http_client_api);
    auto tmp_folder = mkdir("/tmp/test", 0777);
    char const* filename = "/tmp/test/test.txt";
    std::ofstream ofs(filename, std::ofstream::binary);
    ofs.write("hello", 5);
    ofs.close();

    std::ifstream file(filename, std::ifstream::binary);
    post_file_stream_request(http_client_api, "/upload", filename);

    file.close();
    std::remove(filename);
}
TEST(HttpClientTest, listFile)
{
    HttpServer svr(PORT);
    svr.setSharedFolder("/tmp");
    auto handler = std::make_shared<HttpFileHandle>("/tmp");
    svr.Get("/",
            std::bind(&HttpFileHandle::handle_file_lists, handler, _1, _2));
    svr.start();
    httplib::Client cli(HOST, PORT);
    auto resp = cli.Get("/");

    EXPECT_NE(resp, nullptr);
    EXPECT_EQ(resp->status, 200);
}

TEST(HttpClientTest, downloadFile)
{
    HttpServer svr(PORT);
    svr.setSharedFolder("/tmp");
    auto handler = std::make_shared<HttpFileHandle>("/tmp");
    svr.Get("/download/(.*)",
            std::bind(&HttpFileHandle::handle_file_download, handler, _1, _2));
    svr.start();
    httplib::Client cli(HOST, PORT);
    auto unix_file = "test.txt";
    std::string filename = "/tmp/test.txt";
    std::ofstream ofs(filename, std::ofstream::binary);
    ofs.write("hello", 5);
    ofs.close();
    auto remote_url = fmt::format("/download/{}", unix_file);
    auto local_file = fmt::format("/tmp/1_{}", unix_file);
    std::ofstream ofsl(local_file, std::ofstream::binary);
    auto resp = cli.Get(
        remote_url,
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
    ofs.close();
    EXPECT_NE(resp, nullptr);
    EXPECT_EQ(resp->status, 200);
    std::remove(local_file.c_str());
    EXPECT_FALSE(std::filesystem::exists(local_file));
}
TEST(HttpClientTest, downloadFile2)
{
    HttpServer svr(PORT);
    svr.setSharedFolder("/tmp");
    auto handler = std::make_shared<HttpFileHandle>("/tmp");
    svr.Get("/download/(.*)",
            std::bind(&HttpFileHandle::handle_file_download, handler, _1, _2));
    svr.start();
    auto unix_file = "test.txt";
    std::string filename = "/tmp/test.txt";
    std::ofstream ofs(filename, std::ofstream::binary);
    ofs.write("hello", 5);
    ofs.close();
    auto remote_url = fmt::format("/download/{}", unix_file);
    auto local_file = fmt::format("/tmp/1_{}", unix_file);
    auto http_client_api = new_http_client(HOST, PORT);
    http_request_initialize(http_client_api);
    sync_file_download(http_client_api, remote_url.c_str(), local_file.c_str());
    std::remove(local_file.c_str());
    EXPECT_FALSE(std::filesystem::exists(local_file));
    svr.stop();
}

TEST(HttpClientTest, postJsonRequest)
{
    HttpServer svr(PORT);
    // Register handler
    auto handler = std::make_shared<HttpJsonHandler>();
    svr.Post(R"(/hello/(\d+))",
             [&](httplib::Request const& req, httplib::Response& res)
             { handler->postMsg(req, res); });
    svr.start();
    // Send request and check response
    auto http_client_api = new_http_client(HOST, PORT);
    http_request_initialize(http_client_api);
    // When
    auto req = yyjson::object();
    req.emplace("id", 200);
    req.emplace("msg", "demo");
    auto body = req.write();
    post_json_request(http_client_api, "/hello/100", body.data());
}
