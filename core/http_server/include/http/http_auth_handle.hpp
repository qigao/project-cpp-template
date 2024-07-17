#ifndef __HTTP_AUTH_HANDLE_H__
#define __HTTP_AUTH_HANDLE_H__

#include "config/pch_headers.hpp"
class AuthHandler : public Singleton<AuthHandler>
{
public:
    explicit AuthHandler(std::string const& key) : api_key(key) {}
    bool is_valid_key(std::string const& key);
    void set_api_key(std::string const& key);
    httplib::Server::HandlerResponse auth_handler(httplib::Request const& req,
                                                  httplib::Response& res);

private:
    std::string api_key;
    std::shared_ptr<spdlog::logger> logger;
};

#endif // __HTTP_AUTH_HANDLE_H__
