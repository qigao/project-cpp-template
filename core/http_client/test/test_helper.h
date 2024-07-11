#ifndef __TEST_HELPER_H__
#define __TEST_HELPER_H__

constexpr char const* yml_demo_data = R"(
    remote:
        port: 5060
        host: 127.0.0.1
        ssl-cert: ./ca-bundle.crt
        auth-token: 123456
        webhook-url: /webhook
        files-url: /upload
)";

#endif // __TEST_HELPER_H__
