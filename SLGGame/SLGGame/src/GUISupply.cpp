#include "GUISupply.h"

#include "DataLibrary.h"
#include "StringTable.h"
#include "StateManager.h"
#include "AVGSquadManager.h"

#include "GUIMessageBox.h"
#include "GUITipsWindow.h"

#include "boost/format.hpp"
#include <string>
#include <map>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>

#include "conversion.h"

const char* gEquipTypeName[EQUIP_RETAINER+1][6] =
{
	"","","","","","",
	"LightHorse","HeavyHorse","","","","",
	"LightArmor","MiddleArmor","HeavyArmor","","","",
	"Shield","","","","","",
	"OneHandSword","OneHandBlunt","TwoHandSword","TwoHandBlunt","ShortSpear","Long",
	"Bow","XBow","","","","",
	"","","","","",""
};

GUISupply::GUISupply(int width,int height):GUIScene("supply.layout",width,height)
{
	setSceneLanguage();

	assignWidget(mSupplyBG,"SupplyBG");
	MyGUI::Window* window;
	assignWidget(window,"TipsWindow");
	
	mTipsWindows=new GUITipsWindow(window,width,height);

	//assignWidget(mSaveButton,"T_SupplySave");
	//assignWidget(mLoadButton,"T_SupplyLoad");
	assignWidget(mExitButton,"T_SupplyExit");
	assignWidget(mEquipmentButton,"Button_Equipment");
	assignWidget(mSkillButton,"Button_Skill");
	assignWidget(mSoilderButton,"Button_Soldier");
	assignWidget(mSoilderButtonText,"TEXT_SoldierSupply");


	//mSaveButton->eventMouseButtonClick+= MyGUI::newDelegate(this, &GUISupply::onSave);
	//mLoadButton->eventMouseButtonClick+= MyGUI::newDelegate(this, &GUISupply::onLoad);
	mExitButton->eventMouseButtonClick+= MyGUI::newDelegate(this, &GUISupply::onExit);
	mEquipmentButton->eventMouseButtonClick+=MyGUI::newDelegate(this, &GUISupply::onEquipment);
	mSkillButton->eventMouseButtonClick+=MyGUI::newDelegate(this, &GUISupply::onSkill);
	mSoilderButton->eventMouseButtonClick+=MyGUI::newDelegate(this, &GUISupply::onSoilder);
	mSoilderButtonText->eventMouseButtonClick+=MyGUI::newDelegate(this, &GUISupply::onSoilder);

	MyGUI::ItemBox* baseItemBox;
	assignWidget(baseItemBox,"PWeaponItemBox");
	mPWeaponItemBox=new WeaponItemBox(baseItemBox);
	mPWeaponItemBox->getItemBox()->eventMouseItemActivate+= MyGUI::newDelegate(this, &GUISupply::eventMouseItemActivate);
	mPWeaponItemBox->getItemBox()->eventSelectItemAccept+= MyGUI::newDelegate(this, &GUISupply::eventSelectItemAccept);

	assignWidget(baseItemBox,"SWeaponItemBox");
	mSWeaponItemBox=new WeaponItemBox(baseItemBox);
	mSWeaponItemBox->getItemBox()->eventMouseItemActivate+= MyGUI::newDelegate(this, &GUISupply::eventMouseItemActivate);
	mSWeaponItemBox->getItemBox()->eventSelectItemAccept+= MyGUI::newDelegate(this, &GUISupply::eventSelectItemAccept);

	assignWidget(baseItemBox,"ShieldItemBox");
	mShieldItemBox=new WeaponItemBox(baseItemBox);
	mShieldItemBox->getItemBox()->eventMouseItemActivate+= MyGUI::newDelegate(this, &GUISupply::eventMouseItemActivate);
	mShieldItemBox->getItemBox()->eventSelectItemAccept+= MyGUI::newDelegate(this, &GUISupply::eventSelectItemAccept);

	assignWidget(baseItemBox,"HorseItemBox");
	mHorseItemBox=new WeaponItemBox(baseItemBox);
	mHorseItemBox->getItemBox()->eventMouseItemActivate+= MyGUI::newDelegate(this, &GUISupply::eventMouseItemActivate);
	mHorseItemBox->getItemBox()->eventSelectItemAccept+= MyGUI::newDelegate(this, &GUISupply::eventSelectItemAccept);

	assignWidget(baseItemBox,"ArmorItemBox");
	mArmorItemBox=new WeaponItemBox(baseItemBox);
	mArmorItemBox->getItemBox()->eventMouseItemActivate+= MyGUI::newDelegate(this, &GUISupply::eventMouseItemActivate);
	mArmorItemBox->getItemBox()->eventSelectItemAccept+= MyGUI::newDelegate(this, &GUISupply::eventSelectItemAccept);

	assignWidget(baseItemBox,"RetainerItemBox");
	mRetainerItemBox=new WeaponItemBox(baseItemBox);
	mRetainerItemBox->getItemBox()->eventMouseItemActivate+= MyGUI::newDelegate(this, &GUISupply::eventMouseItemActivate);
	mRetainerItemBox->getItemBox()->eventSelectItemAccept+= MyGUI::newDelegate(this, &GUISupply::eventSelectItemAccept);

	assignWidget(baseItemBox,"ArmyList");
	mSquadItemBox=new SquadItemBox(baseItemBox);
	mSquadItemBox->getItemBox()->eventMouseItemActivate+= MyGUI::newDelegate(this, &GUISupply::eventSquadMouseItemActivate);

	assignWidget(baseItemBox,"ActiveItemBox");
	mActiveItemBox=new SkillItemBox(baseItemBox);
	mActiveItemBox->getItemBox()->eventMouseItemActivate+= MyGUI::newDelegate(this, &GUISupply::eventMouseSkillActivate);
	mActiveItemBox->getItemBox()->eventSelectItemAccept+= MyGUI::newDelegate(this, &GUISupply::eventSelectSkillAccept);

	assignWidget(baseItemBox,"PassiveItemBox");
	mPassiveItemBox=new SkillItemBox(baseItemBox);
	mPassiveItemBox->getItemBox()->eventMouseItemActivate+= MyGUI::newDelegate(this, &GUISupply::eventMouseSkillActivate);
	mPassiveItemBox->getItemBox()->eventSelectItemAccept+= MyGUI::newDelegate(this, &GUISupply::eventSelectSkillAccept);

	assignWidget(baseItemBox,"EquipItemBox");
	mEquipItemBox=new SkillItemBox(baseItemBox);
	mEquipItemBox->getItemBox()->eventMouseItemActivate+= MyGUI::newDelegate(this, &GUISupply::eventMouseSkillActivate);
	mEquipItemBox->getItemBox()->eventSelectItemAccept+= MyGUI::newDelegate(this, &GUISupply::eventSelectSkillAccept);

	assignWidget(mWeaponTabControl,"WeaponTabControl");
	mWeaponTabControl->eventTabChangeSelect+=MyGUI::newDelegate(this, &GUISupply::eventTabChangeSelect);
	assignWidget(mSkillTabControl,"SkillTabControl");
	mSkillTabControl->eventTabChangeSelect+=MyGUI::newDelegate(this, &GUISupply::eventSkillTabChangeSelect);
	
	mWeaponTabControl->setVisible(true);
	mSkillTabControl->setVisible(false);

	assignWidget(mSquadPassivePoint,"SquadPassivePoint");
	assignWidget(mSquadActivePoint,"SquadActivePoint");
	assignWidget(mSquadEquipPoint,"SquadEquipPoint");

	assignWidget(mArmyWindow,"ArmyWindow");
	assignWidget(mControlWindow,"ControlWindow");
	assignWidget(mAttributeWindow,"AttributeWindow");
	assignWidget(mItemWindow,"ItemWindow");
	assignWidget(mItemAttributeWindow,"ItemAttributeWindow");

	mArmyWindow->setPosition(-235,0);
	mAttributeWindow->setPosition(295,-310);
	mControlWindow->setPosition(-230,575);
	mItemWindow->setPosition(240,800);
	mItemAttributeWindow->setPosition(1280,310);
	mIsHideScene=false;

	assignWidget(mSquadImage,"SquadImage");
	assignWidget(mTextSquadLeadName,"SquadLeadName");
	assignWidget(mTextSquadType,"SquadType");
	assignWidget(mTextSquadLv,"SquadLv");
	assignWidget(mTextSquadExp,"SquadExp");
	assignWidget(mTextSquadPWeapon,"SquadPWeapon");
	assignWidget(mTextSquadSWeapon,"SquadSWeapon");
	assignWidget(mTextSquadShield,"SquadShield");
	assignWidget(mTextSquadArmor,"SquadArmor");
	assignWidget(mTextSqureHorse,"SqureHorse");
	assignWidget(mTextSquadAttack,"SquadAttack");
	assignWidget(mTextSquadRangeAttack,"SquadRangeAttack");
	assignWidget(mTextSquadDefense,"SquadDefense");
	assignWidget(mTextSquadArray,"SquadArray");
	assignWidget(mTextSquadAgility,"SquadAgility");
	assignWidget(mTextSquadDetect,"SquadDetect");
	assignWidget(mTextSquadConcealment,"SquadConcealment");
	assignWidget(mTextSquadTenacity,"SquadTenacity");
	assignWidget(mTextSquadCounterattack,"SquadCounterattack");
	assignWidget(mTextSquadPeople,"SquadPeople");
	assignWidget(mTextSquadAP,"SquadAP");
	assignWidget(mTextRetainer,"Retainer");
	assignWidget(mRetainerSkill1,"RetainerSkill1");
	assignWidget(mRetainerSkill2,"RetainerSkill2");
	assignWidget(mRetainerImage,"RetainerImage");

	assignWidget(mTextItemName,"ItemName");
	assignWidget(mTextItemPrice,"ItemPrice");
	assignWidget(mItemIcon,"ItemIcon");
	assignWidget(mTextItemInfo,"ItemInfo");

	assignWidget(mTextArmyInfo,"ArmyInfo");

	int i=0;
	for(i=0;i<SQUAD_SKILL_NUM;i++)
	{
		assignWidget(mSquadSkillIcon[i],str(boost::format("SquadSkillIcon%1%")%(i+1)));
	}

	for(i=0;i<SQUAD_PASSIVE_SKILL_NUM;i++)
	{
		assignWidget(mSquadPassiveSkillIcon[i],str(boost::format("SquadPassiveSkillIcon%1%")%(i+1)));
	}

	for(i=0;i<SQUAD_USE_EQUIP_NUM;i++)
	{
		assignWidget(mSquadUseEquipIcon[i],str(boost::format("SquadUseEquipIcon%1%")%(i+1)));
	}

	for(i=0;i<SQUAD_EFFECT_NUM;i++)
	{
		assignWidget(mSquadEffectIcon[i],str(boost::format("SquadEffectIcon%1%")%(i+1)));
	}

	createBattleSquad();
	refreshArmyList();

	m_CurrSquadEquipItem=NULL;
	m_CurrSelectType=EQUIP_PWEAPON;
	m_CurrSquadIndex=-1;;
	m_Money=0;
	DataLibrary::getSingletonPtr()->getData("GameData/StoryData/Gold",m_Money);

	mSquadTypeMap.clear();
	std::vector<std::string> child=DataLibrary::getSingletonPtr()->getChildList("StaticData/SkillListData");
	for(std::vector<std::string>::iterator it=child.begin();it!=child.end();it++)
	{
		int type;
		DataLibrary::getSingletonPtr()->getData("StaticData/SkillListData/"+(*it),type);
		mSquadTypeMap[type]=(*it);
	}
	showArmyInfo();

	mIsRequestMsgBox=false;
}

