#include "client_yaml.hpp"
#include "fs.hpp"
#include "test_helper.h"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include <filesystem>
#include <iostream>
#include <memory>
#include <yaml-cpp/yaml.h>

namespace fs = std::filesystem;
class RemoteYmlTest
{
public:
    RemoteYmlTest()
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

    ~RemoteYmlTest() { fs::remove_all(full_path_file_name->filename()); }

    std::unique_ptr<ClientYml> yml_properties_;
    std::unique_ptr<fs::path> full_path_file_name;
};
TEST_CASE_METHOD(RemoteYmlTest, "ParseTest", "[parse_yml]")
{
    yml_properties_->parse();
    auto config = yml_properties_->getConfig();
    REQUIRE_THAT(config.host, Catch::Matchers::Equals("127.0.0.1"));
    REQUIRE(config.port == 5060);
    REQUIRE_THAT(config.auth_token, Catch::Matchers::Equals("123456"));
    REQUIRE_THAT(config.files_url, Catch::Matchers::Equals("/upload"));
    REQUIRE_THAT(config.webhook_url, Catch::Matchers::Equals("/webhook"));
}
