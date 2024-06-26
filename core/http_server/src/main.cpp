#include "config/server_config.hpp"
#include "config/yml_properties.hpp"
#include "constants.hpp"
#include "http/http_auth_handle.hpp"
#include "http/http_file_handle.hpp"
#include "http/http_json_handle.hpp"
#include "http/http_server.hpp"
#include "http/http_web_hook.hpp"
#include <CLI/CLI.hpp>
#include <functional>
#include <memory>
#include <string>
#include <thread>
using namespace std::placeholders;

void startServerSSL(server_config const& config)
{
    HttpServer server(config.port);
    auto shared_folder = config.shared_folder;
    bool delete_after_download = config.delete_after_download;
    std::shared_ptr<HttpFileHandle> mHttpFile =
        std::make_shared<HttpFileHandle>(shared_folder, delete_after_download);
    std::shared_ptr<HttpJsonHandler> mJsonHandle =
        std::make_shared<HttpJsonHandler>();
    if (config.auth.enabled)
    {
        auto instance = AuthHandler::GetInstance();
        server.auth_handler(
            std::bind(&AuthHandler::auth_handler, instance, _1, _2));
    }
    server.Get("/",
               std::bind(&HttpFileHandle::list_upload_form, mHttpFile, _1, _2));
    server.PostWithReader(
        "/upload",
        std::bind(&HttpFileHandle::handle_file_upload, mHttpFile, _1, _2, _3));
    server.Post("/download", std::bind(&HttpFileHandle::handle_file_download,
                                       mHttpFile, _1, _2));
    server.Get("/list", std::bind(&HttpFileHandle::handle_file_lists, mHttpFile,
                                  _1, _2));
    server.Post("/webhook",
                std::bind(&HttpJsonHandler::web_hook, mJsonHandle, _1, _2));
    server.Post("/dump",
                std::bind(&HttpJsonHandler::dump, mJsonHandle, _1, _2));
    server.start();
}
void setup_webhook(server_config const& config)
{
    if (config.webhook.enabled)
    {
        WebHook::Construct(config.webhook.url);
        auto instance = WebHook::GetInstance();
        for (auto const& header : config.webhook.headers)
        {
            instance->set_header(header.header, header.value);
        }
        instance->set_header(API_KEY, config.webhook.api_key);
        instance->set_header(USER_AGENT, "Micro View Http client");
    }
}
void setup_auth(server_config const& config)
{
    if (config.auth.enabled)
    {
        AuthHandler::Construct(config.auth.token);
        auto instance = AuthHandler::GetInstance();
        instance->set_api_key(config.auth.token);
    }
}
int main(int argc, char** argv)
{
    CLI::App app{"FileServer - A http based file server"};
    std::string filename;
    app.add_option("-f,--file", filename,
                   "Input config file name, applicaton.yml")
        ->required();
    app.add_option("-h --help", "Usage:  FileServer -f application.yml");
    CLI11_PARSE(app, argc, argv);
    auto yaml = std::make_unique<YamlProperties>(filename);
    auto server_config = yaml->getConfig();
    setup_webhook(server_config);
    setup_auth(server_config);
    std::thread serverSSL(startServerSSL, server_config);
    serverSSL.join();
}
