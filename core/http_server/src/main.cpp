#include <cstdlib>

#include "helper/constants.hpp"
#include "helper/helpers.hpp"
#include "http/HttpServer.hpp"
#include <fmt/core.h>
#include <functional>
#include <httplib.h>
#include <string>
using namespace httplib;
using namespace std::placeholders;

void startServerSSL()
{
    HttpServer server(4443);
    auto shared_folder = "./shared";
    std::shared_ptr<HttpFileHandle> mHttpFile =
        std::make_shared<HttpFileHandle>(shared_folder);

    server.Get("/upload",
               std::bind(&HttpFileHandle::uploadForm, mHttpFile, _1, _2));
    server.PostWithContentHandler(
        "/upload",
        std::bind(&HttpFileHandle::handle_file_request, mHttpFile, _1, _2, _3));
    server.Post(
        "/download/(*)",
        std::bind(&HttpFileHandle::handle_range_request, mHttpFile, _1, _2));
    server.Get("/list",
               std::bind(&HttpFileHandle::GetFileList, mHttpFile, _1, _2));

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
