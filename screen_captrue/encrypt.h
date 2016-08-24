#ifndef __ENCRYPT_H__
#define __ENCRYPT_H__

#include "stdafx.h"

#pragma comment(lib, "encrypt.lib")

#define  DLL_EXPORT __declspec(dllexport)

using namespace std;

const unsigned short encrypt_value = 16;
const char szchar[25] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N'
, 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y' };

class DLL_EXPORT Encrypt
{
public:
	Encrypt(){}
	~Encrypt(){}

	int GetStrIndex(char c);
	bool DoEncrypt(LPCTSTR enchar);
	wstring UnEncrypt();
private:
	char szcharbuf[256];
	unsigned char* pstrbuf;
	TCHAR sznewbuf[128];
	wstring szencryptstr;
	int len;
};

#endif // !__ENCRYPT_H__