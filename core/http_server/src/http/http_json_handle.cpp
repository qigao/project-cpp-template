#include "http/http_json_handle.hpp"
#include "constants.hpp"
#include "http_lib_header.hpp"
#include "spdlog/spdlog.h"
#include "yyjson.h"
#include <BS_thread_pool.hpp>
#include <cstdlib>
#include <fmt/core.h>

#include "http/http_web_hook.hpp"
HttpJsonHandler::HttpJsonHandler() : pool(std::make_shared<BS::thread_pool>(2))
{
}
void HttpJsonHandler::getMsg(httplib::Request const& req,
                             httplib::Response& res)
{
    auto pathVar = req.matches[1];
    yyjson_mut_doc* docResp = yyjson_mut_doc_new(nullptr);
    yyjson_mut_val* rootResp = yyjson_mut_obj(docResp);
    yyjson_mut_doc_set_root(docResp, rootResp);

    yyjson_mut_obj_add_str(docResp, rootResp, "msg", "demo");
    yyjson_mut_obj_add_int(docResp, rootResp, "id", std::stoi(pathVar));
    char const* resp_json = yyjson_mut_write(docResp, 0, nullptr);
    res.set_content(resp_json, APP_JSON);
    yyjson_mut_doc_free(docResp);
}

void HttpJsonHandler::postMsg(httplib::Request const& req,
                              httplib::Response& res)
{
    auto pathVar = req.matches[1];
    spdlog::info("request info:{},path params: {}", req.body, pathVar.str());
    // Read JSON string
    yyjson_doc* doc = yyjson_read(req.body.c_str(), req.body.length(), 0);
    yyjson_val* root = yyjson_doc_get_root(doc);
    auto msgObj = yyjson_obj_get(root, "msg");
    auto msg = yyjson_get_str(msgObj);
    auto idObj = yyjson_obj_get(root, "id");
    auto id = yyjson_get_int(idObj);

    yyjson_mut_doc* docResp = yyjson_mut_doc_new(nullptr);
    yyjson_mut_val* rootResp = yyjson_mut_obj(docResp);
    yyjson_mut_doc_set_root(docResp, rootResp);

    yyjson_mut_obj_add_str(docResp, rootResp, "msg", "demo");
    yyjson_mut_obj_add_int(docResp, rootResp, "id", id * 20 + 1);
    char const* resp_json = yyjson_mut_write(docResp, 0, nullptr);
    res.set_content(resp_json, APP_JSON);
    yyjson_mut_doc_free(docResp);
    yyjson_doc_free(doc);
}

void HttpJsonHandler::dump(httplib::Request const& req, httplib::Response& res)
{
    std::string sha256_header = req.get_header_value(SHA_256_HASH_HEADER);
    // Create a mutable doc
    yyjson_mut_doc* doc = yyjson_mut_doc_new(nullptr);
    yyjson_mut_val* root = yyjson_mut_obj(doc);
    yyjson_mut_doc_set_root(doc, root);
    for (auto const& param : req.params)
    {
        spdlog::info("Parameter: {} = {}", param.first, param.second);
        yyjson_mut_val* obj = yyjson_mut_obj(doc);
        yyjson_mut_obj_add_str(doc, obj, param.first.c_str(),
                               param.second.c_str());
        yyjson_mut_arr_append(root, obj);
    }
    if (req.body.empty())
    {
        spdlog::info("message: empty body");
        yyjson_mut_obj_add_str(doc, root, "message", "empty body");
        char const* jsonResp = yyjson_mut_write(doc, 0, nullptr);
        if (jsonResp)
        {
            spdlog::info("Response: {}", jsonResp);
            res.set_content(jsonResp, APP_JSON);
            free((void*)jsonResp);
            yyjson_mut_doc_free(doc);
        }
        return;
    }
    spdlog::info("body: {}", req.body);
    yyjson_mut_obj_add_str(doc, root, "message", req.body.c_str());
    char const* jsonResp = yyjson_mut_write(doc, 0, nullptr);
    if (jsonResp)
    {
        spdlog::info("Response: {}", jsonResp);
        res.set_content(jsonResp, APP_JSON);

        // yyjson_mut_doc_free(doc);free((void*)jsonResp);
        return;
    }
    yyjson_mut_doc_free(doc);

    char const* error_msg = R"({"message":"error"})";
    res.set_content(error_msg, strlen(error_msg), APP_JSON);
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
        {
            spdlog::info("waiting");
        }
        else
        {
            break;
        }
    }
    res.set_content(req.body.c_str(), APP_JSON);
    res.status = 200;
}
