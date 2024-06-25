#include "http/http_web_hook.hpp"
#include "constants.hpp"
#include <memory>
#include <spdlog/spdlog.h>

WebHook::WebHook(std::string const& url)
    : web_hook_url(url), pool(std::make_shared<BS::thread_pool>(2)),
      hash_func(std::make_shared<hash_function>()),
      http_request(std::make_shared<http::Request>(url))
{
    headers.push_back({CONTENT_TYPE, APP_JSON});
}

void WebHook::call(std::string const& json)
{
    pool->detach_task(
        [&]
        {
            auto sha256_hash = hash_func->sha_256_hash(json);
            auto sha256_value = fmt::format("sha256=%s", sha256_hash);
            headers.push_back({SHA_256_HASH_HEADER, sha256_value.c_str()});
            auto const response = http_request->send("POST", json, headers);
            auto resp = std::string{response.body.begin(), response.body.end()};
            spdlog::info("body: {}", resp);
        });
}
void WebHook::set_header(std::string const& header, std::string const& value)
{
    headers.push_back({header, value});
}
