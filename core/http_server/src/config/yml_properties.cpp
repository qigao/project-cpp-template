#include "config/yml_properties.hpp"
#include <filesystem>
#include <map>
#include <spdlog/spdlog.h>
#include <string>
#include <yaml-cpp/yaml.h>
namespace fs = std::filesystem;
class YmlProperties::ymlImpl
{
    std::string file_name_;
    YAML::Node tree_;

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

        tree_ = YAML::LoadFile(file);
    }
    std::string get(std::string const& key)
    {

        std::string result;
        if (tree_[key])
        {
            result = tree_[key].as<std::string>();
        }
        return result;
    }
};

YmlProperties::YmlProperties(std::string const& file_name)
    : pimpl(new ymlImpl(file_name))
{
}

std::string YmlProperties::getValue(std::string const& key)
{
    if (pimpl)
    {
        return pimpl->get(key);
    }
    return "";
}
void YmlProperties::parse()
{
    if (pimpl)
    {
        pimpl->parse();
    }
}
