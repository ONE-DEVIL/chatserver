#pragma once
#include<string>
#include<queue>
#include"Connection.h"
#include<mutex>
#include<memory>
#include<atomic>
#include<functional>
#include<condition_variable>
using namespace std;
/*
实现数据库连接池
*/
class ConnectionPool {
public:
	static ConnectionPool* getConnectionPool();//获取连接池对象
	shared_ptr<Connection> getConnection();//给外部提供接口，从连接池中获取一个可用连接
private:
    ConnectionPool();//单例，构造函数私有化
    bool loadConfigFile();//从文件中加载配置信息
    void produceConnectionTask();//运行在独立的线程中，用于生产连接
    void scanConnectionTask();//扫描超过最大空闲时间的连接，进行回收
	string _ip;//ip地址
	unsigned short _port;//端口号
    string _username;//用户名
    string _password;//密码
    string _dbname;//数据库名
	int _initSize;//初始连接数
    int _maxSize;//最大连接数
    int _maxIdleTime;//最大空闲时间
    int _connectionTimeout;//连接池获取连接的超时时间
    queue<Connection*> _connectionQueue;//存储连接的队列
    mutex _queueMutex;//维护队列安全的互斥锁
    atomic_int _connectionCount;//记录连接所创建的数量不能超过最大连接数,线程安全
    condition_variable _cv;//条件变量,用于连接生产线程和消费线程的通信
};