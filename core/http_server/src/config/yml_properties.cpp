#include "config/yml_properties.hpp"

#include "config/server_config.hpp"
#include "logs.hpp"

#include <filesystem>
#include <spdlog/spdlog.h>
#include <string>
#include <yaml-cpp/yaml.h>
namespace fs = std::filesystem;
class YamlProperties::ymlImpl
{

public:
    explicit ymlImpl(std::string const& file_name) : file_name_(file_name)
    {

        logger = Logger::GetInstance()->get();
    }

    void parse()
    {
        auto file = fs::path(file_name_);
        if (!fs::exists(file))
        {
            logger->error("file {} does not exist", file_name_);
            return;
        }
        tree_ = YAML::LoadFile(file_name_);
    }

    server_config get(std::string const& key)
    {
        return tree_[key].as<server_config>();
    }

private:
    std::string file_name_;
    YAML::Node tree_;
    std::shared_ptr<spdlog::logger> logger;
};

YamlProperties::YamlProperties(std::string const& file_name)
    : pimpl(std::make_unique<ymlImpl>(file_name))
{
}

server_config YamlProperties::getConfig() { return pimpl->get("server"); }
void YamlProperties::parse() { pimpl->parse(); }
YamlProperties::~YamlProperties() = default;
