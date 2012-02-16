#pragma once

#include <string>

#include "squaddefine.h"
#include "DataLibrary.h"

#define INTVAL(a) int get##a() \
	{ \
		int val = 0; \
		DataLibrary::getSingleton().getData(getPath() + "/" + #a, val); \
		return val; \
	} \
	void set##a(int val) \
	{ \
		DataLibrary::getSingleton().setData(getPath() + "/" + #a, val); \
	}

#define FLOATVAL(a) float get##a() \
	{ \
		float val = 0.0f; \
		DataLibrary::getSingleton().getData(getPath() + "/" + #a, val); \
		return val; \
	} \
	void set##a(float val) \
	{ \
		DataLibrary::getSingleton().setData(getPath() + "/" + #a, val); \
	}

#define STRVAL(a) std::string get##a() \
	{ \
		std::string val(""); \
		DataLibrary::getSingleton().getData(getPath() + "/" + #a, val); \
		return val; \
	} \
	void set##a(std::string val) \
	{ \
		DataLibrary::getSingleton().setData(getPath() + "/" + #a, val); \
	}

class Squad
{
public:
	Squad(std::string path, std::string srcpath);
	Squad(std::string path);
	virtual ~Squad();

	bool isInit() { return mInit; } 
protected:
	bool mInit;
	std::string mPath;

public:
	std::string getPath() { return mPath; }

protected:
	std::string mSquadId;

public:
	std::string getSquadId() { return mSquadId; }

//补给：装备更换
public:
	bool equipEquipment(enumtype equiptype, std::string equipid);
	void unloadEquipment(enumtype equiptype);

//补给：技能升级
public:
	bool learnSkill(enumtype skilltype, std::string skillid);
	void forgetSkill(std::string skillid);

//补给：随从更换
public:
	bool hireRetainer(std::string retainerid);
	void fireRetainer();

public:
	bool applyModifer(std::string modifierpath, std::string &modifierid);
	bool applyModifer(AttrModifier* modifier, std::string &modifierid);
	void removeModifier(std::string modifierid);
	
	bool addSkill(std::string skillid);
	void removeSkill(std::string skillid);


	bool applyEffect(std::string effectid, std::string &eid);
	void removeEffect(std::string eid);

public:
	virtual float getAttr(enumtype attrtype , enumtype calctype);


//获取属性
public:
	INTVAL(Type);
	INTVAL(SquadType);

	STRVAL(Name);

	INTVAL(UnitNum);
	INTVAL(Level);
	INTVAL(Exp);

	STRVAL(SoilderId);
	STRVAL(HorseId);
	STRVAL(PweaponId);
	STRVAL(ShieldId);
	STRVAL(SweaponId);
	STRVAL(ArmorId);
	STRVAL(LeaderId);
	STRVAL(RetainerID);
};