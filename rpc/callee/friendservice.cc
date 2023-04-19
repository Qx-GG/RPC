#include <iostream>
using namespace std;
#include <string.h>
#include <vector>
#include "friends.pb.h"
#include "mprpcapplication.h"
#include "rpcprovider.h"
#include "logger.h"

void log_write()
{
    logger::instance()->init("log", 2000);
}

class FriendsService:public RPC::FriendsServiceRpc
{
    vector<string> GetFriendsList(uint32_t userid)
    {
        cout<<"启动GetFriendsLists Service"<<endl;
        vector<string> vec{"QQ", "XX", "QX"};
        return vec;
    }

    //FriendServiceRpc类中的基函数，此时进行函数重写
    void GetFriendsList(::google::protobuf::RpcController* controller,
                         const ::RPC::GetFriendsListRequest* request,
                         ::RPC::GetFriendsListResponse* response,
                         ::google::protobuf::Closure* done)
    {
        uint32_t userid=request->userid();  //取出请求参数
        vector<string> friendList=GetFriendsList(userid);//将参数传入我们重写的GetFriendsLists这个函数中，此处就简单输出一条语句

        response->mutable_result()->set_errcode(0);
        response->mutable_result()->set_errmsg("GetFriendList");

        //循环打印friendList中的数据
        for(int i=0;i<friendList.size();i++)
        {
            auto *p=response->add_friends();
            *p=friendList[i];
            cout<<friendList[i]<<endl;
        }
        
        //将结果返回给er端
        done->Run();
    }
};

int main(int argc,char **argv)
{
    log_write();

    //初始化RPC框架
    MprpcApplication::Init(argc,argv);

    //创建一个rpc对象，作用是将FriendService对象发布到rpc结点上
    RpcProvider provider;
    provider.NotifyService(new FriendsService());

    //启动一个rpc服务发布结点, 进行阻塞等待远程调用请求
    provider.Run();

    return 0;
}