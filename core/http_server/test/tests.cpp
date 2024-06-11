#include "helper/fs.hpp"
#include "helper/human_readable.hpp"
#include "http/http_file_handle.hpp"
#include "http/http_json_handle.hpp"
#include "http/http_server.hpp"
#include <catch2/catch_test_macros.hpp>
#include <cpp_yyjson.hpp>
#include <cstdio>
#include <functional>
#include <helper/helpers.hpp>

#include <httplib.h>
#include <memory>

static const char* const HOST = "127.0.0.1";
static const int PORT = 5060;
using namespace std::placeholders;
constexpr size_t IntegerRepresentableBoundary()
{
    static_assert(std::numeric_limits<double>::radix == 2,
                  "Method only valid for binary floating point format.");
    return size_t{2} << (std::numeric_limits<double>::digits - 1);
}
TEST_CASE("some_fun") { REQUIRE(some_fun() == 0); }
TEST_CASE("HumanReadable output stream operator test")
{
    HumanReadable hr;
    hr.size = 1024;

    std::ostringstream oss;
    oss << hr;

    REQUIRE(oss.str() == "1KB (1024)");
}
TEST_CASE("Human Readable cast tests")
{
    REQUIRE(static_cast<size_t>(NarrowCast<double>(
                size_t{IntegerRepresentableBoundary() - 2})) ==
            size_t{IntegerRepresentableBoundary() - 2});
    REQUIRE(static_cast<size_t>(NarrowCast<double>(
                size_t{IntegerRepresentableBoundary() - 1})) ==
            size_t{IntegerRepresentableBoundary() - 1});
    REQUIRE(static_cast<size_t>(
                NarrowCast<double>(size_t{IntegerRepresentableBoundary()})) ==
            size_t{IntegerRepresentableBoundary()});
    REQUIRE_THROWS_AS(
        NarrowCast<double>(size_t{IntegerRepresentableBoundary() + 1}),
        std::exception);
    REQUIRE(static_cast<size_t>(NarrowCast<double>(
                size_t{IntegerRepresentableBoundary() + 2})) ==
            size_t{IntegerRepresentableBoundary() + 2});
    REQUIRE_THROWS_AS(
        NarrowCast<double>(size_t{IntegerRepresentableBoundary() + 3}),
        std::exception);
    REQUIRE(static_cast<size_t>(NarrowCast<double>(
                size_t{IntegerRepresentableBoundary() + 4})) ==
            size_t{IntegerRepresentableBoundary() + 4});
    REQUIRE_THROWS_AS(
        NarrowCast<double>(size_t{IntegerRepresentableBoundary() + 5}),
        std::exception);
}

TEST_CASE("HttpServer set shared folder", "[server]")
{
    HttpServer svr(PORT);
    svr.setSharedFolder("test");
    REQUIRE(svr.getSharedFolder() == "test");
}
// Handler function to test
void handle_get(const httplib::Request& req, httplib::Response& res)
{
    res.set_content("Hello World!", "text/plain");
}

TEST_CASE("Server GET handler works")
{
    httplib::Server svr;
    // Register handler
    svr.Get("/hello", handle_get);
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
    auto res = cli.Get("/hello");
    REQUIRE(res != nullptr);
    REQUIRE(res->status == 200);
    REQUIRE(res->body == "Hello World!");
}

