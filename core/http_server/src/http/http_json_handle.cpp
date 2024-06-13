#include "http/http_json_handle.hpp"
#include "constants.hpp"
#include "cpp_yyjson.hpp"
#include "fs.hpp"
#include "spdlog/spdlog.h"
#include <BS_thread_pool.hpp>
#include <fmt/core.h>
#include <httplib.h>
using namespace yyjson;

class HttpJsonHandler::JsonImpl
{
    std::string web_hook_url;
    void post_json_to_remote_web(char* const json);
    BS::thread_pool pool;

public:
    void set_web_hook(std::string const& url) { web_hook_url = url; }
};
void HttpJsonHandler::JsonImpl::post_json_to_remote_web(char* const json)
{
    std::string remote_host, relative_url;
    auto is_valid = breakup_url(web_hook_url, remote_host, relative_url);
    if (is_valid)
    {
#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
        auto cli = std::make_shared<httplib::SSLClient>(remote_host);
        cli->enable_server_certificate_verification(true);
#else
        auto cli = std::make_shared<httplib::Client>(remote_host);
#endif
        if (!cli->is_valid())
        {
            spdlog::error("cannot make remote call to {} with data {}",
                          web_hook_url, json);
            return;
        }
        pool.detach_task(
            [&]
            {
                auto resp = cli->Post(relative_url, json, APP_JSON);
                if (resp == nullptr || resp->status != 200)
                {
                    spdlog::error("failed to post json request");
                    return;
                }
            });
    }
}
HttpJsonHandler::HttpJsonHandler() : pimpl(std::make_unique<JsonImpl>()) {}
HttpJsonHandler::~HttpJsonHandler() = default;
void HttpJsonHandler::getMsg(httplib::Request const& req,
                             httplib::Response& res)
{
    auto pathVar = req.matches[1];
    auto obj = yyjson::object();
    obj.emplace("id", std::stoi(pathVar));
    obj.emplace("msg", "hello");
    auto result = obj.write();
    res.set_content(result.data(), APP_JSON);
}

void HttpJsonHandler::postMsg(httplib::Request const& req,
                              httplib::Response& res)
{
    auto pathVar = req.matches[1];
    spdlog::info("request info:{},path params: {}", req.body, pathVar.str());
    // Read JSON string
    auto val = read(req.body);
    auto obj = *val.as_object();
    // Key access to the JSON object class
    auto id = *obj["id"].as_int();
    auto msg = *obj["msg"].as_string();

    auto result = yyjson::object();
    result.emplace("id", id * 20 + 1);
    result.emplace("msg", msg);
    auto body = result.write();
    res.set_content(body.data(), APP_JSON);
}

void HttpJsonHandler::patchMsg(httplib::Request const& req,
                               httplib::Response& res)
{
    auto n = req.matches[1];
    auto pathVar = req.matches[1];
    spdlog::debug("request info:{},path params: {}", req.body, pathVar.str());
    // Read JSON string
    auto val = read(req.body);
    auto obj = *val.as_object();
    // Key access to the JSON object class
    auto id = *obj["id"].as_int();
    auto msg = *obj["msg"].as_string();

    auto result = yyjson::object();
    result.emplace("id", id);
    result.emplace("msg", msg);
    auto body = result.write();
    res.set_content(body.data(), APP_JSON);
}
void HttpJsonHandler::deleteMsg(httplib::Request const& req,
                                httplib::Response& res)
{
    auto n = req.matches[1];
    auto obj = yyjson::object();
    obj.emplace("id", std::stoi(n));
    obj.emplace("msg", "hello");
    auto result = obj.write();
    res.set_content(result.data(), APP_JSON);
}
void HttpJsonHandler::set_webhook(std::string const& web_hook)
{
    if (pimpl)
    {
        pimpl->set_web_hook(web_hook);
    }
}
