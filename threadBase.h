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
线程基类
*/
class CThreadBase
{
public:
	CThreadBase(CThreadManager* pThreadManager);								//构造函数
	virtual ~CThreadBase();						//析构函数
	bool					ThreadStart();		//启动线程
	void					ThreadStop();		//停止线程
	void					WaitForQuit();
	virtual int				ThreadOnRun()=0;	//必须重载ThreadOnRun虚函数，ThreadStart()以后，线程进入ThreadOnRun();
	bool					ThreadIsRunning() {	//返回线程的运行状态
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
	CThreadManager*			threadManager;		//线程管理
	HANDLE					m_hThread;			//线程句柄
	unsigned long           m_ThreadId;			//线程ID
	bool					m_NeedStop;			//线程需要停止
	bool                    m_finished;			//线程已经完成
	bool					m_IsRunning;        //线程是否在运行
};
