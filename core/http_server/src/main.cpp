#include "config/pch_headers.hpp"
#include "config/server_config.hpp"
#include "config/yml_properties.hpp"
#include "http/http_auth_handle.hpp"
#include "http/http_file_handle.hpp"
#include "http/http_json_handle.hpp"
#include "http/http_server.hpp"
#include "http/http_web_hook.hpp"

using namespace std::placeholders;
namespace fs = std::filesystem;
std::shared_ptr<spdlog::logger> logger;

bool check_ssl_config(server_config const& config)
{
    if (config.ssl.cert_file.empty() || config.ssl.key_file.empty())
    {
        logger->error("ssl cert file or key file is empty");
        return false;
    }
    fs::path fs_cert = config.ssl.cert_file;
    fs::path fs_key = config.ssl.key_file;
    logger->debug("ssl cert file: {},ssl key file: {}", config.ssl.cert_file,
                  config.ssl.key_file);

    return true;
}

void config_handle(HttpServer* http_server, server_config const& config)
{
    fs::path shared_folder = config.shared_folder;
    bool delete_after_download = config.delete_after_download;
    std::shared_ptr<HttpFileHandle> mHttpFile =
        std::make_shared<HttpFileHandle>(fs::absolute(shared_folder).string(),
                                         delete_after_download);
    std::shared_ptr<HttpJsonHandler> mJsonHandle =
        std::make_shared<HttpJsonHandler>();
    if (config.auth.enabled)
    {
        auto instance = AuthHandler::GetInstance();
        http_server->auth_handler(
            std::bind(&AuthHandler::auth_handler, instance, _1, _2));
    }
    http_server->Get(
        "/", std::bind(&HttpFileHandle::list_upload_form, mHttpFile, _1, _2));
    http_server->PostWithReader(
        "/upload/(.*)",
        std::bind(&HttpFileHandle::handle_file_upload, mHttpFile, _1, _2, _3));
    http_server->Post(
        "/download/(.*)",
        std::bind(&HttpFileHandle::handle_file_download, mHttpFile, _1, _2));
    http_server->Get("/list", std::bind(&HttpFileHandle::handle_file_lists,
                                        mHttpFile, _1, _2));
    http_server->Post(
        "/webhook", std::bind(&HttpJsonHandler::web_hook, mJsonHandle, _1, _2));
    http_server->Post("/dump",
                      std::bind(&HttpJsonHandler::dump, mJsonHandle, _1, _2));
    http_server->start();

    logger->info("Server is running. Press Anykey to stop the server.");
    std::cin.get();
    logger->info("Stoping server...");
    http_server->stop();
}
std::unique_ptr<HttpServer> createServer(server_config const& config)
{

    fs::path fs_cert = config.ssl.cert_file;
    fs::path fs_key = config.ssl.key_file;
    return std::make_unique<HttpServer>(config.port, 4,
                                        fs::absolute(fs_cert).string(),
                                        fs::absolute(fs_key).string());
}

void start_server(server_config const& config)
{
    auto http_server = createServer(config);
    http_server->config_server();
    config_handle(http_server.get(), config);
}
void setup_webhook(server_config const& config)
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
void setup_auth(server_config const& config)
{
    if (config.auth.enabled)
    {
        AuthHandler::Construct(config.auth.token);
        auto instance = AuthHandler::GetInstance();
        instance->set_api_key(config.auth.token);
    }
}
server_config parseOptions(int argc, char** argv)
{
    server_config config;
    try
    {
        cxxopts::Options options("http_server", "HTTP Server");
        options.add_options()("d,debug", "Enable verbose mode")(
            "f,file", "Configuration file",
            cxxopts::value<std::string>())("h,help", "Print help");

        auto result = options.parse(argc, argv);

        if (result.count("help"))
        {
            logger->info("{}", options.help());
        }

        if (result.arguments().empty())
        {
            logger->info("{}", options.help());
            std::string filename = "application.yml";
            fs::path current_path = filename;
            if (!fs::exists(current_path))
            {
                auto msg =
                    fmt::format("config file {} does not exist", filename);
                logger->error("{}", msg);
                throw std::runtime_error(msg);
            }
            auto yaml = std::make_unique<YamlProperties>(
                fs::absolute(current_path).string());
            yaml->parse();
            config = yaml->getConfig();
            return config;
        }

        if (result["debug"].as<bool>())
        {
            std::cout << "Verbose mode enabled" << std::endl;
            spdlog::set_level(spdlog::level::debug);
        }

        if (result.count("file"))
        {
            std::string filename = result["file"].as<std::string>();
            if (!fs::exists(filename))
            {
                auto msg =
                    fmt::format("config file {} does not exist", filename);
                logger->error("{}", msg);
                throw std::runtime_error(msg);
            }
            auto yaml = std::make_unique<YamlProperties>(filename);
            yaml->parse();
            config = yaml->getConfig();
        }
    }
    catch (cxxopts::exceptions::exception const& e)
    {
        logger->error("Error parsing options: {}", e.what());
        exit(1);
    }
    return config;
}
void setup_logger()
{
    Logger::Construct();
    logger = Logger::GetInstance()->get();
}
int main(int argc, char** argv)
{
    setup_logger();
    std::string filename;
    cxxopts::Options options("http_server",
                             "A http server which process upload/download file "
                             "and forward json request");

    // clang-format off
    options.add_options()
        ("h,help", "Print usage")
        ("f,file", "yaml config file",
                cxxopts::value<std::string>()->default_value("application.yml"))
        ("d,debug", "Enable debugging",
                 cxxopts::value<bool>()->default_value("false"));
    // clang-format on
    server_config config = parseOptions(argc, argv);
    if (config.shared_folder.empty())
    {
        logger->error("shared folder is empty");
        return 0;
    }
    setup_webhook(config);
    setup_auth(config);
    start_server(config);
    return 0;
}
