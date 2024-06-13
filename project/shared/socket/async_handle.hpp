#ifndef __ASYNC_HANDLE_H__
#define __ASYNC_HANDLE_H__
#include "typedef.h"
#include <iostream>
#include <string>
#include <uvw.hpp>
class AsyncHandle
{
public:
    AsyncHandle(async_handle& handle) : m_handle(handle) {}
    void setup()
    {
        m_handle->on<uvw::ErrorEvent>([](auto&&...) {});
        m_handle->on<uvw::AsyncEvent>(
            [&](auto const&, auto& hndl)
            {
                std::cout << "async data event ..." << std::endl;
                // auto data  = handle.data<char[]>();
                // auto len   = strlen(data.get());
                // auto frame = std::make_shared<DataFrame>(len);
                // std::memcpy(frame->data.get(), data.get(), len);
                // m_queue->enqueue(frame);
            });
    }
    void close() { m_handle->close(); }

    void send_event(std::string const& data)
    {
        std::unique_ptr<char[]> bytes(
            std::make_unique<char[]>(data.size() + 1));
        std::copy(std::make_move_iterator(std::begin(data)),
                  std::make_move_iterator(std::end(data)), bytes.get());
        m_handle->data(std::move(bytes));
        m_handle->send();
    }

private:
    /* data */
    async_handle m_handle;
    tcp_handle tcp;
    sharedQueue m_queue;
};

#endif // __ASYNC_HANDLE_H__
