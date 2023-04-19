#include "./include/zookeeperutil.h"
#include "./include/mprpcapplication.h"
#include "./include/logger.h"

// 全局的watcher观察器(是一个异步线程), zkserver给zkclient发送通知
void global_watcher(zhandle_t *zh,int type,int state,const char *path,void *watcherCtx)
{
    if(type==ZOO_SESSION_EVENT)
    {
        //回调消息类型是和会话相关的消息类型
        if(state==ZOO_CONNECTED_STATE)
        {   
            //zkclient和zkserver连接成功
            sem_t *sem=(sem_t*)zoo_get_context(zh);
            sem_post(sem);
        }
    }
}

//构造函数
ZKClient::ZKClient():m_zhangdle(nullptr)
{

}

//析构函数
ZKClient::~ZKClient()
{
    if(m_zhangdle!=nullptr)
    {
        zookeeper_close(m_zhangdle);
    }
}

//zk客户端启动连接zk服务端
void ZKClient::Start()
{
    //加载zk的IP地址和端口号(默认为2181)
    std::string host=MprpcApplication::GetInstance().GetConfig().Load("zookeeperip");
    std::string port=MprpcApplication::GetInstance().GetConfig().Load("zookeeperport");
    std::string connect=host+":"+port;

    m_zhangdle=zookeeper_init(connect.c_str(),global_watcher,30000,nullptr,nullptr,0);
    if(m_zhangdle==nullptr)
    {
        LOG_ERROR("zookeeper初始化失败!");
        exit(EXIT_FAILURE);
    }
    //定义一个信号量
    sem_t sem;
    sem_init(&sem,0,0); //初始化资源为0
    zoo_set_context(m_zhangdle,&sem);   //设置上下文,添加额外信息

    sem_wait(&sem);
    LOG_INFO("zookeeper初始化成功!");
}

// 在zkserver上根据指定的path创建znode结点
void ZKClient::Create(const char* path,const char *data,int datalen,int state)
{
    char path_buf[258];
    int buf_len=sizeof(path_buf);

    int flag=zoo_exists(m_zhangdle,path,0,nullptr);
    if(flag==ZNONODE)
    {
        //结点不存在，可以创建znode结点
        flag=zoo_create(m_zhangdle,path,data,datalen,&ZOO_OPEN_ACL_UNSAFE,state,path_buf,buf_len);

        if(flag==ZOK)
        {
            LOG_INFO("创建znode结点成功~!");
        }
        else
        {
            LOG_ERROR("创建znode结点失败~!");
            cout<<"flag:"<<flag<<endl;
            cout<<"znode创建失败~   path:"<<path<<endl;
            exit(EXIT_FAILURE);
        }
    }
}

//获得指定结点的值
std::string ZKClient::GetData(const char* path)
{
    char buf[64];
    int buf_len=sizeof(buf);
    int flag=zoo_get(m_zhangdle,path,0,buf,&buf_len,nullptr);
    if(flag!=ZOK)
    {
        LOG_ERROR("获取失败~! path:%s",path);
        cout<<"获取失败~   path:"<<path<<endl;
        return "";
    }
    else
    {
        LOG_INFO("获取成功~!");
        return buf;
    }
}
