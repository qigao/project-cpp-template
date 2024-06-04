#ifndef CPP_CORE_PROJECT_HTTP_SERVER_HPP
#define CPP_CORE_PROJECT_HTTP_SERVER_HPP
#include <httplib.h>
#include <string>

#include "my_exe/constants.hpp"
#include "my_exe/helpers.hpp"
#include <algorithm>
#include <cstdio>
#include <filesystem>
#include <fmt/core.h>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <nlohmann/json.hpp>
#include <vector>
using json = nlohmann::json;
#define SHARED_PATH "Shared"
namespace bf = std::filesystem;
using namespace std::placeholders;
// larger chunk size get faster download speed, more memory usage, more
// bandwidth usage, more disk I/O usage
const size_t chunk_size = 32 * 1024;
class MyServer
{
public:
    // 1.服务器创建---创建共享文件
    MyServer()
    {
        // 不存在则创建
        if (!bf::exists(SHARED_PATH))
        {
            bf::create_directory(SHARED_PATH);
        }
    }
    std::map<int, std::string> db;
    size_t id = 0;
    // 2.服务器启动 --- 注册回调函数,开始监听
    void ServerStart(uint16_t port)
    {
        _server.Get("/hostpair",
                    std::bind(&MyServer::GetHostPair, this, _1, _2));
        _server.Get("/list", std::bind(&MyServer::GetFileList, this, _1, _2));
        _server.Get("/list/(.*)",
                    std::bind(&MyServer::getFileByRange, this, _1, _2));
        _server.Get("/", [](const httplib::Request&, httplib::Response& res)
                    { res.set_content("Hello World!", TEXT_PLAIN); });

        _server.Get(R"(/msg/(\d+))",
                    std::bind(&MyServer::getMsg, this, _1, _2));
        _server.Post("/",
                     [&](const httplib::Request& req, httplib::Response& res)
                     { this->postMsg(req, res); });

        _server.Patch("/",
                      [&](const httplib::Request& req, httplib::Response& res)
                      { this->patchMsg(req, res); });

        _server.Post("/multipart",
                     [&](const httplib::Request& req, httplib::Response& res)
                     { this->uploadByMultiForm(req, res); });
        _server.Get("/upload",
                    [&](const httplib::Request& req, httplib::Response& res)
                    { this->upload(req, res); });

        _server.Delete(R"(/msg/(\d+))",
                       [&](const httplib::Request& req, httplib::Response& res)
                       { this->deleteMsg(req, res); });
        _server.Get("download/:id",
                    [&](const httplib::Request& req, httplib::Response& res)
                    { this->getFileByOrder(req, res); });
        _server.set_error_handler(
            [&](const httplib::Request& req, httplib::Response& res)
            {
                char buf[BUFSIZ];
                snprintf(
                    buf, sizeof(buf),
                    "<p>Error Status: <span style='color:red;'>%d</span></p>",
                    res.status);
                res.set_content(buf, "text/html");
            });

        // Logging
        _server.set_logger(
            [&](const httplib::Request& req, const httplib::Response& res)
            {
                for (std::multimap<std::string, std::string>::const_iterator
                         it = req.params.begin();
                     it != req.params.end(); ++it)
                {
                    std::string params = it->first + "=" + it->second;
                    std::cout << "[" << __FILE__ << ":" << __LINE__ << "] "
                              << it->first << "=" << it->second << std::endl;
                }
            });
        _server.listen("0.0.0.0", port);
    }

private:
    // 3.响应配对请求
    void GetHostPair(const httplib::Request& req, httplib::Response& rsp)
    {
        rsp.status = 200;
    }

    // 4.响应文件列表请求
    void GetFileList(const httplib::Request& req, httplib::Response& rsp)
    {
        // 扫描目录 --- bf库
        bf::directory_iterator item_begin(SHARED_PATH);
        bf::directory_iterator item_end;
        for (; item_begin != item_end; ++item_begin)
        {
            // 文件夹跳过
            if (bf::is_directory(*item_begin))
                continue;

            // 文件则放入body中,用\n分割标记
            auto pathname = item_begin->path();
            auto path = pathname.filename().string();
            rsp.body += path + '\n';
        }
        // 设置body格式--文本文件 和状态码
        rsp.set_header(CONTENT_TYPE, "text/html");
        rsp.status = 200;
    }

