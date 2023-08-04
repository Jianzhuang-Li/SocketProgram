#pragma once
//ʹ�õ�����ģ��ʵ�����̹߳�����
#include <WinSock2.h>
#include "threadBase.h"
#include <vector>

class CThreadManager
{
public:
    CThreadManager(void);   //���캯��
    ~CThreadManager(void);   //��������
   //��ȡ�̵߳�����
    size_t getThreadNum();
    //��ȡ�������е��߳�����
    size_t getRunningThreadNum();
    //�����µ��߳�
    bool addNewThread(CThreadBase* pThread);
    //ͨ��ָ������߳�
    void quitThreadByPoint(CThreadBase* pThread);
    //���������߳�
    void quitAllThread();
    //֪ͨ�����߳��˳�
    void notifyAllThreadQuit();
    // �Ƴ��Ѿ��˳����߳�
    void removeExitedThread();
    //����Ƿ����˳����߳�
    bool checkExitedThread();
private:
     HANDLE m_hMutex;     //д��ռ����
    static vector<CThreadBase*> m_arrThread; //�߳�����
};
