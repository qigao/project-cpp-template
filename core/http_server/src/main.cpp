#include <cstdlib>

#include "http/http_file_handle.hpp"
#include "http/http_server.hpp"
#include <functional>
#include <httplib.h>
#include <memory>
#include <thread>

using namespace httplib;
using namespace std::placeholders;

void startServerSSL()
{
    HttpServer server(4443);
    auto shared_folder = "./shared";
    std::shared_ptr<HttpFileHandle> mHttpFile =
        std::make_shared<HttpFileHandle>(shared_folder);

    server.Get("/upload",
               std::bind(&HttpFileHandle::list_upload_form, mHttpFile, _1, _2));
    server.PostWithContentHandler(
        "/upload",
        std::bind(&HttpFileHandle::handle_file_upload, mHttpFile, _1, _2, _3));
    server.Post(
        "/download/(*)",
        std::bind(&HttpFileHandle::handle_file_download, mHttpFile, _1, _2));
    server.Get("/list", std::bind(&HttpFileHandle::handle_file_lists, mHttpFile,
                                  _1, _2));

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
