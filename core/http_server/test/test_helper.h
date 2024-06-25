#ifndef __TEST_HELPER_H__
#define __TEST_HELPER_H__

constexpr char const* yml_demo_data = R"(server:
    port: 4443
    shared_folder: ./shared
    delete_after_download: true
    ssl:
        enabled: false
        key_file: ./server.key
        cert_file: ./server.crt
        root_ca_file: ./rootCA/rootCA-crt.pem
        root_ca_key_file: ./rootCA/rootCA-key.pem
    auth:
        enabled: true
        token: 123456
    webhook:
        enabled: true
        url: http://127.0.0.1:8080/webhook
        api_key: 123456
        headers:
            - header: x-test
              value: true
            - header: x-agent
              value: mview_http_webhook
)";

#endif // __TEST_HELPER_H__
