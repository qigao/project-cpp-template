#include "../include/config/logs.hpp"
#include "constants.hpp"
#include "fs.hpp"
#include "http/http_file_handle.hpp"
#include "http/http_json_handle.hpp"
#include "http/http_server.hpp"
#include "http/http_web_hook.hpp"
#include "http_client_lib.h"
#include "http_lib_header.hpp"

#include <catch2/catch_all.hpp>
#include <cstdio>
#include <fstream>
#include <functional>
#include <httplib.h>
#include <memory>
#include <vector>
#include <yyjson.h>
#define DEMO_JSON "{\"msg\":\"demo\",\"id\":200}"
static char const* HOST = "127.0.0.1";
static int const PORT = 5060;
using namespace std::placeholders;
static char const* const SHARED_FOLDER = ".";

std::pair<std::string, int> parse_json(char* json, int len)
{
    yyjson_doc* doc = yyjson_read(json, len, 0);
    yyjson_val* root = yyjson_doc_get_root(doc);
    auto msgObj = yyjson_obj_get(root, "msg");
    auto msg = yyjson_get_str(msgObj);
    auto idObj = yyjson_obj_get(root, "id");
    auto id = yyjson_get_int(idObj);
    std::pair<std::string, int> result;
    result.first = msg;
    result.second = id;
    yyjson_doc_free(doc);
    return result;
}

// Handler function to test
void handle_get(httplib::Request const& req, httplib::Response& res)
{
    res.set_content("Hello World!", "text/plain");
}

TEST_CASE("HttpServerTes getHelloJson", "[http_server]")
{
    Logger::Construct();
    HttpServer svr(PORT);
    // Register handler
    auto handler = std::make_shared<HttpJsonHandler>();
    svr.Get(R"(/hello/(\d+))",
            std::bind(&HttpJsonHandler::getMsg, handler, _1, _2));
    svr.start();
    auto se = httplib::detail::scope_exit([&] { svr.stop(); });

    // Send request and check response
    httplib::Client cli(HOST, PORT);
    auto resp = cli.Get("/hello/100");
    REQUIRE(resp != nullptr);
    REQUIRE(resp->status == 200);
    auto result = parse_json(resp->body.data(), resp->body.length());
    REQUIRE(result.second == 100);
    REQUIRE_THAT(result.first, Catch::Matchers::Equals("demo"));
}

TEST_CASE("HttpServerTest  postJson", "[http_server]")
{
    Logger::Construct();
    httplib::Server svr;
    // Register handler
    auto handler = std::make_shared<HttpJsonHandler>();
    svr.Post(R"(/hello/(\d+))",
             [&](httplib::Request const& req, httplib::Response& res)
             { handler->postMsg(req, res); });
    svr.listen_after_bind();
    auto listen_thread = std::thread([&svr] { svr.listen(HOST, PORT); });
    auto se = httplib::detail::scope_exit(
        [&]
        {
            svr.stop();
            listen_thread.join();
            REQUIRE_FALSE(svr.is_running());
        });

    svr.wait_until_ready();
    // Send request and check response
    httplib::Client cli(HOST, PORT);
    httplib::Headers headers = {{"Content-Type", "application/json"}};
    auto resp = cli.Post("/hello/100", headers, DEMO_JSON, APP_JSON);
    REQUIRE(resp != nullptr);
    REQUIRE(resp->status == 200);

    auto result = parse_json(resp->body.data(), resp->body.length());
    REQUIRE(result.second == 4001);
    REQUIRE_THAT(result.first, Catch::Matchers::Equals("demo"));
}

TEST_CASE("HttpServerTest postJsonReturn4001", "[http_server]")
{
    Logger::Construct();
    HttpServer svr(PORT);
    // Register handler
    auto handler = std::make_shared<HttpJsonHandler>();
    svr.Post(R"(/hello/(\d+))",
             [&](httplib::Request const& req, httplib::Response& res)
             { handler->postMsg(req, res); });
    svr.start();
    // Send request and check response
    httplib::Client cli(HOST, PORT);
    // When
    auto resp = cli.Post("/hello/100", DEMO_JSON, APP_JSON);
    REQUIRE(resp != nullptr);
    REQUIRE(resp->status == 200);
    auto result = parse_json(resp->body.data(), resp->body.length());
    REQUIRE(result.second == 4001);
    REQUIRE_THAT(result.first, Catch::Matchers::Equals("demo"));
}

// Helper function to generate a large file
void generate_large_file(std::string const& filename, size_t size_mb)
{
    std::ofstream file(filename, std::ios::binary);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);

    for (size_t i = 0; i < size_mb * 1024 * 1024; ++i)
    {
        char byte = static_cast<char>(dis(gen));
        file.write(&byte, 1);
    }
    file.close();
}

