#pragma once
#include "stdint.h"
#include <string>
#include "threadManager.h"
using namespace std;
#define MAX_CONTENT_LENGTH	32

//Э�����
class protcolBase {
public:
	protcolBase();
	virtual bool setMsg(char* pMsg, size_t length) { return 0; };
	virtual void getMsgID(){};
	virtual bool packMsg(void) { return false; };
	virtual bool unpackMsg(void){ return false; };
	virtual int  GetLength() { return 0; };
	virtual bool pCrcCheck();
	virtual bool protcolFunc(){ return false; };		 //��Ϣ����
protected:
	bool compress_(unsigned int i, char* buf, size_t& len);		//ѹ��
	bool uncompress_(char* buf, size_t len, unsigned int i);	//��ѹ

protected:
	void pWriteInit();		//д���ʼ��
	bool pWriteCString(const char* str, unsigned int len);
	bool pWriteString(const string& str);
	bool pWriteDouble(double value, bool isNULL = false);
	bool pWriteInt32(int32_t i, bool isNULL = false);
	bool pWriteInt64(int64_t i, bool isNULL = false);
	bool pWriteShort(short i, bool isNULL = false);
	bool pWriteChar(char c, bool isNULL = false);
	bool pWriteUint32(uint32_t i, bool isNULL = false);
	bool pWriteUint16(uint16_t i, bool isNULL = false);
	bool pWriteUint8(uint8_t i, bool isNULL = false);
	bool pWriteHeader();
	bool pWriteCRC();
	size_t GetSize() const;
	void Clear();

protected:
	size_t pReadInit();	//������ʼ��
	bool pReadLengthWithoutOffset(size_t& headlen, size_t& outlen);
	bool pReadString(string* str, size_t maxlen, size_t& outlen);
	bool pReadCString(char* str, size_t strlen, size_t& len);
	bool pReadInt32(int32_t& i);
	bool pReadInt64(int64_t& i);
	bool pReadUint32(uint32_t& i);
	bool pReadUint16(uint16_t& i);
	bool pReadUint8(uint8_t& i);
	bool pReadDouble(double& i);
	bool pReadShort(short& i);
	bool pReadChar(char& c);
	bool pReadCRC(uint8_t crc_result[]);
	bool isEnd();
	const char* GetData();
	size_t ReadAll(char* szbuff, size_t ilen) const;
public:
	uint16_t msg_id;		//��ϢID
	string msg_data;		//��Ϣ��
	//string return_msg_data;	//��Ҫ�ظ�����Ϣ
protected:
	const char* ptr;		//��Ϣ����ͷָ��
	size_t len;				//��Ϣ����ʵ�ʳ���
	size_t lenInside;		//��Ϣ���ж�������Ϣ����
	const char* cur;				//��Ϣ����ָ��
	uint8_t crc_result[2]={0};		//CRCУ��
};