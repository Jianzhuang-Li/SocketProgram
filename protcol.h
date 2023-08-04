#pragma once
#include "unility.h"
#include <stdint.h>
#include "client.h"
#include "protcolClass.h"

//协议
typedef struct _BLE_CMD {
		char* name;
		char* value;
	}SERVER_CMD;
	
protcolBase* ParseCommand(uint8_t* recvbuff, uint16_t recv_length); //解析指令
protcolBase* ProcessCommand(protcolBase* newMsg,CClient* pClient);	//执行指令
void		SendHeatBeatReturn(CClient* pClient);	//发送心跳
void		CrcCheck(uint8_t* data, uint16_t length,uint8_t crc_result[]); //CRC校验
uint16_t	AppendCrc(uint8_t* sendbuf); //添加CRC校验
bool		CompareCmdCRC(char* crc, uint8_t crc_result[]);	//CRF校验对比

extern ProtcolPond* protcolPond; //协议池