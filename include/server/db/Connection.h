#pragma once
/*
实现MySQL数据库操作
*/
#include <mysql/mysql.h>
#include <string>
#include<ctime>
using namespace std;
/*
实现MySQL数据库操作
*/
// 数据库操作类
class Connection
{
public:
	// 初始化数据库连接
	Connection();
	// 释放数据库连接资源
	~Connection();
	// 连接数据库
	bool connect(string ip, unsigned short port, string username, string password,
		string dbname);
	// 更新操作 insert、delete、update
	bool update(string sql);
	// 查询操作 select
	MYSQL_RES* query(string sql);
	// 刷新当前连接的起始空闲时间
	void refreshAliveTime(){_alivetime = clock();}
	// 获取当前连接的空闲总时间
	clock_t getAliveTime(){return clock() - _alivetime;}
	//获取当前连接
	MYSQL* getConnetion();
private:
	MYSQL* _conn; // 表示和MySQL Server的一条连接
	clock_t _alivetime;// 记录空闲状态的起始存活时间
};