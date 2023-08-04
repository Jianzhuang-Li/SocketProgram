#pragma once

//#include <Windows.h>
//#include <winnt.h>
//#include <stdio.h>
#include <iostream>
using namespace std;

#include <string>
using namespace std;

class CThreadManager;
/**
�̻߳���
*/
class CThreadBase
{
public:
	CThreadBase(CThreadManager* pThreadManager);								//���캯��
	virtual ~CThreadBase();						//��������
	bool					ThreadStart();		//�����߳�
	void					ThreadStop();		//ֹͣ�߳�
	void					WaitForQuit();
	virtual int				ThreadOnRun()=0;	//��������ThreadOnRun�麯����ThreadStart()�Ժ��߳̽���ThreadOnRun();
	bool					ThreadIsRunning() {	//�����̵߳�����״̬
		return m_IsRunning;
	}
	void					setNeedStop() {
		m_NeedStop = true;
	}
	void setFinished() {
		m_finished = true;
	}
	bool					getFinished() {
		return	m_finished;
	}
	bool getNeedStop() {
		return m_NeedStop;
	}
	void setStopRunning() {
		m_IsRunning = false;
	}
	unsigned long getThreadID() {
		return  m_ThreadId;
	}
public:
	static  DWORD	__stdcall  ThreadFunc(void* pData);

protected:
	CThreadManager*			threadManager;		//�̹߳���
	HANDLE					m_hThread;			//�߳̾��
	unsigned long           m_ThreadId;			//�߳�ID
	bool					m_NeedStop;			//�߳���Ҫֹͣ
	bool                    m_finished;			//�߳��Ѿ����
	bool					m_IsRunning;        //�߳��Ƿ�������
};