TEST_CASE("Server Get Msg handler works", "[server]")
{

    HttpServer svr(PORT);
    // Register handler
    auto handler = std::make_shared<HttpHandler>();
    svr.Get(R"(/hello/(\d+))",
            std::bind(&HttpHandler::getMsg, handler, _1, _2));
    svr.start();
    auto se = httplib::detail::scope_exit([&] { svr.stop(); });

    // Send request and check response
    httplib::Client cli(HOST, PORT);
    auto res = cli.Get("/hello/100");
    REQUIRE(res != nullptr);
    REQUIRE(res->status == 200);
    auto val = yyjson::read(res->body.c_str());
    auto obj = *val.as_object();
    auto id = *obj["id"].as_int();
    REQUIRE(id == 100);
    auto msg = *obj["msg"].as_string();
    REQUIRE(msg == "hello");
}
TEST_CASE("Server post example", "[server]")
{
    httplib::Server svr;
    // Register handler
    auto handler = std::make_shared<HttpHandler>();
    svr.Post(R"(/hello/(\d+))",
             [&](const httplib::Request& req, httplib::Response& res)
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
    auto req = yyjson::object();
    req.emplace("id", 200);
    req.emplace("msg", "demo");
    auto body = req.write();
    auto resp = cli.Post("/hello/100", headers, body.data(), APP_JSON);
    REQUIRE(resp != nullptr);
    REQUIRE(resp->status == 200);
    auto val = yyjson::read(resp->body);
    auto obj = *val.as_object();
    auto id = *obj["id"].as_int();
    REQUIRE(id == 4001);
    auto msg = *obj["msg"].as_string();
    REQUIRE(msg == "demo");
}
TEST_CASE("HttpServer post example", "[server]")
{
    HttpServer svr(PORT);
    // Register handler
    auto handler = std::make_shared<HttpHandler>();
    svr.Post(R"(/hello/(\d+))",
             [&](const httplib::Request& req, httplib::Response& res)
             { handler->postMsg(req, res); });
    svr.start();
    // Send request and check response
    httplib::Client cli(HOST, PORT);
    // When
    auto req = yyjson::object();
    req.emplace("id", 200);
    req.emplace("msg", "demo");
    auto body = req.write();
    auto resp = cli.Post("/hello/100", body.data(), APP_JSON);
    REQUIRE(resp != nullptr);
    REQUIRE(resp->status == 200);
    auto val = yyjson::read(resp->body);
    auto obj = *val.as_object();
    auto id = *obj["id"].as_int();
    REQUIRE(id == 4001);
    auto msg = *obj["msg"].as_string();
    REQUIRE(msg == "demo");
    svr.stop();
}
TEST_CASE("HttpServer upload file by multipart", "[server]")
{

    HttpServer svr(PORT);
    svr.setSharedFolder("/tmp");
    // Register handler
    auto handler = std::make_shared<HttpFileHandle>("/tmp");
    svr.Get("/", std::bind(&HttpFileHandle::list_upload_form, handler, _1, _2));
    svr.Post("/multipart", std::bind(&HttpFileHandle::upload_file_by_multiform,
                                     handler, _1, _2));
    svr.start();
    httplib::MultipartFormDataItems items = {
        {"text_file", "h\ne\n\nl\nl\no\n", "hello.txt", "text/plain"},
        {"binary_file", "h\ne\n\nl\nl\no\n", "hello.bin",
         "application/octet-stream"}};
    httplib::Client cli(HOST, PORT);
    auto resp = cli.Post("/multipart", items);

    REQUIRE(resp != nullptr);
    REQUIRE(resp->status == 200);
}

TEST_CASE("HttpServer upload file by stream with reader", "[server]")
{

    HttpServer svr(PORT);
    svr.setSharedFolder("/tmp");
    // Register handler
    auto handler = std::make_shared<HttpFileHandle>("/tmp");
    svr.PostWithContentHandler(
        R"(/upload)", [&](const httplib::Request& req, httplib::Response& res,
                          const httplib::ContentReader& content_reader)
        { handler->handle_file_upload(req, res, content_reader); });
    svr.start();
    std::string filename = "/tmp/test.txt";
    std::ofstream ofs(filename, std::ofstream::binary);
    ofs.write("hello", 5);
    ofs.close();
    httplib::Headers headers = {
        {"Content-Disposition", "attachment; filename=\"hello.txt\""}};

    std::ifstream file(filename, std::ifstream::binary);
    httplib::Client cli(HOST, PORT);
    auto resp = cli.Post(
        "/upload", headers,
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
        "application/octet-stream");

    REQUIRE(resp != nullptr);
    REQUIRE(resp->status == 200);

    file.close();
    std::remove(filename.c_str());
}

TEST_CASE("HttpServer upload file by multipart with reader", "[server]")
{
    HttpServer svr(PORT);
    svr.setSharedFolder("/tmp");
    auto handler = std::make_shared<HttpFileHandle>("/tmp");
    svr.PostWithContentHandler(
        R"(/upload)", [&](const httplib::Request& req, httplib::Response& res,
                          const httplib::ContentReader& content_reader)
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

TEST_CASE("HttpServer list all files", "[server]")
{
    HttpServer svr(PORT);
    svr.setSharedFolder("/tmp");
    auto handler = std::make_shared<HttpFileHandle>("/tmp");
    svr.Get("/",
            std::bind(&HttpFileHandle::handle_file_lists, handler, _1, _2));
    svr.start();
    httplib::Client cli(HOST, PORT);
    auto resp = cli.Get("/");

    REQUIRE(resp != nullptr);
    REQUIRE(resp->status == 200);
}

TEST_CASE("HttpServer download file", "[server]")
{
    HttpServer svr(PORT);
    svr.setSharedFolder("/tmp");
    auto handler = std::make_shared<HttpFileHandle>("/tmp");
    svr.Get("/download/(.*)",
            std::bind(&HttpFileHandle::handle_file_download, handler, _1, _2));
    svr.start();
    httplib::Client cli(HOST, PORT);
    auto unix_file = "hello.txt";
    auto remote_url = fmt::format("/download/{}", unix_file);
    auto local_file = fmt::format("./{}", unix_file);
    std::ofstream ofs(local_file, std::ofstream::binary);
    auto resp = cli.Get(
        remote_url,
        [&](const httplib::Response& response)
        {
            std::cerr << "Client read:" << response.status << std::endl;
            return true;
        },
        [&](const char* data, size_t data_length)
        {
            ofs.write(data, data_length);
            std::cerr << "Client write:" << data_length << std::endl;
            return true;
        });
    ofs.close();
    REQUIRE(resp != nullptr);
    REQUIRE(resp->status == 200);
    std::remove(local_file.c_str());
}
