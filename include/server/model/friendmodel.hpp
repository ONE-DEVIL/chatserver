#ifndef FRIEND_H
#define FRIEND_H
#include"user.hpp"
#include<vector>
using namespace std;
//维护好友信息的操作接口方法
class FriendModel{
public:
    //添加好友关系
    void insert(int userid,int friendid);
    //查询好友列表 friendid,friendname
    vector<User> query(int userid);

};

#endif