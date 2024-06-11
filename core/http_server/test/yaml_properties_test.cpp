#include "config/yml_properties.hpp"
#include "fs.hpp"
#include "test_helper.h"
#include <filesystem>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <iostream>
#include <yaml-cpp/yaml.h>
using ::testing::Eq;
namespace fs = std::filesystem;
class YamlPropertiesTest : public testing::Test
{
protected:
    void SetUp() override
    {
        auto const file_name = "test.yaml";
        YAML::Node node = YAML::Load(yml_demo_data);
        auto random_dir = fs::temp_directory_path();
        if (!createRandomDir(random_dir))
        {
            std::cerr << "Failed to create temporary directory" << std::endl;
            return;
        }
        full_path_file_name =
            std::make_unique<fs::path>(random_dir / file_name);

        YAML::Emitter out;
        out << node;
        write(full_path_file_name->string(), out.c_str(), out.size());
        yml_properties_ =
            std::make_unique<YamlProperties>(full_path_file_name->string());
    }

    void TearDown() override
    {
        fs::remove_all(full_path_file_name->filename());
    }

    std::unique_ptr<YamlProperties> yml_properties_;
    std::unique_ptr<fs::path> full_path_file_name;
};
TEST_F(YamlPropertiesTest, Parse)
{
    yml_properties_->parse();
    auto const config = yml_properties_->getConfig();
    EXPECT_THAT(config.auth.token, Eq("123456"));
    EXPECT_THAT(config.auth.enabled, Eq(true));
    EXPECT_THAT(config.port, Eq(4443));
    EXPECT_THAT(config.shared_folder, Eq("./shared"));
    EXPECT_THAT(config.delete_after_download, Eq(true));
    EXPECT_THAT(config.ssl.enabled, Eq(false));
    EXPECT_THAT(config.ssl.key_file, Eq("./server.key"));
    EXPECT_THAT(config.ssl.cert_file, Eq("./server.crt"));
    EXPECT_THAT(config.ssl.root_ca_file, Eq("./rootCA/rootCA-crt.pem"));
    EXPECT_THAT(config.ssl.root_ca_key_file, Eq("./rootCA/rootCA-key.pem"));
    EXPECT_THAT(config.webhook.url, Eq("http://127.0.0.1:8080/webhook"));
    EXPECT_THAT(config.webhook.api_key, Eq("123456"));
    EXPECT_THAT(config.webhook.headers[0].header, Eq("x-test"));
    EXPECT_THAT(config.webhook.headers[0].value, Eq("true"));
    EXPECT_THAT(config.webhook.headers[1].header, Eq("x-agent"));
    EXPECT_THAT(config.webhook.headers[1].value, Eq("mview_http_webhook"));
}
