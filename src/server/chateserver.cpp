#include "chatserver.hpp"
#include"chatservice.hpp"
#include "json.hpp"
#include <string>
#include <functional>
using namespace std;
using namespace placeholders;
using namespace nlohmann;
Chatserver::Chatserver(EventLoop *loop,
                       const InetAddress &listenAddr,
                       const string &nameArg)
    : _server(loop, listenAddr, nameArg),
      _loop(loop)
{
    // 注册链接回调
    _server.setConnectionCallback(std::bind(&Chatserver::onConnection, this, _1));
    // 消息链接回调
    _server.setMessageCallback(std::bind(&Chatserver::onMessage, this, _1, _2, _3));
    // 设置线程数量
    _server.setThreadNum(4);
}
// 启动服务
void Chatserver::start()
{
    _server.start();
}
// 上报链接相关连接的回调函数
void Chatserver::onConnection(const TcpConnectionPtr &conn)
{
    if (!conn->connected())
    {
        Chatservice::instance()->clientCloseException(conn);
        conn->shutdown();
    }
}
// 上报链接相关信息的回调函数
void Chatserver::onMessage(const TcpConnectionPtr &conn,
                           Buffer *buffer,
                           Timestamp time)
{
    string buf = buffer->retrieveAllAsString();
    //数据反序列化
    json js = json::parse(buf);
    //完全解耦网络模块的代码和业务模块的代码
    //通过js[“msgid”] 获取业务handler=》conn js time
    auto msgHandler=Chatservice::instance()->getHandler(js["msgid"].get<int>());
    //回调消息绑定好的事件处理器，来执行相应的业务处理
    msgHandler(conn,js ,time);
}