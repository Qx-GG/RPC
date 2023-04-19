#include "./include/mprpcchannel.h"
#include <string>
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include "rpcheader.pb.h"
#include "./include/mprpcapplication.h"
#include "./include/logger.h"
#include "./include/zookeeperutil.h"

void MprpcChannel::CallMethod(const google::protobuf::MethodDescriptor* method,
                    google::protobuf::RpcController* controller, 
                    const google::protobuf::Message* request,
                    google::protobuf::Message* response,
                    google::protobuf::Closure* done)
{
    const google::protobuf::ServiceDescriptor *std=method->service();
    string service_name=std->name();
    string method_name=method->name();

    //获取参数序列化后的字符串的长度
    uint32_t args_size=0;
    std::string args_str;

    //进行序列化
    if(request->SerializeToString(&args_str))
    {
        LOG_INFO("获取参数序列化成功");
        args_size=args_str.size();
    }
    else
    {
        LOG_ERROR("args_str SerializeToString error!");
        controller->SetFailed("序列化错误");
        return;
    }

    //定义RPC的请求Header
    mprpc::RpcHeader rpcHeader;
    rpcHeader.set_service_name(service_name);
    rpcHeader.set_method_name(method_name);
    rpcHeader.set_args_size(args_size);

    uint32_t header_size=0;
    std::string rpc_header_str;
    if(rpcHeader.SerializeToString(&rpc_header_str))
    {
        header_size=rpc_header_str.size();
    }
    else
    {
        LOG_ERROR("rpc_header_str SerializeToString error!");
        controller->SetFailed("反序列化错误");
        return;
    }

    //将待发送的RPC请求字符串重组
    string send_rpc_str;
    send_rpc_str.insert(0,std::string((char*)&header_size,6));   //header_size
    send_rpc_str+=rpc_header_str;       //rpcheader
    send_rpc_str+=args_str;             //args

    //打印信息
    cout << "======================" << endl;
    cout << "header_size: " << header_size << endl;
    cout << "rpc_header_str: " << rpc_header_str << endl;
    cout << "service_name: " << service_name << endl;
    cout << "method_name: " << method_name << endl;
    cout << "args_str: " << args_str << endl;
    cout << "======================" << endl;

    /* 读取配置文件rpcserver的信息
    std::string ip = MprpcApplication::GetInstance().GetConfig().Load("rpcserverip");
    uint16_t port = atoi(MprpcApplication::GetInstance().GetConfig().Load("rpcserverport").c_str());
    */

   // rpc调用服务, 需要查询zookeeper上该服务的ip:port信息
   ZKClient zkCli;
   zkCli.Start();

   std::string method_path="/" + service_name + "/" + method_name;
   std::string host_data=zkCli.GetData(method_path.c_str());
   if(host_data=="")
   {
        LOG_ERROR("%s id not exists!",method_path);
        controller->SetFailed(method_path + "id not exists!");
        return;
   }

   int idx=host_data.find(":");
   if(idx==-1)
   {
        LOG_ERROR("%s address is invalid!",method_path);
        controller->SetFailed(method_path + "address is invalid!");
        return;
   }

   //获取ip:port
    std::string ip=host_data.substr(0,idx);
    uint16_t port=atoi(host_data.substr(idx+1,host_data.size()-idx).c_str());

    //调用TCP完成RPC的远程调用
    int client=socket(AF_INET,SOCK_STREAM,0);
    if(client<0)
    {
        controller->SetFailed("create socket error!");
        LOG_ERROR("create socket failed!");
    }
    struct sockaddr_in server;
    server.sin_family=AF_INET;
    server.sin_port=htons(port);
    server.sin_addr.s_addr=inet_addr(ip.c_str());

    int ret=connect(client,(const sockaddr*)&server,sizeof(server));
    if(ret!=0)
    {
        LOG_ERROR("connect error!");
        controller->SetFailed("connect error!");
        close(client);
        exit(EXIT_FAILURE);
    }

    //发送RPC请求
    ret=send(client,send_rpc_str.c_str(),send_rpc_str.size(),0);
    if(ret<=0)
    {
        LOG_ERROR("send error!");
        controller->SetFailed("send error!");
        close(client);
        return;
    }

    //接收RPC应答
    char buf[1024] = {0};
    int recv_size = 0;
    if((recv_size = recv(client, buf, 1024, 0)) <= 0) 
    {
        LOG_ERROR("accept error!");
        controller->SetFailed("accept error!");
        close(client);
        return;
    }

    //反序列化RPC应答
    if(!response->ParseFromArray(buf, recv_size)) 
    {
        LOG_ERROR("parse error!");
        controller->SetFailed("parse error!");
    }
   
    close(client);
    return;
}