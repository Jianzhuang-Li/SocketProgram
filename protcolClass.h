#pragma once
#include <map>
#include "threadManager.h"
using namespace std;
#include "protcolBase.h"
/*
//协议类型
//客户端发往服务器
#define HEART_BEAT					0xFF11
#define SENDER_REGIST				0xFF12
#define SENDER_INQUIRY				0xFF13
#define SENDER_PAIR					0xFF14
#define RECEIVER_REGIST				0xFF15
#define RECEIVER_INQUIRY			0xFF16
#define RECEIVER_NEED_PAIR			0xFF17
#define RECEIVER_PAIR_RETURN		0xFF18
#define DATA_SEND_TO_MASTER			0xFF31
//服务器发往客户端
#define HEART_BEAT_RETURN			0xFF01
#define SENDER_REGIST_RETURN		0xFF02
#define SENDER_INQUIRY_RETURN		0xFF03
#define SENDER_PAIR_RETURN			0xFF04
#define RECEIVER_REGIST_RETURN		0xFF05
#define RECEIVER_INQUIRY_RETURN		0xFF06
#define RECEIVER_INQUIRY_PAIR		0xFF07
#define RECEIVER_PAIR_SUCCESS		0xFF08
#define DATA_SEND_TO_SLAVE			0xFF21
*/
#define DEVICE_ID_LEN	8  //设备ID的长度
#define CONTROL_DATA_LEN 8 //控制数据的长度
//协议池
typedef protcolBase* (*transObject)(protcolBase*);
class ProtcolPond {
public:
	ProtcolPond();
	map<uint16_t, transObject> m_protcolPond;//用于保存协议的图
	void insertMap(uint16_t msgType, transObject returnObject);  //将协议插入图；msgType:消息类型，四位16进制数；
																 //returnObject的功能是将以协议基类保存消息指针转换为该条消息对应的子类指针
																 //该函数是在各消息子类中定义的静态函数
	transObject findInMap(uint16_t msgType, transObject pFunc);//在协议图（map）中查找消息，并转换为对应的消息类型
};

//消息基类
class msgBase :public protcolBase {
	//定义了所有消息的共有属性
public:
	msgBase();
	char hostDeviceID[8] = { 0 };		//主机ID
	char SlaveDeviceID[8] = { 0 };		//从机ID
	uint8_t DeviceType;				//设备类型
};


//-------------message-class-------------//

//心跳消息
class HeartBeat :public msgBase {
public:
	HeartBeat();
	virtual bool packMsg(void);
	static protcolBase* returnObject(protcolBase* pBase) {
		return (HeartBeat*)pBase;
	}
};
//发射机注册
class SenderRegist :public msgBase {
public:
	virtual bool unpackMsg(void);
	static protcolBase* returnObject(protcolBase* pBase) {
		return (SenderRegist*)pBase;
	}
};

//发射机配对
class SenderPair :public msgBase {
public:
	virtual bool unpackMsg(void);
	static protcolBase* returnObject(protcolBase* pBase) {
		return (SenderPair*)pBase;
	}
};

//发射机询问
class SenderInquiry :public msgBase {
public:
	virtual bool unpackMsg(void);
	static protcolBase* returnObject(protcolBase* pBase) {
		return (SenderInquiry*)pBase;
	}
};

//接收机注册
class ReceiverRegist :public msgBase {
public:
	virtual bool unpackMsg();
	static protcolBase* returnObject(protcolBase* pBase) {
		return (ReceiverRegist*)pBase;
	}
};

//接收机询问
class ReceiverInquiry :public msgBase {
public:
	virtual bool unpackMsg();
	static protcolBase* returnObject(protcolBase* pBase) {
		return (ReceiverInquiry*)pBase;
	}
};

//设置接收机为待配对状态
class ReceiverNeedPair :public msgBase {
public:
	virtual bool unpackMsg();		 //消息解包
	static protcolBase* returnObject(protcolBase* pBase) {
		return (ReceiverNeedPair*)pBase;
	}
};

//接收机收到配对请求后回复
class ReceiverPairReturn :public msgBase {
public:
	virtual bool unpackMsg();
	static protcolBase* returnObject(protcolBase* pBase) {
		return (ReceiverPairReturn*)pBase;
	}
};

//发射机发送控制数据到服务器
class SendDataToMaster :public msgBase {
public:
	string controlData;		 //控制数据
	virtual bool unpackMsg();
	static protcolBase* returnObject(protcolBase* pBase) {
		return (SendDataToMaster*)pBase;
	}
};

//服务器心跳回复
class HeartBeatReturn :public msgBase {
public:
	HeartBeatReturn();
	virtual bool packMsg(void);
	static protcolBase* returnObject(protcolBase* pBase) {
		return  (HeartBeatReturn*)pBase;
	}
};

//发送注册回复
class SenderRigistReturn :public msgBase {
public:
	//SenderRigistReturn();
	virtual bool packMsg(void);
	static protcolBase* returnObject(protcolBase* pBase) {
		return (SenderRigistReturn*)pBase;
	}
};