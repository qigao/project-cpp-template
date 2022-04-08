#ifndef __UV_LOOP_H__
#define __UV_LOOP_H__

#include "typedef.h"
#include <future>
#include <thread>
#include <uvw.hpp>
#include <uvw/idle.h>
#include <uvw/loop.h>
class UvLoop
{
public:
  UvLoop() : m_loop(uvw::Loop::getDefault()) {}
  auto get_loop() { return m_loop; }
  auto get_idle() { return m_loop->resource<uvw::IdleHandle>(); }
  auto get_async() { return m_loop->resource<uvw::AsyncHandle>(); }
  auto get_tcp() { return m_loop->resource<uvw::TCPHandle>(); }
  auto get_udp() { return m_loop->resource<uvw::UDPHandle>(); }
  void run()
  {
    std::thread t([&]() { m_loop->run(); });
    t.detach();
  }
  void stop() { m_loop->stop(); }

private:
  const shared_loop m_loop;
};
#endif// __UV_LOOP_H__