GUISupply::~GUISupply(void)
{
	clearBattleSquad();
	delete mTipsWindows;
}

void GUISupply::clearBattleSquad()
{
	//销毁BattleDate
	for(std::vector<Squad*>::iterator it=mBattleSquad.begin();it!=mBattleSquad.end();it++)
	{
		delete (*it);
	}

	mBattleSquad.clear();
}

void GUISupply::createBattleSquad()
{
	//读取BattleData
	clearBattleSquad();

	std::vector<std::string> child=DataLibrary::getSingletonPtr()->getChildList("GameData/StoryData/SquadData");
	for(std::vector<std::string>::iterator it=child.begin();it!=child.end();it++)
	{
		Squad* bs=new Squad(std::string("GameData/StoryData/Supply/")+(*it));
		bs->init(std::string("GameData/StoryData/SquadData/")+(*it));
		mBattleSquad.push_back(bs);
	}
}

void GUISupply::refreshArmyList()
{
	mSquadItemBox->removeAllItems();
	for(std::vector<Squad*>::iterator it=mBattleSquad.begin();it!=mBattleSquad.end();it++)
	{
		mSquadItemBox->addItem(new SquadItemData((*it)->getSquadType(),(*it)->getName(),(*it)->getLevel()));
	}
}

void GUISupply::showArmy( int index )
{
	Squad* army=mBattleSquad.at(index);
	
	std::string tempstr,temppath;
	tempstr = army->getLeaderId();
	temppath = str(boost::format("StaticData/LeaderDate/%1%/Picture")%tempstr);
	bool re = DataLibrary::getSingletonPtr()->getData(temppath, tempstr);
	mSquadImage->setImageTexture(tempstr);

	mTextSquadLeadName->setCaption(army->getName());
	mTextSquadType->setCaption(getItemNameFormLanguage("LeaderDate",army->getLeaderId()));
	mTextSquadLv->setCaption(Ogre::StringConverter::toString(army->getLevel()));
	mTextSquadExp->setCaption(Ogre::StringConverter::toString(army->getExp()));
	mTextSquadPWeapon->setCaption(getItemNameFormLanguage("PweaponData",army->getPweaponId()));
	mTextSquadSWeapon->setCaption(getItemNameFormLanguage("SweaponData",army->getSweaponId()));
	mTextSquadShield->setCaption(getItemNameFormLanguage("ShieldData",army->getShieldId()));
	mTextSquadArmor->setCaption(getItemNameFormLanguage("ArmorData",army->getArmorId()));
	mTextSqureHorse->setCaption(getItemNameFormLanguage("HorseData",army->getHorseId()));
	mTextRetainer->setCaption(getItemNameFormLanguage("RetainerData",army->getRetainerId()));
	if(army->getRetainerId()!="none")
	{
		tempstr = army->getRetainerId();
		std::string p;
		temppath = str(boost::format("StaticData/RetainerData/%1%/Picture")%tempstr);
		DataLibrary::getSingletonPtr()->getData(temppath, p);
		mRetainerImage->setImageTexture(p);

		
		temppath=str(boost::format("StaticData/RetainerData/%1%/Skill")%tempstr);
		DataLibrary::getSingletonPtr()->getData(temppath, p);
		if(tempstr!="none")
		{
			temppath=str(boost::format("StaticData/SkillData/%1%/Icon")%p);
			std::string temppath2=str(boost::format("StaticData/EffectData/%1%/Icon")%p);
			std::string skillIcon;
			if(DataLibrary::getSingletonPtr()->getData(temppath, skillIcon))
			{
				mRetainerSkill1->setItemResource("skill");
				mRetainerSkill1->setItemGroup(skillIcon);
				mRetainerSkill1->setItemName("normal");
				mRetainerSkill1->setNeedToolTip(true);
				std::string skillDes;
				std::string desPath=str(boost::format("StaticData/SkillData/%1%/Describe")%p);
				DataLibrary::getSingletonPtr()->getData(desPath, skillDes);

				mRetainerSkill1->setUserString("Tips",skillDes);
				GUITipsWindow::getSingletonPtr()->addToolTipEvent(mRetainerSkill1);

			}
			else if(DataLibrary::getSingletonPtr()->getData(temppath2, skillIcon))
			{
				mRetainerSkill1->setItemResource("skillpass");
				mRetainerSkill1->setItemGroup(skillIcon);
				mRetainerSkill1->setItemName("normal");
				mRetainerSkill1->setNeedToolTip(true);
				std::string skillDes;
				std::string desPath=str(boost::format("StaticData/EffectData/%1%/Describe")%p);
				DataLibrary::getSingletonPtr()->getData(desPath, skillDes);

				mRetainerSkill1->setUserString("Tips",skillDes);
				GUITipsWindow::getSingletonPtr()->addToolTipEvent(mRetainerSkill1);
			}

			
		}
	}

	showAttribute(index,0,"");
	int p=0;
	if(army->getType()==0)
	{
		p=50;
	}
	else
	{
		p=20;
	}

	if(army->getUnitNum()!=p)
	{
		tempstr = army->getSoilderId();
		temppath = str(boost::format("StaticData/SoilderData/%1%/Value")%tempstr);
		int value;
		DataLibrary::getSingletonPtr()->getData(temppath, value);
		int money=(50-army->getUnitNum())*value+army->getLevel()*5;
		mSoilderButtonText->setCaption(str(boost::format(StringTable::getSingletonPtr()->getString("TEXT_SoldierSupplyNeed"))%money));
	}
	else
	{
		mSoilderButtonText->setCaption(StringTable::getSingletonPtr()->getString("TEXT_SoldierFull"));
	}
}

