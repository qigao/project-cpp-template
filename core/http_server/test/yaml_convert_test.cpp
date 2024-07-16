#define CATCH_CONFIG_MAIN

#include "config/server_config.hpp"
#include "test_helper.h"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include <yaml-cpp/node/node.h>
#include <yaml-cpp/yaml.h>
TEST_CASE("YmlTest auth config", "[yml_convert]")
{
    auto node = YAML::Load("auth: {enabled: false, token: 123456} ");
    auto value = node["auth"].as<auth_config>();
    REQUIRE_THAT(value.token, Catch::Matchers::Equals("123456"));
}

TEST_CASE("YmlTest container", "[yml_convert]")
{
    auto node = YAML::Load("label: {name: tom, age: 12} ");
    auto value = node["label"].as<container>();
    REQUIRE(value.age == 12);
    REQUIRE_THAT(value.name, Catch::Matchers::Equals("tom"));
}

TEST_CASE("YmlTest webhook", "[yml_convert]")
{
    auto node = YAML::Load(yml_demo_data);
    auto root = node["server"];
    auto webhook = root["webhook"].as<webhook_config>();
    REQUIRE_THAT(webhook.url,
                 Catch::Matchers::Equals("http://127.0.0.1:8080/webhook"));
    REQUIRE_THAT(webhook.api_key, Catch::Matchers::Equals("123456"));
    auto headers = webhook.headers;
    REQUIRE(headers.size() == 2);
    REQUIRE_THAT(headers[0].header, Catch::Matchers::Equals("x-test"));
    REQUIRE_THAT(headers[0].value, Catch::Matchers::Equals("true"));
    REQUIRE_THAT(headers[1].header, Catch::Matchers::Equals("x-agent"));
    REQUIRE_THAT(headers[1].value,
                 Catch::Matchers::Equals("mview_http_webhook"));
}
TEST_CASE("YmlTest ssl config", "[yml_convert]")
{
    auto node = YAML::Load(yml_demo_data);
    auto root = node["server"];
    auto ssl = root["ssl"].as<ssl_config>();
    REQUIRE_THAT(ssl.key_file, Catch::Matchers::Equals("./server.key"));
    REQUIRE_THAT(ssl.cert_file, Catch::Matchers::Equals("./server.crt"));
}

TEST_CASE("YmlTest server config", "[yml_convert]")
{
    auto node = YAML::Load(yml_demo_data);
    auto root = node["server"];
    auto server = root.as<server_config>();
    REQUIRE(server.port == 4443);
    REQUIRE_THAT(server.shared_folder, Catch::Matchers::Equals("./shared"));
    REQUIRE(server.delete_after_download);
    REQUIRE(server.auth.enabled);
    REQUIRE_THAT(server.auth.token, Catch::Matchers::Equals("123456"));
    REQUIRE_THAT(server.webhook.url,
                 Catch::Matchers::Equals("http://127.0.0.1:8080/webhook"));
    REQUIRE_THAT(server.webhook.api_key, Catch::Matchers::Equals("123456"));
    auto headers = server.webhook.headers;
    REQUIRE(headers.size() == 2);
    REQUIRE_THAT(headers[0].header, Catch::Matchers::Equals("x-test"));
    REQUIRE_THAT(headers[0].value, Catch::Matchers::Equals("true"));
    REQUIRE_THAT(headers[1].header, Catch::Matchers::Equals("x-agent"));
    REQUIRE_THAT(headers[1].value,
                 Catch::Matchers::Equals("mview_http_webhook"));
}

TEST_CASE("YmlTest auth config encode", "[yml_convert]")
{
    auth_config auth;
    auth.enabled = true;
    auth.token = "123456";
    auto node = YAML::Node(auth);
    auto encoded = YAML::Dump(node);
    REQUIRE_THAT(encoded, Catch::Matchers::ContainsSubstring("123456"));
    REQUIRE_THAT(encoded, Catch::Matchers::ContainsSubstring("true"));
    REQUIRE_THAT(encoded, Catch::Matchers::ContainsSubstring("auth"));
    REQUIRE_THAT(encoded, Catch::Matchers::ContainsSubstring("enabled"));
    REQUIRE_THAT(encoded, Catch::Matchers::ContainsSubstring("token"));
}

TEST_CASE("YmlTest encode auth", "[yml_convert]")
{
    auth_config config = {true, "my_token"};
    YAML::Node node = YAML::convert<auth_config>::encode(config);
    REQUIRE(node["auth"]["enabled"].as<bool>());
    REQUIRE_THAT(node["auth"]["token"].as<std::string>(),
                 Catch::Matchers::ContainsSubstring("my_token"));
}
