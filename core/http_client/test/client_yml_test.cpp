#include "client_yaml.hpp"
#include "test_helper.h"

#include <catch2/catch_all.hpp>
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
        fs_write(full_path_file_name->string(), out.c_str(), out.size());
    }
    ~YamlPropertiesTest()
    {
        std::cout << "path name" << full_path_file_name->string() << std::endl;
        fs::remove_all(full_path_file_name->filename());
    }

    std::unique_ptr<fs::path> full_path_file_name;
};
TEST_CASE_METHOD(YamlPropertiesTest, "ParseTest", "[parse_yml]")
{

    auto yml = ClientYml(full_path_file_name->string());
    yml.parse();
    auto config = yml.getConfig();
    REQUIRE_THAT(config.host, Catch::Matchers::Equals("127.0.0.1"));
    REQUIRE(config.port == 5060);
    REQUIRE_THAT(config.auth_token, Catch::Matchers::Equals("123456"));
    REQUIRE_THAT(config.files_url, Catch::Matchers::Equals("/upload"));
    REQUIRE_THAT(config.webhook_url, Catch::Matchers::Equals("/webhook"));
}
