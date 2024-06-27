#include "config/server_config.hpp"
#include "test_helper.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <yaml-cpp/node/node.h>
#include <yaml-cpp/yaml.h>
TEST(YmlTest, authConfig)
{
    auto node = YAML::Load("auth: {enabled: false, token: 123456} ");
    auto value = node["auth"].as<auth_config>();
    EXPECT_EQ(value.token, "123456");
}

TEST(YmlTest, container)
{
    auto node = YAML::Load("label: {name: tom, age: 12} ");
    auto value = node["label"].as<container>();
    EXPECT_EQ(value.age, 12);
    EXPECT_EQ(value.name, "tom");
}

TEST(YmlTest, webhook)
{
    auto node = YAML::Load(yml_demo_data);
    auto root = node["server"];
    auto webhook = root["webhook"].as<webhook_config>();
    EXPECT_EQ(webhook.enabled, true);
    EXPECT_EQ(webhook.url, "http://127.0.0.1:8080/webhook");
    EXPECT_EQ(webhook.api_key, "123456");
    auto headers = webhook.headers;
    EXPECT_EQ(headers.size(), 2);
    EXPECT_EQ(headers[0].header, "x-test");
    EXPECT_EQ(headers[0].value, "true");
    EXPECT_EQ(headers[1].header, "x-agent");
    EXPECT_EQ(headers[1].value, "mview_http_webhook");
}
TEST(YmlTest, sslConfig)
{
    auto node = YAML::Load(yml_demo_data);
    auto root = node["server"];
    auto ssl = root["ssl"].as<ssl_config>();
    EXPECT_EQ(ssl.enabled, false);
    EXPECT_EQ(ssl.key_file, "./server.key");
    EXPECT_EQ(ssl.cert_file, "./server.crt");
    EXPECT_EQ(ssl.root_ca_file, "./rootCA/rootCA-crt.pem");
    EXPECT_EQ(ssl.root_ca_key_file, "./rootCA/rootCA-key.pem");
}

TEST(YmlTest, serverConfig)
{
    auto node = YAML::Load(yml_demo_data);
    auto root = node["server"];
    auto server = root.as<server_config>();
    EXPECT_EQ(server.port, 4443);
    EXPECT_EQ(server.shared_folder, "./shared");
    EXPECT_EQ(server.delete_after_download, true);
    EXPECT_EQ(server.ssl.enabled, false);
    EXPECT_EQ(server.auth.enabled, true);
    EXPECT_EQ(server.auth.token, "123456");
    EXPECT_EQ(server.webhook.enabled, true);
    EXPECT_EQ(server.webhook.url, "http://127.0.0.1:8080/webhook");
    EXPECT_EQ(server.webhook.api_key, "123456");
    auto headers = server.webhook.headers;
    EXPECT_EQ(headers.size(), 2);
    EXPECT_EQ(headers[0].header, "x-test");
    EXPECT_EQ(headers[0].value, "true");
    EXPECT_EQ(headers[1].header, "x-agent");
    EXPECT_EQ(headers[1].value, "mview_http_webhook");
}

TEST(YmlTest, authConfigEncode)
{
    auth_config auth;
    auth.enabled = true;
    auth.token = "123456";
    auto node = YAML::Node(auth);
    auto encoded = YAML::Dump(node);
    EXPECT_THAT(encoded, testing::HasSubstr("123456"));
    EXPECT_THAT(encoded, testing::HasSubstr("true"));
    EXPECT_THAT(encoded, testing::HasSubstr("auth"));
    EXPECT_THAT(encoded, testing::HasSubstr("enabled"));
    EXPECT_THAT(encoded, testing::HasSubstr("token"));
}

TEST(YmlTest, Encode)
{
    auth_config config = {true, "my_token"};
    YAML::Node node = YAML::convert<auth_config>::encode(config);
    EXPECT_EQ(node["auth"]["enabled"].as<bool>(), true);
    EXPECT_EQ(node["auth"]["token"].as<std::string>(), "my_token");
}
