#include "client.h"
#include "overall.h"
//声明协议中的函数
protcolBase* ParseCommand(uint8_t* recvbuff, uint16_t recv_length);
//---------------
/*
 * 构造函数
 */
CClient::CClient(CThreadManager* pManager,const SOCKET sClient, const sockaddr_in& addrClient)
	:CThreadBase(pManager)//将基类包含在初始化列表中来间接初始化继承成员函数
{
	//初始化变量
	m_socket = sClient;
	m_addr = addrClient;
	m_bConning = FALSE;
	m_bExit = FALSE;
	//创建事件
	m_hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);//手动设置信号状态，初始化为无信号状态
	m_sMutex = CreateMutex(NULL, FALSE, NULL);	//初始化互斥量，数据发送独占
}
/*
 * 析构函数
 */
CClient::~CClient()
{
	closesocket(m_socket);			//关闭套接字
	m_socket = INVALID_SOCKET;		//套接字无效
	CloseHandle(m_hEvent);			//释放事件对象
}
/*
* 接收数据和发送数据线程主函数
*/
int CClient::ThreadOnRun() {
	int		reVal;							//返回值
	char	temp[MAX_NUM_BUF];				//临时变量
	memset(temp, 0, MAX_NUM_BUF);			//清空临时变量
	m_bConning = TRUE;
	//在while循环中接收数据
		for (;m_bConning&&!getNeedStop();)				//连接状态
		{
			reVal = recv(m_socket, temp, MAX_NUM_BUF, 0);	//接收数据
			//处理错误返回值
			if (SOCKET_ERROR == reVal)
			{
				int nErrCode = WSAGetLastError();

				if (WSAEWOULDBLOCK == nErrCode)	//接受数据缓冲区不可用
				{
					continue;						//继续循环
				}
				else if (WSAENETDOWN == nErrCode ||//客户端关闭了连接
					WSAETIMEDOUT == nErrCode ||
					WSAECONNRESET == nErrCode)
				{
					break;							//线程退出				
				}
			}

			//客户端关闭了连接
			if (reVal == 0)
			{
				break;
			}

			//收到数据
			if (reVal >= MIN_NUM_BUF)
			{
				protcolBase* recevMsg;
				recevMsg = ParseCommand((uint8_t *)&temp,MAX_NUM_BUF);	//解析指令,返回一个消息对象
				//recevMsg->unpackMsg();
				//std::cout << temp << std::endl;
				memset(temp, 0, MAX_NUM_BUF);	//清空临时变量
			}
			Sleep(TIMEFOR_THREAD_CLIENT);		//线程睡眠
		}
		m_bConning = FALSE;			//与客户端的连接断开

		setFinished();		//线程完成
		setStopRunning();	//线程停止
		return 1;
}

 /* 
 *向客户端发送数据
 */
void CClient::sendData(const char* send_buff, uint16_t nSendlen)
{
	if(m_bConning)//连接状态
	{
			//当客户端的连接断开时
			if (!m_bConning)
			{
				m_bExit = TRUE;
				return;
			}
			WaitForSingleObject(m_sMutex, INFINITE);//等待互斥量被触发(避免多个线程同时向一个客户端发送数据)
			int val = send(m_socket,send_buff,nSendlen, 0);
			ReleaseMutex(m_sMutex);//触发互斥量

			//处理返回错误
			if (SOCKET_ERROR == val)
			{
				int nErrCode = WSAGetLastError();
				if (nErrCode == WSAEWOULDBLOCK)//发送数据缓冲区不可用
				{
					return;
				}
				else if (WSAENETDOWN == nErrCode ||
					WSAETIMEDOUT == nErrCode ||
					WSAECONNRESET == nErrCode)//客户端关闭了连接
				{
				
					m_bConning = FALSE;	//连接断开
					m_bExit = TRUE;		//线程退出
					return;
				}
				else {
					m_bConning = FALSE;	//连接断开
					m_bExit = TRUE;		//线程退出
					return;
				}
			}
			//成功发送数据
		}
}