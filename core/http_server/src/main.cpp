#include "config/server_config.hpp"
#include "config/yml_properties.hpp"
#include "constants.hpp"
#include "http/http_auth_handle.hpp"
#include "http/http_file_handle.hpp"
#include "http/http_json_handle.hpp"
#include "http/http_server.hpp"
#include "http/http_web_hook.hpp"

#include <cxxopts.hpp>
#include <filesystem>
#include <functional>
#include <memory>
#include <optional>
#include <spdlog/spdlog.h>
#include <string>
#include <thread>
using namespace std::placeholders;
namespace fs = std::filesystem;

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
    server.Post(
        "/download/(.*)",
        std::bind(&HttpFileHandle::handle_file_download, mHttpFile, _1, _2));
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
    /*     CLI::App app{"FileServer - A http based file server"};

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
        serverSSL.join(); */

    std::optional<bool> minimized(false);
    bool enableDebugLogs = false;
    std::string filename;
    cxxopts::Options options("http_server",
                             "A http server which process upload/download file "
                             "and forward json request");
    server_config config;
    // clang-format off
    options.add_options()
        ("h,help", "Print usage")
        ("f,file", "yaml config file",
                cxxopts::value<std::string>()->default_value("application.yml"))
        ("d,debug", "Enable debugging",
                 cxxopts::value<bool>()->default_value("false"));
    // clang-format on
    try
    {
        auto result = options.parse(argc, argv);

        if (result.count("help"))
        {
            std::cout << options.help() << std::endl;
            return 0;
        }

        // Check if any options were provided
        if (result.arguments().empty())
        {
            std::cout << options.help() << std::endl;
            // Implement your default behavior here
        }
        else
        {

            if (result["debug"].as<bool>())
            {
                std::cout << "Verbose mode enabled" << std::endl;
                spdlog::set_level(spdlog::level::debug);
            }
            // Process the provided options
            if (result.count("file"))
            {
                filename = result["file"].as<std::string>();
                std::cout << "File: " << filename << std::endl;
                auto file = fs::path(filename);
                if (!fs::exists(file))
                {
                    spdlog::error("file {} does not exist", filename);
                    return 0;
                }
                auto yaml = std::make_unique<YamlProperties>(filename);
                config = yaml->getConfig();
                setup_webhook(config);
                setup_auth(config);
                std::thread serverSSL(startServerSSL, config);
                serverSSL.join();
            }
        }
    }
    catch (cxxopts::exceptions::exception const& e)
    {
        std::cerr << "Error parsing options: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
