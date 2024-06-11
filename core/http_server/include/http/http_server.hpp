#ifndef CPP_CORE_PROJECT_HTTPSERVER_HPP
#define CPP_CORE_PROJECT_HTTPSERVER_HPP

#include <httplib.h>
#include <memory>
#include <string>
class HttpServer
{
public:
    HttpServer(int port = 0, unsigned int numThreads = 4,
               const std::string& certFile = "",
               const std::string& keyFile = "",
               const std::string& rootCaFile = "");
    virtual ~HttpServer();

    void start();
    void stop();
    inline std::shared_ptr<httplib::Server> getInternalServer()
    {
        return mServer;
    }

    inline int getBoundPort() const { return mBoundPort; }

    inline int getPort() const { return mPort; }

    inline void setPort(int value) { mPort = value; }
    inline void Get(const std::string& path, httplib::Server::Handler handler)
    {
        mServer->Get(path, handler);
    }
    /**
     * Registers a POST handler for the specified path.
     *
     * @param path The path to register the handler for.
     * @param handler The handler function to be called when a POST request is
     *                received for the specified path. The function should take
     * a const httplib::Request& and httplib::Response& as arguments and return
     * void.
     */
    inline void Post(const std::string& path, httplib::Server::Handler handler)
    {
        mServer->Post(path, handler);
    }

    /**
     * Registers a POST handler for the specified path.
     *
     * @param path The path to register the handler for.
     * @param handler The handler function to be called when a POST request is
     *                received for the specified path. The function should take
     * a const httplib::Request&, httplib::Response&, and const
     * httplib::ContentReader& as arguments and return void.
     */
    inline void
    PostWithContentHandler(const std::string& path,
                           httplib::Server::HandlerWithContentReader handler)
    {
        mServer->Post(path, handler);
    }

    inline void setSharedFolder(const std::string& folder)
    {
        mSharedFolder = folder;
    }

    inline std::string getSharedFolder() { return mSharedFolder; }

private:
    void error_handler(const httplib::Request& req, httplib::Response& res);
    void log_handler(const httplib::Request& req, const httplib::Response& res);
    std::string dump_headers(const httplib::Headers& headers);

    void post_route_handler(const httplib::Request& req,
                            httplib::Response& res);
    // private vars
    int mPort = 0;
    int mBoundPort = 0;
    bool mStopSignal = false;
    std::string mSharedFolder;
#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
    std::shared_ptr<httplib::SSLServer> mServer;
#else
    std::shared_ptr<httplib::Server> mServer;
#endif
    std::thread mListenThread;
    HttpServer(HttpServer const&) = delete;
    HttpServer& operator=(HttpServer const&) = delete;
};

#endif // CPP_CORE_PROJECT_HTTPSERVER_HPP
