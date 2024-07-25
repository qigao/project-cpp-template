#ifndef __HELPERS_H__
#define __HELPERS_H__

#include "constants.hpp"
#include "http_lib_header.hpp"

#include <iostream>

inline std::string dump_headers(httplib::Headers const& headers)
{
    std::string result;
    result.reserve(headers.size() * 64); // Rough estimate to reduce allocations

    for (auto const& header : headers)
    {
        result += header.first;
        result += ": ";
        result += header.second;
        result += "\n";
    }

    return result;
}

inline static void dump_http_request(httplib::Request const& r)
{
    std::cout <<"REQ.PATH = "<<r.path <<" REQ.METHOD = " << r.method << std::endl;
    for (auto it = r.headers.begin(); it != r.headers.end(); ++it)
    {
        std::cerr <<"REQ.PATH = "<<r.path << " HEADER[" << it->first << "]" << it->second
                  << std::endl;
    }
}

inline static void dump_http_response(httplib::Response const& r)
{
    std::cout << "RESP.STATUS=" << r.status << std::endl;
    for (auto it = r.headers.begin(); it != r.headers.end(); ++it)
    {
        std::cerr << "RESP.HEADER[" << it->first << "]" << it->second
                  << std::endl;
    }
    if (r.status != 200 && r.status != 206)
    {
        std::cerr << "RESP.BODY = " << r.body << std::endl;
    }
}

inline static void dump_request_response(httplib::Request const& req,
                                         httplib::Response const& res)
{
    dump_http_request(req);
    dump_http_response(res);
}

#endif // __HELPERS_H__
