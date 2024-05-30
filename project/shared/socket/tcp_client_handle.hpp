#ifndef __TCP_CLIENT_HANDLE_H__
#define __TCP_CLIENT_HANDLE_H__

#include "base_queue.hpp"
#include "typedef.h"
#include "utils/types.h"
#include <future>
#include <iostream>
#include <memory>
#include <thread>
#include <uvw.hpp>


class TcpClientHandle : public BaseQueue
{
public:
  TcpClientHandle(std::string &ip, int &port, shared_loop &loop)
    : m_tcp(loop->resource<uvw::TCPHandle>()),//
      m_ip(ip), m_port(port),//
      m_shouldExit(false)
  {}

  bool is_active()const { return !m_shouldExit; }
  void start()
  {
    m_shouldExit = false;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    m_thread_write = std::thread(&TcpClientHandle::sock_write_data, this);
  }

  void close()
  {
    m_tcp->stop();
    m_tcp->close();
    m_shouldExit = true;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    if (m_thread_write.joinable()) m_thread_write.join();
  }
  void setup()
  {
    m_tcp->on<uvw::ErrorEvent>(
      [](const uvw::ErrorEvent &event, uvw::TCPHandle &) {
        std::cout << "client error: " << event.what() << std::endl;
      });
    m_tcp->once<uvw::ConnectEvent>([&](const auto &, auto &) {
      std::cout << "connect event ..." << std::endl;
    });
    m_tcp->once<uvw::CloseEvent>([&](const auto &, auto &) {
      std::cout << "close event ..." << std::endl;
      m_shouldExit = true;
    });
    m_tcp->once<uvw::WriteEvent>([&](const auto &, auto &) {
      std::cout << "write event ..." << std::endl;
    });
    m_tcp->connect(m_ip, m_port);
    m_shouldExit = false;
  }
  void queue_data(const std::string &data)
  {
    auto len    = data.length();
    auto frame  = std::make_shared<DataFrame>();
    frame->data = make_array<u8>(len + 1);
    frame->len  = len;
    std::memcpy(frame->data.get(), data.data(), len);
    write_queue(frame);
  }

  void queue_data(const char *data, size_t len)
  {
    auto frame  = std::make_shared<DataFrame>();
    frame->data = make_array<u8>(len);
    frame->len  = len;
    std::memcpy(frame->data.get(), data, len);
    write_queue(frame);
  }

private:
  void sock_write_data()
  {
    while (!m_shouldExit) {
      auto frame  = std::make_shared<DataFrame>();
      frame->data = make_array<u8>(256);
      bool status = read_queue(frame);
      if (status) {
        auto len = frame->len;

        auto dataWrite = std::make_unique<char[]>(len);
        std::memcpy(dataWrite.get(), frame->data.get(), len);
        m_tcp->tryWrite(std::move(dataWrite), len);
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
  tcp_handle        m_tcp;
  std::string       m_ip;
  int               m_port;
  std::atomic<bool> m_shouldExit;
  std::thread       m_thread_write;
};

#endif// __TCP_CLIENT_HANDLE_H__
