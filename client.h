#pragma once
//#include <iostream>
using namespace std;
#include <winsock2.h>
#include <process.h>
#include "threadBase.h"



//�ͻ����߳��࣬�̳����̻߳���
class CClient:public CThreadBase
{
public:
	CClient(CThreadManager* pManager, const SOCKET sClient, const sockaddr_in& addrClient);
	virtual ~CClient();

public:
	BOOL		IsConning(void) {					//�Ƿ����Ӵ���
		return m_bConning;
	}
	void		DisConning(void) {					//�Ͽ���ͻ��˵�����
		m_bConning = FALSE;
	}
	BOOL		IsExit(void) {						//���պͷ����߳��Ƿ��Ѿ��˳�
		return m_bExit;
	}

public:
	int ThreadOnRun();													//�̺߳����������������أ�����,��������������
	void sendData(const char* send_buff, uint16_t nSendlen);			//�������ݵ��ͻ���

public:
	uint8_t		m_deviceType=0;		//�豸����
	uint8_t		m_deviceID[8]={0};		//�豸ID
	uint8_t		m_pairDeviceID[8]={0};	//����豸��ID
private:
	SOCKET		m_socket;			//�׽���
	sockaddr_in	m_addr;				//��ַ
	HANDLE		m_hEvent;			//�¼�����
	BOOL		m_bConning;			//�ͻ�������״̬
	BOOL		m_bExit;			//�߳��˳�
	HANDLE      m_sMutex;			//���ݷ��Ͷ�ռ����
};