    // 5.1计算分块大小
    bool RangeParse(std::string& range, int64_t& start, int64_t& len)
    {
        // RANGE: bytes=start-end
        // 检查格式
        auto pos1 = range.find('=');
        auto pos2 = range.find('-');
        if (pos1 == std::string::npos || pos2 == std::string::npos)
        {
            std::cout << "range格式错误\n";
            return false;
        }

        // 解析start,end
        int64_t end;
        auto sstart = range.substr(pos1 + 1, pos2 - pos1 - 1);
        auto send = range.substr(pos2 + 1);
        std::stringstream tmp;
        tmp << sstart;
        tmp >> start;
        tmp.clear();
        tmp << send;
        tmp >> end;
        len = end - start + 1;

        return true;
    }
    // 5.响应下载请求
    void getFileByRange(const httplib::Request& req, httplib::Response& rsp)
    {
        // 5.1解析下载路径--- req: /list/a.txt ---> Download/a.txt
        bf::path path(req.path);
        // stringstream拼接字符串 , filename是文件名
        std::stringstream name;
        name << SHARED_PATH << "/" + path.filename().string();

        // 没有请求的文件,返回404
        if (!bf::exists(name.str()))
        {
            rsp.status = 404;
            return;
        }
        // 请求的是目录,返回403
        if (bf::is_directory(name.str()))
        {
            rsp.status = 403;
            return;
        }

        // 使用bf::file_size获取文件大小,返回HEAD响应
        auto fsize = bf::file_size(name.str());
        if (req.method == "HEAD")
        {
            rsp.status = 200;
            std::string len = std::to_string(fsize);
            rsp.set_header("Content-Length", len);

            return;
        }

        // 分块下载
        // 解析req,看没有有分块下载
        if (!req.has_header("Range"))
        {
            rsp.status = 400;
            return;
        }
        // 计算分块大小
        auto range = req.get_header_value("Range");
        int64_t start, len;
        bool ret = RangeParse(range, start, len);
        if (!ret)
        {
            return;
        }

        // file读入文件
        std::ifstream file(name.str(), std::ios::binary);
        // 文件没打开
        if (!file.is_open())
        {
            std::cout << "文件打开失败\n";
            return;
        }
        // 跳转到下载位置,写入rsp
        file.seekg(start, std::ios::beg);
        rsp.body.resize(len);
        file.read(rsp.body.data(), len);
        // 文件出错
        if (!file.good())
        {
            std::cout << "文件出错\n";
            rsp.status = 500;
            return;
        }
        file.close();

        // 组织rsp
        rsp.status = 206;
        rsp.set_header(CONTENT_TYPE, OCTET_STREAM);
        std::cout << range << "文件传输成功!\n";
    }
    bool bValidReq(json jsonObj, std::vector<std::string> keys)
    {
        std::vector<bool> vTest;

        for (size_t i = 0; i < keys.size(); i++)
        {
            if (!jsonObj[keys[i]].empty())
                vTest.push_back(true);
            else
                vTest.push_back(false);
        }

        std::vector<bool>::iterator it =
            std::find(vTest.begin(), vTest.end(), false);

        return !it != vTest.end();
    }
    void getMsg(const httplib::Request& req, httplib::Response& res)
    {

        auto n = req.matches[1];

        nlohmann::json jRes;

        try
        {
            jRes["id"] = std::stoi(n);
            jRes["msg"] = db[std::stoi(n)];

            res.set_content(jRes.dump(), APP_JSON);
        }
        catch (const std::exception&)
        {
            res.set_content("Cannot find the requested message.", TEXT_PLAIN);
        }
    }
    void postMsg(const httplib::Request& req, httplib::Response& res)
    {
        nlohmann::json json = nlohmann::json::parse(req.body);

        if (bValidReq(json, {"msg"}))
        {
            db.insert({id, json["msg"]});

            nlohmann::json jRes;

            jRes["id"] = id;
            jRes["msg"] = db[id];

            id++;

            res.set_content(jRes.dump(), APP_JSON);
        }
        else
        {
            res.set_content("The request doesn't meet the requirements",
                            TEXT_PLAIN);
        }
    }
    void patchMsg(const httplib::Request& req, httplib::Response& res)
    {
        nlohmann::json json = nlohmann::json::parse(req.body);

        if (bValidReq(json, {"id", "msg"}))
        {
            db[json["id"]] = json["msg"];

            nlohmann::json jRes;

            jRes["id"] = json["id"];
            jRes["msg"] = db[json["id"]];

            res.set_content(jRes.dump(), APP_JSON);
        }
        else
            res.set_content("The request doesn't meet the requirements",
                            TEXT_PLAIN);
    }
    void deleteMsg(const httplib::Request& req, httplib::Response& res)
    {

        auto n = req.matches[1];

        db[std::stoi(n)] = "deleted";

        nlohmann::json jRes;

        jRes["id"] = std::stoi(n);
        jRes["msg"] = "deleted";

        res.set_content(jRes.dump(), APP_JSON);
    }
    void uploadByMultiForm(const httplib::Request& req, httplib::Response& res)
    {
        auto binary_file = req.get_file_value("image_file");
        auto size = req.files.size();
        auto hasImageFile = req.has_file("image_file");
        if (!hasImageFile && size == 0)
        {
            res.set_content("invalid multipart form", "text/html");
            return;
        }
        std::cout << "image file length: " << binary_file.content.length()
                  << std::endl
                  << "image file name: " << binary_file.filename << std::endl;
        {
            std::ofstream ofs(binary_file.filename, std::ios::binary);
            ofs << binary_file.content;
        }
    }
    /**
     *  the upload html form
     * @param req
     * @param res
     */
    void upload(const httplib::Request& req, httplib::Response& res)
    {
        auto html = R"(<form id="formElem">
                     <input type="file" name="image_file" accept="image/*"><br>
                        <input type="submit">
                        </form>
                        <script>
                            formElem.onsubmit = async (e) => {
                            e.preventDefault();
                            let res = await fetch('/multipart', {
                                method: 'POST',
                                body: new FormData(formElem)
                            });
                            console.log(await res.text());
                        };
                   </script>
                   )";
        res.set_content(html, "text/html");
    }

    /**
     * download file by byte order in binary form
     * @param req
     * @param res
     */
    void getFileByOrder(const httplib::Request& req, httplib::Response& res)
    {
        // 解析下载路径--- req: /list/a.txt ---> Download/a.txt
        bf::path file_path(req.path);
        std::stringstream name;
        name << SHARED_PATH << "/" + file_path.filename().string();

        std::ifstream file_reader(file_path,
                                  std::ifstream::binary | std::ifstream::in);

        // 404 not found
        if (!file_reader.good())
        {
            res.status = 404;
            res.set_content("404 not found", "text/plain; charset=UTF-8");
            return;
        }

        res.set_content_provider(
            OCTET_STREAM, // Content type
            [&](size_t offset, httplib::DataSink& sink)
            {
                // open file
                std::ifstream file_reader(file_path, std::ifstream::binary |
                                                         std::ifstream::in);
                // can't open file, cancel process
                if (!file_reader.good())
                {
                    return false;
                }

                // get file size
                file_reader.seekg(0, file_reader.end);
                size_t file_size = static_cast<size_t>(file_reader.tellg());
                file_reader.seekg(0, file_reader.beg);

                // check offset and file size, cancel process if offset >=
                // file_size
                if (offset >= file_size)
                {
                    return false;
                }

                // prepare read size of chunk
                size_t read_size = 0;
                if (file_size - offset > chunk_size)
                {
                    read_size = chunk_size;
                }
                else
                {
                    read_size = file_size - offset;
                }

                // allocate temp buffer, and read file chunk into buffer
                std::vector<char> buffer;
                buffer.reserve(chunk_size);
                file_reader.seekg(static_cast<long>(offset), file_reader.beg);
                file_reader.read(&buffer[0], static_cast<long>(read_size));
                file_reader.close();

                // write buffer to sink
                sink.write(&buffer[0], read_size);
                return true; // return 'false' if you want to cancel the
                             // process.
            });
    }
#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
    httplib::SSLServer _server("../server/server-crt.pem",
                               "../server/server-key.pem",
                               "../rootCA/rootCA-crt.pem");
#else
    httplib::Server _server;
#endif
};
#endif // CPP_CORE_PROJECT_HTTP_SERVER_HPP
