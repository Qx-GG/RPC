#pragma once
#include <iostream>
#include <unordered_map>
#include <string.h>

class MprpcConfig
{
public:
    //负责解析加载配置文件
    void LoadConfigFile(const char* config_file);

    //负责配置项信息
    string Load(const string key);
private:
    /* rpcserverip rpcserverport zookeeperip zookeeperprot*/
    unordered_map<string,string> hashmap;

    //去掉字符串的前后空格
    void Trim(string& buf);
};