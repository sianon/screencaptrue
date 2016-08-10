#include "res_singleton.h"

ResSingleton* ResSingleton::instance_ = nullptr;

ResSingleton::ResSingleton()
{
}

ResSingleton* ResSingleton::GetInstance()
{
	return instance_ ? instance_ : (instance_ = new ResSingleton());
}

void ResSingleton::DestroyInstance()
{
	if (instance_){
		delete instance_;
		instance_ = nullptr;
	}
}

//XmlManager* ResSingleton::GetXmlManager()
//{
//	return &xml_manager_;
//}

//IpManager* ResSingleton::GetIpManager()
//{
//	return &ip_manager_;
//}