std::string GUISupply::itemCompare(Squad* compareSquad,Squad* squad,AttrType type)
{
	float oldAttr=squad->getAttr(type,ATTRCALC_FULL);

	if(compareSquad!=NULL)
	{
		float newAttr=compareSquad->getAttr(type,ATTRCALC_FULL);

		float difference=newAttr-oldAttr;
		if(difference>0)
		{
			return "#00FF00" + Ogre::StringConverter::toString(newAttr) +" (+"+Ogre::StringConverter::toString(difference)+")";
		}
		else if(difference==0)
		{
			return Ogre::StringConverter::toString(oldAttr);
		}
		else
		{
			return "#FF0000"+Ogre::StringConverter::toString(newAttr) +" (-"+Ogre::StringConverter::toString(abs(difference))+")";
		}
	}
	else
	{
		return Ogre::StringConverter::toString(oldAttr);
	}
}

void GUISupply::eventSquadMouseItemActivate(MyGUI::ItemBox* _sender, size_t _index)
{
	if(_index!=-1)
	{
		m_CurrSquadIndex=_index;
		showArmy(_index);
		setItemInfo(NULL);
		mWeaponTabControl->setIndexSelected(0);
		mSkillTabControl->setIndexSelected(0);
		showItem(EQUIP_PWEAPON);
		showSkill(SKILLTYPE_ACTIVE);
	}
}