// Helper function to compare two files
bool compare_files(std::string const& file1, std::string const& file2)
{
    std::ifstream f1(file1, std::ios::binary);
    std::ifstream f2(file2, std::ios::binary);

    if (!f1.is_open() || !f2.is_open())
    {
        return false;
    }

    std::istreambuf_iterator<char> begin1(f1), end1;
    std::istreambuf_iterator<char> begin2(f2), end2;

    return std::equal(begin1, end1, begin2, end2);
}

TEST_CASE("HttpServerTest  streamUpload", "[http_server]")
{
    Logger::Construct();
    HttpServer svr(PORT);
    // Register handler
    auto handler = std::make_shared<HttpFileHandle>(SHARED_FOLDER);
    svr.PostWithReader(
        R"(/upload/(.*))",
        [&](httplib::Request const& req, httplib::Response& res,
            httplib::ContentReader const& content_reader)
        { handler->handle_file_upload(req, res, content_reader); });
    svr.start();
    std::string const test_file = "test_large_file.jpg";
    std::string const received_file = "received_image.jpg";
    size_t const file_size_mb = 2;

    // Generate a large test file
    generate_large_file(test_file, file_size_mb);
    httplib::Client cli(HOST, PORT);
    std::ifstream file(test_file, std::ios::binary);
    std::string file_content((std::istreambuf_iterator<char>(file)),
                             std::istreambuf_iterator<char>());
    file.close();
    auto res = cli.Post(
        "/upload/received_image.jpg", file_content.size(),
        [&](size_t offset, size_t length, httplib::DataSink& sink)
        {
            sink.write(file_content.data() + offset, length);
            return true; // return 'false' if you want to cancel the request.
        },
        "text/plain");
    REQUIRE(res != nullptr);
    REQUIRE(res->status == 200);
    REQUIRE(res->body == "File uploaded successfully");

    // Compare the original and received files
    REQUIRE(compare_files(test_file, received_file));

    // Clean up
    std::remove(test_file.c_str());
    std::remove(received_file.c_str());
}

TEST_CASE("HttpServerTest multipartUpload", "[http_server]")
{
    Logger::Construct();
    HttpServer svr(PORT);
    auto handler = std::make_shared<HttpFileHandle>(SHARED_FOLDER);
    svr.PostWithReader(
        R"(/upload)", [&](httplib::Request const& req, httplib::Response& res,
                          httplib::ContentReader const& content_reader)
        { handler->handle_file_upload(req, res, content_reader); });
    svr.start();
    httplib::MultipartFormDataItems items = {
        {"text_file", "h\ne\n\nl\nl\no\n", "hello.txt", "text/plain"},
        {"binary_file", "h\ne\n\nl\nl\no\n", "hello.bin",
         "application/octet-stream"}};
    httplib::Client cli(HOST, PORT);
    auto resp = cli.Post("/upload", items);
    REQUIRE(resp != nullptr);
    REQUIRE(resp->status == 200);
}

TEST_CASE("HttpServerTest  getFileLists", "[http_server]")
{
    Logger::Construct();
    HttpServer svr(PORT);
    auto handler = std::make_shared<HttpFileHandle>(SHARED_FOLDER);
    svr.Get("/",
            std::bind(&HttpFileHandle::handle_file_lists, handler, _1, _2));
    svr.start();
    httplib::Client cli(HOST, PORT);
    auto resp = cli.Get("/");
    std::cout << resp->body << std::endl;
    REQUIRE(resp != nullptr);
    REQUIRE(resp->status == 200);
}

TEST_CASE("HttpServerTest  downloadFile", "[http_server]")
{
    Logger::Construct();
    HttpServer svr(PORT);
    auto handler = std::make_shared<HttpFileHandle>(SHARED_FOLDER);
    svr.Get("/download/(.*)",
            std::bind(&HttpFileHandle::handle_file_download, handler, _1, _2));
    svr.start();
    httplib::Client cli(HOST, PORT);
    std::string const unix_file = "test_large_file.jpg";
    std::string const local_file = "received_image.jpg";
    size_t const file_size_mb = 2;

    // Generate a large test file
    generate_large_file(unix_file, file_size_mb);
    std::ofstream file(local_file, std::ios::binary);
    auto remote_url = fmt::format("/download/{}", unix_file);

    std::cout << "remote url: " << remote_url << std::endl;
    auto res = cli.Get(
        remote_url,
        [&](char const* data, size_t data_length)
        {
            file.write(data, data_length);
            return true; // Return false if you want to cancel the download
        });

    if (res)
    {
        // Download successful
        file.close();
    }
    else
    {
        // Handle error
        auto err = res.error();
    }
    REQUIRE(res != nullptr);
    REQUIRE(res->status == 200);
    REQUIRE(compare_files(unix_file, local_file));

    std::remove(unix_file.c_str());
    std::remove(local_file.c_str());
}
void create_test_file(std::string const& filename, std::string const& content)
{
    std::ofstream file(filename);
    file << content;
}
template <typename T>
T custom_min(T a, T b)
{
    return (a < b) ? a : b;
}

