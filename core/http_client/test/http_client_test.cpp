#include "client_yaml.hpp"
#include "constants.hpp"
#include "fs.hpp"
#include "http/http_file_handle.hpp"
#include "http/http_json_handle.hpp"
#include "http/http_server.hpp"
#include "http/http_web_hook.hpp"
#include "http_client_lib.h"
#include "test_helper.h"

#include <cstdio>
#include <filesystem>
#include <fmt/core.h>
#include <functional>
#include <gtest/gtest.h>
#include <iostream>
#include <memory>
#include <string>
#include <yaml-cpp/yaml.h>
using namespace std::placeholders;
static char const* HOST = "127.0.0.1";
static int const PORT = 5060;
static char const* SHARED_FOLDER = "/tmp";
class HttpClientTest : public testing::Test
{
protected:
    void SetUp() override
    {
        auto const file_name = "client.yaml";
        YAML::Node node = YAML::Load(yml_demo_data);
        auto random_dir = fs::temp_directory_path();
        if (!createRandomDir(random_dir))
        {
            std::cerr << "Failed to create temporary directory" << std::endl;
            return;
        }
        full_path_file_name =
            std::make_shared<fs::path>(random_dir / file_name);

        YAML::Emitter out;
        out << node;
        write(full_path_file_name->string(), out.c_str(), out.size());
    }

    void TearDown() override
    {
        fs::remove_all(full_path_file_name->filename());
    }

    std::shared_ptr<fs::path> full_path_file_name;
};

TEST_F(HttpClientTest, uploadFileByStream)
{
    HttpServer svr(PORT);
    svr.setSharedFolder(SHARED_FOLDER);

    auto handler = std::make_shared<HttpFileHandle>(SHARED_FOLDER);
    svr.PostWithReader(
        R"(/upload)", [&](httplib::Request const& req, httplib::Response& res,
                          httplib::ContentReader const& content_reader)
        { handler->handle_file_upload(req, res, content_reader); });
    svr.start();

    auto http_client_api = new_http_client(full_path_file_name->c_str());
    set_auth_token(http_client_api, "123456");
    http_request_initialize(http_client_api);
    fs::path tmp{std::filesystem::temp_directory_path()};
    std::string filename = tmp.append("test.txt").string();

    std::ofstream ofs(filename, std::ofstream::binary);
    ofs.write("hello", 5);
    ofs.close();

    std::ifstream file(filename, std::ifstream::binary);
    post_file_stream_request(http_client_api, "/upload", filename.c_str());

    file.close();
    fs::remove(filename);
}

TEST_F(HttpClientTest, uploadByStream)
{
    HttpServer svr(PORT);
    svr.setSharedFolder(SHARED_FOLDER);

    auto handler = std::make_shared<HttpFileHandle>(SHARED_FOLDER);
    svr.PostWithReader(
        R"(/upload)", [&](httplib::Request const& req, httplib::Response& res,
                          httplib::ContentReader const& content_reader)
        { handler->handle_file_upload(req, res, content_reader); });
    svr.start();

    auto http_client_api = new_http_client(full_path_file_name->c_str());
    set_auth_token(http_client_api, "123456");
    http_request_initialize(http_client_api);

    post_file_stream(http_client_api, "/upload", "101", "hello", 5);
    std::string filename = fmt::format("{}/{}", SHARED_FOLDER, 101);
    EXPECT_TRUE(std::filesystem::exists(filename));
}

TEST_F(HttpClientTest, listFile)
{
    HttpServer svr(PORT);
    svr.setSharedFolder(SHARED_FOLDER);
    auto handler = std::make_shared<HttpFileHandle>(SHARED_FOLDER);
    svr.Get("/",
            std::bind(&HttpFileHandle::handle_file_lists, handler, _1, _2));
    svr.start();
    httplib::Client cli(HOST, PORT);
    auto resp = cli.Get("/");

    EXPECT_NE(resp, nullptr);
    EXPECT_EQ(resp->status, 200);
}

TEST_F(HttpClientTest, downloadFile)
{
    HttpServer svr(PORT);
    svr.setSharedFolder(SHARED_FOLDER);
    auto handler = std::make_shared<HttpFileHandle>(SHARED_FOLDER);
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

TEST_F(HttpClientTest, downloadFile2)
{
    HttpServer svr(PORT);
    svr.setSharedFolder(SHARED_FOLDER);
    auto handler = std::make_shared<HttpFileHandle>(SHARED_FOLDER);
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
    auto http_client_api = new_http_client(full_path_file_name->c_str());
    http_request_initialize(http_client_api);
    sync_file_download(http_client_api, remote_url.c_str(), local_file.c_str());
    std::remove(local_file.c_str());
    EXPECT_FALSE(std::filesystem::exists(local_file));
    svr.stop();
}

TEST_F(HttpClientTest, postJsonRequest)
{
    HttpServer svr(PORT);
    // Register handler
    auto handler = std::make_shared<HttpJsonHandler>();
    svr.Post(R"(/hello/(\d+))",
             [&](httplib::Request const& req, httplib::Response& res)
             { handler->postMsg(req, res); });
    svr.start();
    // Send request and check response
    auto http_client_api = new_http_client(full_path_file_name->c_str());
    http_request_initialize(http_client_api);
    // When
    std::string msg = "demo";
    int id = 30;

    std::string json = fmt::format(R"({{"msg": "{}","id": {}}})", msg, id);

    post_json_request(http_client_api, "/hello/100", json.c_str());
    free_client_handle(http_client_api);
}

TEST_F(HttpClientTest, postJsonImageRequest)
{
    WebHook::Construct("http://127.0.0.1:5060/dump");
    WebHook::GetInstance()->set_header(USER_AGENT, "MVIEW");
    // Register handler
    auto handler = std::make_shared<HttpJsonHandler>();
    HttpServer svr(PORT);

    svr.Post("/dump", std::bind(&HttpJsonHandler::dump, handler, _1, _2));
    svr.Post(R"(/hello/(\d+))",
             [&](httplib::Request const& req, httplib::Response& res)
             { handler->web_hook(req, res); });
    svr.start();
    auto se = httplib::detail::scope_exit([&] { svr.stop(); });
    // Send request and check response
    auto http_client_api = new_http_client(full_path_file_name->c_str());
    http_request_initialize(http_client_api);
    // When
    std::string msg = "demo";
    int id = 30;
    std::string json = fmt::format(
        R"({{ "msg": "{}", "id": {},  "height": {}, "width": {} }})", msg, id,
        720, 1080);
    post_image_info_request(http_client_api, "/hello/100", json.c_str());
    free_client_handle(http_client_api);
}
