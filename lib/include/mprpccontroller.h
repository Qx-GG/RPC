#pragma once
#include <iostream>
#include <google/protobuf/service.h>
#include <string>

class MprpcController:public google::protobuf::RpcController
{
public:
    MprpcController();
    void Reset();

    //记录是否失败
    bool Failed()const;

    //记录错误信息
    string ErrorText()const;

    void SetFailed(const string& reason);
    
    //目前未实现具体的功能
    void StartCancel();
    bool IsCanceled()const;
    void NotifyOnCancel(google::protobuf::Closure *callback);

private:
    bool m_failed;  //RPC执行过程中的状态
    string m_errtext;   //RPC方法执行过程中的错误信息
};