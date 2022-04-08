
#include "version.h"
#include <baselib.h>
#include <fstream>
#include <iostream>
#include <zmq.hpp>
#include <zmq_addon.hpp>
namespace baselib {

void printInfo()
{
  zmq::context_t ctx;
  zmq::socket_t  sock1(ctx, zmq::socket_type::push);
  zmq::socket_t  sock2(ctx, zmq::socket_type::pull);
  sock1.bind("tcp://127.0.0.1:*");
  const std::string last_endpoint = sock1.get(zmq::sockopt::last_endpoint);
  std::cout << "Connecting to " << last_endpoint << std::endl;
  sock2.connect(last_endpoint);

  std::array<zmq::const_buffer, 2> send_msgs = { zmq::str_buffer("foo"),
    zmq::str_buffer("bar!") };
  if (!zmq::send_multipart(sock1, send_msgs)) return;
  std::vector<zmq::message_t> recv_msgs;
  const auto ret = zmq::recv_multipart(sock2, std::back_inserter(recv_msgs));
  if (!ret) return;
  std::cout << "Got " << *ret << " messages" << std::endl;
  std::string dataPath = "data";
  std::cout << "Version: " << PROJECT_VERSION << "\n"
            << "Author: " << GIT_AUTHOR_NAME << "\n"
            << "Branch: " << GIT_BRANCH << "\n"
            << "Date: " << GIT_COMMIT_DATE_ISO8601 << "\n\n"
            << std::endl;
  // Library name
  std::cout << "Library template::baselib" << std::endl;
  std::cout << "========================================" << std::endl;

// Library type (static or dynamic)
#ifdef BASELIB_STATIC_DEFINE
  std::cout << "Library type: STATIC" << std::endl;
#else
  std::cout << "Library type: SHARED" << std::endl;
#endif

  // Data directory
  std::cout << "Data path:    " << dataPath << std::endl;
  std::cout << std::endl;

  // Read file
  std::cout << "Data directory access" << std::endl;
  std::cout << "========================================" << std::endl;

  std::string fileName = dataPath + "/DATA_FOLDER.txt";
  std::cout << "Reading from '" << fileName << "': " << std::endl;
  std::cout << std::endl;

  std::ifstream f(fileName);
  if (f.is_open()) {
    std::string line;
    while (getline(f, line)) { std::cout << line << '\n'; }
    f.close();
  } else {
    std::cout << "Unable to open file." << std::endl;
  }
}

}// namespace baselib
