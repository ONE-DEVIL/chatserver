#include "usermodel.hpp"
#include "db.h"
#include"CommonConnectionPool.h"
bool UserModel::insert(User &user)
{
    // 组织sql语句
    char sql[1024] = {0};
    sprintf(sql, "insert into user(name,password,state) values('%s', '%s','%s')", user.getName().c_str(), user.getpassword().c_str(), user.getState().c_str());
    //MySQL mysql;
    //从连接池中获取一个连接
    ConnectionPool *cp = ConnectionPool::getConnectionPool();
    shared_ptr<Connection> sp = cp->getConnection();
    if(sp!=nullptr)
    {
        if (sp->update(sql))
        {
            LOG_INFO << "add User success => sql:" << sql;
            // 获取插入成功的用户数据生成的主键id
            user.setId(mysql_insert_id(sp->getConnetion()));
            return true;
        }
    }
    LOG_INFO << "add User error => sql:" << sql;
    return false;
}
User UserModel::query(int id)
{
    // 组织sql语句
    char sql[1024] = {0};
    sprintf(sql, "select * from user where id=%d", id);
    //MySQL mysql;
    //从连接池中获取一个连接
    ConnectionPool *cp = ConnectionPool::getConnectionPool();
    shared_ptr<Connection> sp = cp->getConnection();
    if(sp!=nullptr)
    {
        MYSQL_RES *res = sp->query(sql);
        if (res != nullptr)
        {
            MYSQL_ROW row = mysql_fetch_row(res);
            if (row != nullptr)
            {
                User user;
                user.setId(atoi(row[0]));
                user.setName(row[1]);
                user.setPassword(row[2]);
                user.setState(row[3]);
                mysql_free_result(res);
                return user;
            }
        }
    }
    return User();
}
bool UserModel::updateState(User &user)
{
    char sql[1024] = {0};
    //sprintf(sql, "update user set name = %s password=%s state = %s where id = %d", user.getName().c_str(), user.getpassword().c_str(), user.getState().c_str(), user.getId());
    sprintf(sql, "update user set state = '%s' where id = %d",user.getState().c_str(),user.getId());
    //MySQL mysql;
    //从连接池中获取一个连接
    ConnectionPool *cp = ConnectionPool::getConnectionPool();
    shared_ptr<Connection> sp = cp->getConnection();
    if(sp!=nullptr)
    {
        if (sp->update(sql))
        {
            return true;
        }
    }
    return false;
}

//重置用户的状态信息
void UserModel::resetState(){
    char sql[1024] = {0};
    //sprintf(sql, "update user set name = %s password=%s state = %s where id = %d", user.getName().c_str(), user.getpassword().c_str(), user.getState().c_str(), user.getId());
    sprintf(sql, "update user set state = 'offline' where state = 'online'");
    //MySQL mysql;
    //从连接池中获取一个连接
    ConnectionPool *cp = ConnectionPool::getConnectionPool();
    shared_ptr<Connection> sp = cp->getConnection();
    if(sp!=nullptr)
    {
        sp->update(sql);
    }
}