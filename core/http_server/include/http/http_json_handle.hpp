#ifndef CPP_CORE_PROJECT_HTTP_HANDLER_HPP
#define CPP_CORE_PROJECT_HTTP_HANDLER_HPP

#include "hash_function.hpp"
#include <BS_thread_pool.hpp>
#include <httplib.h>
#include <memory>
class HttpJsonHandler
{
public:
    void getMsg(httplib::Request const& req, httplib::Response& res);
    void postMsg(httplib::Request const& req, httplib::Response& res);
    void dump(httplib::Request const& req, httplib::Response& res);
    void web_hook(httplib::Request const& req, httplib::Response& res);
};

#endif // CPP_CORE_PROJECT_HTTP_HANDLER_HPP
