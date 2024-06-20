
#include <map>
#include <memory>
#include <string>

class YmlProperties
{
public:
    YmlProperties() = default;
    YmlProperties(YmlProperties&&) = default;
    YmlProperties(YmlProperties const&) = delete;
    YmlProperties& operator=(YmlProperties&&) = default;
    YmlProperties& operator=(YmlProperties const&) = delete;
    ~YmlProperties() = default;
    explicit YmlProperties(std::string const& file_name);
    std::string getValue(std::string const& key);
    void parse();
    std::map<std::string, std::string> getMap(std::string const& key);

private:
    class ymlImpl;
    std::unique_ptr<ymlImpl> pimpl;
};
