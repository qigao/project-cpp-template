#include "client_yaml.hpp"

#include "pch_headers.hpp"

class ClientYml::remoteCfg
{
public:
    explicit remoteCfg(std::string const& file_name) : file_name_(file_name) {}

    inline void parse()
    {
        auto file = fs::path(file_name_);
        if (!fs::exists(file))
        {
            throw std::runtime_error(file_name_ + "not exits");
            return;
        }
        tree_ = YAML::LoadFile(file_name_);
    }

    inline client_config get(std::string const& key)
    {
        return tree_[key].as<client_config>();
    }

private:
    std::string file_name_;
    YAML::Node tree_;
};

ClientYml::ClientYml(std::string const& file_name)
    : pimpl(std::make_unique<remoteCfg>(file_name))
{
}

client_config ClientYml::getConfig() { return pimpl->get("remote"); }
void ClientYml::parse() { pimpl->parse(); }
ClientYml::~ClientYml() {}
