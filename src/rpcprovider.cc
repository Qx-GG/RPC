#include <string>
#include <functional>
#include <google/protobuf/descriptor.h>

#include "./include/rpcprovider.h"
#include "./include/mprpcapplication.h"
#include "./include/rpcheader.pb.h"
#include "./include/logger.h"
#include "./include/zookeeperutil.h"

//将Service服务发布成RPC服务的接口，提供给外部调用
void RpcProvider::NotifyService(google::protobuf::Service* service)
{
    ServiceInfo serviceInfo;

    //获取服务对象的描述信息
    const google::protobuf::ServiceDescriptor* descriptor=service->GetDescriptor();

    //获取服务对象的名字
    string service_name=descriptor->name();

    //获取服务对象的数量
    uint32_t method_count=descriptor->method_count();
    LOG_INFO("服务名为:%s",service_name.c_str());

    //将服务名和服务对象一一映射，添加到哈希表里
    for(int i=0;i<method_count;i++)
    {
        const google::protobuf::MethodDescriptor* temp=descriptor->method(i);
        string method_name=temp->name();
        serviceInfo.m_methodMap[method_name]=temp;
        LOG_INFO("method_name: %s", method_name.c_str());
    }
    serviceInfo.m_service=service;
    m_serviceMap[service_name]=serviceInfo;
}

void RpcProvider::Run()
{
    std::string ip=MprpcApplication::GetInstance().GetConfig().Load("rpcserverip");
    uint16_t port=atoi(MprpcApplication::GetInstance().GetConfig().Load("rpcserverport").c_str());
    muduo::net::InetAddress address(ip,port);

    //创建TCP server
    muduo::net::TcpServer server(&m_eventLoop,address,"RpcProvider");

    //绑定连接回调
    server.setConnectionCallback(std::bind(&RpcProvider::OnConnection,this,std::placeholders::_1));

    //消息读写回调
    server.setMessageCallback(std::bind(&RpcProvider::OnMessage,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3));

    //设置muduo库的线程数量
    server.setThreadNum(4);

    //把当前rpc结点要发布的服务注册到zk上，让zk Client可以发现服务
    ZKClient zkCli;
    zkCli.Start();

    //service_name为永久性结点,method_name为临时结点
    for(auto it=m_serviceMap.begin();it!=m_serviceMap.end();it++)
    {
        //找到servcice的路径
        std::string service_path="/"+it->first;
        zkCli.Create(service_path.c_str(),nullptr,0);

        for(auto temp=it->second.m_methodMap.begin();temp!=it->second.m_methodMap.end();temp++)
        {
            //找到method的路径，/service_name/method_name,存储当前这个结点的IP和port
            std::string method_path=service_path + "/" + temp->first;
            char method_path_data[128]={0};
            sprintf(method_path_data, "%s:%d", ip.c_str(), port);

            // ZOO_EPHEERAL表示临时性结点
            zkCli.Create(method_path.c_str(), method_path_data, strlen(method_path_data), ZOO_EPHEMERAL);
            LOG_INFO("创建临时节点~  %s",method_path.c_str());
        }
    }

    cout<<"RpcProvider start ip: "<<ip.c_str()<<" port: "<<port<<endl;
    LOG_INFO("RpcProvider start ip:%s port:%d",ip.c_str(),port);

    //启动网络服务
    server.start();
    m_eventLoop.loop();
}

void RpcProvider::OnConnection(const muduo::net::TcpConnectionPtr& connection)
{
    if(!connection->connected())
    {
        //连接已经断开
        connection->shutdown();
    }
}

/*
 *  在框架内部, RpcProvider和RpcConsumer协商好通信用的protobuf数据类型
 *  需要传入service_name method_name args(参数, 如name pwd)给到框架, 这些可以在.proto文件进行定义
 *  为了解决粘包问题, 我们需要对数据长度进行区分, 包括header_size + header_str + args_size
 */

void RpcProvider::OnMessage(const muduo::net::TcpConnectionPtr& connection,muduo::net::Buffer* buffer,muduo::Timestamp)
{
    //网络上接收的远程rpc调用请求的字符流
    string recv_buf=buffer->retrieveAllAsString();

    //从字符流中读取前6个字节的数据
    //将recv_buf的前四个字节复制到header_size中
    uint32_t header_size=0;
    recv_buf.copy((char*)&header_size,6,0);

    //读取数字头的原始字符流
    string rpc_header_str=recv_buf.substr(6,header_size);

    //反序列化
    mprpc::RpcHeader rpcHeader;
    string service_name;        //请求的服务对象名
    string method_name;         //请求的方法名
    string args_str;            //参数内容
    uint32_t args_size;         //参数大小
    if(rpcHeader.ParseFromString(rpc_header_str.c_str()))
    {
        service_name=rpcHeader.service_name();
        method_name=rpcHeader.method_name();
        args_size=rpcHeader.args_size();

        args_str=recv_buf.substr(6+header_size,args_size);
    }
    else
    {
        LOG_ERROR("rpcHeader反序列化失败~!");
        return;
    }

    //打印信息
    cout << "======================" << endl;
    cout << "header_size: " << header_size << endl;
    cout << "rpc_header_str: " << rpc_header_str << endl;
    cout << "service_name: " << service_name << endl;
    cout << "method_name: " << method_name << endl;
    cout << "args_str: " << args_str << endl;
    cout << "======================" << endl;

    //获取service对象
    auto it=m_serviceMap.find(service_name);
    if(it==m_serviceMap.end())
    {
        LOG_INFO("service: %s Not Found!", service_name.c_str());
    }

    //获取method对象
    auto mit=it->second.m_methodMap.find(method_name);
    if(mit==it->second.m_methodMap.end())
    {
        LOG_ERROR("service: %s's method: %s Not Found!", service_name.c_str(), method_name.c_str());
    }

    google::protobuf::Service* service=it->second.m_service;
    const google::protobuf::MethodDescriptor* method=mit->second;

    //生成rpc方法调用的请求request和响应response参数
    google::protobuf::Message* request=service->GetRequestPrototype(method).New();
    if(!request->ParseFromString(args_str))
    {
        LOG_ERROR("参数反序列换失败!");
        return;
    }
    google::protobuf::Message* response = service->GetResponsePrototype(method).New();

    //  绑定一个Closure的回调函数, 供下面的CallMethod调用
    google::protobuf::Closure* done = google::protobuf::NewCallback<RpcProvider, const muduo::net::TcpConnectionPtr&,
                                                                google::protobuf::Message*>(this, &RpcProvider::SendRpcResponse, 
                                                                                            connection, response);

    // 在框架上根据远端rpc请求, 调用当前rpc结点上发布的方法
    service->CallMethod(method, nullptr, request, response, done); 
}

void RpcProvider::SendRpcResponse(const muduo::net::TcpConnectionPtr& connection, google::protobuf::Message* response) {
    std::string response_str;
    if(response->SerializeToString(&response_str)) { //进行序列化
        // 接下来通过网络发送出去
        connection->send(response_str);
        connection->shutdown();       //短连接, RpcProvide主动断开连接 
    } else {
        LOG_ERROR("RpcProvider response serialized failed!");
    }
}