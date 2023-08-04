#pragma once
#include <map>
#include "threadManager.h"
using namespace std;
#include "protcolBase.h"
/*
//Э������
//�ͻ��˷���������
#define HEART_BEAT					0xFF11
#define SENDER_REGIST				0xFF12
#define SENDER_INQUIRY				0xFF13
#define SENDER_PAIR					0xFF14
#define RECEIVER_REGIST				0xFF15
#define RECEIVER_INQUIRY			0xFF16
#define RECEIVER_NEED_PAIR			0xFF17
#define RECEIVER_PAIR_RETURN		0xFF18
#define DATA_SEND_TO_MASTER			0xFF31
//�����������ͻ���
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
#define DEVICE_ID_LEN	8  //�豸ID�ĳ���
#define CONTROL_DATA_LEN 8 //�������ݵĳ���
//Э���
typedef protcolBase* (*transObject)(protcolBase*);
class ProtcolPond {
public:
	ProtcolPond();
	map<uint16_t, transObject> m_protcolPond;//���ڱ���Э���ͼ
	void insertMap(uint16_t msgType, transObject returnObject);  //��Э�����ͼ��msgType:��Ϣ���ͣ���λ16��������
																 //returnObject�Ĺ����ǽ���Э����ౣ����Ϣָ��ת��Ϊ������Ϣ��Ӧ������ָ��
																 //�ú������ڸ���Ϣ�����ж���ľ�̬����
	transObject findInMap(uint16_t msgType, transObject pFunc);//��Э��ͼ��map���в�����Ϣ����ת��Ϊ��Ӧ����Ϣ����
};

//��Ϣ����
class msgBase :public protcolBase {
	//������������Ϣ�Ĺ�������
public:
	msgBase();
	char hostDeviceID[8] = { 0 };		//����ID
	char SlaveDeviceID[8] = { 0 };		//�ӻ�ID
	uint8_t DeviceType;				//�豸����
};


//-------------message-class-------------//

//������Ϣ
class HeartBeat :public msgBase {
public:
	HeartBeat();
	virtual bool packMsg(void);
	static protcolBase* returnObject(protcolBase* pBase) {
		return (HeartBeat*)pBase;
	}
};
//�����ע��
class SenderRegist :public msgBase {
public:
	virtual bool unpackMsg(void);
	static protcolBase* returnObject(protcolBase* pBase) {
		return (SenderRegist*)pBase;
	}
};

//��������
class SenderPair :public msgBase {
public:
	virtual bool unpackMsg(void);
	static protcolBase* returnObject(protcolBase* pBase) {
		return (SenderPair*)pBase;
	}
};

//�����ѯ��
class SenderInquiry :public msgBase {
public:
	virtual bool unpackMsg(void);
	static protcolBase* returnObject(protcolBase* pBase) {
		return (SenderInquiry*)pBase;
	}
};

//���ջ�ע��
class ReceiverRegist :public msgBase {
public:
	virtual bool unpackMsg();
	static protcolBase* returnObject(protcolBase* pBase) {
		return (ReceiverRegist*)pBase;
	}
};

//���ջ�ѯ��
class ReceiverInquiry :public msgBase {
public:
	virtual bool unpackMsg();
	static protcolBase* returnObject(protcolBase* pBase) {
		return (ReceiverInquiry*)pBase;
	}
};

//���ý��ջ�Ϊ�����״̬
class ReceiverNeedPair :public msgBase {
public:
	virtual bool unpackMsg();		 //��Ϣ���
	static protcolBase* returnObject(protcolBase* pBase) {
		return (ReceiverNeedPair*)pBase;
	}
};

//���ջ��յ���������ظ�
class ReceiverPairReturn :public msgBase {
public:
	virtual bool unpackMsg();
	static protcolBase* returnObject(protcolBase* pBase) {
		return (ReceiverPairReturn*)pBase;
	}
};

//��������Ϳ������ݵ�������
class SendDataToMaster :public msgBase {
public:
	string controlData;		 //��������
	virtual bool unpackMsg();
	static protcolBase* returnObject(protcolBase* pBase) {
		return (SendDataToMaster*)pBase;
	}
};

//�����������ظ�
class HeartBeatReturn :public msgBase {
public:
	HeartBeatReturn();
	virtual bool packMsg(void);
	static protcolBase* returnObject(protcolBase* pBase) {
		return  (HeartBeatReturn*)pBase;
	}
};

//����ע��ظ�
class SenderRigistReturn :public msgBase {
public:
	//SenderRigistReturn();
	virtual bool packMsg(void);
	static protcolBase* returnObject(protcolBase* pBase) {
		return (SenderRigistReturn*)pBase;
	}
};