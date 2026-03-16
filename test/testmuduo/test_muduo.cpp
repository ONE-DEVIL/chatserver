#include <muduo/net/TcpServer.h>
#include <muduo/base/Logging.h>
#include<functional>
#include <muduo/net/EventLoop.h>
#include<iostream>
#include<string>
using namespace std::placeholders;
using namespace std;
// 使用muduo开发回显服务器
class EchoServer
{
 public:
  EchoServer(muduo::net::EventLoop* loop,
             const muduo::net::InetAddress& listenAddr);

  void start(); 

 private:
  void onConnection(const muduo::net::TcpConnectionPtr& conn);

  void onMessage(const muduo::net::TcpConnectionPtr& conn,
                 muduo::net::Buffer* buf,
                 muduo::Timestamp time);

  muduo::net::TcpServer server_;
};

EchoServer::EchoServer(muduo::net::EventLoop* loop,
                       const muduo::net::InetAddress& listenAddr)
  : server_(loop, listenAddr, "EchoServer")
{
  server_.setConnectionCallback(
      std::bind(&EchoServer::onConnection, this, _1));
  server_.setMessageCallback(
      std::bind(&EchoServer::onMessage, this, _1, _2, _3));
}

void EchoServer::start()
{
  server_.start();
}

void EchoServer::onConnection(const muduo::net::TcpConnectionPtr& conn)
{
  LOG_INFO << "EchoServer - " << conn->peerAddress().toIpPort() << " -> "
           << conn->localAddress().toIpPort() << " is "
           << (conn->connected() ? "UP" : "DOWN");
}

void EchoServer::onMessage(const muduo::net::TcpConnectionPtr& conn,
                           muduo::net::Buffer* buf,
                           muduo::Timestamp time)
{
  // 接收到所有的消息，然后回显
  string buff = buf->retrieveAllAsString();
  std::cout<<"Msg:"<<buff<<" "<<time.toFormattedString()<<std::endl;
}


int main()
{
  LOG_INFO << "pid = " << getpid();
  muduo::net::EventLoop loop;
  muduo::net::InetAddress listenAddr(6666);
  EchoServer server(&loop, listenAddr);
  server.start();
  loop.loop();
}