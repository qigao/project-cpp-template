#ifndef __YML_PROPERTIES_H__
#define __YML_PROPERTIES_H__

#include "config/server_config.hpp"

#include <memory>
#include <string>

class YamlProperties
{
public:
    ~YamlProperties();
    explicit YamlProperties(std::string const& file_name);
    server_config getConfig();
    void parse();

private:
    class ymlImpl;
    std::unique_ptr<ymlImpl> pimpl;
};

#endif // __YML_PROPERTIES_H__
