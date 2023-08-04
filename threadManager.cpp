#include "threadManager.h"
#include "overall.h"
//���캯��

vector<CThreadBase*> CThreadManager::m_arrThread; //��̬���Ա������Ҫ�������ʼ��

CThreadManager::CThreadManager() {
	m_hMutex = CreateMutex(NULL, FALSE, NULL);	//��ʼ��������
}

CThreadManager::~CThreadManager() {
	quitAllThread();
	CloseHandle(m_hMutex); //���ٻ�����
}

//��ȡ�̵߳�����
size_t CThreadManager::getThreadNum() {
	return m_arrThread.size(); //�������ɵ�Ԫ����
}

//��ȡ�������е��̵߳�����
size_t CThreadManager::getRunningThreadNum() {
	vector<CThreadBase*>::iterator iter;	//���������
	int runningThreadNum = 0;
	WaitForSingleObject(m_hMutex, INFINITE);//�ȴ�������������
	for (iter = m_arrThread.begin(); iter != m_arrThread.end(); iter++) {
		if ((*iter)->ThreadIsRunning()) runningThreadNum++;
	}
	ReleaseMutex(m_hMutex);//����������
	return runningThreadNum;
}

//����µ��߳�
bool CThreadManager::addNewThread(CThreadBase *pThread) {
	WaitForSingleObject(m_hMutex, INFINITE);//�ȴ�������������
	m_arrThread.push_back(pThread); //���߳�ָ����ӵ�����ĩβ
	ReleaseMutex(m_hMutex);//����������
	return 0;
}

//�˳������߳�
void CThreadManager::quitAllThread() {
	vector<CThreadBase*>::iterator iter;//���������
	WaitForSingleObject(m_hMutex, INFINITE);//�ȴ�������������
	for (iter = m_arrThread.begin(); iter != m_arrThread.end(); iter++) {
		(*iter)->ThreadStop();
	}
	ReleaseMutex(m_hMutex);//����������
}

//ͨ��ָ������߳�
void CThreadManager::quitThreadByPoint(CThreadBase* pThread) {
	vector<CThreadBase*>::iterator iter;//���������
	WaitForSingleObject(m_hMutex, INFINITE);//�ȴ�������������
	for (iter = m_arrThread.begin(); iter != m_arrThread.end(); iter++) {
		if ((*iter) == pThread) {
			(*iter)->ThreadStop();
		}
	}
	ReleaseMutex(m_hMutex);//����������
}

//֪ͨ���������߳�
void CThreadManager::notifyAllThreadQuit() {
	vector<CThreadBase*>::iterator iter;//���������
	WaitForSingleObject(m_hMutex, INFINITE);//�ȴ�������������
	for (iter = m_arrThread.begin(); iter != m_arrThread.end(); iter++) {
		(*iter)->setNeedStop();
	}
	ReleaseMutex(m_hMutex);//����������
}

//����Ƿ����Ѿ��˳����߳�
bool CThreadManager::checkExitedThread() {
	vector<CThreadBase*>::iterator iter;//���������
	for (iter = m_arrThread.begin(); iter != m_arrThread.end(); iter++) {
		if ((*iter)->getFinished())	return 1;
	}
	return 0;
}

//�Ƴ��Ѿ��˳����߳�
void CThreadManager::removeExitedThread()
{
	vector<CThreadBase*>::iterator iter;//���������
	WaitForSingleObject(m_hMutex, INFINITE);//�ȴ�������������
	for (iter = m_arrThread.begin(); iter != m_arrThread.end();) {
		if ((*iter)->getFinished()) {
			printf("erase thread: %d\n", (*iter)->getThreadID());
			delete	*iter;//�ͷ��ڴ�
			iter = m_arrThread.erase(iter);//��������ɾ���Ѿ��˳����߳�
		}
		else {
			iter++;
		}
	}
	ReleaseMutex(m_hMutex);//����������
	Sleep(TIMEFOR_THREAD_HELP);
}