#ifndef USERMODEL_H
#define USERMODEL_H
#include <muduo/base/Logging.h>
#include"db.h"
#include"user.hpp"
using namespace muduo;

//user表的数据操作类
class UserModel
{
public:
    // insertd接口方法，实现增加用户操作
    bool insert(User &user);
    // query方法
    User query(int id);
    //更新用户状态信息
    bool updateState(User &user);
    //重置用户的状态信息
    void resetState();
};
#endif