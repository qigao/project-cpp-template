#include <cstdlib>

#include "http/http_file_handle.hpp"
#include "http/http_json_handle.hpp"
#include "http/http_server.hpp"
#include <functional>
#include <memory>
#include <thread>

using namespace httplib;
using namespace std::placeholders;

void startServerSSL()
{
    HttpServer server(4443);
    auto shared_folder = "./shared";
    bool delete_after_download = true;
    std::shared_ptr<HttpFileHandle> mHttpFile =
        std::make_shared<HttpFileHandle>(shared_folder, delete_after_download);
    std::shared_ptr<HttpJsonHandler> mJsonHandle =
        std::make_shared<HttpJsonHandler>();
    server.Get("/",
               std::bind(&HttpFileHandle::list_upload_form, mHttpFile, _1, _2));
    server.PostWithReader(
        "/upload",
        std::bind(&HttpFileHandle::handle_file_upload, mHttpFile, _1, _2, _3));
    server.Post(
        "/download/(*)",
        std::bind(&HttpFileHandle::handle_file_download, mHttpFile, _1, _2));
    server.Get("/list", std::bind(&HttpFileHandle::handle_file_lists, mHttpFile,
                                  _1, _2));
    server.Post("/webhook",
                std::bind(&HttpJsonHandler::web_hook, mJsonHandle, _1, _2));
    server.Post("/dump/(.*)",
                std::bind(&HttpJsonHandler::dump, mJsonHandle, _1, _2));
    server.start();
}

int main(int argc, char** argv)
{
    int sec = 2;
    if (argc > 1)
    {
        sec = atoi(argv[1]);
    }
    std::thread serverSSL(startServerSSL);
    serverSSL.join();
}
