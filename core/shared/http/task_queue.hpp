#ifndef __TASK_QUEUE_H__
#define __TASK_QUEUE_H__

#include "BS_thread_pool.hpp"
#include "http_lib_header.hpp"

class TaskQueue : public httplib::TaskQueue
{
public:
    TaskQueue(unsigned int n) : pool_(n) {}

    bool enqueue(std::function<void()> fn) override
    {
        // 将任务fn加入到你的线程池任务队列中
        // 如果队列已满无法加入,返回false,这会导致连接被关闭
        pool_.detach_task(fn);
        return true;
    }

    void shutdown() override { pool_.purge(); }

private:
    BS::thread_pool pool_;
};

#endif // __TASK_QUEUE_H__
