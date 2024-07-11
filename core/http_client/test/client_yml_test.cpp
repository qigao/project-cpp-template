#include "client_yaml.hpp"
#include "fs.hpp"
#include "test_helper.h"

#include <filesystem>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <iostream>
#include <yaml-cpp/yaml.h>
using ::testing::Eq;
namespace fs = std::filesystem;
class RemoteYmlTest : public testing::Test
{
protected:
    void SetUp() override
    {
        auto const file_name = "client.yaml";
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
            std::make_unique<ClientYml>(full_path_file_name->string());
    }

    void TearDown() override
    {
        fs::remove_all(full_path_file_name->filename());
    }

    std::unique_ptr<ClientYml> yml_properties_;
    std::unique_ptr<fs::path> full_path_file_name;
};
TEST_F(RemoteYmlTest, ParseTest)
{
    yml_properties_->parse();
    auto config = yml_properties_->getConfig();
    EXPECT_THAT(config.host, Eq("127.0.0.1"));
    EXPECT_THAT(config.port, Eq(5060));
    EXPECT_THAT(config.auth_token, Eq("123456"));
    EXPECT_THAT(config.files_url, Eq("/upload"));
    EXPECT_THAT(config.webhook_url, Eq("/webhook"));
}
