#pragma once

#include <functional>
#include <unordered_map>

#include "google/protobuf/service.h"
#include <google/protobuf/descriptor.h>

#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>
#include <muduo/net/TcpConnection.h>

#include "mprpcapplication.h"

// 框架提供的专门用来发布rpc服务的网络对象类
class RpcProvider
{
public:
    //因为传入的参数不同，为了满足RPC框架能够满足不同的需求，所以传入Service基类
    void NotifyService(google::protobuf::Service* service);

    //启动rpc服务节点，开始提供rpc远程网络调用
    void Run();

private:
    //服务类型信息结构体
    struct ServiceInfo
    {
        //保存服务对象
        google::protobuf::Service *m_service;

        //用哈希表来记录服务方法名和服务方法descriptor的映射
        unordered_map<string,const google::protobuf::MethodDescriptor*> m_methodMap;
    };

    //存储注册为rpc方法成功的服务对象名和其服务方法的所有信息
    unordered_map<string,struct ServiceInfo> m_serviceMap;

    //组合EventLoop
    muduo::net::EventLoop m_eventLoop;

    //新的socket连接回调
    void OnConnection(const muduo::net::TcpConnectionPtr& connection);

    //已建立连接用户的读写事件回调
    void OnMessage(const muduo::net::TcpConnectionPtr& connection,muduo::net::Buffer* buffer,muduo::Timestamp);

    //Closure的回调,用于序列化rpc响应和网络传输
    void SendRpcResponse(const muduo::net::TcpConnectionPtr& connection, google::protobuf::Message* response);

};