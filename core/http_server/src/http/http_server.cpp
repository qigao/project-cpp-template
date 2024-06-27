#include "http/http_server.hpp"
#include "helpers.hpp"
#include "spdlog/spdlog.h"
#include "task_queue.hpp"
#include <exception>
#include <fmt/core.h>
#include <functional>
#include <httplib.h>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <thread>
using namespace std::placeholders;

HttpServer::HttpServer(int port, unsigned int numThreads,
                       std::string const& certFile, std::string const& keyFile,
                       std::string const& rootCaFile)
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
        spdlog::error("server init error");
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

void HttpServer::error_handler(httplib::Request const& /* req */,
                               httplib::Response& res)
{
    auto error_info = fmt::format(HTTP_TEXT_RESPONSE_FORMATTER, res.status);
    res.set_content(error_info, "text/html");
}
void HttpServer::log_handler(httplib::Request const& req,
                             httplib::Response const& res)
{
    dump_request_response(req, res);
}

void HttpServer::post_route_handler(httplib::Request const& req,
                                    httplib::Response& res)
{
    res.set_header("User-Agent", "MicroView http file server");
}
