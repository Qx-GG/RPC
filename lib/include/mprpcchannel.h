#pragma once
#include <google/protobuf/service.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>

//所用通过stub代理对象调用的rpc都会走到这一步
class MprpcChannel:public google::protobuf::RpcChannel  
{
public:
    // 统一做rpc方法的数据序列化和网络传送
    void CallMethod(const google::protobuf::MethodDescriptor* method,
                    google::protobuf::RpcController* controller, 
                    const google::protobuf::Message* request,
                    google::protobuf::Message* response,
                    google::protobuf::Closure* done); 
};