#ifndef __CryptKeyUtils_H_20131030_
#define __CryptKeyUtils_H_20131030_

///*
//	功能：加密或解密一段数据
//	参数：
//		szPud	   - 产品标识，录播系统为："KHR"
//		pIn	     - 输入数据
//		nIn  	   - 输入数据长度
//    pOut     - 输出数据缓冲
//    nOut 	   - 输出数据缓冲长度
//    bEncrypt - 加密或解密
//	返回：成功返回输出数据长度, 失败返回0
// */
//
//unsigned Crypt(const char *szPud, unsigned char *pIn, unsigned nIn, unsigned char *pOut, unsigned nOut, bool bEncrypt);
//
///*
//	功能：授权码检测
//	参数：
//		szPud	- 产品标识，录播系统为："KHR"
//		pBuf	- 识别码（机器码，板卡ID等等）
//		nLen	- 识别码长度(字符串不包括结束符0,使用strlen获取长度即可)
//		szKey	- 注册码
//	返回：检测成功返回true,失败返回false 
//	示例：CryptKeyCheck("KHR", "111", 3, "KHR-C0D6-7127-4E7F3A21");
// */
//
//bool CryptKeyCheck(const char *szPud, const void *pBuf, int nLen, const char *szKey);
//
///*
//	功能：获取机器标识
//	参数：
//		szID	- 机器标识码
//	返回：返回机器标识码实际长度，失败返回0
// */
//
//int GetSysID(char szID[64]);
//
///*
//	功能：加密狗检测
//	参数：
//		szPud	- 产品标识，录播系统为："KHR"
//	返回：成功返回TRUE，失败返回FALSE
// */
//
//bool DogCheck(const char *szPud);

#endif //__CryptKeyUtils_H_20131030_