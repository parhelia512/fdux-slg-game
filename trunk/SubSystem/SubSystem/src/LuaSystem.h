#pragma once

#include "iisingleton.hpp"

#include "GUICommon.h"

#include <string>
#include <list>
#include <vector>

struct lua_State;
struct luaL_Reg;

using namespace izayoi;

class LuaSystemListener
{
	public:
		virtual void onError(std::string error)=0;//���ű�ִ�д���ʱ
		virtual void onComplete()=0;//��ȫ���ű�����ʱ
};

class LuaSystem:public IISingleton<LuaSystem> 
{
public:
	LuaSystem(void);
	~LuaSystem(void);

	void onFrameUpdate();

	void runScriptFromFile(const std::string& filename,int lineNum);

	void runScript(const std::string script);

	std::string luaCallString(const std::string Szcode);

	void registerCLib(const std::string libName,const luaL_Reg* lib);

	void registerListener(LuaSystemListener* listener);

	void (*LuaBreakupFun)();//��������ָ��


	std::string getContext();
	std::string getFileName();
	//bool executeFile(std::string filename, std::string context);
	bool executeFunction(std::string filename, std::string funcname, std::string context);

private:
	lua_State* L;

	std::list<std::string> mScriptBuffer;

	LuaSystemListener* mListener;

	std::vector<std::string> mContextStack;
	std::vector<std::string> mFileNameStack;
};

