
#include "cpp_yyjson.hpp"
#include "helper/constants.hpp"
#include "helper/helpers.hpp"
#include "spdlog/spdlog.h"
#include <fmt/core.h>
#include <httplib.h>
#include <map>
using namespace yyjson;
#include "http/http_handler.hpp"
void HttpHandler::getMsg(const httplib::Request& req, httplib::Response& res)
{
    auto pathVar = req.matches[1];
    auto obj = yyjson::object();
    obj.emplace("id", std::stoi(pathVar));
    obj.emplace("msg", "hello");
    auto result = obj.write();
    res.set_content(result.data(), APP_JSON);
}

void HttpHandler::postMsg(const httplib::Request& req, httplib::Response& res)
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

void HttpHandler::patchMsg(const httplib::Request& req, httplib::Response& res)
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
void HttpHandler::deleteMsg(const httplib::Request& req, httplib::Response& res)
{

    auto n = req.matches[1];

    auto obj = yyjson::object();
    obj.emplace("id", std::stoi(n));
    obj.emplace("msg", "hello");
    auto result = obj.write();
    res.set_content(result.data(), APP_JSON);
}
