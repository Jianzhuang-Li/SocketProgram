#pragma once
//#include <iostream>
using namespace std;
#include <winsock2.h>
#include <process.h>
#include "threadBase.h"



//客户端线程类，继承于线程基类
class CClient:public CThreadBase
{
public:
	CClient(CThreadManager* pManager, const SOCKET sClient, const sockaddr_in& addrClient);
	virtual ~CClient();

public:
	BOOL		IsConning(void) {					//是否连接存在
		return m_bConning;
	}
	void		DisConning(void) {					//断开与客户端的连接
		m_bConning = FALSE;
	}
	BOOL		IsExit(void) {						//接收和发送线程是否已经退出
		return m_bExit;
	}

public:
	int ThreadOnRun();													//线程函数（在子类中重载）接收,解析，发送数据
	void sendData(const char* send_buff, uint16_t nSendlen);			//发送数据到客户端

public:
	uint8_t		m_deviceType=0;		//设备类型
	uint8_t		m_deviceID[8]={0};		//设备ID
	uint8_t		m_pairDeviceID[8]={0};	//配对设备的ID
private:
	SOCKET		m_socket;			//套接字
	sockaddr_in	m_addr;				//地址
	HANDLE		m_hEvent;			//事件对象
	BOOL		m_bConning;			//客户端连接状态
	BOOL		m_bExit;			//线程退出
	HANDLE      m_sMutex;			//数据发送独占控制
};

