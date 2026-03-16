#include"friendmodel.hpp"
#include"db.h"
#include"CommonConnectionPool.h"

//添加好友关系
void FriendModel::insert(int userid,int friendid){
    // 组织sql语句
    char sql[1024] = {0};
    sprintf(sql, "insert into friend(userid,friendid) values(%d,%d)", userid,friendid);
    // MySQL mysql;
    // if (mysql.connect())
    // {
    //     mysql.update(sql);
    // }
    //从连接池中获取一个连接
    ConnectionPool *cp = ConnectionPool::getConnectionPool();
    shared_ptr<Connection> sp = cp->getConnection();
    if(sp!=nullptr){
        sp->update(sql);
    }
}
//查询好友列表 friendid,friendname
vector<User> FriendModel::query(int userid){
    vector<User> vec;
    // 组织sql语句
    char sql[1024] = {0};
    sprintf(sql, "select a.id,a.name,a.state from user a inner join friend b on b.friendid = a.id where userid=%d", userid);
    // MySQL mysql;
    // if (mysql.connect())
    // {
    //     MYSQL_RES *res = mysql.query(sql);
    //     if (res != nullptr)
    //     {
    //         MYSQL_ROW row;
    //         //把userid的所有离线消息放入vec中返回
    //         while((row = mysql_fetch_row(res))!= nullptr){
    //             User user;
    //             user.setId(atoi(row[0]));
    //             user.setName(row[1]);
    //             user.setState(row[2]);
    //             vec.push_back(user);
    //         }
    //         mysql_free_result(res);
    //         return vec;
    //     }
    // }
    // return vec;
    //从连接池中获取一个连接
    ConnectionPool *cp = ConnectionPool::getConnectionPool();
    shared_ptr<Connection> sp = cp->getConnection();
    if(sp!=nullptr){
        MYSQL_RES *res = sp->query(sql);
        if (res != nullptr)
        {
            MYSQL_ROW row;
            //把userid的所有离线消息放入vec中返回
            while((row = mysql_fetch_row(res))!= nullptr){
                User user;
                user.setId(atoi(row[0]));
                user.setName(row[1]);
                user.setState(row[2]);
                vec.push_back(user);
            }
            mysql_free_result(res);
            return vec;
        }
    }
    return vec;
}