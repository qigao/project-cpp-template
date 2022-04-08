#ifndef __SERVER_H__
#define __SERVER_H__

#include "socket/base_queue.hpp"
#include "typedef.h"
#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <uvw.hpp>

class TcpServerHandle : public BaseQueue
{
private:
  /* data */
  tcp_handle   m_handle;
  std::string  m_ip;
  int          m_port;

public:
  /**---------------------------------------------------------------------------
   **                             public method
   *
   *----------------------------------------------------------------------------**/
  TcpServerHandle(std::string &ip, int port, tcp_handle &handle)
    : m_handle(handle),//
      m_ip(ip), m_port(port)
  {}
  ~TcpServerHandle() {}
  void listen()
  {
    m_handle->on<uvw::ErrorEvent>(
      [](const uvw::ErrorEvent &event, uvw::TCPHandle &) {
        std::cout << "server error: " << event.what() << std::endl;
      });

    m_handle->on<uvw::ListenEvent>([&](const uvw::ListenEvent &,
                                     uvw::TCPHandle &tcp_handle) {
      std::cout << "listen" << std::endl;

      auto peer_handler = tcp_handle.loop().resource<uvw::TCPHandle>();
      peer_handler->on<uvw::ErrorEvent>(
        [&](
          const uvw::ErrorEvent &event, uvw::TCPHandle &) { on_error(event); });

      peer_handler->on<uvw::CloseEvent>(
        [&](const uvw::CloseEvent &, uvw::TCPHandle &) { on_socket_close(); });

      peer_handler->on<uvw::DataEvent>(
        [&](const uvw::DataEvent &event, uvw::TCPHandle &handle) {
          on_read(event);
        });

      peer_handler->on<uvw::EndEvent>(
        [&](const uvw::EndEvent &, uvw::TCPHandle &handle) {
          after_read(handle);
        });

      tcp_handle.accept(*peer_handler);

      uvw::Addr remote_peer = peer_handler->peer();
      std::cout << "remote: " << remote_peer.ip << " " << remote_peer.port
                << std::endl;

      peer_handler->read();
    });

    m_handle->once<uvw::CloseEvent>(
      [&](const uvw::CloseEvent &, uvw::TCPHandle &) { on_socket_close(); });

    m_handle->bind(m_ip, m_port);
    m_handle->listen();
  }
  void loop_close() { m_handle->close(); }

private:
  /**-------------------------------------------------------------------------
   **                       private socket method
   *--------------------------------------------------------------------------**/
  TcpServerHandle(/* args */) = delete;
  static void on_socket_close() { std::cout << "socket closed" << std::endl; }

  static void on_error(const uvw::ErrorEvent &event)
  {
    std::cout << "Server error " << event.what() << std::endl;
  }
  void after_read(uvw::TCPHandle &handle)
  {
    std::cout << "data stream end " << std::endl;
    int count = 0;
    handle.loop().walk([&count](auto &) { ++count; });
    std::cout << "still alive: " << count << " handles" << std::endl;
    handle.close();
  }
  static void on_read(const uvw::DataEvent &event)
  {
    std::cout.write(event.data.get(), event.length) << std::endl;
    std::cout << "data length: " << event.length << std::endl;
  }
};

#endif// __SERVER_H__
