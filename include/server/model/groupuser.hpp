#ifndef DROUPUSER_H
#define GROUPUSER_H
#include <string>
#include"user.hpp"
using namespace std;

//user表的映射类
class GroupUser :public User
{
public:
    void setRole(string role){this->role = role;}
    string getRole(){return this->role;}
private:
    string role;
};
#endif