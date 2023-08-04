#pragma once
#include <iostream>
using namespace std;

template <typename T>
class Singleton //单体类模板
{
private:
	static T*  obj;
	~Singleton() { DeleteInstatnce(); }
public:
	static T* instance() { //"&函数名"可以获得返回值的地址
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