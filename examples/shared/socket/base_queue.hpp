#ifndef __BASE_QUEUE_H__
#define __BASE_QUEUE_H__

#include "typedef.h"
#include <atomic>
#include <memory>
class BaseQueue
{
public:
    BaseQueue() : m_queue(std::make_shared<dataFrameQueue>(QUEUE_SIZE)) {}
    void write_queue(std::shared_ptr<DataFrame>& frame)
    {
        m_queue->enqueue(frame);
    }
    bool read_queue(std::shared_ptr<DataFrame>& frame)
    {
        return m_queue->try_dequeue(frame);
    }
    int get_queue_size() { return m_queue->size_approx(); }

private:
    /* data */
    sharedQueue m_queue;
};

#endif // __BASE_QUEUE_H__
