#include "http/http_json_handle.hpp"

#include "config/pch_headers.hpp"
#include "http/http_web_hook.hpp"

HttpJsonHandler::HttpJsonHandler() : pool(std::make_shared<BS::thread_pool>(2))
{
    logger = Logger::GetInstance()->get();
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
    logger->info("request info:{},path params: {}", req.body, pathVar.str());
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
    if (sha256_header.empty())
    {
        logger->info("no sha header");
    }
    for (auto const& param : req.params)
    {
        logger->info("Parameter: {} = {}", param.first, param.second);
    }
    if (req.body.empty())
    {
        logger->info("message: empty body");

        return;
    }
    logger->info("dump body: {}", req.body);
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
            logger->info("waiting");
        }
        else
        {
            break;
        }
    }
    res.set_content(req.body.c_str(), APP_JSON);
    res.status = 200;
}
