#include "soldiermanager.h"
#include "conversion.h"
#include "SquadEquip.h"
#include <Windows.h>

SoldierManager::SoldierManager()
{

}

SoldierManager::~SoldierManager()
{

}

bool SoldierManager::LoadMod(std::wstring modName, std::wstring langName, bool editorMode)
{
	mModPath = L".\\..\\Mod\\" + modName + L"\\";
	
	std::wstring tempPath;
	tempPath = mModPath + L"data\\soldier.xml";

	UnicodeToANSI(tempPath, mDataPath);

	WIN32_FIND_DATA findFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	memset(&findFileData,0,sizeof(findFileData));

	hFind = FindFirstFile(tempPath.c_str(),&findFileData);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		mDataFile.LoadFile(mDataPath.c_str(),TIXML_ENCODING_UTF8);
		FindClose(hFind);
	}
	else if(!editorMode)
	{
		mModPath = L".\\..\\Mod\\common\\";
		tempPath = mModPath + L"data\\soldier.xml";
		UnicodeToANSI(tempPath, mDataPath);
		hFind = FindFirstFile(tempPath.c_str(),&findFileData);
		if (hFind != INVALID_HANDLE_VALUE)
		{
			mDataFile.LoadFile(mDataPath.c_str(),TIXML_ENCODING_UTF8);
			FindClose(hFind);
		}
		else
		{
			CreateDataFile();
		}
	}
	else
	{
		CreateDataFile();
	}

	LoadLang(langName);

	return true;
}

bool SoldierManager::LoadLang(std::wstring langName)
{
	std::wstring tempPath;
	tempPath = mModPath + L"Lang\\" + langName + L"\\soldier.xml";
	
	UnicodeToANSI(tempPath, mLangPath);

	WIN32_FIND_DATA findFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	memset(&findFileData,0,sizeof(findFileData));

	hFind = FindFirstFile(tempPath.c_str(),&findFileData);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		mLangFile.LoadFile(mLangPath.c_str(),TIXML_ENCODING_UTF8);
		FindClose(hFind);
	}
	else
	{
		CreateLangFile();
	}
	return true;
}

bool SoldierManager::CreateDataFile()
{
	mDataFile.Clear();
	ticpp::Declaration * decl = new ticpp::Declaration( "1.0", "utf-8", "" );
	ticpp::Element * element = new ticpp::Element( "Soldier" );
	mDataFile.LinkEndChild(decl);
	mDataFile.LinkEndChild(element);
	return true;
}

bool SoldierManager::CreateLangFile()
{
	mLangFile.Clear();
	ticpp::Declaration * decl = new ticpp::Declaration( "1.0", "utf-8", "" );
	ticpp::Element * element = new ticpp::Element( "Soldier" );
	mLangFile.LinkEndChild(decl);
	mLangFile.LinkEndChild(element);
	return true;
}

bool SoldierManager::SaveData()
{
	if(mDataPath.size()> 0)
	{
		ticpp::Element *element = mDataFile.FirstChildElement("Soldier");
		if(element)
		{
			if(!element->NoChildren())
				mDataFile.SaveFile(mDataPath.c_str());
			else
				DeleteFileA(mDataPath.c_str());
		}

	}
	return true;
}

bool SoldierManager::SaveLang()
{
	if(mLangPath.size() > 0)
	{
		ticpp::Element *element = mLangFile.FirstChildElement("Soldier");
		if(element)
		{
			if(!element->NoChildren())
				mLangFile.SaveFile(mLangPath.c_str());
			else
				DeleteFileA(mLangPath.c_str());
		}
	}
	return true;
}

int SoldierManager::GetNum()
{
	ticpp::Element *element = mDataFile.FirstChildElement("Soldier");
	if(element)
	{
		if(element->NoChildren())
			return 0;
		else
		{
			int num = 0;
			ticpp::Iterator<ticpp::Element> child;
			for(child = child.begin(element); child != child.end(); child++)
				num++;
			return num;
		}
	}
	return 0;
}

void SoldierManager::AddSoldier()
{
	char newid[20];
	int n = 0;
	sprintf_s(newid,20,"newsoldier%d",n);
	ticpp::Element *rootelement = mDataFile.FirstChildElement("Soldier");
	while(rootelement->FirstChildElement(newid,false))
	{
		n = n + 1;
		sprintf_s(newid,20,"newsoldier%d",n);
	}
	ticpp::Element *element = new ticpp::Element(newid);
	element->SetAttribute("Attack",0);
	element->SetAttribute("RangedAttack",0);
	element->SetAttribute("Defence",0);
	element->SetAttribute("Formation",0);
	element->SetAttribute("Initiative",0);
	element->SetAttribute("ActionPoint",0);
	element->SetAttribute("Detection",0);
	element->SetAttribute("Covert",0);
	element->SetAttribute("Injury",0);
	element->SetAttribute("Conter",0);
	element->SetAttribute("Script","");
	element->SetAttribute("Value",0);

	rootelement->LinkEndChild(element);

	ticpp::Element *langrootelement = mLangFile.FirstChildElement("Soldier");
	ticpp::Element *langelement = langrootelement->FirstChildElement(newid,false);
	if(langelement == NULL)
	{
		langelement = new ticpp::Element(newid);
		langelement->SetAttribute("Name",newid);
		langelement->SetText("Please add a description.");
		langrootelement->LinkEndChild(langelement);
	}
}

