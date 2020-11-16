
#include "timer.h"
#include "epoll.h"
#include <unordered_map>
#include <string>
#include <sys/time.h>
#include <unistd.h>
#include <deque>
#include <queue>

#include <iostream>
using namespace std;


TimerNode::TimerNode(SP_ReqData _request_data, int timeout): //设置过期时间
    deleted(false), 
    request_data(_request_data)
{
    //cout << "TimerNode()" << endl;
    struct timeval now;
    gettimeofday(&now, NULL);
    // 以毫秒计
    expired_time = ((now.tv_sec * 1000) + (now.tv_usec / 1000)) + timeout;//换算成毫秒
}

TimerNode::~TimerNode()//从监听队列中删除
{
    //cout << "~TimerNode()" << endl;
    if (request_data)
    {
        Epoll::epoll_del(request_data->getFd());
    }
    //request_data.reset();
    // if (request_data)
    // {
    //     cout << "request_data=" << request_data << endl;
    //     delete request_data;
    //     request_data = NULL;
    // }
}

void TimerNode::update(int timeout)//更新过期时间
{
    struct timeval now;
    gettimeofday(&now, NULL);
    expired_time = ((now.tv_sec * 1000) + (now.tv_usec / 1000)) + timeout;
}

bool TimerNode::isvalid()//判断超时
{
    struct timeval now;
    gettimeofday(&now, NULL);
    size_t temp = ((now.tv_sec * 1000) + (now.tv_usec / 1000));
    if (temp < expired_time)
    {
        return true;
    }
    else
    {
        this->setDeleted();
        return false;
    }
}

void TimerNode::clearReq()//清空
{
    request_data.reset();
    this->setDeleted();
}

void TimerNode::setDeleted()//设置清除状态
{
    deleted = true;
}

bool TimerNode::isDeleted() const
{
    return deleted;
}

size_t TimerNode::getExpTime() const//获取过期时间
{
    return expired_time;
}

TimerManager::TimerManager()
{
}

TimerManager::~TimerManager()
{
}

void TimerManager::addTimer(SP_ReqData request_data, int timeout)//设置过期时间
{
    SP_TimerNode new_node(new TimerNode(request_data, timeout));//创建新节点
    {
        MutexLockGuard locker(lock);
        TimerNodeQueue.push(new_node);
    }
    request_data->linkTimer(new_node);
}

void TimerManager::addTimer(SP_TimerNode timer_node)
{

}

/* 处理逻辑是这样的~
因为(1) 优先队列不支持随机访问
(2) 即使支持，随机删除某节点后破坏了堆的结构，需要重新更新堆结构。
所以对于被置为deleted的时间节点，会延迟到它(1)超时 或 (2)它前面的节点都被删除时，它才会被删除。
一个点被置为deleted,它最迟会在TIMER_TIME_OUT时间后被删除。
这样做有两个好处：
(1) 第一个好处是不需要遍历优先队列，省时。
(2) 第二个好处是给超时时间一个容忍的时间，就是设定的超时时间是删除的下限(并不是一到超时时间就立即删除)，如果监听的请求在超时后的下一次请求中又一次出现了，
就不用再重新申请RequestData节点了，这样可以继续重复利用前面的RequestData，减少了一次delete和一次new的时间。
*/

void TimerManager::handle_expired_event()//处理过期事件
1{
    MutexLockGuard locker(lock);
    while (!TimerNodeQueue.empty())
    {
        SP_TimerNode ptimer_now = TimerNodeQueue.top();//获取第一个队列元素
        if (ptimer_now->isDeleted())
        {
            TimerNodeQueue.pop();
            //delete ptimer_now;
        }
        else if (ptimer_now->isvalid() == false)
        {
            TimerNodeQueue.pop();
            //delete ptimer_now;
        }
        else
        {
            break;
        }
    }
}