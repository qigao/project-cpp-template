#include "constants.hpp"
#include "fs.hpp"
#include "http/http_file_handle.hpp"
#include "http/http_json_handle.hpp"
#include "http/http_server.hpp"
#include "http/http_web_hook.hpp"
#include "http_lib_header.hpp"
#include <cstdio>
#include <functional>
#include <gtest/gtest.h>
#include <memory>
#include <yyjson.h>

#define DEMO_JSON "{\"msg\":\"demo\",\"id\":200}"
static char const* HOST = "127.0.0.1";
static int const PORT = 5061;
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
TEST(HttpServerTest, checkSharedFolder)
{
    HttpServer svr(PORT);
    svr.setSharedFolder("test");
    EXPECT_EQ(svr.getSharedFolder(), "test");
}
// Handler function to test
void handle_get(httplib::Request const& req, httplib::Response& res)
{
    res.set_content("Hello World!", "text/plain");
}

TEST(HttpServerTest, getHelloJson)
{
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
    EXPECT_NE(resp, nullptr);
    EXPECT_EQ(resp->status, 200);
    auto result = parse_json(resp->body.data(), resp->body.length());
    EXPECT_EQ(result.second, 100);
    ASSERT_EQ(result.first, "demo");
}
TEST(HttpServerTest, postJson)
{
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
            EXPECT_FALSE(svr.is_running());
        });

    svr.wait_until_ready();
    // Send request and check response
    httplib::Client cli(HOST, PORT);
    httplib::Headers headers = {{"Content-Type", "application/json"}};
    auto resp = cli.Post("/hello/100", headers, DEMO_JSON, APP_JSON);
    EXPECT_NE(resp, nullptr);
    EXPECT_EQ(resp->status, 200);

    auto result = parse_json(resp->body.data(), resp->body.length());
    EXPECT_EQ(result.second, 4001);
    ASSERT_EQ(result.first, "demo");
}
TEST(HttpServerTest, postJsonReturn4001)
{
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
    EXPECT_NE(resp, nullptr);
    EXPECT_EQ(resp->status, 200);
    auto result = parse_json(resp->body.data(), resp->body.length());
    EXPECT_EQ(result.second, 4001);
    ASSERT_EQ(result.first, "demo");
}
TEST(HttpServerTest, multipartForm)
{
    HttpServer svr(PORT);
    svr.setSharedFolder(SHARED_FOLDER);
    // Register handler
    auto handler = std::make_shared<HttpFileHandle>(SHARED_FOLDER);
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

    EXPECT_NE(resp, nullptr);
    EXPECT_EQ(resp->status, 200);
}

TEST(HttpServerTest, streamUpload)
{
    HttpServer svr(PORT);
    svr.setSharedFolder(SHARED_FOLDER);
    // Register handler
    auto handler = std::make_shared<HttpFileHandle>(SHARED_FOLDER);
    svr.PostWithReader(
        R"(/upload)", [&](httplib::Request const& req, httplib::Response& res,
                          httplib::ContentReader const& content_reader)
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

    EXPECT_NE(resp, nullptr);
    EXPECT_EQ(resp->status, 200);

    file.close();
    std::remove(filename.c_str());
}

TEST(HttpServerTest, multipartUpload)
{
    HttpServer svr(PORT);
    svr.setSharedFolder(SHARED_FOLDER);
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

    EXPECT_NE(resp, nullptr);
    EXPECT_EQ(resp->status, 200);
}

TEST(HttpServerTest, getFileLists)
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

TEST(HttpServerTest, downloadFile)
{
    HttpServer svr(PORT);
    svr.setSharedFolder(SHARED_FOLDER);
    auto handler = std::make_shared<HttpFileHandle>(SHARED_FOLDER);
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
        [&](httplib::Response const& response)
        {
            std::cerr << "Client read:" << response.status << std::endl;
            return true;
        },
        [&](char const* data, size_t data_length)
        {
            ofs.write(data, data_length);
            std::cerr << "Client write:" << data_length << std::endl;
            return true;
        });
    ofs.close();
    EXPECT_NE(resp, nullptr);
    EXPECT_EQ(resp->status, 200);
    std::remove(local_file.c_str());
}
TEST(HttpServerTest, dump)
{
    HttpServer svr(PORT);
    // Register handler
    auto handler = std::make_shared<HttpJsonHandler>();
    svr.Post("/dump", std::bind(&HttpJsonHandler::dump, handler, _1, _2));
    svr.start();
    auto se = httplib::detail::scope_exit([&] { svr.stop(); });

    // Send request and check response
    httplib::Client cli(HOST, PORT);
    httplib::Headers headers = {
        {"X-Hub-Signature-256",
         "sha256="
         "fa090c43f504e30497b7ef441500f9666e13aa1368e394d7d668eeb7de983bcb"}};
    auto resp = cli.Post("/dump", DEMO_JSON, APP_JSON);
    EXPECT_NE(resp, nullptr);
    EXPECT_EQ(resp->status, 200);
    /*     auto result = parse_json(resp->body.data(), resp->body.length());
        EXPECT_EQ(result.second, 200);
        ASSERT_EQ(result.first, std::string("demo"));
        sleep(5); */
}

TEST(HttpServerTest, webHookDemo)
{
    WebHook::Construct("http://127.0.0.1:5061/dump");
    WebHook::GetInstance()->set_header(USER_AGENT, "MVIEW");
    HttpServer svr(PORT);
    // Register handler
    auto handler = std::make_shared<HttpJsonHandler>();
    svr.Post("/webhook",
             std::bind(&HttpJsonHandler::web_hook, handler, _1, _2));
    svr.Post("/dump", std::bind(&HttpJsonHandler::dump, handler, _1, _2));
    svr.start();
    auto se = httplib::detail::scope_exit([&] { svr.stop(); });

    // Send request and check response
    httplib::Client cli(HOST, PORT);
    auto resp = cli.Post("/webhook", DEMO_JSON, APP_JSON);
    EXPECT_NE(resp, nullptr);
    EXPECT_EQ(resp->status, 200);
    auto result = parse_json(resp->body.data(), resp->body.length());
    EXPECT_EQ(result.second, 200);
    ASSERT_EQ(result.first, "demo");
}
