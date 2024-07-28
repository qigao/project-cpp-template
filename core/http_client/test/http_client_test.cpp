#include "http_client_lib.h"
#include "mock_server.hpp"

#include <catch2/catch_all.hpp>
#include <catch2/catch_test_macros.hpp>
#include <cstring>
#include <httplib.h>
template <typename T>
T custom_min(T a, T b)
{
    return (a < b) ? a : b;
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
TEST_CASE("HTTP SUM test", "[httplib]") { REQUIRE(Sum(2, 2) == 4); }
TEST_CASE("HTTP GET request", "[httplib]")
{
    MockServer mock_server;
    httplib::Client cli("localhost", 5060);

    SECTION("Successful GET request")
    {
        auto res = cli.Get("/test");
        REQUIRE(res);
        REQUIRE(res->status == 200);
        REQUIRE(res->body == "Hello, World!");
        REQUIRE(res->get_header_value("Content-Type") == "text/plain");
    }

    SECTION("JSON GET request")
    {
        auto res = cli.Get("/json");
        REQUIRE(res);
        REQUIRE(res->status == 200);
        REQUIRE(res->body == "{\"message\":\"Hello, JSON!\"}");
        REQUIRE(res->get_header_value("Content-Type") == "application/json");
    }

    SECTION("Error GET request")
    {
        auto res = cli.Get("/error");
        REQUIRE(res);
        REQUIRE(res->status == 500);
        REQUIRE(res->body == "Internal Server Error");
    }

    SECTION("Non-existent endpoint")
    {
        auto res = cli.Get("/nonexistent");
        REQUIRE(res);
        REQUIRE(res->status == 404);
    }
}

TEST_CASE("HTTP GET request with headers", "[httplib]")
{
    MockServer mock_server;
    httplib::Client cli("localhost", 5060);

    SECTION("GET request with custom header")
    {
        httplib::Headers headers = {{"X-Custom-Header", "TestValue"}};
        auto res = cli.Get("/test", headers);
        REQUIRE(res);
        REQUIRE(res->status == 200);
        // Note: In a real scenario, you'd typically check if the server
        // received and processed the custom header
    }
}

TEST_CASE("HTTP GET request with timeout", "[httplib]")
{
    MockServer mock_server;
    httplib::Client cli("localhost", 5060);

    SECTION("GET request with short timeout")
    {
        cli.set_connection_timeout(1); // 1 second timeout
        auto res = cli.Get("/test");
        REQUIRE(res);
        REQUIRE(res->status == 200);
    }
}
TEST_CASE("HTTP POST request with JSON", "[httplib][yyjson]")
{
    MockServer mock_server;
    httplib::Client cli("localhost", 5060);

    SECTION("Successful POST request")
    {
        // Prepare request JSON
        char const* request_json = R"({"name":"John Doe","age":30})";

        // Send POST request
        auto res = cli.Post("/api/user", request_json, "application/json");

        REQUIRE(res);
        REQUIRE(res->status == 200);
        REQUIRE(res->get_header_value("Content-Type") == "application/json");

        // Parse response JSON
        yyjson_doc* doc = yyjson_read(res->body.c_str(), res->body.length(), 0);
        yyjson_val* root = yyjson_doc_get_root(doc);

        // Check response values
        REQUIRE(strcmp(yyjson_get_str(yyjson_obj_get(root, "status")),
                       "success") == 0);
        REQUIRE(strcmp(yyjson_get_str(yyjson_obj_get(root, "message")),
                       "User created") == 0);
        REQUIRE(strcmp(yyjson_get_str(yyjson_obj_get(root, "name")),
                       "John Doe") == 0);
        REQUIRE(yyjson_get_int(yyjson_obj_get(root, "age")) == 30);

        // Clean up
        yyjson_doc_free(doc);
    }
}

TEST_CASE("HTTP POST file upload", "[httplib]")
{
    MockServer mock_server;
    httplib::Client cli("localhost", 5060);

    SECTION("Successful file upload")
    {
        // Create a temporary file
        std::string filename = "test_file.txt";
        std::string file_content = "This is a test file content.";
        std::ofstream temp_file(filename);
        temp_file << file_content;
        temp_file.close();
        std::remove(filename.c_str());
        // Prepare multipart form data
        httplib::MultipartFormDataItems items = {
            {"file", file_content, filename, "text/plain"}};

        // Send POST request
        auto res = cli.Post("/upload/m.txt", items);

        REQUIRE(res);
        REQUIRE(res->status == 200);
        // Clean up
        std::remove(filename.c_str());
    }
}

TEST_CASE("HTTP POST file upload by dll", "[httplib]")
{
    MockServer mock_server;
    auto http_client_api = new_http_client("client.yml");
    http_request_initialize(http_client_api);

    SECTION("Successful file upload")
    {
        std::string filename = "test_file_client.txt";
        std::string file_content = "This is a test file content.";
        std::ofstream temp_file(filename);
        temp_file << file_content;
        temp_file.close();
        // Send POST request
        auto res =
            post_file_request(http_client_api, "/upload/txt", filename.c_str());

        REQUIRE(res == 200);

        // Clean up
        std::remove(filename.c_str());
    }
}
bool downloadFile(std::string const& url, std::string const& outputPath)
{
    httplib::Client cli(url);
    auto res = cli.Get("/download");
    if (res && res->status == 200)
    {
        std::ofstream outFile(outputPath, std::ios::binary);
        outFile.write(res->body.c_str(), res->body.length());
        return true;
    }
    return false;
}

TEST_CASE("HTTP file download", "[httplib]")
{
    MockServer mock_server;

    SECTION("Successful file download")
    {
        std::string outputPath = "downloaded_file.txt";
        bool result = downloadFile("http://localhost:5060", outputPath);

        REQUIRE(result);

        // Verify file content
        std::ifstream file(outputPath);
        std::stringstream buffer;
        buffer << file.rdbuf();
        REQUIRE(buffer.str() == "This is a mock file content");

        // Clean up
        std::remove(outputPath.c_str());
    }
}
TEST_CASE("HTTP file download by dll", "[httplib]")
{
    MockServer mock_server;
    auto http_client_api = new_http_client("client.yml");
    http_request_initialize(http_client_api);

    SECTION("Successful file download")
    {
        std::string outputPath = "downloaded_file.txt";
        file_download(http_client_api, "/download", ".", outputPath.c_str());

        // Verify file content
        std::ifstream file(outputPath);
        std::stringstream buffer;
        buffer << file.rdbuf();
        REQUIRE(buffer.str() == "This is a mock file content");

        // Clean up
        std::remove(outputPath.c_str());
    }
}

TEST_CASE("HTTP POST upload large byte array  by dll", "[httplib]")
{
    MockServer mock_server;
    auto http_client_api = new_http_client("client.yml");
    http_request_initialize(http_client_api);

    SECTION("Upload large file (10MB)")
    {
        size_t const file_size = 10 * 1024 * 1024; // 10MB
        std::string large_content = generate_large_content(file_size);

        // Send POST request
        auto res = post_file_stream(http_client_api, "/upload/large_file.txt",
                                    large_content.data(), file_size);

        REQUIRE(res == 200);

        // Clean up
        std::remove("large_file.txt");
    }
}
TEST_CASE("HTTP POST large file upload by dll", "[httplib]")
{
    MockServer mock_server;
    auto http_client_api = new_http_client("client.yml");
    http_request_initialize(http_client_api);

    SECTION("Upload large file (10MB)")
    {
        size_t const file_size = 10 * 1024 * 1024; // 10MB
        std::string large_content = generate_large_content(file_size);
        std::string filename = "large_file.txt";
        std::ofstream ofs(filename, std::ofstream::binary);
        ofs.write(large_content.c_str(), file_size);
        ofs.close();
        // Send POST request
        auto res = post_file_request(http_client_api, "/upload/large",
                                     filename.c_str());

        REQUIRE(res == 200);
        std::remove(filename.c_str());
    }
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
TEST_CASE("File download test", "[file_download]")
{

    httplib::Server svr;
    svr.Get("/download/(.*)", handle_file_download);

    // Start the server in a separate thread
    std::thread server_thread([&]() { svr.listen("localhost", 8080); });

    // Give the server a moment to start
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    SECTION("Successful download")
    {
        // Create a test file

        std::string const unix_file = "test.jpg";
        std::string const local_file = "received_image.jpg";
        size_t const file_size_mb = 2;

        // Generate a large test file
        generate_large_file(unix_file, file_size_mb);
        // Open output file
        std::ofstream outfile(local_file, std::ios::binary);
        if (!outfile)
        {
            std::cerr << "Failed to open output file: " << local_file
                      << std::endl;
            REQUIRE(false);
        }
        // Define content receiver callback
        size_t total_bytes = 0;
        auto content_receiver = [&](char const* data, size_t data_length)
        {
            outfile.write(data, data_length);
            total_bytes += data_length;
            return true; // Return false to cancel the download
        };
        // Make a request to download the file
        httplib::Client cli("localhost", 8080);
        auto res = cli.Get("/download/test.jpg", content_receiver);
        outfile.close();
        REQUIRE(res);
        REQUIRE(res->status == 200);
        REQUIRE(res->get_header_value("Content-Type") ==
                "application/octet-stream");
        REQUIRE(res->get_header_value("Content-Disposition") ==
                "attachment; filename=\"test.jpg\"");

        REQUIRE(compare_files(unix_file, local_file));
        // Clean up
        std::remove(unix_file.c_str());
        std::remove(local_file.c_str());
    }

    SECTION("File not found")
    {
        httplib::Client cli("localhost", 8080);
        auto res = cli.Get("/download/nonexistent.txt");

        REQUIRE(res);
        REQUIRE(res->status == 404);
        REQUIRE(res->body == "File not found");
    }

    // Stop the server
    svr.stop();
    server_thread.join();
}

TEST_CASE("HTTP Client DLL functionality", "[http_client]")
{
    SECTION("Creation and destruction")
    {
        http_client_handle handle = new_http_client("client.yml");
        REQUIRE(handle != nullptr);
        http_request_initialize(handle);
    }

    SECTION("Setting headers")
    {
        http_client_handle handle = new_http_client("client.yml");
        REQUIRE(handle != nullptr);
        http_request_initialize(handle);
    }
}
TEST_CASE("http request post JSON ", "[httplib][yyjson]")
{
    MockServer mock_server;
    auto http_client_api = new_http_client("client.yml");
    http_request_initialize(http_client_api);

    SECTION("Successful POST request")
    {
        // Prepare request JSON
        char const* request_json = R"({"name":"John Doe","age":30})";

        // Send POST request
        char* data = new char[1024];
        auto resp_code = post_json_with_resp(http_client_api, "/api/user",
                                             request_json, data);

        REQUIRE(resp_code == 200);
        std::cout << "resp body: " << data << std::endl;
        yyjson_doc* doc = yyjson_read(data, strlen(data) - 1, 0);
        if (doc == nullptr)
        {
            std::cerr << "Failed to parse JSON" << std::endl;
            // Handle error appropriately
            return;
        }
        yyjson_val* root = yyjson_doc_get_root(doc);
        if (root == nullptr || !yyjson_is_obj(root))
        {
            std::cerr << "Invalid JSON structure" << std::endl;
            yyjson_doc_free(doc);
            return;
        }

        yyjson_val* status = yyjson_obj_get(root, "status");
        if (status != nullptr)
        {
            std::cout << "Status: " << yyjson_get_str(status) << std::endl;
        }

        yyjson_val* message = yyjson_obj_get(root, "message");
        if (message != nullptr)
        {
            std::cout << "Message: " << yyjson_get_str(message) << std::endl;
        }
        yyjson_doc_free(doc);
    }
    SECTION("Successful POST request without check resp")
    {
        // Prepare request JSON
        char const* request_json = R"({"name":"John Doe","age":30})";

        // Send POST request
        auto resp_code =
            post_json_request(http_client_api, "/api/user", request_json);

        REQUIRE(resp_code == 200);
    }
}
