#pragma once
//使用单体类模板实例化线程管理类
#include <WinSock2.h>
#include "threadBase.h"
#include <vector>

class CThreadManager
{
public:
    CThreadManager(void);   //构造函数
    ~CThreadManager(void);   //析构函数
   //获取线程的数量
    size_t getThreadNum();
    //获取正在运行的线程数量
    size_t getRunningThreadNum();
    //加入新的线程
    bool addNewThread(CThreadBase* pThread);
    //通过指针结束线程
    void quitThreadByPoint(CThreadBase* pThread);
    //结束所有线程
    void quitAllThread();
    //通知所有线程退出
    void notifyAllThreadQuit();
    // 移除已经退出的线程
    void removeExitedThread();
    //检查是否有退出的线程
    bool checkExitedThread();
private:
     HANDLE m_hMutex;     //写独占控制
    static vector<CThreadBase*> m_arrThread; //线程向量
};
