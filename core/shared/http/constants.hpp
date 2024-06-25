#ifndef CPP_CORE_PROJECT_CONSTANTS_HPP
#define CPP_CORE_PROJECT_CONSTANTS_HPP

#define APP_JSON "application/json"
#define TEXT_PLAIN "text/plain"
#define OCTET_STREAM "application/octet-stream"
#define CONTENT_TYPE "Content-Type"
#define CACHE_CONTROL "Cache-Control"
#define CONTENT_RANGE "Content-Range"
#define CONTENT_LENGTH "Content-Length"
#define USER_AGENT "User-Agent"
#define CONTENT_DISPOSITION "Content-Disposition"
#define ACCEPT_RANGES "Accept-Ranges"
#define API_KEY "X-API-Key"
#define HTTP_TEXT_RESPONSE_FORMATTER                                           \
    "<p>Error Status: <span style='color:red;'>{}</span></p>"
#define SHA_256_HASH_HEADER "X-Hub-Signature-256"

#define CHUNK_SIZE (32 * 1024)

#endif // CPP_CORE_PROJECT_CONSTANTS_HPP
