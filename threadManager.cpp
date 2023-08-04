#include "threadManager.h"
#include "overall.h"
//构造函数

vector<CThreadBase*> CThreadManager::m_arrThread; //静态类成员变量需要在类外初始化

CThreadManager::CThreadManager() {
	m_hMutex = CreateMutex(NULL, FALSE, NULL);	//初始化互斥量
}

CThreadManager::~CThreadManager() {
	quitAllThread();
	CloseHandle(m_hMutex); //销毁互斥量
}

//获取线程的数量
size_t CThreadManager::getThreadNum() {
	return m_arrThread.size(); //返回容纳的元素数
}

//获取正在运行的线程的数量
size_t CThreadManager::getRunningThreadNum() {
	vector<CThreadBase*>::iterator iter;	//定义迭代器
	int runningThreadNum = 0;
	WaitForSingleObject(m_hMutex, INFINITE);//等待互斥量被触发
	for (iter = m_arrThread.begin(); iter != m_arrThread.end(); iter++) {
		if ((*iter)->ThreadIsRunning()) runningThreadNum++;
	}
	ReleaseMutex(m_hMutex);//触发互斥量
	return runningThreadNum;
}

//添加新的线程
bool CThreadManager::addNewThread(CThreadBase *pThread) {
	WaitForSingleObject(m_hMutex, INFINITE);//等待互斥量被触发
	m_arrThread.push_back(pThread); //将线程指针添加到向量末尾
	ReleaseMutex(m_hMutex);//触发互斥量
	return 0;
}

//退出所有线程
void CThreadManager::quitAllThread() {
	vector<CThreadBase*>::iterator iter;//定义迭代器
	WaitForSingleObject(m_hMutex, INFINITE);//等待互斥量被触发
	for (iter = m_arrThread.begin(); iter != m_arrThread.end(); iter++) {
		(*iter)->ThreadStop();
	}
	ReleaseMutex(m_hMutex);//触发互斥量
}

//通过指针结束线程
void CThreadManager::quitThreadByPoint(CThreadBase* pThread) {
	vector<CThreadBase*>::iterator iter;//定义迭代器
	WaitForSingleObject(m_hMutex, INFINITE);//等待互斥量被触发
	for (iter = m_arrThread.begin(); iter != m_arrThread.end(); iter++) {
		if ((*iter) == pThread) {
			(*iter)->ThreadStop();
		}
	}
	ReleaseMutex(m_hMutex);//触发互斥量
}

//通知结束所有线程
void CThreadManager::notifyAllThreadQuit() {
	vector<CThreadBase*>::iterator iter;//定义迭代器
	WaitForSingleObject(m_hMutex, INFINITE);//等待互斥量被触发
	for (iter = m_arrThread.begin(); iter != m_arrThread.end(); iter++) {
		(*iter)->setNeedStop();
	}
	ReleaseMutex(m_hMutex);//触发互斥量
}

//检查是否有已经退出的线程
bool CThreadManager::checkExitedThread() {
	vector<CThreadBase*>::iterator iter;//定义迭代器
	for (iter = m_arrThread.begin(); iter != m_arrThread.end(); iter++) {
		if ((*iter)->getFinished())	return 1;
	}
	return 0;
}

//移除已经退出的线程
void CThreadManager::removeExitedThread()
{
	vector<CThreadBase*>::iterator iter;//定义迭代器
	WaitForSingleObject(m_hMutex, INFINITE);//等待互斥量被触发
	for (iter = m_arrThread.begin(); iter != m_arrThread.end();) {
		if ((*iter)->getFinished()) {
			printf("erase thread: %d\n", (*iter)->getThreadID());
			delete	*iter;//释放内存
			iter = m_arrThread.erase(iter);//从向量中删除已经退出的线程
		}
		else {
			iter++;
		}
	}
	ReleaseMutex(m_hMutex);//触发互斥量
	Sleep(TIMEFOR_THREAD_HELP);
}