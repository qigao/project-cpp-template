#include "client_config.hpp"

#include <gtest/gtest.h>
#include <yaml-cpp/yaml.h>

TEST(YAMLConverterTest, EncodeDecode)
{
    client_config config;
    config.webhook_url = "https://example.com/webhook";
    config.files_url = "https://example.com/files";
    config.ssl_cert = "path/to/ssl/cert";
    config.auth_token = "my_auth_token";
    config.port = 8080;
    config.host = "example.com";

    YAML::Node node = YAML::convert<client_config>::encode(config);

    client_config decoded_config;
    YAML::convert<client_config>::decode(node, decoded_config);

    EXPECT_EQ(config.webhook_url, decoded_config.webhook_url);
    EXPECT_EQ(config.files_url, decoded_config.files_url);
    EXPECT_EQ(config.ssl_cert, decoded_config.ssl_cert);
    EXPECT_EQ(config.auth_token, decoded_config.auth_token);
    EXPECT_EQ(config.port, decoded_config.port);
    EXPECT_EQ(config.host, decoded_config.host);
}

TEST(YAMLConverterTest, DecodeEmptyNode)
{
    YAML::Node node;
    client_config config;
    EXPECT_FALSE(YAML::convert<client_config>::decode(node, config));
}

TEST(YAMLConverterTest, DecodeMissingFields)
{
    YAML::Node node;
    node["webhook-url"] = "https://example.com/webhook";
    node["files-url"] = "https://example.com/files";

    client_config config;
    EXPECT_FALSE(YAML::convert<client_config>::decode(node, config));
}
