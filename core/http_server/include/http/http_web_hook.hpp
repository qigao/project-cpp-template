#ifndef __WEB_HOOK_SINGLETON_H__
#define __WEB_HOOK_SINGLETON_H__

#include "hash_function.hpp"
#include "http_request.hpp"
#include "singleton.hpp"

#include <memory>
#include <spdlog/logger.h>
#include <string>
#include <vector>
class WebHook : public Singleton<WebHook>
{
public:
    explicit WebHook(std::string const& url);
    void set_header(std::string const& header, std::string const& value);
    void call(std::string const& json);

private:
    std::vector<http::HeaderField> headers;
    std::string web_hook_url;
    std::shared_ptr<hash_function> hash_func;
    std::shared_ptr<http::Request> http_request;
    std::shared_ptr<spdlog::logger> logger;
};

#endif // __WEB_HOOK_SINGLETON_H__
