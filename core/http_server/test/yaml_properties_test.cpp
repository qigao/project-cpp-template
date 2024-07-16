#include "logs.hpp"
#define CATCH_CONFIG_MAIN

#include "config/yml_properties.hpp"
#include "fs.hpp"
#include "test_helper.h"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include <filesystem>
#include <iostream>
#include <yaml-cpp/yaml.h>

namespace fs = std::filesystem;
class YamlPropertiesTest
{
protected:
    YamlPropertiesTest()
    {
        fs::path file_name = "test.yaml";
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
        Logger::Construct();
        yml_properties_ =
            std::make_unique<YamlProperties>(full_path_file_name->string());
    }
    ~YamlPropertiesTest() { fs::remove_all(full_path_file_name->filename()); }

    std::unique_ptr<YamlProperties> yml_properties_;
    std::unique_ptr<fs::path> full_path_file_name;
};
TEST_CASE_METHOD(YamlPropertiesTest, "YamlPropertiesTest yaml config",
                 "[yml_parse]")
{
    yml_properties_->parse();
    auto const config = yml_properties_->getConfig();
    REQUIRE_THAT(config.auth.token, Catch::Matchers::Equals("123456"));
    REQUIRE(config.auth.enabled);
    REQUIRE(config.port == 4443);
    REQUIRE_THAT(config.shared_folder, Catch::Matchers::Equals("./shared"));
    REQUIRE(config.delete_after_download);
    REQUIRE_THAT(config.ssl.key_file, Catch::Matchers::Equals("./server.key"));
    REQUIRE_THAT(config.ssl.cert_file, Catch::Matchers::Equals("./server.crt"));
    REQUIRE_THAT(config.webhook.url,
                 Catch::Matchers::Equals("http://127.0.0.1:8080/webhook"));
    REQUIRE_THAT(config.webhook.api_key, Catch::Matchers::Equals("123456"));
    REQUIRE_THAT(config.webhook.headers[0].header,
                 Catch::Matchers::Equals("x-test"));
    REQUIRE_THAT(config.webhook.headers[0].value,
                 Catch::Matchers::Equals("true"));
    REQUIRE_THAT(config.webhook.headers[1].header,
                 Catch::Matchers::Equals("x-agent"));
    REQUIRE_THAT(config.webhook.headers[1].value,
                 Catch::Matchers::Equals("mview_http_webhook"));
}
