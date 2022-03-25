#ifndef __SERVER_H__
#define __SERVER_H__

#include "typedef.h"
#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <uvw.hpp>

class raw_server
{
private:
  /* data */

  shared_loop loop;
  /* --------------------------- private raw server --------------------------- */
  raw_server(/* args */) {}

public:
  /* --------------------------- public raw server --------------------------- */
  //* INFO: only one server can be created in one process
  raw_server(shared_loop default_loop) : loop(std::move(default_loop)) {}
  ~raw_server() {}
  void listen(std::string &ip, int port)
  {
    auto tcp = loop->resource<uvw::TCPHandle>();
    tcp->on<uvw::ErrorEvent>([](const uvw::ErrorEvent &, uvw::TCPHandle &) { assert(false); });

    tcp->once<uvw::ListenEvent>([](const uvw::ListenEvent &, uvw::TCPHandle &srv) {
      std::cout << "listen" << std::endl;

      auto client = srv.loop().resource<uvw::TCPHandle>();
      client->on<uvw::ErrorEvent>([](const uvw::ErrorEvent &, uvw::TCPHandle &) { assert(false); });

      client->on<uvw::CloseEvent>(
        [ptr = srv.shared_from_this()](const uvw::CloseEvent &, uvw::TCPHandle &) {
          std::cout << "close" << std::endl;
          ptr->close();
        });

      srv.accept(*client);

      uvw::Addr local = srv.sock();
      std::cout << "local: " << local.ip << " " << local.port << std::endl;

      uvw::Addr remote = client->peer();
      std::cout << "remote: " << remote.ip << " " << remote.port << std::endl;

      client->on<uvw::DataEvent>([](const uvw::DataEvent &event, uvw::TCPHandle &) {
        std::cout.write(event.data.get(), event.length) << std::endl;
        std::cout << "data length: " << event.length << std::endl;
      });

      client->on<uvw::EndEvent>([](const uvw::EndEvent &, uvw::TCPHandle &handle) {
        std::cout << "end" << std::endl;
        int count = 0;
        handle.loop().walk([&count](auto &) { ++count; });
        std::cout << "still alive: " << count << " handles" << std::endl;
        handle.close();
      });

      client->read();
    });
    tcp->once<uvw::CloseEvent>(
      [](const uvw::CloseEvent &, uvw::TCPHandle &) { std::cout << "close" << std::endl; });

    tcp->bind(ip, port);
    tcp->listen();
  }
};

#endif// __SERVER_H__
