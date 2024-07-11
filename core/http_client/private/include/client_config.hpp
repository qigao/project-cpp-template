#ifndef __REMOTE_CONFIG_H__
#define __REMOTE_CONFIG_H__

#include <string>
#include <yaml-cpp/yaml.h>

class client_config
{
public:
    int port;
    std::string host;
    std::string ssl_cert;
    std::string auth_token;
    std::string webhook_url;
    std::string files_url;
};

namespace YAML
{
template <>
struct convert<client_config>
{
    static YAML::Node encode(client_config const& rhs)
    {
        YAML::Node node;
        node["webhook-url"] = rhs.webhook_url;
        node["files-url"] = rhs.files_url;
        node["ssl-cert"] = rhs.ssl_cert;
        node["auth-token"] = rhs.auth_token;
        node["port"] = rhs.port;
        node["host"] = rhs.host;
        return node;
    }

    static bool decode(YAML::Node const& node, client_config& rhs)
    {
        if (node["webhook-url"])
        {
            rhs.webhook_url = node["webhook-url"].as<std::string>();
        }
        else
        {
            return false;
        }

        if (node["files-url"])
        {
            rhs.files_url = node["files-url"].as<std::string>();
        }
        else
        {
            return false;
        }

        if (node["ssl-cert"])
        {
            rhs.ssl_cert = node["ssl-cert"].as<std::string>();
        }

        if (node["auth-token"])
        {
            rhs.auth_token = node["auth-token"].as<std::string>();
        }
        else
        {
            return false;
        }

        if (node["port"])
        {
            rhs.port = node["port"].as<int>();
        }
        else
        {
            return false;
        }

        if (node["host"])
        {
            rhs.host = node["host"].as<std::string>();
        }
        else
        {
            return false;
        }

        return true;
    }
};
} // namespace YAML

#endif // __REMOTE_CONFIG_H__
