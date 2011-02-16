#pragma once

#include "iisingleton.hpp"

#include "GUICommon.h"

#include <string>
#include <list>

struct lua_State;

using namespace izayoi;

class LuaSystemListener
{
	public:
		virtual void onError(std::string error)=0;//当脚本执行错误时
		virtual void onComplete()=0;//当全部脚本结束时
};

class LuaSystem:public IISingleton<LuaSystem> 
{
public:
	LuaSystem(void);
	~LuaSystem(void);

	void onFrameUpdate();

	void runScript(const std::string script);

	std::string luaCallString(const std::string Szcode);

	void registerListener(LuaSystemListener* listener);

	void (*LuaBreakupFun)();//阻塞函数指针

private:
	lua_State* L;

	std::list<std::string> mScriptBuffer;

	LuaSystemListener* mListener;

};