void SoldierManager::DelSoldier(std::wstring id)
{
	std::string tempid;
	UnicodeToUTF8(id,tempid);
	ticpp::Element *dataelement = mDataFile.FirstChildElement("Soldier");
	ticpp::Node *datachildelement = dataelement->FirstChildElement(tempid,false);
	if(datachildelement)
	{
		dataelement->RemoveChild(datachildelement);
	}
	ticpp::Element *langelement = mLangFile.FirstChildElement("Soldier");
	ticpp::Node *langchildelement = langelement->FirstChildElement(tempid,false);
	if(langchildelement)
	{
		langelement->RemoveChild(langchildelement);
	}
}

std::wstring SoldierManager::GetID(int index)
{
	int n = 0;
	std::string id;
	ticpp::Element *element = mDataFile.FirstChildElement("Soldier");
	ticpp::Iterator<ticpp::Element> child;
	child = child.begin(element);
	while(n < index  )
	{
		child++;
		n++;
	}
	child->GetValue(&id);
	std::wstring tempid;
	UTF8ToUnicode(id,tempid);
	return tempid;
}

std::wstring SoldierManager::GetName(std::wstring id)
{
	std::string tempid;
	UnicodeToUTF8(id,tempid);
	ticpp::Element *element = mLangFile.FirstChildElement("Soldier");
	ticpp::Element *langelement = element->FirstChildElement(tempid,false);
	if(langelement == NULL)
	{
		langelement = new ticpp::Element(tempid);
		langelement->SetAttribute("Name",tempid);
		langelement->SetText("Please add a description.");
		element->LinkEndChild(langelement);
	}
	std::string name;
	name = langelement->GetAttribute("Name");
	std::wstring tempname;
	UTF8ToUnicode(name,tempname);
	return tempname;
}

std::wstring SoldierManager::GetDescription(std::wstring id)
{
	std::string tempid;
	UnicodeToUTF8(id,tempid);
	ticpp::Element *element = mLangFile.FirstChildElement("Soldier");
	ticpp::Element *langelement = element->FirstChildElement(tempid,false);
	if(langelement == NULL)
	{
		langelement = new ticpp::Element(tempid);
		langelement->SetAttribute("Name",tempid);
		langelement->SetText("Please add a description.");
		element->LinkEndChild(langelement);
	}
	std::string description;
	description = langelement->GetText();
	std::wstring tempdescription;
	UTF8ToUnicode(description,tempdescription);
	return tempdescription;
}

std::wstring SoldierManager::GetScriptName(std::wstring id)
{
	std::string tempid;
	UnicodeToUTF8(id,tempid);
	ticpp::Element *element = mDataFile.FirstChildElement("Soldier");
	ticpp::Element *dataelement = element->FirstChildElement(tempid);
	std::string script;
	script = dataelement->GetAttribute("Script");
	std::wstring tempscript;
	UTF8ToUnicode(script,tempscript);
	return tempscript;
}

int SoldierManager::GetAttr(std::wstring id, BasicAttr attrType)
{
	std::string tempid;
	UnicodeToUTF8(id,tempid);
	ticpp::Element *element = mDataFile.FirstChildElement("Soldier");
	ticpp::Element *dataelement = element->FirstChildElement(tempid);
	int attr = 0;
	switch(attrType)
	{
	case ATTR_ATTACK:
		dataelement->GetAttribute("Attack",&attr);
		break;
	case ATTR_RANGEDATTACK:
		dataelement->GetAttribute("RangedAttack",&attr);
		break;
	case ATTR_DEFENSE:
		dataelement->GetAttribute("Defence",&attr);
		break;
	case ATTR_FORMATION:
		dataelement->GetAttribute("Formation",&attr);
		break;
	case ATTR_INITIATIVE:
		dataelement->GetAttribute("Initiative",&attr);
		break;
	case ATTR_ACTIONPOINT:
		dataelement->GetAttribute("ActionPoint",&attr);
		break;
	case ATTR_DETECTION:
		dataelement->GetAttribute("Detection",&attr);
		break;
	case ATTR_COVERT:
		dataelement->GetAttribute("Covert",&attr);
		break;
	case ATTR_INJURY:
		dataelement->GetAttribute("Injury",&attr);
		break;
	case ATTR_COUNTER:
		dataelement->GetAttribute("Conter",&attr);
		break;
	default:
		return attr;
	}
	return attr;
}

