#ifndef __TCP_CLIENT_HANDLE_H__
#define __TCP_CLIENT_HANDLE_H__

#include "socket/base_queue.hpp"
#include "typedef.h"
#include <future>
#include <iostream>
#include <rpcndr.h>
#include <thread>
#include <uvw.hpp>
#include <uvw/stream.h>
#include <uvw/tcp.h>
class TcpClientHandle : public BaseQueue
{
public:
  TcpClientHandle(std::string &ip, int &port, tcp_handle &handle)
    : m_handle(handle),//
      m_ip(ip), m_port(port),//
      m_shouldExit(false)
  {}

  bool is_active() { return !m_shouldExit; }
  void start()
  {
    m_shouldExit = false;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    m_thread_write = std::thread(&TcpClientHandle::sock_write_data, this);
  }

  void close()
  {
    m_handle->stop();
    m_handle->close();
    m_shouldExit = true;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    if (m_thread_write.joinable()) m_thread_write.join();
  }
  void setup()
  {
    m_handle->on<uvw::ErrorEvent>(
      [](const uvw::ErrorEvent &event, uvw::TCPHandle &) {
        std::cout << "client error: " << event.what() << std::endl;
      });
    m_handle->once<uvw::ConnectEvent>([&](const auto &, auto &) {
      std::cout << "connect event ..." << std::endl;
    });
    m_handle->once<uvw::CloseEvent>([&](const auto &, auto &) {
      std::cout << "close event ..." << std::endl;
      m_shouldExit = true;
    });
    m_handle->once<uvw::WriteEvent>([&](const auto &, auto &) {
      std::cout << "write event ..." << std::endl;
    });
    m_handle->connect(m_ip, m_port);
    m_shouldExit = false;
  }
  void queue_data(const std::string &data)
  {
    auto len    = data.length();
    auto frame  = std::make_shared<DataFrame>();
    frame->data = std::make_shared<char[]>(len + 1);
    frame->len  = len;
    std::memcpy(frame->data.get(), data.data(), len);
    write_queue(frame);
  }

  void queue_data(const char *data, size_t len)
  {
    auto frame  = std::make_shared<DataFrame>();
    frame->data = std::make_shared<char[]>(len + 1);
    frame->len  = len;
    std::memcpy(frame->data.get(), data, len);
    write_queue(frame);
  }

private:
  void sock_write_data()
  {
    while (!m_shouldExit) {
      auto frame  = std::make_shared<DataFrame>();
      frame->data = std::make_shared<char[]>(1024);
      bool status = read_queue(frame);
      if (status) {
        auto len = frame->len;

        auto dataWrite = std::make_unique<char[]>(len);
        std::memcpy(dataWrite.get(), frame->data.get(), len);
        m_handle->tryWrite(std::move(dataWrite), len);
      } else {
        std::cout << " empty queue" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
      }
    }
  }

private:
  TcpClientHandle() = delete;

private:
  /* data */
  tcp_handle        m_handle;
  std::string       m_ip;
  int               m_port;
  std::atomic<bool> m_shouldExit;
  std::thread       m_thread_write;
};

#endif// __TCP_CLIENT_HANDLE_H__
