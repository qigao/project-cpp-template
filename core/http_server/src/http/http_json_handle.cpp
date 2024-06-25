#include "http/http_json_handle.hpp"
#include "constants.hpp"
#include "cpp_yyjson.hpp"
#include "spdlog/spdlog.h"
#include <BS_thread_pool.hpp>
#include <fmt/core.h>

#include <httplib.h>

#include "http/http_web_hook.hpp"
HttpJsonHandler::HttpJsonHandler() : pool(std::make_shared<BS::thread_pool>(2))
{
}
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
    auto val = yyjson::read(req.body);
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

void HttpJsonHandler::dump(httplib::Request const& req, httplib::Response& res)
{
    std::string sha256_header = req.get_header_value(SHA_256_HASH_HEADER);
    auto jsonResp = yyjson::object();
    for (auto const& param : req.params)
    {
        std::cout << "Parameter: " << param.first << " = " << param.second
                  << std::endl;
        jsonResp.emplace(param.first, param.second);
    }
    if (sha256_header.empty())
    {
        std::string missing_header =
            fmt::format("missing header {}", SHA_256_HASH_HEADER);
        jsonResp.emplace("message", missing_header);
        res.set_content(jsonResp.write().data(), APP_JSON);
    }
    if (req.body.empty())
    {
        jsonResp.emplace("message", "empty body");
        res.set_content(jsonResp.write().data(), APP_JSON);
    }
    res.set_content(req.body, APP_JSON);
}

void HttpJsonHandler::web_hook(httplib::Request const& req,
                               httplib::Response& res)
{

    std::future<void> const my_future = pool->submit_task(
        [&]
        {
            auto web_hook_call = WebHook::GetInstance();
            web_hook_call->call(req.body);
        });
    while (true)
    {
        if (my_future.wait_for(std::chrono::milliseconds(2000)) !=
            std::future_status::ready)
            std::cout << "Sorry, the task is not done yet.\n";
        else
            break;
    }
    auto val = yyjson::read(req.body);
    res.set_content(val.write().data(), APP_JSON);
    res.status = 200;
}
