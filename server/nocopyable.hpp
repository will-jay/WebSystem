#pragma once
class noncopyable//实现一个noncopyable类，使得类不可复制
{
protected:
    noncopyable() {}
    ~noncopyable() {}
private://把复制构造函数和拷贝赋值函数放private
    noncopyable(const noncopyable&);
    const noncopyable& operator=(const noncopyable&);
};