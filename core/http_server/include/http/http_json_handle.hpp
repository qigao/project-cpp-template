#ifndef CPP_CORE_PROJECT_HTTP_HANDLER_HPP
#define CPP_CORE_PROJECT_HTTP_HANDLER_HPP

#include <fmt/core.h>
#include <httplib.h>
#include <rapidjson/document.h>


class HttpHandler
{
public:
    void getMsg(const httplib::Request& req, httplib::Response& res);
    void postMsg(const httplib::Request& req, httplib::Response& res);
    void patchMsg(const httplib::Request& req, httplib::Response& res);
    void deleteMsg(const httplib::Request& req, httplib::Response& res);


};

#endif // CPP_CORE_PROJECT_HTTP_HANDLER_HPP
