#include "LuaSystem.h"

#include <Ogre.h>

#include "LuaFun.cpp"

extern "C"
{

	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>

}

LuaSystem::LuaSystem(void)
{
	L=lua_open();
	luaL_openlibs(L);
	LuaBreakupFun=NULL;
	mListener=NULL;
	mContextStack.push_back("\0");
	luaL_register(L, "GUI",GUILib);
}

LuaSystem::~LuaSystem(void)
{
	lua_close(L);
}

std::string LuaSystem::luaCallString(const std::string Szcode)
{
	luaL_loadstring(L,Szcode.c_str());
	int error=lua_pcall(L, 0 , 0, 0);
	if (error)
	{
		const char* error = lua_tostring(L, -1);//��ӡ������
		lua_pop(L, 1);
		return std::string(error);
	}
	return std::string();
}


void LuaSystem::runScript( const std::string script )
{
	mScriptBuffer.clear();

	std::stringstream ss(script);
	std::string sub_str;

	while(std::getline(ss,sub_str,';'))//�Էֺ��з����
	{
		mScriptBuffer.push_back(sub_str);
	}
}

void LuaSystem::onFrameUpdate()
{
	if (!mScriptBuffer.empty())
	{

		if (LuaBreakupFun==NULL)
		{
			std::string lua=(*mScriptBuffer.begin());
			mScriptBuffer.erase(mScriptBuffer.begin());
			
			std::string ret=luaCallString(lua);
			if (ret!="")//�����������
			{
				if (mListener!=NULL)
				{
					mScriptBuffer.clear();
					LuaBreakupFun=NULL;
					mListener->onError(ret);
				}

				return;
			}

			if (mScriptBuffer.empty())//��������һ�����
			{
				if (mListener!=NULL)
				{
					LuaBreakupFun=NULL;
					mListener->onComplete();
				}
			}
		}
		else//��������ʱ
		{
			LuaBreakupFun();
		}
	}
}

void LuaSystem::registerListener( LuaSystemListener* listener )
{
	mListener=listener;
}

void LuaSystem::registerCLib( const std::string libName,const luaL_Reg* lib )
{
	luaL_register(L, libName.c_str(),lib);
}

void LuaSystem::runScriptFromFile( const std::string& filename,int lineNum)
{
	mScriptBuffer.clear();

	Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::getSingleton().openResource(filename, "General", true);

	runScript(stream->getAsString());

	//����ָ����������
	
	if (lineNum!=0)
	{
		for (int i=0;i<lineNum;i++ )
		{
			mScriptBuffer.erase(mScriptBuffer.begin());
		}
	}
}


std::string LuaSystem::getContext()
{
	return mContextStack.back();
}

std::string LuaSystem::getFileName()
{
	return mFileNameStack.back();
}
/*
bool LuaSystem::executeFile(std::string filename, std::string context)
{
	if(context == "\0")
		return false;
	mContextStack.push_back(context);
	mFileNameStack.push_back(filename);
	Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::getSingleton().openResource(filename, "General", true);
	int result;
	result = luaL_loadstring(L,stream->getAsString().c_str());
	if(!result)
		result = lua_pcall(L,0 ,0, 0);
	mFileNameStack.pop_back();
	mContextStack.pop_back();
	return !result;
}
*/
bool LuaSystem::executeFunction(std::string filename, std::string funcname, std::string context)
{
	if(context == "\0")
		return false;
	mContextStack.push_back(context);
	mFileNameStack.push_back(filename);
	Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::getSingleton().openResource(filename, "General", true);
	int result;
	result = luaL_loadstring(L,stream->getAsString().c_str());
	if(!result)
	{
		result = lua_pcall(L,0 ,0, 0);
		if(!result)
		{
			lua_getglobal(L, funcname.c_str());
			result = lua_pcall(L,0 ,0, 0);
		}
	}
	mFileNameStack.pop_back();
	mContextStack.pop_back();
	return !result;
}