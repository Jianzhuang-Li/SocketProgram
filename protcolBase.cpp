#include "protcolBase.h"
#include <winsock.h>
#include <assert.h>
#include <math.h>
#define PACK_HEADER		  2		//��ͷ����		
#define PACK_HEADER_LEN   4		//��Ϣͷ��ռ���ȣ���ͷռ4�ֽڣ���Ϣ����ռ���ֽڣ�
#define COMPRESS_BUFF_LEN 5		//����ѹ����ռ�õ������
#define PACK_CRC_LEN      2		//CRCУ��λ�ĳ���

void CrcCheck(uint8_t* data, size_t length, uint8_t crc_result[]);	//CRCУ�麯��

protcolBase::protcolBase()
{
	char m_char[PACK_HEADER_LEN] = { 0 };
	msg_data.append(m_char, PACK_HEADER_LEN);	//Ϊ��ͷռλ,����Ϊ��ʼ��
	ptr = msg_data.c_str();						//���ݰ��׵�ַ
	msg_id = 0;									//��ϢID
	len = 0;									//��Ϣ����
	lenInside = 0;								//����Ϣ���ж����ĳ���
	cur = msg_data.c_str();						//��Ϣ����ָ��
}
//CRC check
bool protcolBase::pCrcCheck() {
	return 0;
}
//ѹ��
bool protcolBase::compress_(unsigned int i, char* buf, size_t& len) {
	len = 0;
	for (int a = (COMPRESS_BUFF_LEN - 1); a >= 0; a--) {
		char c;
		c = i >> (a * 7) & 0x7f;
		if (c == 0x00 && len == 0) continue;
		if (a == 0) c &= 0x7f;
		else c |= 0x80;
		buf[len] = c;
		len++;
	}
	if (len == 0) {
		len++;
		buf[0] = 0;
	}
	return true;
}

//��ѹ
bool protcolBase::uncompress_(char* buf, size_t len, unsigned int i) {
	i = 0;
	for (int index = 0; index < (int)len; index++) {
		char c = *(buf + index);
		i = i << 7;
		c &= 0x7f;
		i |= c;
	}
	return true;
}

//-------------------protcolWriteStream--------------------//
void protcolBase::pWriteInit() {
	msg_data.clear();
	char str[PACK_HEADER_LEN] = { 'F','F','F','F' };
	msg_data.append(str, sizeof(str));//��Ϊ��Ϣͷռ��4���ֽ�
}

//д���ַ�����
bool protcolBase::pWriteCString(const char* str, unsigned int len) {
	char buf[PACK_HEADER_LEN];
	size_t buflen;
	compress_(len, buf, buflen);
	msg_data.append(buf, sizeof(char) * buflen);
	msg_data.append(str, len);
	return true;
}

//д���ַ���
bool protcolBase::pWriteString(const string& str) {
	return pWriteCString(str.c_str(), str.length());
}

//д��int32_t
bool protcolBase::pWriteInt32(int32_t i, bool isNULL) {
	int32_t i2 = 0;
	if (isNULL == false) i2 = htonl(i);//ת��Ϊ�����ֽ�˳��
	msg_data.append((char*)&i2, sizeof(i2));
	return true;
}

//д��int64_t
bool protcolBase::pWriteInt64(int64_t i, bool isNULL) {
	char int64str[128];
	if (isNULL == false) {
#ifndef _WIN32
		sprintf(int64str, "%ld", i);
#else
		sprintf_s(int64str, "%lld", i);
#endif
		pWriteCString(int64str, strlen(int64str));
	}
	else pWriteCString(int64str, 0);
	return true;
}

//д��short
bool protcolBase::pWriteShort(short i, bool isNULL) {
	short i2 = 0;
	if (isNULL == false)i2 = htons(i);
	msg_data.append((char*)&i2, sizeof(i2));
	return true;
}

//д��char
bool protcolBase::pWriteChar(char c, bool isNULL) {
	char c2 = 0;
	if (isNULL == false)
		c2 = c;
	msg_data += c2;
	return true;
}

