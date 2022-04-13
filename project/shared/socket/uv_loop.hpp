#ifndef __UV_LOOP_H__
#define __UV_LOOP_H__

#include "typedef.h"
#include <future>
#include <thread>
#include <uvw.hpp>

class UvLoop
{
public:
  UvLoop()
    : m_loop(uvw::Loop::getDefault()),//
      m_async_handle(m_loop->resource<uvw::AsyncHandle>()),//
      m_idle_handle(m_loop->resource<uvw::IdleHandle>())//
  {}
  auto get_loop() const { return m_loop; }
  void setup()
  {
    m_async_handle->on<uvw::ErrorEvent>([](auto const &, auto &) {});
    m_async_handle->on<uvw::CloseEvent>([&](auto const &, auto &) {
      m_idle_handle->stop();
      m_idle_handle->close();
    });
  }
  void run()
  {
    std::thread t([&]() {
      // to keep loop running
      m_idle_handle->start();
      m_loop->run();
    });
    t.detach();
  }
  void stop()
  {
    m_async_handle->send();
    m_async_handle->close();
  }

private:
  shared_loop  m_loop;
  async_handle m_async_handle;
  idle_handle  m_idle_handle;
};
#endif// __UV_LOOP_H__
