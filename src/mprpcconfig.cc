#include "./include/mprpcconfig.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include "./include/logger.h"

void MprpcConfig::LoadConfigFile(const char* config_file)
{
    FILE* pf=fopen(config_file,"r");
    if(pf==nullptr)
    {
        LOG_ERROR("%s is not exist!",config_file);
        exit(EXIT_FAILURE);
    }

    while(!feof(pf))
    {
        char buf[1024]={0};
        fgets(buf,1024,pf);

        //去掉字符串前面和后面多余的空格
        string src_buf(buf);
        Trim(src_buf);

        //判断#的注释
        if(src_buf[0]=='#'||src_buf.empty())
        {
            continue;
        }

        //解析配置项
        int idx=src_buf.find('=');
        if(idx==-1)
        {
            continue;
        }

        //储存配置项
        string key=src_buf.substr(0,idx);
        Trim(key);

        //127.0.0.1    \n
        int endidx=src_buf.find('\n',idx);
        string value=src_buf.substr(idx+1,endidx-idx-1);
        Trim(value);

        hashmap[key]=value;
    }
}

string MprpcConfig::Load(const string key)
{
    if(hashmap.find(key)==hashmap.end())
    {
        return "";
    }
    return hashmap[key];
}

void MprpcConfig::Trim(string &buf)
{
    //去掉buf前面的空格
    int idx=buf.find_first_not_of(' ');
    if(idx!=-1)
    {
        buf=buf.substr(idx,buf.size()-idx);
    }

    //去掉buf后面的空格
    idx=buf.find_last_not_of(' ');
    if(idx!=-1)
    {
        buf=buf.substr(0,idx+1);
    }
}