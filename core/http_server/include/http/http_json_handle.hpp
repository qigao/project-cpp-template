#ifndef CPP_CORE_PROJECT_HTTP_HANDLER_HPP
#define CPP_CORE_PROJECT_HTTP_HANDLER_HPP

#include <fmt/core.h>
#include <httplib.h>
#include <memory>
class HttpJsonHandler
{
public:
    HttpJsonHandler();
    ~HttpJsonHandler();
    void getMsg(httplib::Request const& req, httplib::Response& res);
    void postMsg(httplib::Request const& req, httplib::Response& res);
    void patchMsg(httplib::Request const& req, httplib::Response& res);
    void deleteMsg(httplib::Request const& req, httplib::Response& res);
    void set_webhook(std::string const& web_hook);

private:
    class JsonImpl;
    std::unique_ptr<JsonImpl> pimpl;
};

#endif // CPP_CORE_PROJECT_HTTP_HANDLER_HPP