void GUISupply::eventMouseItemActivate(MyGUI::ItemBox* _sender, size_t _index)
{
	if(_index!=-1)
	{
		WeaponItemData* item=*(_sender->getItemDataAt<WeaponItemData*>(_index));
		if(item->getType()!=EQUIP_RETAINER)
			showAttribute(m_CurrSquadIndex,m_CurrSelectType,item->getID());
		setItemInfo(item);
	}
	else
	{
		showAttribute(m_CurrSquadIndex,0,"");
		setItemInfo(NULL);
	}
}

void GUISupply::eventSelectItemAccept(MyGUI::ItemBox* _sender, size_t _index)
{
	if(_index!=-1)
	{
		WeaponItemData* item=*(_sender->getItemDataAt<WeaponItemData*>(_index));
		if(mIsRequestMsgBox && mMsgBoxReturn)
		{
			mIsRequestMsgBox=false;
			GUISystem::getSingletonPtr()->destoryScene(MessageBoxScene);
			buyItem(m_CurrSquadIndex,item);
		}
		else if(mIsRequestMsgBox && !mMsgBoxReturn)
		{
			mIsRequestMsgBox=false;
		}
		else
		{
			mIsRequestMsgBox=true;
			mItem_Sender=_sender;
			mItem_Index=_index;
			GUIMessageBox* messagebox=(GUIMessageBox*)GUISystem::getSingletonPtr()->createScene(MessageBoxScene);
			messagebox->setNotify(this);
			if(m_CurrSelectType!=EQUIP_RETAINER)
				messagebox->showScene(str(boost::format(StringTable::getSingletonPtr()->getString("ConfirmBuyItem"))%item->getPriceValue()%item->getName()));
			else
			{
				int wage=0;
				DataLibrary::getSingletonPtr()->getData(str(boost::format("StaticData/RetainerData/%1%/Wage")%item->getID()),wage);
				messagebox->showScene(str(boost::format(StringTable::getSingletonPtr()->getString("ConfirmHireItem"))%item->getPriceValue()%item->getName()%wage));
			}
		}
	}
}

