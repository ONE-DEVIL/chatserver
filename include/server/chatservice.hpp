#ifndef CHATSERVICE_H
#define CHATSSERVICE_H
#include<unordered_map>
#include<functional>
#include<mutex>
#include<muduo/net/TcpConnection.h>
#include"usermodel.hpp"
#include"offflinemessagemodel.hpp"
#include"friendmodel.hpp"
#include"groupmodel.hpp"
#include"redis.hpp"
using namespace std;
using namespace muduo;
using namespace muduo::net;
#include"json.hpp"
using namespace nlohmann;
//处理消息的事件处理回调类型
using MsgHandler = std::function<void(const TcpConnectionPtr &conn,json &js,Timestamp)>;
//聊天服务器业务类 (单例模式)
class Chatservice{
public:
    //获取单例对象的函数接口
    static Chatservice* instance();
    //处理登录业务
    void login(const TcpConnectionPtr &conn,json &js,Timestamp time);
    //处理注册业务
    void reg(const TcpConnectionPtr &conn,json &js,Timestamp time);
    //一对一聊天业务
    void oneChat(const TcpConnectionPtr &conn,json &js,Timestamp time);
    //添加好友业务
    void addFriend(const TcpConnectionPtr &conn,json &js,Timestamp time);
    //创建群组业务
    void createGroup(const TcpConnectionPtr &conn,json &js,Timestamp time);
    //加入群组业务
    void addGroup(const TcpConnectionPtr &conn,json &js,Timestamp time);
    //群组聊天业务
    void groupChat(const TcpConnectionPtr &conn,json &js,Timestamp time);
    //获取消息对应的处理器
    MsgHandler getHandler(int msgid);
    //处理redis订阅消息
    void handleRedisSubscribeMessage(int channel,string message);
    //处理注销业务
    void loginout(const TcpConnectionPtr &conn,json &js,Timestamp time);
    //处理客户端异常退出
    void clientCloseException(const TcpConnectionPtr &conn);
    //服务器异常后业务重置方法
    void reset();
private:
    Chatservice();
    //存储消息的id和对应的业务处理方法
    unordered_map<int,MsgHandler> _msgHandlerMap;
    //存储在线用户的通信连接
    unordered_map<int,TcpConnectionPtr> _userConnMap;
    //定义互斥锁，保证_userConnMap的线程安全
    mutex _connMtx;

    //数据操作类
    UserModel _userModel;
    OfflineMsgModel _offlineMsgModel;
    FriendModel _friendModel;
    GroupModel _groupModel;

    //Redis操作对象
    Redis _redis;
};

#endif