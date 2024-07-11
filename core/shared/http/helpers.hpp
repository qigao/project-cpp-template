#ifndef __HELPERS_H__
#define __HELPERS_H__

#include "constants.hpp"
#include "http_lib_header.hpp"

#include <fmt/core.h>
#include <iostream>
#include <spdlog/spdlog.h>
inline int some_fun()
{
    fmt::print("Hello {} !", "helpers");
    return 0;
}

inline void login(httplib::Request const& req, httplib::Response& res)
{
    std::string str;
    if (req.body == R"({"password": "secret"})")
    {
        str = "success";
    }
    else
    {
        str = "error";
    }
    res.set_content(R"({"result": ")" + str + "\"}", APP_JSON);
}

inline std::string load_assets(std::string const& path)
{

    std::ifstream file(path.c_str(), std::ios::in);

    std::stringstream stream;

    stream << file.rdbuf();

    file.close();

    std::string assets(stream.str());

    return assets;
}

inline std::string dump_headers(httplib::Headers const& headers)
{
    std::string s;
    char buf[BUFSIZ];

    for (auto const& x : headers)
    {
        fmt::print("%s: %s\n", x.first.c_str(), x.second.c_str());
        s += buf;
    }

    return s;
}

inline static void dump_http_request(httplib::Request const& r)
{
    spdlog::info("REQ.METHOD = {}", r.method);
    for (auto it = r.headers.begin(); it != r.headers.end(); ++it)
    {
        spdlog::info("REQ.HEADER[{}] = {} ", it->first, it->second);
    }
}

inline static void dump_http_response(httplib::Response const& r)
{
    spdlog::debug("res.status={}", r.status);
    for (auto it = r.headers.begin(); it != r.headers.end(); ++it)
    {
        spdlog::debug("RES.HEADER[{}] = {} ", it->first, it->second);
    }
    if (r.status != 200 && r.status != 206)
    {
        spdlog::debug("RES.BODY = {}", r.body);
    }
}

inline static void dump_request_response(httplib::Request const& req,
                                         httplib::Response const& res)
{
    dump_http_request(req);
    dump_http_response(res);
}

#endif // __HELPERS_H__
