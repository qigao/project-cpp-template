#include "http/http_web_hook.hpp"

#include "constants.hpp"
#include "logs.hpp"

#include <memory>
#include <spdlog/spdlog.h>

WebHook::WebHook(std::string const& url)
    : web_hook_url(url), hash_func(std::make_shared<hash_function>()),
      http_request(std::make_shared<http::Request>(url))
{
    headers.push_back({CONTENT_TYPE, APP_JSON});
    logger = Logger::GetInstance()->get();
}

void WebHook::call(std::string const& json)
{
    auto sha256_hash = hash_func->sha_256_hash(json);
    auto sha1_hash = hash_func->sha1_hash(json);
    auto sha256_value = fmt::format("sha256={}", sha256_hash);
    auto sha1_value = fmt::format("sha1={}", sha1_hash);
    headers.push_back({SHA_256_HASH_HEADER, sha256_value.c_str()});
    headers.push_back({SHA1_HASH_HEADER, sha1_value.c_str()});
    auto const response = http_request->send("POST", json, headers);
    auto resp = std::string{response.body.begin(), response.body.end()};
    logger->info("forward call resp body: {}", resp);
}

void WebHook::set_header(std::string const& header, std::string const& value)
{
    headers.push_back({header, value});
}
