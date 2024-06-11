
#ifndef CPP_CORE_PROJECT_HTTP_HANDLER_HPP
#define CPP_CORE_PROJECT_HTTP_HANDLER_HPP

#include <cstdio>
#include <fmt/core.h>
#include <httplib.h>
#include <map>
#include <rapidjson/document.h>
#include <vector>

class HttpHandler
{
public:
    void getMsg(const httplib::Request& req, httplib::Response& res);
    void postMsg(const httplib::Request& req, httplib::Response& res);
    void patchMsg(const httplib::Request& req, httplib::Response& res);
    void deleteMsg(const httplib::Request& req, httplib::Response& res);

private:
    bool bValidReq(rapidjson::Document jsonDoc, std::vector<std::string> keys);
};

#endif // CPP_CORE_PROJECT_HTTP_HANDLER_HPP
