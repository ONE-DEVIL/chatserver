#include<iostream>
#include<thread>
#include"Connection.h"
#include<muduo/base/Logging.h>
#include"CommonConnectionPool.h"
using namespace std;
//线程安全的懒汉式单例模式
ConnectionPool* ConnectionPool::getConnectionPool()
{ 
	static ConnectionPool pool; //lock和unlock
    return &pool;
}
bool ConnectionPool::loadConfigFile() {
    FILE* pf = fopen("/home/ubuntu/workplace/code/CHAT/chatserver/mysql.ini", "r");
    if (pf == nullptr) {
        LOG_INFO << "mysql.ini open error!";
        return false;
    }
    while (!feof(pf)) {
        char line[1024] = { 0 };
        fgets(line, 1024, pf);
        string str=line;
        int idx = str.find("=");
        if (idx == -1) {//无效配置项
            continue;
        }
        int edx = str.find("\n",idx);
        string key = str.substr(0, idx);
        string value = str.substr(idx + 1, edx - idx - 1);
        
        if (key == "ip") {
            _ip = value;
        }
        else if (key == "port") {
            _port = atoi(value.c_str());
        }
        else if (key == "username") {
            _username = value;
        }
        else if (key == "password") {
            _password = value;
        }
        else if (key == "dbName") {
            _dbname = value;
        }
        else if (key == "maxSize") {
            _maxSize = atoi(value.c_str());
        }
        else if (key == "initSize") {
            _initSize = atoi(value.c_str());
        }
        else if (key == "maxIdleTime") {
            _maxIdleTime = atoi(value.c_str());
        }
        else if (key == "connectTimeout") {
            _connectionTimeout = atoi(value.c_str());
        }
    }
    cout << "ip:" << _ip <<" "<< _port <<" "<< _username <<" "<< _password <<" "<< _dbname <<" "<< _maxSize <<" "<< _initSize <<" "<< _maxIdleTime <<" "<< _connectionTimeout << endl;
    fclose(pf);
    return true;
}

ConnectionPool::ConnectionPool()
{//创建连接池
    if (!loadConfigFile()) {
        return;
    }
    //创建初始连接
    for (int i = 0; i < _initSize; i++) {//不用考虑线程安全，因为在创建的时候，其他线程不能访问
        Connection* p = new Connection();
        p->connect(_ip, _port, _username, _password, _dbname);
        p->refreshAliveTime();
        _connectionQueue.push(p);
        _connectionCount++;
    }
    //启动一个线程，作为连接的生产者
    thread produce(&ConnectionPool::produceConnectionTask, this);
    produce.detach();//守护线程，分离线程
    //启动一个定时线程，定时检查超过maxIdleTime的空闲连接，进行回收
    thread timer(&ConnectionPool::scanConnectionTask, this);
    timer.detach();
}
//运行在独立的线程中，专门负责生产新连接
void ConnectionPool::produceConnectionTask() {
    for (;;) {
        unique_lock<mutex> lock(_queueMutex);
        while (!_connectionQueue.empty()) {
            _cv.wait(lock);//队列非空,此处生产者线程进入等待状态，
        }
        //连接数量没有到达最大值，继续创建连接
        if (_connectionCount < _maxSize) {//创建一个连接
            Connection* p = new Connection();
            p->connect(_ip, _port, _username, _password, _dbname);
            p->refreshAliveTime();
            _connectionQueue.push(p);
            _connectionCount++;
        }
        //创建完成之后，通知消费者线程，可以消费连接了
        _cv.notify_all();
    }

}
/*
shared_ptr<Connection>智能指针析构时，会把connection资源直接delete掉，
相当于调用connection的析构函数，connection就会close掉
*/
//获取一个可用连接
shared_ptr<Connection> ConnectionPool::getConnection() {
    unique_lock<mutex> lock(_queueMutex);
    while (_connectionQueue.empty()) {
        //等待
        if (cv_status::timeout == _cv.wait_for(lock, chrono::milliseconds(_connectionTimeout))) {
            if (_connectionQueue.empty()) {
                LOG_INFO << "获取连接超时！";
                return nullptr;
            }
        }
    }
    shared_ptr<Connection> ptr(_connectionQueue.front(), [&](Connection* pcon) {
        //这里是在服务器应用线程中调用的，所以一定要考虑队列的线程安全
        unique_lock<mutex> lock(_queueMutex);
        _connectionQueue.push(pcon);
        pcon->refreshAliveTime();//归还时刷新时间
        });
    _connectionQueue.pop();
    if (_connectionQueue.empty()) {//可加可不加
        //谁消费了队列中最后一个元素，就通知生产者线程，可以继续生产连接了
        _cv.notify_all();//消费完连接以后，通知生产者线程检查一下，如果队列空了，赶紧生产
    }
    return ptr;
}
//扫描超过最大空闲时间的连接，进行回收
void ConnectionPool::scanConnectionTask() {
    for (;;) {
        this_thread::sleep_for(chrono::seconds(_maxIdleTime));
        //扫描队列，释放多于连接
        unique_lock<mutex> lock(_queueMutex);
        while (_connectionCount > _initSize) {
            Connection* p = _connectionQueue.front();
            if (p->getAliveTime() > (_maxIdleTime*1000)) {
                _connectionQueue.pop();
                _connectionCount--;
                delete p;
            }
            else {//队头不超过最大空闲时间，退出循环
                break;
            }
        }
    }
}