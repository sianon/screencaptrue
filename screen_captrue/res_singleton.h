#ifndef __RES_SINGLETON_H__
#define __RES_SINGLETON_H__

#include "ivga_engine.h"

class ResSingleton
{
private:
	ResSingleton();

public:
	static ResSingleton* GetInstance();
	static void DestroyInstance();

	//Syscfg * GetSyscfg() { return &syscfg_; }
	IvgaEngine* GetIvgaEngine(){ return &ivga_engine_; }
	//Syscfg * GetSyscfg();

private:
	static ResSingleton* instance_;
	//Syscfg syscfg_;
	IvgaEngine ivga_engine_;
};

#endif // !__RES_SINGLETON_H__