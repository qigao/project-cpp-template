#include "http/HttpServer.hpp"
#include "helper/task_queue.hpp"
#include "spdlog/spdlog.h"
#include <functional>
#include <httplib.h>
#include <iostream>
#include <memory>
#include <thread>
using namespace std::placeholders;
HttpServer::HttpServer(int port, unsigned int numThreads,
                       const std::string& certFile, const std::string& keyFile,
                       const std::string& rootCaFile)
    : mPort(port), mBoundPort(0), mStopSignal(false)

{
#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
    mServer =
        std::make_shared<httplib::SSLServer>(certFile, keyFile, rootCaFile);
#else
    mServer = std::make_shared<httplib::Server>();
#endif
    mServer->new_task_queue = [numThreads]
    { return new TaskQueue(numThreads); };
    mServer->set_keep_alive_max_count(10);
    mServer->set_keep_alive_timeout(5);
    mServer->set_payload_max_length(1024 * 1024 * 512);
}

HttpServer::~HttpServer()
{
    try
    {
        stop();
    }
    catch (std::exception const& e)
    {
        std::clog << "[c++ exception] " << e.what() << std::endl;
    }
    catch (...)
    {
    }
}

void HttpServer::start()
{
    mServer->set_logger(std::bind(&HttpServer::log_handler, this, _1, _2));
    mServer->set_error_handler(
        std::bind(&HttpServer::error_handler, this, _1, _2));
    mServer->set_post_routing_handler(
        std::bind(&HttpServer::post_route_handler, this, _1, _2));
    if (!mSharedFolder.empty())
    {
        mServer->set_base_dir(mSharedFolder);
    }
    if (mBoundPort != 0)
    {
        return;
    }
    spdlog::info("server start ....");
    mStopSignal = false;
    if (!mServer)
    {
        mServer = std::make_shared<httplib::Server>();
    }
    std::string host = "0.0.0.0";
    if (mPort == 0)
    {
        mBoundPort = mServer->bind_to_any_port(host);
        if (mBoundPort <= 0 || mBoundPort > 65536)
        {
            spdlog::error("invalid bind port {} ", mBoundPort);
            throw std::runtime_error("bind_to_any_port");
        }
    }
    else
    {
        if (!mServer->bind_to_port(host, mPort))
        {
            auto msg = fmt::format("bind error {}:{}", host, mPort);
            spdlog::error(msg);
            throw std::runtime_error(msg);
        }
        mBoundPort = mPort;
    }

    spdlog::info("bind to {}:{}", host, mBoundPort);

    std::exception_ptr ep;
    mListenThread = std::thread(
        [this, &ep]()
        {
            try
            {
                if (!mServer->listen_after_bind())
                {

                    throw std::runtime_error("listen_after_bind");
                }
            }
            catch (...)
            {
                if (!mStopSignal)
                {
                    ep = std::current_exception();
                }
            }
        });
    while (!mServer->is_valid() || !mServer->is_running())
    {
        if (ep)
        {
            std::rethrow_exception(ep);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    spdlog::info("server started. ");
}

void HttpServer::stop()
{
    if (!mServer)
    {
        return;
    }
    if (mBoundPort == 0)
    {
        return;
    }
    spdlog::info("server stop ...");
    mStopSignal = true;
    if (mServer->is_running())
    {
        mServer->stop();
    }
    if (mListenThread.joinable())
    {
        mListenThread.join();
        spdlog::info("unbound from 0.0.0.0:{}", mBoundPort);
    }
    mBoundPort = 0;
    mServer = nullptr;
    spdlog::info("server stopped.");
}

void HttpServer::error_handler(const httplib::Request& /* req */,
                               httplib::Response& res)
{
    char buf[BUFSIZ];
    auto formatter = "<p>Error Status: <span style='color:red;'>%d</span></p>";
    snprintf(buf, sizeof(buf), formatter, res.status);
    res.set_content(buf, "text/html");
}
void HttpServer::log_handler(const httplib::Request& req,
                             const httplib::Response& res)
{
    std::string s;
    char buf[BUFSIZ];
    s += buf;
    s += "-----------------------------------------------------\n";
    s += "Request Method\n";
    s += "-----------------------------------------------------\n";
    s += "\n";
    snprintf(buf, sizeof(buf), "%s %s %s", req.method.c_str(),
             req.version.c_str(), req.path.c_str());
    s += buf;
    s += "\n\n";
    s += "Request Header:\n";
    s += dump_headers(req.headers);
    std::string query;
    for (auto it = req.params.begin(); it != req.params.end(); ++it)
    {
        const auto& x = *it;
        snprintf(buf, sizeof(buf), "%c%s=%s",
                 (it == req.params.begin()) ? '?' : '&', x.first.c_str(),
                 x.second.c_str());
        query += buf;
    }
    snprintf(buf, sizeof(buf), "%s\n", query.c_str());
    s += buf;
    s += "Request Body:\n";
    if (!req.body.empty())
    {
        s += req.body;
    }
    s += buf;
    s += "\n";
    s += "-----------------------------------------------------\n";
    s += "Response:\n";
    s += "-----------------------------------------------------\n";
    s += "\n";
    s += "Response Header:\n";
    snprintf(buf, sizeof(buf), "%d %s\n", res.status, res.version.c_str());
    s += "\n";
    s += buf;
    s += dump_headers(res.headers);
    s += "\n";
    s += "Response Body:\n";
    s += "\n";
    if (!res.body.empty())
    {
        s += res.body;
    }
    s += "\n";
    spdlog::info("\n{}", s);
}

std::string HttpServer::dump_headers(const httplib::Headers& headers)
{
    std::string s;
    char buf[BUFSIZ];

    for (auto it = headers.begin(); it != headers.end(); ++it)
    {
        const auto& x = *it;
        snprintf(buf, sizeof(buf), "%s: %s\n", x.first.c_str(),
                 x.second.c_str());
        s += buf;
    }

    return s;
}
void HttpServer::post_route_handler(const httplib::Request& req,
                                    httplib::Response& res)
{
    res.set_header("User-Agent", "MicroView http file server");
}
