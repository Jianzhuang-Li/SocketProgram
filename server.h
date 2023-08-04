#pragma once
#include <winsock2.h>
#include "threadBase.h"
#include "client.h"

class CThreadManager;	//�����̹߳�����

class CServer :public CThreadBase 
{
public:
	CServer(CThreadManager* pThreadManager, unsigned short port);		//���캯��
	~CServer();		//��������
public:
	int				ThreadOnRun();		//�̺߳����������������أ������û�������Ʒ�����
	BOOL			InitSocket();	//��ʼ��socket
	BOOL			exitServer();		//�˳�������
	void			waitForServerExit() {
		WaitForSingleObject(hServerEvent, INFINITE);
	}
private:
	HANDLE			hServerEvent;	
	unsigned short  port;
	SOCKET			sServer;											//�����׽���
};