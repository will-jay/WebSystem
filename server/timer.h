#pragma once
#include "requestData.h"
#include "./base/nocopyable.hpp"
#include "./base/mutexLock.hpp"
#include <unistd.h>
#include <memory>
#include <queue>
#include <deque>

class RequestData;


class TimerNode
{
    typedef std::shared_ptr<RequestData> SP_ReqData;
private:
    bool deleted;
    size_t expired_time;//过期时间
    SP_ReqData request_data;
public:
    TimerNode(SP_ReqData _request_data, int timeout);//设置过期时间
    ~TimerNode();//从监听队列中删除
    void update(int timeout);//更新过期时间
    bool isvalid();////判断超时
    void clearReq();//清除
    void setDeleted();//删除
    bool isDeleted() const;
    size_t getExpTime() const;//获取过期时间
};


struct timerCmp//比较过期时间是否相等
{
    bool operator()(std::shared_ptr<TimerNode> &a, std::shared_ptr<TimerNode> &b) const
    {
        return a->getExpTime() > b->getExpTime();
    }
};

class TimerManager//管理者
{
    typedef std::shared_ptr<RequestData> SP_ReqData;
    typedef std::shared_ptr<TimerNode> SP_TimerNode;
private:
    std::priority_queue<SP_TimerNode, std::deque<SP_TimerNode>, timerCmp> TimerNodeQueue;//优先队列
    MutexLock lock;
public:
    TimerManager();
    ~TimerManager();
    void addTimer(SP_ReqData request_data, int timeout);
    void addTimer(SP_TimerNode timer_node);
    void handle_expired_event();//处理过期事件
};