void handle_file_download(httplib::Request const& req, httplib::Response& res)
{
    std::string file_path = req.matches[1];
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

// TEST_CASE("HttpServerTest dump", "[http_server]")
// {
//     Logger::Construct();
//     WebHook::Construct("http://127.0.0.1:5060/dump");
//     WebHook::GetInstance()->set_header(USER_AGENT, "MVIEW");
//     HttpServer svr(PORT);
//     // Register handler
//     auto handler = std::make_shared<HttpJsonHandler>();
//     svr.Post("/dump", std::bind(&HttpJsonHandler::dump, handler, _1, _2));
//     svr.start();
//     auto se = httplib::detail::scope_exit([&] { svr.stop(); });

//     // Send request and check response
//     httplib::Client cli(HOST, PORT);
//     httplib::Headers headers = {
//         {"X-Hub-Signature-256",
//          "sha256="
//          "fa090c43f504e30497b7ef441500f9666e13aa1368e394d7d668eeb7de983bcb"}};
//     auto resp = cli.Post("/dump", DEMO_JSON, APP_JSON);
//     REQUIRE(resp != nullptr);
//     REQUIRE(resp->status == 200);
// }

TEST_CASE("HttpServerTest webHookDemo", "[http_server]")
{
    Logger::Construct();
    WebHook::Construct("http://127.0.0.1:5060/dump");
    WebHook::GetInstance()->set_header(USER_AGENT, "MVIEW");
    HttpServer svr(PORT);
    // Register handler
    auto handler = std::make_shared<HttpJsonHandler>();
    svr.Post("/webhook",
             std::bind(&HttpJsonHandler::web_hook, handler, _1, _2));
    svr.Post("/dump", std::bind(&HttpJsonHandler::dump, handler, _1, _2));
    svr.start();
    auto se = httplib::detail::scope_exit([&] { svr.stop(); });
    SECTION("webhook demo")
    {
        // Send request and check response
        httplib::Client cli(HOST, PORT);
        auto resp = cli.Post("/webhook", DEMO_JSON, APP_JSON);
        REQUIRE(resp != nullptr);
        REQUIRE(resp->status == 200);
    }
    SECTION("dump")
    {
        httplib::Client cli(HOST, PORT);
        httplib::Headers headers = {{"X-Hub-Signature-256",
                                     "sha256="
                                     "fa090c43f504e30497b7ef441500f9666e13aa136"
                                     "8e394d7d668eeb7de983bcb"}};
        auto resp = cli.Post("/dump", DEMO_JSON, APP_JSON);
        REQUIRE(resp != nullptr);
        REQUIRE(resp->status == 200);
    }
}
std::string generate_large_content(size_t size)
{
    std::string const chars =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    std::string result;
    result.reserve(size);
    for (size_t i = 0; i < size; ++i)
    {
        result += chars[rand() % chars.length()];
    }
    return result;
}

TEST_CASE("HttpServerTest with client", "[http_server]")
{
    Logger::Construct();
    WebHook::Construct("http://127.0.0.1:5060/dump");
    WebHook::GetInstance()->set_header(USER_AGENT, "MVIEW");
    HttpServer svr(PORT);
    // Register handler
    auto handler = std::make_shared<HttpJsonHandler>();
    auto fileHandle = std::make_shared<HttpFileHandle>(SHARED_FOLDER);
    svr.PostWithReader(
        R"(/upload/(.*))",
        [&](httplib::Request const& req, httplib::Response& res,
            httplib::ContentReader const& content_reader)
        { fileHandle->handle_file_upload(req, res, content_reader); });
    svr.Post("/webhook",
             std::bind(&HttpJsonHandler::web_hook, handler, _1, _2));
    svr.Post("/dump", std::bind(&HttpJsonHandler::dump, handler, _1, _2));
    svr.start();
    auto se = httplib::detail::scope_exit([&] { svr.stop(); });
    SECTION("webhook demo")
    {
        auto http_client_api = new_http_client("client.yml");
        http_request_initialize(http_client_api);
        auto resp_code =
            post_json_request(http_client_api, "/webhook", DEMO_JSON);

        REQUIRE(resp_code == 200);
        // Send request and check response
    }
    SECTION(" upload file stream")
    {
        size_t const file_size = 10 * 1024 * 1024; // 10MB
        std::string large_content = generate_large_content(file_size);
        std::string filename = "large_file.txt";
        std::ofstream ofs(filename, std::ofstream::binary);
        ofs.write(large_content.c_str(), file_size);
        ofs.close();
        auto http_client_api = new_http_client("client.yml");
        http_request_initialize(http_client_api);

        // Send POST request
        int res = post_file_stream(http_client_api, "/upload/large.txt",
                                    large_content.data(), file_size);

        REQUIRE(res == 200);

        // Compare the original and received files
        REQUIRE(compare_files(filename, "large.txt"));

        // Clean up
        std::remove(filename.c_str());
        std::remove("large.txt");
    }
}
