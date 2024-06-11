#include "config/yml_properties.hpp"
#include "config/server_config.hpp"
#include <filesystem>
#include <spdlog/spdlog.h>
#include <string>
#include <yaml-cpp/yaml.h>
namespace fs = std::filesystem;
class YamlProperties::ymlImpl
{

public:
    explicit ymlImpl(std::string const& file_name) : file_name_(file_name) {}

    void parse()
    {
        auto file = fs::path(file_name_);
        if (!fs::exists(file))
        {
            spdlog::error("file {} does not exist", file_name_);
            return;
        }
        tree_ = YAML::LoadFile(file_name_);
    }

    server_config get(std::string const& key)
    {
        return tree_["server"].as<server_config>();
    }

private:
    std::string file_name_;
    YAML::Node tree_;
};

YamlProperties::YamlProperties(std::string const& file_name)
    : pimpl(std::make_unique<ymlImpl>(file_name))
{
}

server_config YamlProperties::getConfig() { return pimpl->get("server"); }
void YamlProperties::parse() { pimpl->parse(); }
YamlProperties::~YamlProperties() = default;