void GUISupply::showAttribute(int index,int itemType,std::string itemID)
{
	Squad* army=mBattleSquad.at(index);
	Squad* copySquad=NULL;
	if(itemID!="")
	{
		DataLibrary::getSingletonPtr()->copyNode(army->getPath(),"GameData/StoryData/SquadData/TempSquad");
		copySquad=new Squad("GameData/StoryData/SquadData/TempSquad");
		copySquad->equipEquipment(itemType,itemID);

	}

	mTextSquadAttack->setCaption(itemCompare(copySquad,army,ATTR_ATTACK));
	mTextSquadRangeAttack->setCaption(itemCompare(copySquad,army,ATTR_RANGEDATTACK));
	mTextSquadDefense->setCaption(itemCompare(copySquad,army,ATTR_DEFENCE));
	mTextSquadArray->setCaption(itemCompare(copySquad,army,ATTR_FORM));
	mTextSquadAgility->setCaption(itemCompare(copySquad,army,ATTR_INITIATIVE));
	mTextSquadDetect->setCaption(itemCompare(copySquad,army,ATTR_DETECTION));
	mTextSquadConcealment->setCaption(itemCompare(copySquad,army,ATTR_COVERT));
	mTextSquadTenacity->setCaption(itemCompare(copySquad,army,ATTR_TOUGHNESS));
	mTextSquadAP->setCaption(itemCompare(copySquad,army,ATTR_ACTIONPOINT));
	mTextSquadCounterattack->setCaption(itemCompare(copySquad,army,ATTR_CONTER));
	mTextSquadPeople->setCaption(Ogre::StringConverter::toString(army->getUnitNum()));
	
	m_SquadSkillNum=0;
	m_SquadPassiveSkillNum=0;
	m_SquadUseEquipNum=0;
	m_SquadEffectNum=0;
	std::map<std::string,int> skillmap;
	if(itemID!="")
		skillmap=copySquad->getSkillTable();
	else
		skillmap=army->getSkillTable();

	MyGUI::ResourceManager& manager = MyGUI::ResourceManager::getInstance();
	for(std::map<std::string,int>::iterator it=skillmap.begin();it!=skillmap.end();it++)
	{
		std::string icon;
		if(it->second==SKILLTYPE_ACTIVE)
		{
			DataLibrary::getSingletonPtr()->getData(str(boost::format("StaticData/SkillData/%1%/Icon")%it->first),icon);

			mSquadSkillIcon[m_SquadSkillNum]->setItemResource("skill");
			mSquadSkillIcon[m_SquadSkillNum]->setItemGroup(icon);
			mSquadSkillIcon[m_SquadSkillNum]->setItemName("normal");
			mSquadSkillIcon[m_SquadSkillNum]->setNeedToolTip(true);
			std::string skillDes;
			std::string desPath=str(boost::format("StaticData/SkillData/%1%/Describe")%it->first);
			DataLibrary::getSingletonPtr()->getData(desPath, skillDes);

			mSquadSkillIcon[m_SquadSkillNum]->setUserString("Tips",skillDes);
			GUITipsWindow::getSingletonPtr()->addToolTipEvent(mSquadSkillIcon[m_SquadSkillNum]);
			m_SquadSkillNum++;
		}
		else if(it->second==SKILLTYPE_PASSIVE)
		{
			DataLibrary::getSingletonPtr()->getData(str(boost::format("StaticData/EffectData/%1%/Icon")%it->first),icon);

			mSquadPassiveSkillIcon[m_SquadPassiveSkillNum]->setItemResource("skillpass");
			mSquadPassiveSkillIcon[m_SquadPassiveSkillNum]->setItemGroup(icon);
			mSquadPassiveSkillIcon[m_SquadPassiveSkillNum]->setItemName("normal");
			mSquadPassiveSkillIcon[m_SquadPassiveSkillNum]->setNeedToolTip(true);
			std::string skillDes;
			std::string desPath=str(boost::format("StaticData/EffectData/%1%/Describe")%it->first);
			DataLibrary::getSingletonPtr()->getData(desPath, skillDes);

			mSquadPassiveSkillIcon[m_SquadPassiveSkillNum]->setUserString("Tips",skillDes);
			GUITipsWindow::getSingletonPtr()->addToolTipEvent(mSquadPassiveSkillIcon[m_SquadPassiveSkillNum]);
			m_SquadPassiveSkillNum++;
		}
		else if (it->second==SKILLTYPE_EQUIP)
		{
			std::string id=it->first;
			std::string mGroup=id;
			  if(id=="OneHandBlunt")
				  mGroup="eqp_oneaxe";
			  else if(id=="OneHandSword")
				  mGroup="OneHandSword";
			  else if(id=="TwoHandSword")
				  mGroup="eqp_twosword";
			  else if(id=="TwoHandBlunt")
				  mGroup="eqp_twoaxe";
			  else if(id=="ShortSpear")
				  mGroup="ShortSpear";
			  else if(id=="Long")
				  mGroup="eqp_halberd";
			  else if(id=="Shield")
				  mGroup="Shield";
			  else if(id=="LightArmor")
				  mGroup="eqp_lightarmor";
			  else if(id=="MiddleArmor")
				  mGroup="eqp_midarmor";
			  else if(id=="HeavyArmor")
				  mGroup="eqp_heavyarmor";
			  else if(id=="Bow")
				  mGroup="Bow";
			  else if(id=="XBow")
				  mGroup="LXBox";
			  else if(id=="LightHorse")
				  mGroup="eqp_lighthorse";
			  else if(id=="HeavyHorse")
				  mGroup="eqp_heavyhorse";

			mSquadUseEquipIcon[m_SquadUseEquipNum]->setItemResource("eqp");
			mSquadUseEquipIcon[m_SquadUseEquipNum]->setItemGroup(mGroup);
			mSquadUseEquipIcon[m_SquadUseEquipNum]->setItemName("normal");
			mSquadUseEquipIcon[m_SquadUseEquipNum]->setNeedToolTip(true);

			mSquadUseEquipIcon[m_SquadUseEquipNum]->setUserString("Tips",StringTable::getSingletonPtr()->getString(id));
			GUITipsWindow::getSingletonPtr()->addToolTipEvent(mSquadUseEquipIcon[m_SquadUseEquipNum]);
			m_SquadUseEquipNum++;
		}
	}

	for(int i=m_SquadSkillNum;i<SQUAD_SKILL_NUM;i++)
	{
		mSquadSkillIcon[i]->setItemGroup("");
		mSquadSkillIcon[i]->setNeedToolTip(false);
	}

	for(int i=m_SquadPassiveSkillNum;i<SQUAD_SKILL_NUM;i++)
	{
		mSquadPassiveSkillIcon[i]->setItemGroup("");
		mSquadPassiveSkillIcon[i]->setNeedToolTip(false);
	}

	for(int i=m_SquadUseEquipNum;i<SQUAD_SKILL_NUM;i++)
	{
		mSquadUseEquipIcon[i]->setItemGroup("");
		mSquadUseEquipIcon[i]->setNeedToolTip(false);
	}

	if(itemID!="")
	{
		delete copySquad;
		DataLibrary::getSingletonPtr()->delNode("GameData/StoryData/SquadData/TempSquad");
	}

}

void GUISupply::showItem(int type)
{
	std::string path;
	WeaponItemBox* itemBox;
	std::string equipID;
	int equipType=type;
	Squad* army=mBattleSquad.at(m_CurrSquadIndex);
	m_CurrSelectType=(EquipmentType)type;

	switch(type)
	{
	case EQUIP_PWEAPON:
		path="StaticData/PweaponData";
		itemBox=mPWeaponItemBox;
		equipID=army->getPweaponId();
		break;
	case EQUIP_SWEAPON:
		path="StaticData/SweaponData";
		itemBox=mSWeaponItemBox;
		equipID=army->getSweaponId();
		break;
	case EQUIP_SHIELD:
		path="StaticData/ShieldData";
		itemBox=mShieldItemBox;
		equipID=army->getShieldId();
		break;
	case EQUIP_ARMOR:
		path="StaticData/ArmorData";
		itemBox=mArmorItemBox;
		equipID=army->getArmorId();
		break;
	case EQUIP_HORSE:
		path="StaticData/HorseData";
		itemBox=mHorseItemBox;
		equipID=army->getHorseId();
		break;
	case EQUIP_SOILDER:
		path="StaticData/SoilderData";
		itemBox=mSoilderItemBox;
		equipID=army->getSoilderId();
		break;
	case EQUIP_RETAINER:
		path="StaticData/RetainerData";
		itemBox=mRetainerItemBox;
		equipID=army->getRetainerId();
	}

	itemBox->removeAllItems();
	std::vector<std::string> child=DataLibrary::getSingletonPtr()->getChildList(path);
	int equipSubType=0;
	m_CurrSquadEquipItem=NULL;
	for(std::vector<std::string>::iterator it=child.begin();it!=child.end();it++)
	{
		std::string temppath = path+"/"+ (*it) + std::string("/Type");
		DataLibrary::getSingletonPtr()->getData(temppath,equipSubType);
		if(equipSubType==EQUIP_PWEAPON_STAVES)
			return;

		if(type!=EQUIP_RETAINER)
		{
			std::map<std::string,int> skillmap;
			skillmap=army->getSkillTable();
			if(skillmap.find(gEquipTypeName[type][equipSubType])==skillmap.end())
				continue;
		}
		else
		{
			std::string p = path+"/"+ (*it) + std::string("/SquadType");
			std::string squadType;
			DataLibrary::getSingletonPtr()->getData(p,squadType);

			std::vector<std::string> squad;
			boost::algorithm::split(squad,squadType, boost::is_any_of(","));
			bool isFind=false;
			for(std::vector<std::string>::iterator it=squad.begin();it!=squad.end();it++)
			{
				if(Ogre::StringConverter::parseInt((*it))==army->getSquadType())
				{
					isFind=true;
					break;
				}
			}
			if(!isFind)
			{
				continue;
			}
		}

		WeaponItemData* data=new WeaponItemData(equipType,(*it),army->getUnitNum());
		data->setIsHaveSkill(true);
		
		if(type==EQUIP_PWEAPON || type==EQUIP_SWEAPON || type==EQUIP_ARMOR || type==EQUIP_SHIELD || type==EQUIP_HORSE)
		{
			data->setSubType(equipSubType);	
		}

		if((*it)==equipID)
		{
			data->setEquip(true);
			m_CurrSquadEquipItem=data;
		}
		itemBox->addItem(data);
	}
}

