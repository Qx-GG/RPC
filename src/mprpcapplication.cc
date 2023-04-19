#include "./include/mprpcapplication.h"
#include <iostream>
using namespace std;
#include <unistd.h>
#include <string.h>

//类静态成员初始化
MprpcConfig MprpcApplication::m_config;

//argc小于2时
void ShowArgsHelp()
{
    cout<<"format:command -i <configfile>"<<endl;
}

//rpc框架初始化
void MprpcApplication::Init(int argc,char **argv)
{
    if(argc<2)
    {
        ShowArgsHelp();
        exit(EXIT_FAILURE);
    }

    int c=0;
    string config_file;

    //运用getopt()分析命令行参数
    while((c=getopt(argc,argv,"i:"))!=-1)
    {
        switch(c)
        {
            case 'i':
                config_file=optarg;
            break;
            
            case '?':
                ShowArgsHelp();
            break;

            case ':':
                ShowArgsHelp();
                exit(EXIT_FAILURE);
            break;

            default:
            break;
        }
    }

    //开始加载配置文件
    /*格式为：
            rpcserver_ip
            rpcserver_port
            zookeeper_ip
            zookeeper_port
    */
    m_config.LoadConfigFile(config_file.c_str());
    cout<<"rpcserver ip:"<<m_config.Load("rpcserverip")<<endl;
    cout<<"rpcserver port:"<<m_config.Load("rpcserverport")<<endl;
    cout<<"zookeeper ip:"<<m_config.Load("zookeeperip")<<endl;
    cout<<"zookeeper port:"<<m_config.Load("zookeeperport")<<endl;
}

MprpcApplication& MprpcApplication::GetInstance()
{
    static MprpcApplication m_rpc;
    return m_rpc;
}

MprpcConfig& MprpcApplication::GetConfig()
{
    return m_config;
}