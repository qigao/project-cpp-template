#include "client_config.hpp"

#include <catch2/catch_all.hpp>
#include <yaml-cpp/yaml.h>

TEST_CASE("YAMLConverterTest EncodeDecode", "[yaml_config]")
{
    client_config config;
    config.webhook_url = "https://example.com/webhook";
    config.files_url = "https://example.com/files";
    config.cert = "path/to/ssl/cert";
    config.auth_token = "my_auth_token";
    config.port = 8080;
    config.host = "example.com";

    YAML::Node node = YAML::convert<client_config>::encode(config);

    client_config decoded_config;
    YAML::convert<client_config>::decode(node, decoded_config);

    REQUIRE(config.webhook_url == decoded_config.webhook_url);
    REQUIRE(config.files_url == decoded_config.files_url);
    REQUIRE(config.cert == decoded_config.cert);
    REQUIRE(config.auth_token == decoded_config.auth_token);
    REQUIRE(config.port == decoded_config.port);
    REQUIRE(config.host == decoded_config.host);
}

TEST_CASE("YAMLConverterTest DecodeEmptyNode", "[yaml_config]")
{
    YAML::Node node;
    client_config config;
    REQUIRE_FALSE(YAML::convert<client_config>::decode(node, config));
}

TEST_CASE("YAMLConverterTest DecodeMissingFields", "[yaml_config]")
{
    YAML::Node node;
    node["webhook-url"] = "https://example.com/webhook";
    node["files-url"] = "https://example.com/files";

    client_config config;
    REQUIRE_FALSE(YAML::convert<client_config>::decode(node, config));
}
