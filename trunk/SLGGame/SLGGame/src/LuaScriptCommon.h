#pragma once

extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#include "LuaSystem.h"
#include "DataLibrary.h"
#include "AudioSystem.h"

static int SetInt(lua_State* L)
{
	std::string id(luaL_checkstring(L, 1));
	int val = luaL_checknumber(L, 2);
	std::string contextpath = LuaSystem::getSingleton().getContext();
	DataLibrary::getSingleton().setData(contextpath + std::string("/")+ id, val);
	return 0;
}

static int GetInt(lua_State* L)
{
	std::string id(luaL_checkstring(L, 1));
	std::string contextpath = LuaSystem::getSingleton().getContext();
	int val;
	DataLibrary::getSingleton().getData(contextpath + std::string("/")+ id, val);
	lua_pushnumber(L,val);
	return 1;
}

static int GetString(lua_State* L)
{
	std::string id(luaL_checkstring(L, 1));
	std::string contextpath = LuaSystem::getSingleton().getContext();
	std::string val;
	DataLibrary::getSingleton().getData(contextpath + std::string("/")+ id, val);
	lua_pushstring(L,val.c_str());
	return 1;
}

static int SetString(lua_State* L)
{
	std::string id(luaL_checkstring(L, 1));
	std::string val(luaL_checkstring(L, 2));
	std::string contextpath = LuaSystem::getSingleton().getContext();
	DataLibrary::getSingleton().setData(contextpath + std::string("/")+ id, val);
	return 0;
}

static int GetSquadCoord(lua_State* L)
{
	std::string squad(luaL_checkstring(L, 1));
	int x = -1,y = -1;
	BattleSquad* battlesquad = BattleSquadManager::getSingleton().getBattleSquad(squad);
	if(battlesquad!=NULL)
	{
		battlesquad->getCrood(&x,&y);
	}
	lua_pushnumber(L,x);
	lua_pushnumber(L,y);
	return 2;
}

static int GetTeamSquadLeft(lua_State* L)
{
	int team = luaL_checknumber(L,1);
	int n = 0;
	n = BattleSquadManager::getSingleton().getTeamSquadLeft(team);
	lua_pushnumber(L,n);
	return 1;
}


static int PlayMusic(lua_State* L)
{
	std::string music(luaL_checkstring(L, 1));
	DataLibrary::getSingleton().setData("GameData/StoryData/MusicName", music);
	AudioSystem::getSingleton().playStream(music,true,2000);
	return 0;
}

#include "CameraContral.h"
#include "Terrain.h"
static int SetCamera(lua_State* L)
{
	int x =  luaL_checknumber(L,1);
	int y =  luaL_checknumber(L,2);
	float xx,yy;
	Terrain::getSingleton().getWorldCoords(x,y,xx,yy);
	CameraContral::getSingleton().moveCameraTo(xx,yy);
	return 0;
}

static int GetSquadIdFromPath(lua_State* L)
{
	std::string squadpath(luaL_checkstring(L, 1));
	std::string::size_type i = squadpath.rfind('/');
	std::string squad = squadpath.substr(i + 1,squadpath.size() - i -1);
	lua_pushstring(L,squad.c_str());
	return 1;
}

static int GetRand(lua_State* L)
{
	int min = luaL_checknumber(L,1);
	int max = luaL_checknumber(L,2);
	int re = min + (rand() % (max- min));
	lua_pushnumber(L,re);
	return 1;
}

#include "StoryCutScene.h"
static int Story(lua_State* L)
{
	std::string script(luaL_checkstring(L, 1));
	StoryCutScene* scs = new StoryCutScene(script);
	BattleSquadManager::getSingleton().setCutScene(scs);
	return 0;
}

#include <boost/format.hpp>
static int CopySquadData(lua_State* L)
{
	BattleSquadManager::BattleSquadIte ite;
	BattleSquadManager* bsm = BattleSquadManager::getSingletonPtr();
	std::string path;
	int num = 0;
	for(ite = bsm->mSquadList.begin(); ite != bsm->mSquadList.end(); ite++)
	{
		if((*ite)->getTeam() != 1)
			continue;
		path = str(boost::format("GameData/StoryData/SquadData/EnableSquad/%1%/UnitNumber")%(*ite)->getId());
		DataLibrary* datalib = DataLibrary::getSingletonPtr();
		if(datalib->getData(path,num,true))
		{
			num = (*ite)->getUnitRealNum() + 0.5f * (num -  (*ite)->getUnitRealNum());
			datalib->setData(path,num);
			path = str(boost::format("%1%/Morale")%(*ite)->getPath());
			datalib->getData(path,num);
			path = str(boost::format("GameData/StoryData/SquadData/EnableSquad/%1%/Morale")%(*ite)->getId());
			datalib->setData(path,num);
		}
	}
	return 0;
}

static const struct luaL_Reg ScriptCommonLib[] =
{
	{"SetInt",SetInt},
	{"GetInt",GetInt},
	{"GetString",GetString},
	{"SetString",SetString},
	{"GetSquadCoord",GetSquadCoord},
	{"GetTeamSquadLeft",GetTeamSquadLeft},
	{"PlayMusic",PlayMusic},
	{"SetCamera",SetCamera},
	{"GetSquadIdFromPath",GetSquadIdFromPath},
	{"GetRand",GetRand},
	{"Story",Story},
	{"CopySquadData",CopySquadData},
	{NULL,NULL}
};