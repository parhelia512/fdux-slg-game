#pragma once

#include <ticpp.h>
#include <string>
#include "Modifier.h"


class SWeaponManager
{
public:
	SWeaponManager();
	~SWeaponManager();

	bool LoadMod(std::wstring modName,std::wstring langName, bool editorMode);
	bool LoadLang(std::wstring langName);

	//编辑器用函数
	bool SaveData();
	bool SaveLang();

	int GetNum(); //获取总数
	std::wstring GetID(int index);
	std::wstring GetName(std::wstring id);
	std::wstring GetDescription (std::wstring id);
	int GetAttr(std::wstring id,BasicAttr attrType);
	std::wstring GetScriptName(std::wstring id);
	int GetSWeaponType(std::wstring id);
	//int GetRange(std::wstring id); refactor to GetMaxRange(std::wstring id)
	//std::wstring GetModuleName(std::wstring id); refactor to GetMeshName(std::wstring id)
	//std::wstring GetIdleAnimName(std::wstring id); refactor to GetMatName(std::wstring id)
	//std::wstring GetActAnimName(std::wstring id); refactor to GetAniGroup(std::wstring id)
	int GetMaxRange(std::wstring id);
	int GetMinRange(std::wstring id);
	std::wstring GetMeshName(std::wstring id);
	std::wstring GetMatName(std::wstring id);
	std::wstring GetAniGroup(std::wstring id);
	int GetValue(std::wstring id);

	void AddSWeapon();
	void DelSWeapon(std::wstring id);

	bool SetID(std::wstring oldid, std::wstring id);
	bool SetName(std::wstring id, std::wstring name);
	bool SetDescription(std::wstring id, std::wstring descripition);
	bool SetAttr(std::wstring id, BasicAttr attrType, int attr);
	bool SetScriptName(std::wstring id, std::wstring script);
	bool SetSWeaponType(std::wstring id, int type);
	//bool SetRange(std::wstring id, int range); refactor to SetMaxRange(std::wstring id, int maxRange)
	//bool SetModuleName(std::wstring id, std::wstring modulename); refactor to SetMeshName(std::wstring id, std::wstring meshname)
	//bool SetIdleAnimName(std::wstring id, std::wstring animname); refactor to SetMatName(std::wstring id, std::wstring matname)
	//bool SetActAnimName(std::wstring id, std::wstring animname); refactor to SetAniGroup(std::wstring id, std::wstring anigroup)
	bool SetMaxRange(std::wstring id, int maxRange);
	bool SetMinRange(std::wstring id, int minRange);
	bool SetMeshName(std::wstring id, std::wstring meshname);
	bool SetMatName(std::wstring id, std::wstring matname);
	bool SetAniGroup(std::wstring id, std::wstring anigroup);
	bool SetValue(std::wstring id, int ivalue);

private:

	std::wstring mModPath;
	std::string mDataPath;
	std::string mLangPath;

	ticpp::Document mDataFile;
	ticpp::Document mLangFile;

	bool CreateDataFile();
	bool CreateLangFile();
};