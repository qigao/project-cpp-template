#include "http_client_lib.h"
#include "http_file_download.hpp"
#include <thread>

int Sum(int const pLhs, int const pRhs) { return pLhs + pRhs; }

void async_file_download(char const* url, char const* local_filename)
{
    HttpFileDownload downloader;
    std::thread thr(&HttpFileDownload::doDownload, downloader, url,
                    local_filename);
    thr.detach();
}

void sync_file_download(char const* url, char const* local_filename)
{
    HttpFileDownload downloader;
    downloader.doDownload(url, local_filename);
}

void post_json_request(char const* url, char const* json) {}
