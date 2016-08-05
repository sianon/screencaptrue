#ifndef __RES_SINGLETON_H__
#define __RES_SINGLETON_H__

#include "xml_manager.h"

class IpManager;

class ResSingleton
{
private:
	ResSingleton();

public:
	static ResSingleton* GetInstance();
	static void DestroyInstance();

	XmlManager* GetXmlManager();
	IpManager* GetIpManager() { return ip_manager_; }

private:
	static ResSingleton* instance_;
	XmlManager xml_manager_;
	IpManager* ip_manager_;
};

#endif // !__RES_SINGLETON_H__