void GUISupply::buyItem(int index,WeaponItemData* item)
{
	Squad* army=mBattleSquad.at(index);

	if(!item->getEquip() && item->getCanBuy() && item->getIsHaveSkill())
	{
		if(item->getType()!=EQUIP_RETAINER)
		{
			if(item->getType()==EQUIP_PWEAPON && (item->getSubType()==EQUIP_PWEAPON_TWOHANDSWORD || item->getSubType()==EQUIP_PWEAPON_TWOHANDBLUNT))
			{
				if(army->getShieldId()!="none")
				{
					int value=0;
					std::string path="StaticData/ShieldData/"+army->getShieldId()+"/Value";
					DataLibrary::getSingletonPtr()->getData(path,value);
					
					m_Money+=(value*50)*0.8;

					army->unloadEquipment(EQUIP_SHIELD);
				}
			}
			else if(item->getType()==EQUIP_SHIELD)
			{
				int value=0;
				std::string path="StaticData/PweaponData/"+army->getPweaponId()+"/Type";
				DataLibrary::getSingletonPtr()->getData(path,value);

				if(value==EQUIP_PWEAPON_TWOHANDSWORD || value==EQUIP_PWEAPON_TWOHANDBLUNT)
				{
					path="StaticData/PweaponData/"+army->getPweaponId()+"/Value";
					DataLibrary::getSingletonPtr()->getData(path,value);
					
					m_Money+=(value*50)*0.8;

					army->unloadEquipment(EQUIP_PWEAPON);
				}
			}

			army->equipEquipment(item->getType(),item->getID());
		}
		else
		{
			if(m_CurrSquadEquipItem!=NULL)
				army->fireRetainer();

			army->hireRetainer(item->getID());
		}

		if(m_CurrSquadEquipItem!=NULL && item->getType()!=EQUIP_RETAINER)
			m_Money+=m_CurrSquadEquipItem->getPriceValue()*0.8;
		m_Money-=item->getPriceValue();

		showArmy(m_CurrSquadIndex);
		showItem(item->getType());

		DataLibrary::getSingletonPtr()->setData("GameData/StoryData/Gold",m_Money);
		showArmyInfo();
	}
}

std::string GUISupply::getItemNameFormLanguage(std::string type,std::string name)
{

	if(name == "none")
		return StringTable::getSingleton().getString(name);
	else
	{
		std::string tempstr;
		std::string temppath = std::string("StaticData/")+type+"/"+ name + std::string("/Name");
		DataLibrary::getSingletonPtr()->getData(temppath,tempstr);
		return tempstr;
	}
}

void GUISupply::eventTabChangeSelect(MyGUI::TabControl* _sender, size_t _index)
{
	if(m_CurrSquadIndex!=-1)
	{
		showAttribute(m_CurrSquadIndex,0,"");
		setItemInfo(NULL);
		switch(_index)
		{
		case 0:
			showItem(EQUIP_PWEAPON);
			break;
		case 1:
			showItem(EQUIP_SWEAPON);
			break;
		case 2:
			showItem(EQUIP_SHIELD);
			break;
		case 3:
			showItem(EQUIP_HORSE);
			break;
		case 4:
			showItem(EQUIP_ARMOR);
			break;
		case 5://副官
			showItem(EQUIP_RETAINER);
			break;
		}
	}
}

void GUISupply::setItemInfo(WeaponItemData* item)
{
	if(item!=NULL)
	{
		Squad* army=mBattleSquad.at(m_CurrSquadIndex);
		mTextItemName->setCaption(item->getName());
		if(item->getType()!=EQUIP_RETAINER)
			mTextItemPrice->setCaption(str(boost::format(StringTable::getSingletonPtr()->getString("ItemPrice"))%item->getPriceValue()%item->getOnePrice()%50));
		else
			mTextItemPrice->setCaption("");
		mItemIcon->setItemResource("eqp");
		mItemIcon->setItemGroup(item->getImage());
		mItemIcon->setVisible(true);
		mTextItemInfo->setCaption("");

		std::wstring tempstring;
		std::string d=item->getDescribe();
		UTF8ToUnicode(d,tempstring);
		UnicodeToANSI(tempstring,d);

		DWORD dwNum = MultiByteToWideChar (CP_ACP, 0, d.c_str(), -1, NULL, 0);
		wchar_t *pwText;
		pwText = new wchar_t[dwNum];

		MultiByteToWideChar (CP_ACP, 0, d.c_str(), -1, pwText, dwNum);

		std::wstring text=pwText;
		while(!text.empty())
		{
			mTextItemInfo->addText(text.substr(0,1));
			if (mTextItemInfo->getHScrollPosition()!=0)//自动换行
			{
				int length=mTextItemInfo->getTextLength();
				mTextItemInfo->eraseText(length-1);
				mTextItemInfo->addText("\n");
				mTextItemInfo->addText(text.substr(0,1));
			}
			text.erase(text.begin());
		}
	}
	else
	{
		mTextItemInfo->setCaption("");
		mTextItemPrice->setCaption("");
		mTextItemName->setCaption("");
		mItemIcon->setVisible(false);
	}
}

