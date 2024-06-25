#ifndef __SERVER_CONFIG_H__
#define __SERVER_CONFIG_H__

#include <string>
#include <yaml-cpp/yaml.h>

class ssl_config
{
public:
    bool enabled = false;
    std::string key_file;
    std::string cert_file;
    std::string root_ca_file;
    std::string root_ca_key_file;
};

struct auth_config
{
    bool enabled;
    std::string token;
};
struct header_config
{
    std::string header;
    std::string value;
};
class webhook_config
{
public:
    bool enabled = false;
    std::string url;
    std::string api_key;
    std::vector<header_config> headers;
};
class server_config
{
public:
    int port;
    std::string shared_folder;
    bool delete_after_download;
    ssl_config ssl;
    auth_config auth;
    webhook_config webhook;
};

struct container
{
    std::string name;
    int age;
};

namespace YAML
{
template <>
struct convert<container>
{
    static Node encode(container const& rhs)
    {
        Node node;
        node.push_back(rhs.name);
        node.push_back(rhs.age);
        return node;
    }

    static bool decode(Node const& node, container& rhs)
    {
        rhs.name = node["name"].as<std::string>();
        rhs.age = node["age"].as<int>();
        return true;
    }
};
template <>
struct convert<header_config>
{
    static Node encode(header_config const& rhs)
    {
        Node node;
        node.push_back(rhs.header);
        node.push_back(rhs.value);
        return node;
    }

    static bool decode(Node const& node, header_config& rhs)
    {
        rhs.header = node["header"].as<std::string>();
        rhs.value = node["value"].as<std::string>();
        return true;
    }
};

template <>
struct convert<auth_config>
{
    static YAML::Node encode(auth_config const& rhs)
    {
        YAML::Node node;
        node["auth"]["enabled"] = rhs.enabled;
        node["auth"]["token"] = rhs.token;
        return node;
    }

    static bool decode(YAML::Node const& node, auth_config& rhs)
    {
        rhs.enabled = node["enabled"].as<bool>();
        rhs.token = node["token"].as<std::string>();
        return true;
    }
};

template <>
struct convert<ssl_config>
{
    static YAML::Node encode(ssl_config const& rhs)
    {
        YAML::Node node;
        node["enabled"] = rhs.enabled;
        node["key_file"] = rhs.key_file;
        node["cert_file"] = rhs.cert_file;
        node["root_ca_file"] = rhs.root_ca_file;
        node["root_ca_key_file"] = rhs.root_ca_key_file;
        return node;
    }

    static bool decode(YAML::Node const& node, ssl_config& rhs)
    {
        rhs.enabled = node["enabled"].as<bool>();
        rhs.cert_file = node["cert_file"].as<std::string>();
        rhs.key_file = node["key_file"].as<std::string>();
        rhs.root_ca_file = node["root_ca_file"].as<std::string>();
        rhs.root_ca_key_file = node["root_ca_key_file"].as<std::string>();
        return true;
    }
};

template <>
struct convert<webhook_config>
{
    static YAML::Node encode(webhook_config const& rhs)
    {
        YAML::Node node;
        node["enabled"] = rhs.enabled;
        node["url"] = rhs.url;
        node["api_key"] = rhs.api_key;
        node["headers"] = rhs.headers;
        return node;
    }

    static bool decode(YAML::Node const& node, webhook_config& rhs)
    {
        rhs.enabled = node["enabled"].as<bool>();
        rhs.url = node["url"].as<std::string>();
        rhs.api_key = node["api_key"].as<std::string>();
        rhs.headers = node["headers"].as<std::vector<header_config>>();
        return true;
    }
};

template <>
struct convert<server_config>
{
    static YAML::Node encode(server_config const& rhs)
    {
        YAML::Node node;
        node["port"] = rhs.port;
        node["shared_folder"] = rhs.shared_folder;
        node["ssl"] = rhs.ssl;
        node["auth"] = rhs.auth;
        node["webhook"] = rhs.webhook;
        node["delete_after_download"] = rhs.delete_after_download;
        return node;
    }

    static bool decode(YAML::Node const& node, server_config& rhs)
    {
        rhs.webhook = node["webhook"].as<webhook_config>();
        rhs.ssl = node["ssl"].as<ssl_config>();
        rhs.shared_folder = node["shared_folder"].as<std::string>();
        rhs.delete_after_download = node["delete_after_download"].as<bool>();
        rhs.port = node["port"].as<int>();
        rhs.auth = node["auth"].as<auth_config>();
        return true;
    }
};
} // namespace YAML
#endif // __SERVER_CONFIG_H__
