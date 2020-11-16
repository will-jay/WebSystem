#pragma once
#include "requestData.h"
//#include "condition.hpp"
#include <pthread.h>
#include <functional>
#include <memory>
#include <vector>

const int THREADPOOL_INVALID = -1;//无效
const int THREADPOOL_LOCK_FAILURE = -2;//锁失败
const int THREADPOOL_QUEUE_FULL = -3;//队列满
const int THREADPOOL_SHUTDOWN = -4;//关闭
const int THREADPOOL_THREAD_FAILURE = -5;//线程失败
const int THREADPOOL_GRACEFUL = 1;//完美

const int MAX_THREADS = 1024;//线程数
const int MAX_QUEUE = 65535;//队列数

typedef enum
{
    immediate_shutdown = 1,//立即关闭
    graceful_shutdown  = 2//正常关闭
} ShutDownOption;//关闭选项

struct ThreadPoolTask//线程任务
{
    std::function<void(std::shared_ptr<void>)> fun;//回调函数
    std::shared_ptr<void> args;//智能指针
};

void myHandler(std::shared_ptr<void> req);

class ThreadPool
{
private:
    static pthread_mutex_t lock;//互斥锁
    static pthread_cond_t notify;//条件变量

    static std::vector<pthread_t> threads;//线程数组
    static std::vector<ThreadPoolTask> queue;//任务队列
    static int thread_count;
    static int queue_size;
    static int head;
    // tail 指向尾节点的下一节点
    static int tail;
    static int count;
    static int shutdown;
    static int started;
public:
    static int threadpool_create(int _thread_count, int _queue_size);//创建线程池
    static int threadpool_add(std::shared_ptr<void> args, std::function<void(std::shared_ptr<void>)> fun = myHandler);//往任务队列里面加任务
    static int threadpool_destroy(ShutDownOption shutdown_option = graceful_shutdown);//释放线程
    static int threadpool_free();//释放锁
    static void *threadpool_thread(void *args);
};
