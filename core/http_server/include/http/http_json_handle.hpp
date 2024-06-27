#ifndef CPP_CORE_PROJECT_HTTP_HANDLER_HPP
#define CPP_CORE_PROJECT_HTTP_HANDLER_HPP

#include "http_lib_header.hpp"
#include <BS_thread_pool.hpp>

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
};

#endif // CPP_CORE_PROJECT_HTTP_HANDLER_HPP
