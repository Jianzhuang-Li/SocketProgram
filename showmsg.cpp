#include "showmsg.h"
#include <iostream>
using namespace std;
/**
 *  ��ʾ��ʾ��Ϣ
 */

/**
 * ��ʾ�����������ɹ���ʧ����Ϣ
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