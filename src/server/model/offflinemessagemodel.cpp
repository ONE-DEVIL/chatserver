#include"offflinemessagemodel.hpp"
#include"db.h"
#include"CommonConnectionPool.h"
#include<iostream>
 //存储用户的离线消息
void OfflineMsgModel::insert(int userid, string msg){
    char sql[1024] = {0};
    sprintf(sql, "insert into offlinemessage values(%d,'%s')",userid,msg.c_str());
    //MySQL mysql;
    //从连接池中获取一个连接
    ConnectionPool *cp = ConnectionPool::getConnectionPool();
    shared_ptr<Connection> sp = cp->getConnection();
    //if (mysql.connect())
    if(sp!=nullptr)
    {
        if(sp->update(sql)){
            std::cout << "离线消息入库成功 | userid=" << userid << std::endl;
        }
        else{
            std::cout << "离线消息入库失败 | SQL 执行错误 | userid=" << userid << std::endl;
        }    
    }
    else{
        std::cout << "离线消息入库失败 | 数据库连接失败 | userid=" << userid << std::endl;
    }
}
//删除用户的离线消息
void OfflineMsgModel::remove(int userid){
    char sql[1024] = {0};
    sprintf(sql,"delete from offlinemessage where userid=%d",userid);
    std::cout << "执行离线消息入库 SQL:" << sql << std::endl;
    //MySQL mysql;
    //从连接池中获取一个连接
    ConnectionPool *cp = ConnectionPool::getConnectionPool();
    shared_ptr<Connection> sp = cp->getConnection();
    if(sp!=nullptr)
    {
        if(sp->update(sql)){
            std::cout << "离线消息删除成功 | userid=" << userid << std::endl;
        }
        else{
            std::cout << "离线消息删除失败 | SQL 执行错误 | userid=" << userid << std::endl;
        }    
    }
    else{
        std::cout << "离线消息删除失败 | 数据库连接失败 | userid=" << userid << std::endl;
    }
}
//查询用户的离线消息
vector<string> OfflineMsgModel::query(int userid){
    vector<string> vec;
    // 组织sql语句
    char sql[1024] = {0};
    sprintf(sql, "select message from offlinemessage where userid=%d", userid);
    //MySQL mysql;
    //从连接池中获取一个连接
    ConnectionPool *cp = ConnectionPool::getConnectionPool();
    shared_ptr<Connection> sp = cp->getConnection();
    if(sp!=nullptr)
    {
        MYSQL_RES *res = sp->query(sql);
        if (res != nullptr)
        {
            MYSQL_ROW row;
            //把userid的所有离线消息放入vec中返回
            while((row = mysql_fetch_row(res))!= nullptr){
                vec.push_back(row[0]);
            }
            mysql_free_result(res);
            return vec;
        }
    }
    return vec;
}