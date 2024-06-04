#include <cstdlib>

#include "my_exe/constants.hpp"
#include "my_exe/helpers.hpp"
#include <fmt/core.h>
#include <httplib.h>
#include <string>
using namespace httplib;
void startServerSSL()
{
#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
    SSLServer svr("../server/server-crt.pem", "../server/server-key.pem",
                  "../rootCA/rootCA-crt.pem");
#else
    Server svr;
#endif
    svr.Post("/", login);
    svr.Get("/", [](const Request& req, Response& res)
            { res.set_content(R"({"result": "success"})", APP_JSON); });
    svr.Delete("/", [](const Request& req, Response& res)
               { res.set_content(R"({"result": "success"})", APP_JSON); });
    svr.Put("/",
            [](const Request& req, Response& res)
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
            });
    svr.set_error_handler(
        [](const Request& req, Response& res)
        { res.set_content(R"({"result": "error"})", APP_JSON); });
    svr.Get("/hi", [](const Request& req, Response& res)
            { res.set_content("Hello World!", "text/plain"); });

    svr.Get(R"(/numbers/(\d+))",
            [&](const Request& req, Response& res)
            {
                auto numbers = req.matches[1];
                res.set_content(numbers, "text/plain");
            });

    svr.Get("/body-header-param",
            [](const Request& req, Response& res)
            {
                if (req.has_header("Content-Length"))
                {
                    auto val = req.get_header_value("Content-Length");
                }
                if (req.has_param("key"))
                {
                    auto val = req.get_param_value("key");
                }
                res.set_content(req.body, "text/plain");
            });

    svr.Get("/stop", [&](const Request& req, Response& res) { svr.stop(); });

    fmt::print("HTTP server start!");
    svr.listen("0.0.0.0", 4443);
}

void startServer()
{
    Server svrs;
    svrs.Get("/",
             [](const Request& req, Response& res)
             {
                 res.set_content(
                     "<a href='https://0.0.0.0:4443/'>Enter SSL</a>",
                     "text/html");
             });
    fmt::print("HTTP server start!");
    svrs.listen("0.0.0.0", 8080);
}

void startPing(int sec)
{
    while (sec > 0)
    {
        sleep(sec);
#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
        httplib::SSLClient cli("https://0.0.0.0:4443",
                               "../client/client-crt.pem",
                               "../client/client-key.pem");
        cli.set_ca_cert_path("../rootCA/rootCA-crt.pem");
        cli.enable_server_certificate_verification(false);
#else
        httplib::Client cli("https://0.0.0.0:4443");
#endif
        auto res = cli.Get("/");
        fmt::print("ping : status = {}, - body = {}",
                   std::to_string(res->status), res->body);

        res = cli.Delete("/");
        fmt::print("https delete : status = {}, - body = {}",
                   std::to_string(res->status), res->body);

        auto body = R"({"password": "secret"})";
        res = cli.Post("/", body, APP_JSON);
        fmt::print("https post : status = {},  - body = {}",
                   std::to_string(res->status), res->body);

        res = cli.Put("/", body, APP_JSON);
        fmt::print("https put : status =  {},  - body = {}",
                   std::to_string(res->status), res->body);
    }
}

int main(int argc, char** argv)
{
    int sec = 2;
    if (argc > 1)
    {
        sec = atoi(argv[1]);
    }
    std::thread serverSSL(startServerSSL), server(startServer),
        pingSSL(startPing, sec);
    serverSSL.join();
    server.join();
    pingSSL.join();
}
