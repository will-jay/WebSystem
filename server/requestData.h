#pragma once
#include "timer.h"
#include <string>
#include <unordered_map>
#include <memory>
#include <sys/epoll.h>

const int STATE_PARSE_URI = 1;//资源
const int STATE_PARSE_HEADERS = 2;//头部
const int STATE_RECV_BODY = 3;//接受
const int STATE_ANALYSIS = 4;//分析
const int STATE_FINISH = 5;//完成

const int MAX_BUFF = 4096;

// 有请求出现但是读不到数据,可能是Request Aborted,请求中止
// 或者来自网络的数据没有达到等原因,
// 对这样的请求尝试超过一定的次数就抛弃
const int AGAIN_MAX_TIMES = 200;

const int PARSE_URI_AGAIN = -1;//重新解析
const int PARSE_URI_ERROR = -2;//解析失败
const int PARSE_URI_SUCCESS = 0;//解析成功

const int PARSE_HEADER_AGAIN = -1;//重新解析
const int PARSE_HEADER_ERROR = -2;//解析错误
const int PARSE_HEADER_SUCCESS = 0;//解析成功

const int ANALYSIS_ERROR = -2;//分析失败
const int ANALYSIS_SUCCESS = 0;//分析成功

const int METHOD_POST = 1;//post方法
const int METHOD_GET = 2;//get方法
const int HTTP_10 = 1;//http1.0
const int HTTP_11 = 2;//http2.0

const int EPOLL_WAIT_TIME = 500;

class MimeType//媒体类型
{
private:
    static void init();//初始化
    static std::unordered_map<std::string, std::string> mime;
    MimeType();
    MimeType(const MimeType &m);

public:
    static std::string getMime(const std::string &suffix);

private:
    static pthread_once_t once_control;//一次初始化
};

enum HeadersState
{
    h_start = 0,//开头
    h_key,//关键字
    h_colon,//冒号
    h_spaces_after_colon,//冒号后的空间
    h_value,//值
    h_CR,//回车
    h_LF,//换行
    h_end_CR,
    h_end_LF
};

class TimerNode;

class RequestData : public std::enable_shared_from_this<RequestData>
{
private:
    std::string path;//路径
    int fd;
    int epollfd;

    std::string inBuffer;//输入缓存
    std::string outBuffer;//输出缓存
    __uint32_t events;
    bool error;

    int method;//方法
    int HTTPversion;//http版本
    std::string file_name;//文件名
    int now_read_pos;//读取位置
    int state;//状态
    int h_state;//头部状态
    bool isfinish;//是否完成
    bool keep_alive;//保持线上
    std::unordered_map<std::string, std::string> headers;//头部
    std::weak_ptr<TimerNode> timer;

    bool isAbleRead;//可读
    bool isAbleWrite;//可写

private:
    int parse_URI();//解析URI
    int parse_Headers();//解析头部
    int analysisRequest();//应答内容

   /* Mat stitch(Mat &src)//opencv的内容
        return src;*/

public:

    RequestData();
    RequestData(int _ep1ollfd, int _fd, std::string _path);
    ~RequestData();
    void linkTimer(std::shared_ptr<TimerNode> mtimer);//时间
    void reset();//重新设置
    void seperateTimer();
    int getFd();
    void setFd(int _fd);
    void handleRead();//处理读取
    void handleWrite();//处理写入
    void handleError(int fd, int err_num, std::string short_msg);//错误应答
    void handleConn();//处理连接

    void disableReadAndWrite();//设置不可读写

    void enableRead();//设置可读

    void enableWrite();//设置可写

    bool canRead();//返回是否可读

    bool canWrite();//返回是否可写
};

