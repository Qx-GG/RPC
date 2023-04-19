#pragma once
#include <iostream>
#include <unordered_map>
#include "string.h"

class MprpcConfig
{
public:
    //负责解析加载配置文件
    void LoadConfigFile(const char* config_file);

    //负责配置项信息
    std::string Load(const std::string key);
private:
    /* rpcserverip rpcserverport zookeeperip zookeeperprot*/
    std::unordered_map<std::string,std::string> hashmap;

    //去掉字符串的前后空格
    void Trim(std::string& buf);
};