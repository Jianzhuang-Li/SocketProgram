#pragma once
#include <iostream>
using namespace std;

template <typename T>
class Singleton //������ģ��
{
private:
	static T*  obj;
	~Singleton() { DeleteInstatnce(); }
public:
	static T* instance() { //"&������"���Ի�÷���ֵ�ĵ�ַ
		if (!obj) {
			obj = new T();
		}
		return obj;
	}
	static void DeleteInstatnce() {
		if (obj)
			delete obj;
		obj = NULL;
	}
};
template<typename T>
T* Singleton<T>::obj = NULL;