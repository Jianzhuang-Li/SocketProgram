#include "server.h"

//构造函数
CServer::CServer(CThreadManager* pThreadManager, unsigned short _port)
:CThreadBase(pThreadManager) {
	sServer = INVALID_SOCKET;
	port = _port;
	hServerEvent = CreateEvent(NULL, TRUE, FALSE, NULL);	//服务器退出事件，初始化为无信号状态
}

//析构函数
CServer::~CServer() {

}

//初始化socket
BOOL CServer::InitSocket()
{
	//返回值
	int reVal;

	//初始化Windows Sockets DLL
	WSADATA  wsData;
	if (WSAStartup(MAKEWORD(2, 2), &wsData)){
		printf("WSAStartup failed!\n");
		return FALSE;
	}

	//创建套接字			
	sServer = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == sServer) {
		printf("socket failed!\n");
		WSACleanup();
		return FALSE;
	}

	//绑定套接字
	sockaddr_in serAddr;
	serAddr.sin_family = AF_INET;
	serAddr.sin_port = htons(port);
	serAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	reVal = bind(sServer, (struct sockaddr*)&serAddr, sizeof(serAddr));
	if (reVal == -1) {
		printf("bind failed!\n");
		closesocket(sServer);
		WSACleanup();
		return FALSE;
	}

	//监听
	reVal = listen(sServer, SOMAXCONN);
	if (INVALID_SOCKET == sServer) {
		printf("lisen failed!\n");
		closesocket(sServer);
		WSACleanup();
		return FALSE;
	}

	return TRUE;
}

//退出服务器
BOOL CServer::exitServer() {
	m_NeedStop = true;
	shutdown(sServer, 2);
	closesocket(sServer);	
	sServer = INVALID_SOCKET;
	return 1;
}

//控制服务器(线程主函数)
int CServer::ThreadOnRun() {
	m_NeedStop = false;	
	while(!m_NeedStop){	
		SOCKET sAccept;
		sockaddr_in addrClient;
		memset(&addrClient, 0, sizeof(sockaddr_in));
		int lenClient = sizeof(sockaddr_in);
		sAccept = accept(sServer, (sockaddr*)&addrClient, &lenClient);
		if (INVALID_SOCKET != sAccept) {			
			CClient* clientThread = new CClient(threadManager, sAccept, addrClient);	//创建客户端线程
			if (!clientThread->ThreadStart()) {
				delete clientThread;
			}
			//printf("client:%d\n", clientThread->getThreadID());
		}
	}	
	if (sServer != INVALID_SOCKET) {
		shutdown(sServer, 2);//关闭连接但是不会关闭套接字（shutdown会等输出缓冲区中的数据传输完毕后再发送FIN包）
		closesocket(sServer);//关闭套接字
		sServer = INVALID_SOCKET;
	}
	::SetEvent(hServerEvent);
	setStopRunning();
	return 0;	//线程退出
}