void GUISupply::setSkillInfo(SkillItemData* item)
{
	if(item!=NULL)
	{
		Squad* army=mBattleSquad.at(m_CurrSquadIndex);
		mTextItemName->setCaption(item->getName());
		mTextItemPrice->setCaption(item->getPrice());
		mItemIcon->setItemResource(item->getResoure());
		mItemIcon->setItemGroup(item->getImage());
		mItemIcon->setItemName("normal");
		mItemIcon->setVisible(true);

		mTextItemInfo->setCaption("");

		std::wstring tempstring;
		std::string d=item->getDescribe();
		UTF8ToUnicode(d,tempstring);
		UnicodeToANSI(tempstring,d);

		DWORD dwNum = MultiByteToWideChar (CP_ACP, 0, d.c_str(), -1, NULL, 0);
		wchar_t *pwText;
		pwText = new wchar_t[dwNum];

		MultiByteToWideChar (CP_ACP, 0, d.c_str(), -1, pwText, dwNum);

		std::wstring text=pwText;
		while(!text.empty())
		{
			mTextItemInfo->addText(text.substr(0,1));
			if (mTextItemInfo->getHScrollPosition()!=0)//自动换行
			{
				int length=mTextItemInfo->getTextLength();
				mTextItemInfo->eraseText(length-1);
				mTextItemInfo->addText("\n");
				mTextItemInfo->addText(text.substr(0,1));
			}
			text.erase(text.begin());
		}
	}
	else
	{
		mTextItemInfo->setCaption("");
		mTextItemPrice->setCaption("");
		mTextItemName->setCaption("");
		mItemIcon->setVisible(false);
	}
}

void GUISupply::showArmyInfo()
{
	DataLibrary::getSingletonPtr()->getData("GameData/StoryData/Gold",m_Money);
	std::string info;
	info+=str(boost::format(StringTable::getSingletonPtr()->getString("ArmyInfo_Money"))%m_Money);
	info+="\n";

	int totalWage=0;
	for(std::vector<Squad*>::iterator it=mBattleSquad.begin();it!=mBattleSquad.end();it++)
	{
		std::string retainerId=(*it)->getRetainerId();
		
		if(retainerId!="none")
		{
			int wage;
			DataLibrary::getSingletonPtr()->getData(str(boost::format("StaticData/RetainerData/%1%/Wage")%retainerId),wage);

			totalWage+=wage;
		}
	}
	info+=str(boost::format(StringTable::getSingletonPtr()->getString("ArmyInfo_RetainerId"))%totalWage);

	mTextArmyInfo->setCaption(info);
}

void GUISupply::onSave(MyGUI::Widget* _sender)
{
	SLWindow= (GUISLWindow*)GUISystem::getSingletonPtr()->createScene(SLScene);
	SLWindow->setCallScene(this);
	SLWindow->showScene("save");
}

void GUISupply::onLoad(MyGUI::Widget* _sender)
{
	SLWindow= (GUISLWindow*)GUISystem::getSingletonPtr()->createScene(SLScene);
	SLWindow->setCallScene(this);
	SLWindow->showScene("load");
}

void GUISupply::onExit(MyGUI::Widget* _sender)
{
	for(std::vector<Squad*>::iterator it=mBattleSquad.begin();it!=mBattleSquad.end();it++)
	{
		AVGSquadManager::getSingletonPtr()->dumpSquad((*it)->getSquadId(),(*it));
	}
	
	hideScene();
}

void GUISupply::onEquipment(MyGUI::Widget* _sender)
{
	mWeaponTabControl->setVisible(true);
	mSkillTabControl->setVisible(false);
}

void GUISupply::onSkill(MyGUI::Widget* _sender)
{
	mWeaponTabControl->setVisible(false);
	mSkillTabControl->setVisible(true);
}

void GUISupply::onSoilder(MyGUI::Widget* _sender)
{
	if(m_CurrSquadIndex!=-1)
	{
		Squad* army=mBattleSquad.at(m_CurrSquadIndex);
		if(army->getType()==1)
			return ;
		if(army->getUnitNum()!=50)
		{
			std::string tempstr = army->getSoilderId();
			std::string temppath = str(boost::format("StaticData/SoilderData/%1%/Value")%tempstr);
			int value;
			DataLibrary::getSingletonPtr()->getData(temppath, value);
			int money=(50-army->getUnitNum())*value;
			int moneyhave=0;
			DataLibrary::getSingletonPtr()->getData("GameData/StoryData/Gold",moneyhave);
			if(moneyhave-money>0)
			{
				mSoilderButtonText->setCaption(StringTable::getSingletonPtr()->getString("TEXT_SoldierFull"));
				army->setUnitNum(50);
				DataLibrary::getSingletonPtr()->setData("GameData/StoryData/Gold",moneyhave-money);
				showArmy(m_CurrSquadIndex);
				showArmyInfo();
			}
		}
	}
}

void GUISupply::showScene( std::string arg )
{
	mNextScript=arg;
	FadeIn(1000,mSupplyBG);
}

void GUISupply::hideScene()
{
	mIsHideScene=true;

	MoveTo(-235,0,1000,mArmyWindow);
	MoveTo(295,-310,1000,mAttributeWindow);
	MoveTo(-230,575,1000,mControlWindow);
	MoveTo(240,800,1000,mItemWindow);
	MoveTo(1280,310,1000,mItemAttributeWindow);
}

void GUISupply::FrameEvent()
{

}

