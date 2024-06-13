#ifndef CPP_CORE_PROJECT_HTTP_HANDLER_HPP
#define CPP_CORE_PROJECT_HTTP_HANDLER_HPP

#include <fmt/core.h>
#include <httplib.h>

class HttpHandler
{
public:
    void getMsg(httplib::Request const& req, httplib::Response& res);
    void postMsg(httplib::Request const& req, httplib::Response& res);
    void patchMsg(httplib::Request const& req, httplib::Response& res);
    void deleteMsg(httplib::Request const& req, httplib::Response& res);
};

#endif // CPP_CORE_PROJECT_HTTP_HANDLER_HPP
