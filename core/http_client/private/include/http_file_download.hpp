#ifndef CPP_CORE_PROJECT_HTTP_FILE_DOWNLOAD_HPP
#define CPP_CORE_PROJECT_HTTP_FILE_DOWNLOAD_HPP

#include "pch_headers.hpp"

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
#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
    bool doDownload(std::shared_ptr<httplib::SSLClient> mClient,
                    std::string const& resource_path,
                    std::string const& filename)
#else
    bool doDownload(std::shared_ptr<httplib::Client> mClient,
                    std::string const& resource_path,
                    std::string const& filename)
#endif
    {
        std::string temporary_filename = filename + partial_suffix_;
        auto res = mClient->Head(resource_path);
        bool support_ranges_request = false;
        bool maybe_support_ranges_request = false;
        int64_t content_length = 0;
        // probe ranges request capability
        if (!res)
        {
            std::cout << "No response" << std::endl;
            return false;
        }
        if (res->status == 200)
        {
            if (res->has_header(ACCEPT_RANGES))
            {
                if (res->get_header_value(ACCEPT_RANGES) == "bytes")
                {
                    support_ranges_request = true;
                    if (res->has_header(CONTENT_LENGTH))
                    {
                        content_length = sstream_cast<int64_t>(
                            res->get_header_value(CONTENT_LENGTH));
                    }
                }
                else
                {
                    // typically Accept-Ranges: none
                    support_ranges_request = false;
                    if (res->has_header(CONTENT_LENGTH))
                    {
                        content_length = sstream_cast<int64_t>(
                            res->get_header_value(CONTENT_LENGTH));
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
                std::cerr << "resource not found!" << std::endl;
                return false;
            }
            else
            {
                // ignore status other than "404 Not Found"
                maybe_support_ranges_request = true;
            }
        }
        bool completed = false;
        std::cout << "before trails" << std::endl;
        for (int trials = 0; trials < max_trials_ && !completed; ++trials)
        {
            std::cout << "trials # " << trials << std::endl;
            int64_t already_downloaded_size = 0;
            try
            {
                already_downloaded_size = get_file_size(temporary_filename);
            }
            catch (std::runtime_error const&)
            {
                // ignore non such file exception
            }
            if (content_length > 0 && already_downloaded_size == content_length)
            {
                // finished
                rename(temporary_filename, filename);
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
            httplib::Response resp;
            httplib::Error err;
            httplib::ContentReceiver receiver =
                [temporary_filename, &resp](char const* data, size_t size)
            {
                try
                {
                    if (resp.status == 206)
                    {
                        // 206 Partial Content
                        append(temporary_filename, data, size);
                    }
                    else if (resp.status == 200)
                    {
                        // 202 OK
                        fs_write(temporary_filename, data, size);
                    }
                    return true;
                }
                catch (std::runtime_error const& e)
                {
                    std::cerr << "[c++ exception] " << e.what() << std::endl;
                    return false;
                }
                catch (...)
                {
                    std::cerr << "[c++ exception] <UNKNOWN>" << std::endl;
                    return false;
                }
            };
            req.method = "GET";
            req.path = resource_path;
            req.content_receiver = [&receiver](char const* data, size_t size,
                                               uint64_t offset, uint64_t length)
            { return receiver(data, size); };
            req.headers = headers;
            if (!mClient->send(req, resp, err))
            {
                std::cerr << "err = " << httplib::to_string(err) << std::endl;
                if (resp.status == 206 || resp.status == 200 ||
                    resp.status == -1)
                {
                    // resp.status == -1 => no response received
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    continue;
                }
                else
                {
                    // FIXME?
                    std::cerr << "resp.status == " << resp.status << std::endl;
                    return false;
                }
            }
            rename(temporary_filename, filename);
            // TODO How to determine
            completed = true;
        }
        return false;
    }

private:
    int max_trials_;
    std::string partial_suffix_;
};

#endif // CPP_CORE_PROJECT_HTTP_FILE_DOWNLOAD_HPP
