#include "protcolClass.h"
//��Ϣ����
//�ͻ��˷���������
#define HEART_BEAT					0xFF11
#define SENDER_REGIST				0xFF12
#define SENDER_INQUIRY				0xFF13
#define SENDER_PAIR					0xFF14
#define RECEIVER_REGIST				0xFF15
#define RECEIVER_INQUIRY			0xFF16
#define RECEIVER_NEED_PAIR			0xFF17
#define RECEIVER_PAIR_RETURN		0xFF18
#define SEND_DATA_TO_MASTER			0xFF31
//�����������ͻ���
#define HEART_BEAT_RETURN			0xFF01
#define SENDER_REGIST_RETURN		0xFF02
#define SENDER_INQUIRY_RETURN		0xFF03
#define SENDER_PAIR_RETURN			0xFF04
#define RECEIVER_REGIST_RETURN		0xFF05
#define RECEIVER_INQUIRY_RETURN		0xFF06
#define RECEIVER_INQUIRY_PAIR		0xFF07
#define RECEIVER_PAIR_SUCCESS		0xFF08
#define DATA_DEND_TO_SLAVE			0xFF21

//Э���
typedef protcolBase* (*transObject)(protcolBase*);//����һ������ָ������
ProtcolPond::ProtcolPond() {
	//��Э����д��Map
	insertMap(HEART_BEAT,HeartBeat::returnObject);//����
	insertMap(SENDER_REGIST, SenderRegist::returnObject);//�����ע��
	insertMap(SENDER_PAIR, SenderPair::returnObject);//��������
	insertMap(SENDER_INQUIRY, SenderInquiry::returnObject);//�����ѯ��
	insertMap(RECEIVER_REGIST, ReceiverRegist::returnObject);//���ջ�ע��
	insertMap(RECEIVER_INQUIRY, ReceiverInquiry::returnObject);//���ջ�ѯ��
	insertMap(RECEIVER_NEED_PAIR, ReceiverNeedPair::returnObject);//���ջ���Ϊ�����״̬
	insertMap(RECEIVER_PAIR_RETURN, ReceiverPairReturn::returnObject);//���ջ��յ���������ظ�
	insertMap(SEND_DATA_TO_MASTER, SendDataToMaster::returnObject);//������������ݵ�������
	insertMap(HEART_BEAT_RETURN, HeartBeatReturn::returnObject);//�����������ظ�
}           

void ProtcolPond::insertMap(uint16_t msgType, transObject returnMsg)
{
	m_protcolPond.insert(make_pair(msgType, returnMsg));
}

transObject ProtcolPond::findInMap(uint16_t msgType,transObject pFunc)
{	
	printf("find:%lld\n", m_protcolPond.count(0xFF11));
	/*if (m_protcolPond.count(msgType)) {
		pFunc = m_protcolPond[msgType];
	}*/
	//����
	if (m_protcolPond.count(0xFF11)) {
		pFunc = m_protcolPond[0xFF11];
	}
	if (pFunc == NULL) {
		printf("error:not find\n");
	}
	return pFunc;
}
//��Ϣ����
msgBase::msgBase(){
	DeviceType = '\0';
}
//-------Message-class------//

//����
HeartBeat::HeartBeat()
{
	msg_id = HEART_BEAT;
}
bool HeartBeat::packMsg(void) {
	 pWriteUint32(msg_id);
	 pWriteHeader();
	 pWriteCRC();
	 return true;
}

//�����ظ�
HeartBeatReturn::HeartBeatReturn()
{
	msg_id = HEART_BEAT_RETURN;
}
bool HeartBeatReturn::packMsg() {
	pWriteUint32(msg_id);
	pWriteHeader();
	pWriteCRC();
	return false;
}

#define ReadDeviceType	pReadUint8(DeviceType);
#define ReadDeviceID	size_t outlen;\
						if (pReadCString(SlaveDeviceID, DEVICE_ID_LEN, outlen))	return false;\
						if (outlen != DEVICE_ID_LEN)	return false;\
						/*if (pReadCString(SlaveDeviceID, DEVICE_ID_LEN, outlen))	return false;\
						if (outlen != DEVICE_ID_LEN)	return false;*/

#define ReadThisDeviceID size_t outlen;\
						if (pReadCString(SlaveDeviceID, DEVICE_ID_LEN, outlen))	return false;\
						if (outlen != DEVICE_ID_LEN)	return false;

#define ReadControlData size_t outlen;\
						char* ctr= &controlData[0];\
						if (pReadCString(ctr,CONTROL_DATA_LEN, outlen))	return false;\
						if (outlen != CONTROL_DATA_LEN)	return false;

#define ReadDeviceTypeAndID ReadDeviceType;\
							ReadDeviceID;


bool SenderRegist::unpackMsg() {
	ReadDeviceTypeAndID;
	return true;
}

bool SenderPair::unpackMsg() {
	ReadDeviceTypeAndID;
	return true;
}

bool SenderInquiry::unpackMsg() {
	ReadDeviceTypeAndID;
	return true;
}

bool ReceiverRegist::unpackMsg() {
	ReadDeviceTypeAndID;
	return true;
}

bool ReceiverInquiry::unpackMsg() {
	ReadDeviceTypeAndID;
	return true;
}

bool ReceiverNeedPair::unpackMsg() {
	ReadDeviceTypeAndID;
	return true;
}

bool ReceiverPairReturn::unpackMsg() {
	ReadDeviceTypeAndID;
	return true;
}

bool SendDataToMaster::unpackMsg() {
	ReadDeviceTypeAndID;
	//ReadControlData;
	return true;
}

bool SenderRigistReturn::packMsg() {
	return 1;
}