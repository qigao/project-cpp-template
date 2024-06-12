#include "http_file_download.hpp"
#include "http_file_transfer.h"
#include <thread>

int Sum(const int pLhs, const int pRhs) { return pLhs + pRhs; }

void async_file_download(const char* url, const char* local_filename)
{
    HttpFileDownload downloader;
    std::thread thr(&HttpFileDownload::doDownload, downloader, url,
                    local_filename);
    thr.detach();
}

void sync_file_download(const char* url, const char* local_filename)
{
    HttpFileDownload downloader;
    downloader.doDownload(url, local_filename);
}
