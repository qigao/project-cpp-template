#ifndef CPP_CORE_PROJECT_HTTP_HANDLER_HPP
#define CPP_CORE_PROJECT_HTTP_HANDLER_HPP

#include "config/pch_headers.hpp"

class HttpJsonHandler
{
public:
    void getMsg(httplib::Request const& req, httplib::Response& res);
    void postMsg(httplib::Request const& req, httplib::Response& res);
    void dump(httplib::Request const& req, httplib::Response& res);
    void web_hook(httplib::Request const& req, httplib::Response& res);
    HttpJsonHandler();

private:
    std::shared_ptr<BS::thread_pool> pool;
    std::shared_ptr<spdlog::logger> logger;
};

#endif // CPP_CORE_PROJECT_HTTP_HANDLER_HPP
