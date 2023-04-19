#include <iostream>
using namespace std;
#include <string.h>
#include "test.pb.h"
using namespace mprpc;

int main()
{
    // // 封装了Login请求对象的信息
    // LoginRequest req;
    // req.set_name("zhang san");
    // req.set_pwd("123456");

    // // 对象数据序列化
    // std::string send_str;
    // if (req.SerializeToString(&send_str))
    // {
    //     std::cout << send_str.c_str() << std::endl;
    // }

    // // 从send_str里反序列化一个login对象
    // LoginRequest req2;
    // if (req2.ParseFromString(send_str))
    // {
    //     std::cout << req2.name() << std::endl;
    //     std::cout << req2.pwd() << std::endl;
    // }

    GetFriendListResponse rsp;
    ResultCode* rc = rsp.mutable_res();
    rc->set_errcode(0);

    User* user1 = rsp.add_friend_list();
    user1->set_name("秦鑫");
    user1->set_age(20);
    user1->set_sex(User::MAN);

    User* user2 = rsp.add_friend_list();
    user2->set_name("小美");
    user2->set_age(19);
    user2->set_sex(User::MAN);

    std::cout << rsp.friend_list_size() << std::endl;
    User friend1 = rsp.friend_list(0);
    User friend2 = rsp.friend_list(1);

    std::cout << friend1.name() << std::endl;
    std::cout << friend1.age() << std::endl;
    std::cout << friend1.sex() << std::endl;

    std::cout << friend2.name() << std::endl;
    std::cout << friend2.age() << std::endl;
    std::cout << friend2.sex() << std::endl;
    return 0;
}