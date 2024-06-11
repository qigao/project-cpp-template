#include "http/download.hpp"
#include "spdlog/async.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"
#include <cstdlib>
#include <iostream>
#include <string>
std::string find_filename(std::string const& url)
{
    std::string::size_type pos1 = url.find_first_of("?#", 0);
    if (pos1 == std::string::npos)
        pos1 = url.length();
    std::string::size_type pos0 = url.rfind("/", pos1);
    if (pos0 == std::string::npos)
        pos0 = url.length();
    else
        pos0 = pos0 + 1;
    return url.substr(pos0, pos1 - pos0);
}
void init_multi_sink_logger()
{
    spdlog::init_thread_pool(8192, 1);
    spdlog::init_thread_pool(8192, 1);
    auto stdout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    auto rotating_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
        "mylog.txt", 1024 * 1024 * 10, 3);
    std::vector<spdlog::sink_ptr> sinks{stdout_sink, rotating_sink};
    auto logger = std::make_shared<spdlog::async_logger>(
        "loggername", sinks.begin(), sinks.end(), spdlog::thread_pool(),
        spdlog::async_overflow_policy::block);
    spdlog::register_logger(logger);
}
int main(int argc, char* argv[])
try
{
    init_multi_sink_logger();
    std::string path = "https://mirrors.tuna.tsinghua.edu.cn/ubuntu-releases/"
                       "14.04/ubuntu-14.04.6-server-amd64.template";
    std::string filename = "";
    if (argc > 1)
        path = argv[1];
    if (argc > 2)
        filename = argv[2];
    if (filename.empty())
        filename = find_filename(path);
    if (filename.empty())
        filename = "unnamed";
    // download(path, filename);
    Downloader downloader;
    downloader.doDownload(path, filename);
    return EXIT_SUCCESS;
}
catch (std::exception const& e)
{
    std::cerr << "[c++ exception] " << e.what() << std::endl;
    return EXIT_FAILURE;
}
catch (...)
{
    std::cerr << "[c++ exception] "
              << "<UNKNOWN>" << std::endl;
    return EXIT_FAILURE;
}
