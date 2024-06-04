#include "client_yaml.hpp"
#include "test_helper.h"

namespace fs = std::filesystem;
class YmlCfg
{
protected:
    YmlCfg()
    {
        fs::path file_name = "config.yaml";
        YAML::Node node = YAML::Load(yml_demo_data);
        auto random_dir = fs::temp_directory_path();
        if (!createRandomDir(random_dir))
        {
            std::cerr << "Failed to create temporary directory" << std::endl;
            return;
        }
        full_path_file_name =
            std::make_shared<fs::path>(random_dir / file_name);
        config_file = full_path_file_name->string();
        YAML::Emitter out;
        out << node;
        fs_write(full_path_file_name->string(), out.c_str(), out.size());
    }
    ~YmlCfg()
    {
        std::cout << "path name" << full_path_file_name->string() << std::endl;
        fs::remove_all(full_path_file_name->filename());
    }
    std::string getCfgFile() { return config_file; }
    std::shared_ptr<fs::path> full_path_file_name;

    std::string config_file;
};
