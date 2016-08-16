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

//Syscfg * ResSingleton::GetSyscfg()
//{
//	if (!syscfg_.GetIsLoad())
//		syscfg_.LoadFile();
//	return &syscfg_;
//}
