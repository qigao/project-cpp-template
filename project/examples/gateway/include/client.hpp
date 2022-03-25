#ifndef __CLIENT_H__
#define __CLIENT_H__

#include "typedef.h"
#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <uvw.hpp>
class raw_client
{
private:
  /* data */

  shared_loop loop;

  /* --------------------------- private raw client --------------------------- */
  raw_client(/* args */) {}

public:
  raw_client(shared_loop default_loop) : loop(std::move(default_loop)) {}
  ~raw_client() {}
  void conn(std::string &ip, int port)
  {
    auto tcp = loop->resource<uvw::TCPHandle>();
    tcp->on<uvw::ErrorEvent>([](const uvw::ErrorEvent &, uvw::TCPHandle &) { assert(false); });

    tcp->once<uvw::WriteEvent>([](const uvw::WriteEvent &, uvw::TCPHandle &handle) {
      std::cout << "write" << std::endl;
      handle.close();
    });

    tcp->once<uvw::ConnectEvent>([](const uvw::ConnectEvent &, uvw::TCPHandle &handle) {
      std::cout << "connect" << std::endl;

      auto dataTryWrite = std::unique_ptr<char[]>(new char[1]{ 'a' });
      int  bw           = handle.tryWrite(std::move(dataTryWrite), 1);
      std::cout << "written: " << ((int)bw) << std::endl;

      auto dataWrite = std::unique_ptr<char[]>(new char[2]{ 'b', 'c' });
      handle.write(std::move(dataWrite), 2);
    });

    tcp->once<uvw::CloseEvent>(
      [](const uvw::CloseEvent &, uvw::TCPHandle &) { std::cout << "close" << std::endl; });
    tcp->connect(ip, port);
  }
};


#endif// __CLIENT_H__
