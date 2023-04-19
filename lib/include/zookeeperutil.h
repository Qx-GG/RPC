#pragma once

#include <iostream>
#include <string>
#include <semaphore.h>
#include <zookeeper/zookeeper.h>

//zookeeper客户端类
class ZKClient
{
public:
    ZKClient();
    ~ZKClient();

    //启动连接zkserver
    void Start();

    //在zkserver上根据path创建znode结点
    void Create(const char* path,const char *data,int datalen,int state=0);

    //根据path获得znode结点的值
    std::string GetData(const char* path);

private:
    //zk客户端句柄
    zhandle_t* m_zhangdle;
};