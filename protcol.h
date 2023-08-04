#pragma once
#include "unility.h"
#include <stdint.h>
#include "client.h"
#include "protcolClass.h"

//Э��
typedef struct _BLE_CMD {
		char* name;
		char* value;
	}SERVER_CMD;
	
protcolBase* ParseCommand(uint8_t* recvbuff, uint16_t recv_length); //����ָ��
protcolBase* ProcessCommand(protcolBase* newMsg,CClient* pClient);	//ִ��ָ��
void		SendHeatBeatReturn(CClient* pClient);	//��������
void		CrcCheck(uint8_t* data, uint16_t length,uint8_t crc_result[]); //CRCУ��
uint16_t	AppendCrc(uint8_t* sendbuf); //���CRCУ��
bool		CompareCmdCRC(char* crc, uint8_t crc_result[]);	//CRFУ��Ա�

extern ProtcolPond* protcolPond; //Э���