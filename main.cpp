#include "overall.h"


/*
#define SEVER_SETUP_FALL			1		//启动服务器失败
#define SERVERPORT					5556	//服务器TCP端口
#define TIMEFOR_THREAD_EXIT			5000	//主线程睡眠时间
#define TIMEFOR_THREAD_HELP			1500	//清理资源线程退出时间
#define TIMEFOR_THREAD_SLEEP		500		//等待客户端请求线程睡眠时间
#define MAX_NUM_BUF		            48		//缓冲区的最大长度
*/


void	ShowTipMsg(bool bFirstInput)
{
	if (bFirstInput)//第一次
	{
		cout << endl;
		cout << endl;
		cout << "**********************" << endl;
		cout << "*                    *" << endl;
		cout << "* s(S): Start server *" << endl;
		cout << "*                    *" << endl;
		cout << "**********************" << endl;
		cout << "Please input:" << endl;

	}
	else {//退出服务器		
		cout << endl;
		cout << endl;
		cout << "**********************" << endl;
		cout << "*                    *" << endl;
		cout << "* e(E): Exit  server *" << endl;
		cout << "*                    *" << endl;
		cout << "**********************" << endl;
		cout << " Please input:" << endl;
	}
}

/*
*显示退出服务器消息
*/
void  ShowServerExitMsg(void)
{

	cout << "**********************" << endl;
	cout << "*                    *" << endl;
	cout << "* Server exit...     *" << endl;
	cout << "*                    *" << endl;
	cout << "**********************" << endl;
}

ProtcolPond* protcolPond;//协议池
int main()
{	
	CThreadManager* pthreadManager = Singleton<CThreadManager>::instance();//创建线程管理
	protcolPond = Singleton<ProtcolPond>::instance();//创建协议池
	bool bFirst = true;
	CServer* mServer = NULL;
	while (1) {
		ShowTipMsg(bFirst);
		//启动服务器线程
		char cInput;
		std::cin >> cInput;
		if (cInput == 'S' || cInput == 's') {//启动服务器
			mServer = new CServer(pthreadManager, SERVERPORT);	//创建服务器线程对象
			if (!mServer->InitSocket() || !mServer->ThreadStart()) {
				ShowServerStartMsg(FALSE);
				delete mServer;
				return 0;
			}
			ShowServerStartMsg(TRUE);
			bFirst = false;
		}
		else if (cInput == 'E' || cInput == 'e') {//退出服务器		
			if (mServer == NULL)
				printf("message:server has stoped!\n");
			else {
				LPCWSTR S1 = TEXT("Are you sure?");
				LPCWSTR S2 = TEXT("Server");
				if (IDOK == MessageBox(NULL, S1, S2, MB_OKCANCEL)) {
					ShowServerExitMsg();
					Sleep(TIMEFOR_THREAD_EXIT);
					mServer->exitServer();
					mServer->waitForServerExit();
				}
				mServer = NULL;
				ShowServerExitMsg();
				bFirst = true;
			}
		}
		else {
			printf("have no command:%c\n", cInput);
		}
	}	
	//确保所有资源都被回收，避免内存泄漏
	while (pthreadManager->getThreadNum()) {
		pthreadManager->removeExitedThread();	//清理退出的线程
		Sleep(TIMEFOR_THREAD_HELP);
	}
	return 1;
}