//д��uint32_t
bool protcolBase::pWriteUint32(uint32_t i,bool isNULL) {
	uint32_t i_ = htonl(i);
	if (isNULL == false) {
		msg_data.append((char*) & i_,sizeof(i_));
	}
	return true;
}
//д��uint16_t
bool protcolBase::pWriteUint16(uint16_t i, bool isNULL)
{
	uint16_t i_ = htons(i);
	if (isNULL == false) {
		msg_data.append((char*)&i_, sizeof(i_));
	}
	return false;
}
//д��uint8_t
bool protcolBase::pWriteUint8(uint8_t i, bool isNULL)
{
	if (isNULL == false) {
		msg_data.push_back(i);
	}
	return true;
}

//д��˫������
bool protcolBase::pWriteDouble(double i, bool isNULL) {
	char doublestr[128];
	if (isNULL == false) {
		sprintf_s(doublestr, "%lf", i);
		pWriteCString(doublestr, strlen(doublestr));
	}
	else
		pWriteCString(doublestr, 0);
	return true;
}

//��ȡ���ݰ�
const char* protcolBase::GetData(){
	return msg_data.data();
}

//��ȡ���ݰ�����
size_t protcolBase::GetSize() const {
	return msg_data.length();
}

//��Ӱ�ͷ�ͳ���
bool protcolBase::pWriteHeader() {
	uint8_t header[] = { 0xFA,0xAF };
	char* ptr =&msg_data[0];
	memcpy(ptr, header, sizeof(header));
	ptr = &msg_data[2];
	uint16_t ulen = htons((msg_data.length())+ PACK_CRC_LEN);
	memcpy(ptr, &ulen, sizeof(ulen));
	return true;
}

//���CRCУ��
bool protcolBase::pWriteCRC() {
	uint8_t crc_result[2];
	size_t datalen = msg_data.length();
	CrcCheck((uint8_t*)msg_data.c_str(), datalen, crc_result);
	pWriteUint8(crc_result[0]);
	pWriteUint8(crc_result[1]);
	return true;
}

void protcolBase::Clear() {
	msg_data.clear();
	char str[PACK_HEADER_LEN]={0};
	msg_data.append(str, sizeof(str));
}

//--------------protcolReadStream------------------//
//�������ݳ�ʼ��
size_t protcolBase::pReadInit() {
	ptr = msg_data.c_str();
	len = msg_data.length();
	cur = ptr + PACK_HEADER;
	uint16_t lenInside_ = 0;
	memcpy(&lenInside_, cur, sizeof(lenInside_));
	lenInside = ntohs(lenInside);
	cur = ptr+ PACK_HEADER_LEN; //������ָ�붨λ�����ݿ�ͷ��
	pReadUint16(msg_id);	//msg_id�������ȶ����������ʱ�����ٶ�msg_id
	return lenInside;
}

bool protcolBase::pReadLengthWithoutOffset(size_t& headlen, size_t& outlen) {
	headlen = 0;
	const char* temp = cur;
	char buf[COMPRESS_BUFF_LEN]={0};
	for (size_t i = 0; i < COMPRESS_BUFF_LEN; i++) {
		memcpy(buf + i, temp, sizeof(char));
		temp++;
		headlen++;
		if ((buf[i] & 0x80) == 0x00)break;
	}
	if (cur + headlen > ptr + len)
		return false;
	unsigned int value=0;
	uncompress_(buf, headlen, value);
	outlen = value;
	return true;
}

bool protcolBase::pReadString(string* str, size_t maxlen, size_t& outlen) {
	size_t headlen;
	size_t fieldlen;
	if (!pReadLengthWithoutOffset(headlen, fieldlen)) return false;
	if (maxlen != 0 && fieldlen > maxlen) return false;
	cur += headlen;
	if (cur + fieldlen > ptr + len) {
		outlen = 0;
		return false;
	}
	str->assign(cur, fieldlen);
	outlen = fieldlen;
	cur += outlen;
	return true;
}

