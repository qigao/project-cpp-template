#include "http/http_auth_handle.hpp"

#include "config/pch_headers.hpp"

bool AuthHandler::is_valid_key(std::string const& key)
{
    return key == api_key;
}
void AuthHandler::set_api_key(std::string const& key) { api_key = key; }
httplib::Server::HandlerResponse
AuthHandler::auth_handler(httplib::Request const& req, httplib::Response& res)
{
    auto key = req.get_header_value(API_KEY);
    if (key.empty() || !is_valid_key(key))
    {
        res.status = 401;
        res.set_content("Unauthorized: Invalid or missing API key",
                        "text/plain");
        return httplib::Server::HandlerResponse::Handled;
    }

    return httplib::Server::HandlerResponse::Unhandled;
}