int SoldierManager::GetValue(std::wstring id)
{
	std::string tempid;
	UnicodeToUTF8(id,tempid);
	ticpp::Element *element = mDataFile.FirstChildElement("Soldier");
	ticpp::Element *dataelement = element->FirstChildElement(tempid);
	int ivalue = 0;
	dataelement->GetAttribute("Value",&ivalue);
	return ivalue;
}


bool SoldierManager::SetID(std::wstring oldid, std::wstring id)
{
	std::string tempoldid;
	UnicodeToUTF8(oldid,tempoldid);
	std::string tempid;
	UnicodeToUTF8(id,tempid);
	ticpp::Element *element = mDataFile.FirstChildElement("Soldier");
	//�ж��Ƿ�����ظ�id
	if(element->FirstChildElement(tempid,false) == NULL)
	{
		ticpp::Element *dataelement = element->FirstChildElement(tempoldid);
		dataelement->SetValue(tempid);
		ticpp::Element *langelement = mLangFile.FirstChildElement("Soldier");
		ticpp::Element *langchildelement = langelement->FirstChildElement(tempoldid,false);
		if(langchildelement == NULL)
		{
			langchildelement = new ticpp::Element(tempid);
			langchildelement->SetAttribute("Name",tempid);
			langchildelement->SetText("Please add a description.");
			langelement->LinkEndChild(langchildelement);
		}
		else
			langchildelement->SetValue(tempid);
		return true;
	}
	return false;
}

bool SoldierManager::SetName(std::wstring id, std::wstring name)
{
	std::string tempid;
	UnicodeToUTF8(id,tempid);
	std::string tempname;
	UnicodeToUTF8(name,tempname);
	ticpp::Element *element = mLangFile.FirstChildElement("Soldier");
	ticpp::Element *langelement = element->FirstChildElement(tempid);
	langelement->SetAttribute("Name",tempname);
	return true;
}

bool SoldierManager::SetDescription(std::wstring id, std::wstring descripition)
{
	std::string tempid;
	UnicodeToUTF8(id,tempid);
	std::string tempdescripition;
	UnicodeToUTF8(descripition,tempdescripition);
	ticpp::Element *element = mLangFile.FirstChildElement("Soldier");
	ticpp::Element *langelement = element->FirstChildElement(tempid);
	langelement->SetText(tempdescripition);
	return true;
}

bool SoldierManager::SetScriptName(std::wstring id, std::wstring script)
{
	std::string tempid;
	UnicodeToUTF8(id,tempid);
	std::string tempscript;
	UnicodeToUTF8(script,tempscript);
	ticpp::Element *element = mDataFile.FirstChildElement("Soldier");
	ticpp::Element *dataelement = element->FirstChildElement(tempid);
	dataelement->SetAttribute("Script",tempscript);
	return true;
}

bool SoldierManager::SetAttr(std::wstring id, BasicAttr attrType, int attr)
{
	std::string tempid;
	UnicodeToUTF8(id,tempid);
	ticpp::Element *element = mDataFile.FirstChildElement("Soldier");
	ticpp::Element *dataelement = element->FirstChildElement(tempid);
	switch(attrType)
	{
	case ATTR_ATTACK:
		dataelement->SetAttribute("Attack",attr);
		break;
	case ATTR_RANGEDATTACK:
		dataelement->SetAttribute("RangedAttack",attr);
		break;
	case ATTR_DEFENSE:
		dataelement->SetAttribute("Defence",attr);
		break;
	case ATTR_FORMATION:
		dataelement->SetAttribute("Formation",attr);
		break;
	case ATTR_INITIATIVE:
		dataelement->SetAttribute("Initiative",attr);
		break;
	case ATTR_ACTIONPOINT:
		dataelement->SetAttribute("ActionPoint",attr);
		break;
	case ATTR_DETECTION:
		dataelement->SetAttribute("Detection",attr);
		break;
	case ATTR_COVERT:
		dataelement->SetAttribute("Covert",attr);
		break;
	case ATTR_INJURY:
		dataelement->SetAttribute("Injury",attr);
		break;
	case ATTR_COUNTER:
		dataelement->SetAttribute("Conter",attr);
		break;
	default:
		return false;
	}
	return true;
}

bool SoldierManager::SetValue(std::wstring id, int ivalue)
{
	std::string tempid;
	UnicodeToUTF8(id,tempid);
	ticpp::Element *element = mDataFile.FirstChildElement("Soldier");
	ticpp::Element *dataelement = element->FirstChildElement(tempid);
	dataelement->SetAttribute("Value",ivalue);
	return true;
}