#include "client.h"
#include "overall.h"
//����Э���еĺ���
protcolBase* ParseCommand(uint8_t* recvbuff, uint16_t recv_length);
//---------------
/*
 * ���캯��
 */
CClient::CClient(CThreadManager* pManager,const SOCKET sClient, const sockaddr_in& addrClient)
	:CThreadBase(pManager)//����������ڳ�ʼ���б�������ӳ�ʼ���̳г�Ա����
{
	//��ʼ������
	m_socket = sClient;
	m_addr = addrClient;
	m_bConning = FALSE;
	m_bExit = FALSE;
	//�����¼�
	m_hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);//�ֶ������ź�״̬����ʼ��Ϊ���ź�״̬
	m_sMutex = CreateMutex(NULL, FALSE, NULL);	//��ʼ�������������ݷ��Ͷ�ռ
}
/*
 * ��������
 */
CClient::~CClient()
{
	closesocket(m_socket);			//�ر��׽���
	m_socket = INVALID_SOCKET;		//�׽�����Ч
	CloseHandle(m_hEvent);			//�ͷ��¼�����
}
/*
* �������ݺͷ��������߳�������
*/
int CClient::ThreadOnRun() {
	int		reVal;							//����ֵ
	char	temp[MAX_NUM_BUF];				//��ʱ����
	memset(temp, 0, MAX_NUM_BUF);			//�����ʱ����
	m_bConning = TRUE;
	//��whileѭ���н�������
		for (;m_bConning&&!getNeedStop();)				//����״̬
		{
			reVal = recv(m_socket, temp, MAX_NUM_BUF, 0);	//��������
			//������󷵻�ֵ
			if (SOCKET_ERROR == reVal)
			{
				int nErrCode = WSAGetLastError();

				if (WSAEWOULDBLOCK == nErrCode)	//�������ݻ�����������
				{
					continue;						//����ѭ��
				}
				else if (WSAENETDOWN == nErrCode ||//�ͻ��˹ر�������
					WSAETIMEDOUT == nErrCode ||
					WSAECONNRESET == nErrCode)
				{
					break;							//�߳��˳�				
				}
			}

			//�ͻ��˹ر�������
			if (reVal == 0)
			{
				break;
			}

			//�յ�����
			if (reVal >= MIN_NUM_BUF)
			{
				protcolBase* recevMsg;
				recevMsg = ParseCommand((uint8_t *)&temp,MAX_NUM_BUF);	//����ָ��,����һ����Ϣ����
				//recevMsg->unpackMsg();
				//std::cout << temp << std::endl;
				memset(temp, 0, MAX_NUM_BUF);	//�����ʱ����
			}
			Sleep(TIMEFOR_THREAD_CLIENT);		//�߳�˯��
		}
		m_bConning = FALSE;			//��ͻ��˵����ӶϿ�

		setFinished();		//�߳����
		setStopRunning();	//�߳�ֹͣ
		return 1;
}

 /* 
 *��ͻ��˷�������
 */
void CClient::sendData(const char* send_buff, uint16_t nSendlen)
{
	if(m_bConning)//����״̬
	{
			//���ͻ��˵����ӶϿ�ʱ
			if (!m_bConning)
			{
				m_bExit = TRUE;
				return;
			}
			WaitForSingleObject(m_sMutex, INFINITE);//�ȴ�������������(�������߳�ͬʱ��һ���ͻ��˷�������)
			int val = send(m_socket,send_buff,nSendlen, 0);
			ReleaseMutex(m_sMutex);//����������

			//�����ش���
			if (SOCKET_ERROR == val)
			{
				int nErrCode = WSAGetLastError();
				if (nErrCode == WSAEWOULDBLOCK)//�������ݻ�����������
				{
					return;
				}
				else if (WSAENETDOWN == nErrCode ||
					WSAETIMEDOUT == nErrCode ||
					WSAECONNRESET == nErrCode)//�ͻ��˹ر�������
				{
				
					m_bConning = FALSE;	//���ӶϿ�
					m_bExit = TRUE;		//�߳��˳�
					return;
				}
				else {
					m_bConning = FALSE;	//���ӶϿ�
					m_bExit = TRUE;		//�߳��˳�
					return;
				}
			}
			//�ɹ���������
		}
}