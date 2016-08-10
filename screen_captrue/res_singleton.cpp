#include "res_singleton.h"

ResSingleton* ResSingleton::instance_ = nullptr;

ResSingleton::ResSingleton()
{
	syscfg_.LoadFile();
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