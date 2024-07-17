#include "fs.hpp"

#include <catch2/catch_test_macros.hpp>
#include <chrono>
#include <httplib.h>
#include <thread>
#include <yyjson.h>
class MockServer
{
public:
    MockServer() : svr(std::make_shared<httplib::Server>())
    {
        svr->Get("/test", [](httplib::Request const&, httplib::Response& res)
                 { res.set_content("Hello, World!", "text/plain"); });

        svr->Get("/json",
                 [](httplib::Request const&, httplib::Response& res) {
                     res.set_content("{\"message\":\"Hello, JSON!\"}",
                                     "application/json");
                 });

        svr->Get("/error",
                 [](httplib::Request const&, httplib::Response& res)
                 {
                     res.status = 500;
                     res.set_content("Internal Server Error", "text/plain");
                 });
        svr->Post(
            "/api/user",
            [](httplib::Request const& req, httplib::Response& res)
            {
                // Parse incoming JSON
                yyjson_doc* doc =
                    yyjson_read(req.body.c_str(), req.body.length(), 0);
                yyjson_val* root = yyjson_doc_get_root(doc);

                // Extract values
                const char* name = yyjson_get_str(yyjson_obj_get(root, "name"));
                int age = yyjson_get_int(yyjson_obj_get(root, "age"));

                // Create response JSON
                char* json = nullptr;
                size_t len = 0;
                yyjson_mut_doc* mut_doc = yyjson_mut_doc_new(NULL);
                yyjson_mut_val* mut_root = yyjson_mut_obj(mut_doc);
                yyjson_mut_doc_set_root(mut_doc, mut_root);

                yyjson_mut_obj_add_str(mut_doc, mut_root, "status", "success");
                yyjson_mut_obj_add_str(mut_doc, mut_root, "message",
                                       "User created");
                yyjson_mut_obj_add_str(mut_doc, mut_root, "name", name);
                yyjson_mut_obj_add_int(mut_doc, mut_root, "age", age);

                json = yyjson_mut_write(mut_doc, 0, &len);

                res.set_content(json, len, "application/json");

                // Clean up
                yyjson_doc_free(doc);
                yyjson_mut_doc_free(mut_doc);
                free(json);
            });
        svr->Post(
            "/upload/(.*)",
            [this](httplib::Request const& req, httplib::Response& res,
                   httplib::ContentReader const& content_reader)
            {
                if (req.is_multipart_form_data())
                {
                    httplib::MultipartFormDataItems files;
                    content_reader(
                        [&](httplib::MultipartFormData const& file)
                        {
                            files.push_back(file);
                            return true;
                        },
                        [&](char const* data, size_t data_length)
                        {
                            auto last_file = files.back();
                            auto file_path = "./remote_" + last_file.filename;

                            auto contentType = last_file.content_type;
                            fs_write(std::string(file_path), data, data_length);
                            std::cout << "upload file: " << last_file.filename
                                      << " file type: " << contentType
                                      << last_file.content.size() << std::endl;
                            return true;
                        });
                    if (files.empty())
                    {
                        res.status = 400;
                        res.set_content("Invalid request", "text/plain");
                    }
                }
                else
                {
                    std::string body;
                    content_reader(
                        [&](char const* data, size_t data_length)
                        {
                            body.append(data, data_length);
                            return true;
                        });
                    std::string filename = req.matches[1];
                    std::ofstream file(filename, std::ios::binary);

                    if (!file)
                    {
                        res.status = 500;
                        res.set_content("Failed to create file", "text/plain");
                        return;
                    }

                    file.write(body.c_str(), body.length());
                    file.close();

                    if (file)
                    {
                        res.set_content("File uploaded successfully",
                                        "text/plain");
                    }
                    else
                    {
                        res.status = 500;
                        res.set_content("Failed to save file", "text/plain");
                    }
                }
            });

        svr->Get("/download",
                 [](httplib::Request const&, httplib::Response& res)
                 {
                     std::string file_content = "This is a mock file content";
                     res.set_content(file_content, "application/octet-stream");
                     res.set_header("Content-Disposition",
                                    "attachment; filename=\"mockfile.txt\"");
                 });
        thread = std::thread([this]() { svr->listen("localhost", 5060); });
        std::this_thread::sleep_for(
            std::chrono::milliseconds(100)); // Give the server time to start
    }
    std::string get_uploaded_filename() const { return uploaded_filename; }
    std::string get_uploaded_content() const { return uploaded_content; }
    ~MockServer()
    {
        svr->stop();
        thread.join();
    }

private:
    std::shared_ptr<httplib::Server> svr;
    std::thread thread;
    std::string uploaded_filename;
    std::string uploaded_content;
    std::map<size_t, std::vector<char>> uploadedChunks_;
    std::vector<std::vector<char>> uploadedFiles_;
};
