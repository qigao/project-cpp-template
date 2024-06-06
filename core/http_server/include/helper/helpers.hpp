#ifndef __HELPERS_H__
#define __HELPERS_H__

#include "constants.hpp"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include <cmath>
#include <fmt/core.h>
#include <httplib.h>
#include <iostream>
using namespace rapidjson;
inline int some_fun()
{
    fmt::print("Hello {} !", "helpers");
    return 0;
}

inline void login(const httplib::Request& req, httplib::Response& res)
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

inline std::string load_assets(const std::string& path)
{

    std::ifstream file(path.c_str(), std::ios::in);

    std::stringstream stream;

    stream << file.rdbuf();

    file.close();

    std::string assets(stream.str());

    return assets;
}

inline std::string dump_headers(const httplib::Headers& headers)
{
    std::string s;
    char buf[BUFSIZ];

    for (const auto& x : headers)
    {
        fmt::print("%s: %s\n", x.first.c_str(), x.second.c_str());
        s += buf;
    }

    return s;
}

inline std::string log(const httplib::Request& req,
                       const httplib::Response& res)
{
    std::string s;
    char buf[BUFSIZ];

    s += "================================\n";

    fmt::print("%s %s %s", req.method.c_str(), req.version.c_str(),
               req.path.c_str());
    s += buf;

    std::string query;
    for (auto it = req.params.begin(); it != req.params.end(); ++it)
    {
        const auto& x = *it;
        fmt::print("%c%s=%s", (it == req.params.begin()) ? '?' : '&',
                   x.first.c_str(), x.second.c_str());
        query += buf;
    }
    fmt::print("%s\n", query.c_str());
    s += buf;

    // s += dump_headers(req.headers);

    s += "--------------------------------\n";

    fmt::print("%d %s\n", res.status, res.version.c_str());
    s += buf;
    s += dump_headers(res.headers);
    s += "\n";

    if (!res.body.empty())
    {
        s += res.body;
    }

    s += "\n";

    return s;
}
inline std::string stringify(const rapidjson::Value& o)
{
    StringBuffer sb;
    Writer<StringBuffer> writer(sb);
    o.Accept(writer);
    return sb.GetString();
}

#endif // __HELPERS_H__