void GUISupply::onOtherSceneNotify( std::string arg )
{
	if(arg=="FadeInOver")
	{
		MoveTo(0,0,1000,mArmyWindow);
		MoveTo(295,5,1000,mAttributeWindow);
		MoveTo(5,575,1000,mControlWindow);
		MoveTo(230,310,1000,mItemWindow);
		MoveTo(980,310,1000,mItemAttributeWindow);
	}
	else if(arg=="MoveToOver" && mIsHideScene)
	{
		mIsHideScene=false;
		FadeOut(1000,mSupplyBG);
	}
	else if(arg=="FadeOutOver")
	{
		GUISystem::getSingletonPtr()->getScene(StageScene)->onOtherSceneNotify("ReturnFromSupply");
		StateManager::getSingletonPtr()->removeAffixationState();
	}
	else if(arg=="MessageBoxYes")
	{
		mMsgBoxReturn=true;
		eventSelectItemAccept(mItem_Sender,mItem_Index);
	}
	else if(arg=="MessageBoxNo")
	{
		mMsgBoxReturn=false;
		eventSelectItemAccept(mItem_Sender,mItem_Index);
	}
}



void GUISupply::eventMouseSkillActivate(MyGUI::ItemBox* _sender, size_t _index)
{
	if(_index!=-1)
	{
		SkillItemData* item=*(_sender->getItemDataAt<SkillItemData*>(_index));
		setSkillInfo(item);
	}
	else
	{
		setSkillInfo(NULL);
	}
}

void GUISupply::eventSelectSkillAccept(MyGUI::ItemBox* _sender, size_t _index)
{
	if(_index!=-1)
	{
		SkillItemData* item=*(_sender->getItemDataAt<SkillItemData*>(_index));
		trainSkill(item);
	}
}

void GUISupply::eventSkillTabChangeSelect(MyGUI::TabControl* _sender, size_t _index)
{
	if(m_CurrSquadIndex==-1)
		return ;

	switch(_index)
	{
	case 0:
		showSkill(1);
		break;
	case 1:
		showSkill(0);
		break;
	case 2:
		showSkill(2);
		break;
	}
	
}

void GUISupply::trainSkill(SkillItemData* skill)
{
	if(m_CurrSquadIndex==-1)
		return ;

	Squad* army=mBattleSquad.at(m_CurrSquadIndex);

	if(!skill->getCanTrain() || !skill->getHaveEnoughPoint() )
		return;

	switch(skill->getType())
	{
	case SKILLTYPE_PASSIVE:
		army->setSkillPointPassive(army->getSkillPointPassive()-1);
		break;
	case SKILLTYPE_ACTIVE:
		army->setSkillPointAction(army->getSkillPointAction()-1);
		break;
	case SKILLTYPE_EQUIP:
		army->setSkillPointEquip(army->getSkillPointEquip()-1);
		break;
	}

	army->learnSkill(skill->getType(),skill->getID());

	showAttribute(m_CurrSquadIndex,0,"");
	showSkill(skill->getType());
}

void GUISupply::showSkill(int type)
{
	Squad* army=mBattleSquad.at(m_CurrSquadIndex);

	switch(type)
	{
	case SKILLTYPE_PASSIVE:
		mSquadPassivePoint->setCaption(str(boost::format(StringTable::getSingletonPtr()->getString("SquadPassivePoint"))%(army->getSkillPointPassive())));
		showSkillItem(SKILLTYPE_PASSIVE,"StaticData/SkillListData/"+mSquadTypeMap[army->getSquadType()]);
		break;
	case SKILLTYPE_ACTIVE:
		mSquadActivePoint->setCaption(str(boost::format(StringTable::getSingletonPtr()->getString("SquadActivePoint"))%(army->getSkillPointAction())));
		showSkillItem(SKILLTYPE_ACTIVE,"StaticData/SkillListData/"+mSquadTypeMap[army->getSquadType()]);
		break;
	case SKILLTYPE_EQUIP:
		mSquadEquipPoint->setCaption(str(boost::format(StringTable::getSingletonPtr()->getString("SquadEquipPoint"))%(army->getSkillPointEquip())));
		showSkillItem(SKILLTYPE_EQUIP,"StaticData/SkillListData/"+mSquadTypeMap[army->getSquadType()]);
		break;
	}
}

void GUISupply::showSkillItem(int type,std::string skillListPath)
{
	std::string path;
	SkillItemBox* itemBox;
	std::string skillID;
	Squad* army=mBattleSquad.at(m_CurrSquadIndex);
	m_CurrSkillType=(SkillType)type;
	int totalPoint;

	switch(type)
	{
	case SKILLTYPE_PASSIVE:
		path=skillListPath+"/Passive";
		itemBox=mPassiveItemBox;
		totalPoint=army->getSkillPointPassive();
		break;
	case SKILLTYPE_ACTIVE:
		path=skillListPath+"/Active";
		itemBox=mActiveItemBox;
		totalPoint=army->getSkillPointAction();
		break;
	case SKILLTYPE_EQUIP:
		path=skillListPath+"/Equip";
		itemBox=mEquipItemBox;
		totalPoint=army->getSkillPointEquip();
		break;
	}

	itemBox->removeAllItems();
	std::map<std::string,int> skillmap;
	skillmap=army->getSkillTable();
	std::vector<std::string> child=DataLibrary::getSingletonPtr()->getChildList(path);
	for(std::vector<std::string>::iterator it=child.begin();it!=child.end();it++)
	{
		int level;
		DataLibrary::getSingletonPtr()->getData(path+"/"+(*it),level);
		SkillItemData* data;
		if(type==SKILLTYPE_PASSIVE)
		{
			std::string effectName;
			DataLibrary::getSingletonPtr()->getData(path+"/"+(*it)+"/Effect",effectName);
			data=new SkillItemData(type,effectName,level,army->getLevel(),(*it),totalPoint);
		}
		else
		{
			data=new SkillItemData(type,(*it),level,army->getLevel(),(*it),totalPoint);
		}
	
		if(skillmap.find((*it))!=skillmap.end())
		{
			data->setCanTrain(false);
		}
		else
		{
			data->setCanTrain(true);
		}

		itemBox->addItem(data);
	}
}
