#ifndef __TYPEDEF_H__
#define __TYPEDEF_H__

#include "data_frame.hpp"
#include <cstring>
#include <functional>
#include <memory>
#include <moodycamel/concurrentqueue.h>
#include <readerwriterqueue/readerwriterqueue.h>
#include <uvw.hpp>

typedef std::shared_ptr<uvw::Loop>      shared_loop;
typedef std::shared_ptr<uvw::TCPHandle> tcp_handle;
typedef std::shared_ptr<uvw::Mutex>     socket_mutex;

typedef std::shared_ptr<uvw::AsyncHandle> async_handle;
typedef std::shared_ptr<uvw::IdleHandle>  idle_handle;

typedef std::shared_ptr<DataFrame>                   sharedDataFrame;
typedef moodycamel::ConcurrentQueue<sharedDataFrame> dataFrameQueue;
typedef std::shared_ptr<dataFrameQueue>              sharedQueue;
#define QUEUE_SIZE 1000
#endif// __TYPEDEF_H__
