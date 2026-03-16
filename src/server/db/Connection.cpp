#include<iostream>
#include<muduo/base/Logging.h>
#include"Connection.h"
using namespace std;
// 数据库配置信息
static string server = "127.0.0.1";
static string user = "root";
static string password = "Devil123456.";
static string dbname = "chat";
// 数据库操作类
// 初始化数据库连接
Connection::Connection()
{
	_conn = mysql_init(nullptr);
}
// 释放数据库连接资源
Connection::~Connection()
{
	if (_conn != nullptr)
		mysql_close(_conn);
}
// 连接数据库
bool Connection::connect(string ip, unsigned short port, string username, string password,
		string dbname)
{
	MYSQL* p = mysql_real_connect(_conn, ip.c_str(), username.c_str(),
		password.c_str(), dbname.c_str(), port, nullptr, 0);
	if (p != nullptr)
    {
        mysql_query(_conn, "set names gbk");
    }
    return p;
}
// 更新操作 insert、delete、update
bool Connection::update(string sql)
{
	if (mysql_query(_conn, sql.c_str()))
    {
        LOG_INFO << __FILE__ << ":" << __LINE__ << ":"
                 << sql << "更新失败!";
        return false;
    }
    return true;
}
// 查询操作 select
MYSQL_RES* Connection::query(string sql)
{
	if (mysql_query(_conn, sql.c_str()))
    {
        LOG_INFO << __FILE__ << ":" << __LINE__ << ":"
                 << sql << "查询失败!";
        return nullptr;
    }
    return mysql_use_result(_conn);
}
MYSQL* Connection::getConnetion(){
    return _conn;
}