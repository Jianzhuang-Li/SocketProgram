#pragma once
#include <winsock2.h>
#include "threadBase.h"
#include "client.h"

class CThreadManager;	//声明线程管理类

class CServer :public CThreadBase 
{
public:
	CServer(CThreadManager* pThreadManager, unsigned short port);		//构造函数
	~CServer();		//析构函数
public:
	int				ThreadOnRun();		//线程函数（在子类中重载）根据用户输入控制服务器
	BOOL			InitSocket();	//初始化socket
	BOOL			exitServer();		//退出服务器
	void			waitForServerExit() {
		WaitForSingleObject(hServerEvent, INFINITE);
	}
private:
	HANDLE			hServerEvent;	
	unsigned short  port;
	SOCKET			sServer;											//监听套接字
};