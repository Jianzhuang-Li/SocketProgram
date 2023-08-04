#include "threadManager.h"
#include "ThreadBase.h"
#include <future>
#include <process.h>

CThreadBase::CThreadBase(CThreadManager* _pThreadManager)
	:threadManager(_pThreadManager)//���캯����ʼ���б���ʼ�����Ա
{
	m_hThread = NULL;
	m_NeedStop = false;
	m_ThreadId = 0;
	m_finished = false;
	m_IsRunning = false;
}

CThreadBase::~CThreadBase()
{
	if (m_hThread) {
		CloseHandle(m_hThread);
	}
	m_hThread = NULL;
}

bool CThreadBase::ThreadStart()
{
	m_hThread = CreateThread(NULL, 0, CThreadBase::ThreadFunc, this, 0, &m_ThreadId);
	if (NULL == m_hThread)
	{
		return false;
	}
	threadManager->addNewThread(this);//�����̹߳���
	return true;
}

void CThreadBase::ThreadStop()
{
	m_NeedStop = true;
	while (!m_finished) {
		Sleep(100);
	}
	if (m_hThread)
		CloseHandle(m_hThread);
	m_hThread = NULL;
}

DWORD CThreadBase::ThreadFunc(void* pData)
{
	if (pData == NULL) {
		return -1;
	}
	CThreadBase* pBase = (CThreadBase*)pData;
	pBase->m_IsRunning = true;
	pBase->m_finished = false;
	int nRet = pBase->ThreadOnRun();
	return nRet;
}

void CThreadBase::WaitForQuit() {
	while (!m_finished) {

	}
}