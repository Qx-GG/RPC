#include <iostream>
using namespace std;
#include "mprpcapplication.h"
#include "friends.pb.h"
#include "logger.h"

void log_write()
{
    logger::instance()->init("log",2000);
}

int main(int argc,char **argv)
{
   log_write();

    //整个程序启动之前，需要先初始化框架
    MprpcApplication::Init(argc,argv);

    /* 演示调用远程发布的rpc方法GetFriendsLists, 底层都是
       调用RpcChannel->CallMethod集中来做rpc方法调用的参
       数序列化和网络发送
    */
   RPC::FriendsServiceRpc_Stub stub(new MprpcChannel());
   RPC::GetFriendsListRequest request;
   request.set_userid(1000);

   RPC::GetFriendsListResponse response;
   MprpcController controller;              //记录错误信息

   stub.GetFriendsList(&controller,&request,&response,nullptr);     //Rpcchannel()->CallMethod

   if(controller.Failed())
   {
      cout<<controller.ErrorText()<<endl;
   }
   else
   {
      //一次rpc调用完成，读取响应
      if(response.result().errcode()==0)
      {
         LOG_INFO("RPC调用成功!");
         cout<<"rpc获取好友列表成功!"<<endl;
         int size=response.friends_size();
         for(int i=0;i<size;i++)
         {
            cout<<"第"<<i<<"个:"<<" "<<"名字:"<<response.friends(i)<<endl;
         }
      }
      else
      {
         cout<<"rpc返回好友列表错误~!"<<endl;
         LOG_ERROR("rpc返回好友列表错误~!  error:%s",response.result().errmsg());
      }
   }
   return 0;
}