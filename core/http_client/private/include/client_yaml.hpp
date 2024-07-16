#ifndef __CLIENT_YML_PROPERTIES_H__
#define __CLIENT_YML_PROPERTIES_H__

#include "client_config.hpp"
#include "utils.hpp"

class ClientYml
{
public:
    ~ClientYml();
    explicit ClientYml(std::string const& file_name);
    client_config getConfig();
    void parse();

private:
    class remoteCfg;
    std::unique_ptr<remoteCfg> pimpl;
};

#endif // __CLIENT_YML_PROPERTIES_H__