bool protcolBase::pReadCString(char* str, size_t strlen, size_t& outlen) {
	size_t fieldlen;
	size_t headlen;
	if (!pReadLengthWithoutOffset(headlen, fieldlen)) return false;
	if (fieldlen > strlen) return false;
	cur += headlen;
	if (cur + fieldlen > ptr + len) {
		outlen = 0;
		return false;
	}
	memcpy(str, cur, fieldlen);
	outlen = fieldlen;
	cur += outlen;
	return true;
}

bool protcolBase::pReadInt32(int32_t& i) {
	const int value_size = sizeof(int32_t);
	if (cur + value_size > ptr + len) return false;
	memcpy(&i, cur, value_size);
	i = ntohl(i);
	cur += value_size;
	return true;
}

bool protcolBase::pReadInt64(int64_t& i) {
	char int64str[128];
	size_t length;
	if (!pReadCString(int64str, 128, length)) return false;
	i = atoll(int64str);
	return true;
}

bool protcolBase::pReadDouble(double& i) {
	char doublestr[128];
	size_t length;
	if (!pReadCString(doublestr, 128, length)) return false;
	i = atof(doublestr);
	return true;
}

bool protcolBase::pReadShort(short& i) {
	const int value_size = sizeof(short);
	if (cur + value_size > ptr + len) return false;
	memcpy(&i, cur, value_size);
	i = ntohs(i);
	cur += value_size;
	return true;
}

bool protcolBase::pReadChar(char& c) {
	const int value_size = sizeof(char);
	if (cur + value_size > ptr + len) return false;
	memcpy(&c, cur, value_size);
	cur += value_size;
	return true;
}

/*����д��ĸת����Сд��ĸ*/
int to_lower(int c)
{
	if (c >= 'A' && c <= 'Z')
	{
		return c + 'a' - 'A';
	}
	else
	{
		return c;
	}
}
//��ʮ�����Ƶ��ַ���ת��������  
int htoi(char s[])
{
	int i = 0;
	int n = 0;
	if (s[0] == '0' && (s[1] == 'x' || s[1] == 'X'))
	{
		i = 2;
	}
	else
	{
		i = 0;
	}
	for (; (s[i] >= '0' && s[i] <= '9') || (s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'Z'); ++i)
	{
		if (tolower(s[i]) > '9')
		{
			n = 16 * n + (10 + to_lower(s[i]) - 'a');
		}
		else
		{
			n = 16 * n + (to_lower(s[i]) - '0');
		}
	}
	return n;
}
bool protcolBase::pReadUint32(uint32_t& i) {
	const int value_size = sizeof(uint32_t);
	if (cur + value_size > ptr + len) return false;
	memcpy(&i, cur, value_size);
	i = ntohl(i);
	cur += value_size;
	return true;
}
bool protcolBase::pReadUint16(uint16_t& i)
{
	const int value_size = sizeof(uint16_t);
	if (cur + value_size > ptr + len) return false;
	memcpy(&i, cur, value_size);
	i = ntohs(i);
	cur += value_size;
	return true;
}
bool protcolBase::pReadUint8(uint8_t& i)
{
	const int value_size = sizeof(char);
	if (cur + value_size > ptr + len) return false;
	memcpy(&i, cur, value_size);
	cur += value_size;
	return true;
}

bool protcolBase::pReadCRC(uint8_t crc_result[]) {
	const int value_size = sizeof(crc_result);
	if (cur + value_size > ptr + len) return false;
	memcpy(crc_result, cur, value_size);
	cur += value_size;
	return true;
}

bool protcolBase::isEnd() {
	assert(cur > ptr + len);
	return cur == ptr + len;
}

size_t protcolBase::ReadAll(char* szbuff,size_t ilen) const {
	size_t iRealLen = min(ilen, len);
	memcpy(szbuff, ptr, iRealLen);
	return iRealLen;
}