#ifndef __CLIENT_CONFIG_H__
#define __CLIENT_CONFIG_H__

#include "pch_headers.hpp"

class client_config
{
public:
    int port;
    std::string host;
    std::string cert;
    std::string auth_token;
    std::string webhook_url;
    std::string files_url;
    bool debug;
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
        node["cert"] = rhs.cert;
        node["auth-token"] = rhs.auth_token;
        node["port"] = rhs.port;
        node["host"] = rhs.host;
        node["debug"] = rhs.debug;
        return node;
    }

    static bool decode(YAML::Node const& node, client_config& rhs)
    {
        if (node["webhook-url"])
        {
            rhs.webhook_url = node["webhook-url"].as<std::string>();
        }

        if (node["files-url"])
        {
            rhs.files_url = node["files-url"].as<std::string>();
        }

        if (node["cert"])
        {
            rhs.cert = node["cert"].as<std::string>();
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
        if (node["debug"])
        {
            rhs.debug = node["debug"].as<bool>();
        }
        else
        {
            rhs.debug = true;
        }
        return true;
    }
};
} // namespace YAML

#endif // __CLIENT_CONFIG_H__
