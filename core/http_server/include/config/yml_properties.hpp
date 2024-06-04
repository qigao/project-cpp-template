#ifndef __YML_PROPERTIES_H__
#define __YML_PROPERTIES_H__

#include "config/pch_headers.hpp"
#include "config/server_config.hpp"

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
    std::shared_ptr<spdlog::logger> logger;
};

#endif // __YML_PROPERTIES_H__
