#ifndef __RES_SINGLETON_H__
#define __RES_SINGLETON_H__

#include "syscfg.h"

class ResSingleton
{
private:
	ResSingleton();

public:
	static ResSingleton* GetInstance();
	static void DestroyInstance();

	Syscfg * GetSyscfg() { return &syscfg_; }

private:
	static ResSingleton* instance_;
	Syscfg syscfg_;
};

#endif // !__RES_SINGLETON_H__