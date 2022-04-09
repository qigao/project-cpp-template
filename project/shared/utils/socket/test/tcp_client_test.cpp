#include "socket/tcp_client_handle.hpp"
#include "socket/tcp_server_handle.hpp"
#include "socket/uv_loop.hpp"
#include <gtest/gtest.h>

#include <future>
#include <limits>
#include <string>
#include <uvw.hpp>

auto ip   = std::string("127.0.0.1");
int  port = 4242;
class TcpClientHandleTest : public ::testing::Test
{
protected:
  // Per-test-suite set-up.
  // Called before the first test in this test suite.
  // Can be omitted if not needed.
  static void SetUpTestSuite()
  {
    // Avoid reallocating static objects if called in subclasses of FooTest.
    if (tcp_server == nullptr) {
      auto loop   = UvLoop();
      auto handle = loop.get_tcp();
      tcp_server  = new TcpServerHandle(ip, port, handle);
      tcp_server->listen();
    }
  }

  // Per-test-suite tear-down.
  // Called after the last test in this test suite.
  // Can be omitted if not needed.
  static void TearDownTestSuite()
  {
    tcp_server->loop_close();
    delete tcp_server;
    tcp_server = nullptr;
  }

  // Some expensive resource shared by all tests.
  static TcpServerHandle *tcp_server;
};
TcpServerHandle *TcpClientHandleTest::tcp_server = nullptr;
TEST_F(TcpClientHandleTest, sendMessage)
{
  UvLoop loop = UvLoop();
  // create & setup tcp_client
  auto handle     = loop.get_tcp();
  auto tcp_client = TcpClientHandle(ip, port, handle);
  tcp_client.setup();
  tcp_client.start();
  loop.run();
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  tcp_client.queue_data("test-1");
  tcp_client.queue_data("test-11");
  ASSERT_TRUE(tcp_client.is_active());

  tcp_client.queue_data("test-12");
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  loop.stop();
  tcp_client.close();
}
