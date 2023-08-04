#include "showmsg.h"
#include <iostream>
using namespace std;
/**
 *  显示提示信息
 */

/**
 * 显示启动服务器成功与失败消息
 */
void  ShowServerStartMsg(bool bSuc)
{
	if (bSuc)
	{
		cout << "**********************" << endl;
		cout << "*                    *" << endl;
		cout << "* Server succeeded!  *" << endl;
		cout << "*                    *" << endl;
		cout << "**********************" << endl;
	}
	else {
		cout << "**********************" << endl;
		cout << "*                    *" << endl;
		cout << "* Server failed   !  *" << endl;
		cout << "*                    *" << endl;
		cout << "**********************" << endl;
	}

}