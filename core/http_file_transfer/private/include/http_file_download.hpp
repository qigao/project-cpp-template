#ifndef CPP_CORE_PROJECT_HTTP_FILE_DOWNLOAD_HPP
#define CPP_CORE_PROJECT_HTTP_FILE_DOWNLOAD_HPP
#include "fs.hpp"
#include "spdlog/spdlog.h"
#include "stream_cast.hpp"
#include <cstdint>
#include <httplib.h>
#include <iostream>
#include <stdexcept>
#include <string>
class HttpFileDownload
{
public:
    explicit HttpFileDownload(std::string const& partial_suffix = ".part",
                              int max_trials = 10)
        : max_trials_(max_trials), partial_suffix_(partial_suffix)
    {
    }

    std::string getPartialSuffix() const { return partial_suffix_; }

    void setPartialSuffix(std::string const& value) { partial_suffix_ = value; }

    int getMaxTrials() const { return max_trials_; }

    void setMaxTrials(int value) { max_trials_ = value; }

    bool doDownload(std::string const& resource_url,
                    std::string const& filename)
    {
        std::string temporary_filename = filename + partial_suffix_;
        std::string scheme_and_host;
        std::string resource_path;
        if (!breakup_url(resource_url, scheme_and_host, resource_path))
        {
            spdlog::info("breakup_url failed");
            return false;
        }

        spdlog::info("schema_and_host={}", scheme_and_host);
        spdlog::info("resource_path = {}", resource_path);
        httplib::Client client(scheme_and_host);
        client.set_logger(
            [](httplib::Request const& req, httplib::Response const& res)
            {
                dump_http_request(req);
                dump_http_response(res);
            });
        auto res = client.Head(resource_path);
        bool support_ranges_request = false;
        bool maybe_support_ranges_request = false;
        int64_t content_length = 0;
        bool resource_not_found = false;
        // probe ranges request capability
        if (!res)
        {
            std::cout << "No response" << std::endl;
            return false;
        }
        if (res->status == 200)
        {
            if (res->has_header("Accept-Ranges"))
            {
                if (res->get_header_value("Accept-Ranges") == "bytes")
                {
                    support_ranges_request = true;
                    if (res->has_header("Content-Length"))
                    {
                        content_length = sstream_cast<int64_t>(
                            res->get_header_value("Content-Length"));
                    }
                }
                else
                {
                    // typically Accept-Ranges: none
                    support_ranges_request = false;
                    if (res->has_header("Content-Length"))
                    {
                        content_length = sstream_cast<int64_t>(
                            res->get_header_value("Content-Length"));
                    }
                }
            }
            else
            {
                // maybe do not support ranges request
            }
        }
        else
        {
            if (res->status == 404)
            {
                spdlog::error("resource not found!");
                resource_not_found = true;
            }
            else
            {
                // ignore status other than "404 Not Found"
                maybe_support_ranges_request = true;
            }
        }
        if (resource_not_found)
            return false;
        bool completed = false;
        spdlog::info("before trails");
        for (int trials = 0; trials < max_trials_ && !completed; ++trials)
        {
            std::cout << "trials #" << trials << std::endl;
            int64_t already_downloaded_size = 0;
            try
            {
                already_downloaded_size = fs::get_file_size(temporary_filename);
            }
            catch (std::runtime_error const&)
            {
                // ignore non such file exception
            }
            if (content_length > 0 && already_downloaded_size == content_length)
            {
                // finished
                fs::rename(temporary_filename, filename);
                return true;
            }
            httplib::Headers headers;
            if (support_ranges_request || maybe_support_ranges_request)
            {
                httplib::Ranges ranges;
                ranges.push_back(std::make_pair(already_downloaded_size, -1));
                headers.insert(httplib::make_range_header(ranges));
            }
            httplib::Request req;
            httplib::Response res;
            httplib::Error err;
            httplib::ContentReceiver receiver =
                [temporary_filename, &res](char const* data, size_t size)
            {
                try
                {
                    if (res.status == 206)
                    {
                        // 206 Partial Content
                        fs::append(temporary_filename, data, size);
                    }
                    else if (res.status == 200)
                    {
                        // 202 OK
                        fs::write(temporary_filename, data, size);
                    }
                    return true;
                }
                catch (std::runtime_error const& e)
                {
                    spdlog::info("[c++ exception] {}", e.what());
                    return false;
                }
                catch (...)
                {
                    spdlog::info("[c++ exception] <UNKNOWN>");
                    return false;
                }
            };
            req.method = "GET";
            req.path = resource_path;
            req.content_receiver = [&receiver](char const* data, size_t size,
                                               uint64_t offset, uint64_t length)
            { return receiver(data, size); };
            req.headers = headers;
            if (!client.send(req, res, err))
            {
                spdlog::error("err = {}", httplib::to_string(err));
                if (res.status == 206 || res.status == 200 || res.status == -1)
                {
                    // res.status == -1 => no response received
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    continue;
                }
                else
                {
                    // FIXME?
                    spdlog::error("res.status == {}", res.status);
                    return false;
                }
            }
            fs::rename(temporary_filename, filename);
            // TODO How to determine
            completed = true;
        }
        return false;
    }

protected:
    static void dump_http_request(httplib::Request const& r)
    {
        std::ostream& os = std::cout;
        spdlog::info("REQ.METHOD = {}", r.method);
        for (auto it = r.headers.begin(); it != r.headers.end(); ++it)
        {
            spdlog::info("REQ.HEADER[{}] = {} ", it->first, it->second);
        }
    }

    static void dump_http_response(httplib::Response const& r)
    {
        std::ostream& os = std::cout;
        os << "RES.STATUS = " << r.status << std::endl;
        for (auto it = r.headers.begin(); it != r.headers.end(); ++it)
        {
            spdlog::info("RES.HEADER[{}] = {} ", it->first, it->second);
        }
        if (r.status != 200 && r.status != 206)
        {
            spdlog::info("RES.BODY = {}", r.body);
        }
    }

    static bool breakup_url(std::string const& url,
                            std::string& scheme_and_host, std::string& path)
    {
        std::string::size_type off = 0;
        std::string scheme_notation = "://";
        std::string::size_type pos = url.find(scheme_notation, off);
        if (pos == std::string::npos)
        {
            return false;
        }
        off = pos + scheme_notation.length();
        pos = url.find("/", off);
        if (pos == std::string::npos)
        {
            pos = url.length();
        }
        scheme_and_host = url.substr(0, pos);
        path = url.substr(pos);
        if (path.empty())
        {
            path = "/";
        }
        return true;
    }

private:
    int max_trials_;
    std::string partial_suffix_;
};

#endif // CPP_CORE_PROJECT_HTTP_FILE_DOWNLOAD_HPP
