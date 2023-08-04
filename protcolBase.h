#pragma once
#include "stdint.h"
#include <string>
#include "threadManager.h"
using namespace std;
#define MAX_CONTENT_LENGTH	32

//协议基类
class protcolBase {
public:
	protcolBase();
	virtual bool setMsg(char* pMsg, size_t length) { return 0; };
	virtual void getMsgID(){};
	virtual bool packMsg(void) { return false; };
	virtual bool unpackMsg(void){ return false; };
	virtual int  GetLength() { return 0; };
	virtual bool pCrcCheck();
	virtual bool protcolFunc(){ return false; };		 //消息函数
protected:
	bool compress_(unsigned int i, char* buf, size_t& len);		//压缩
	bool uncompress_(char* buf, size_t len, unsigned int i);	//解压

protected:
	void pWriteInit();		//写入初始化
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
	size_t pReadInit();	//读出初始化
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
	uint16_t msg_id;		//消息ID
	string msg_data;		//消息包
	//string return_msg_data;	//需要回复的消息
protected:
	const char* ptr;		//消息包开头指针
	size_t len;				//消息包的实际长度
	size_t lenInside;		//消息包中读出的消息长度
	const char* cur;				//消息解析指针
	uint8_t crc_result[2]={0};		//CRC校验
};