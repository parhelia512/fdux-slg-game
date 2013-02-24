#pragma once

#include "GUISystem.h"
#include "GUIStage.h"
#include "LuaSystem.h"
#include "AudioSystem.h"
#ifndef SCRIPT_EDITOR
#include "StateManager.h"
#endif

#define DefaultShowTextTime 0.1//Ĭ�ϴ���Ч���ٶ�
#define DefaultCursorTime 0.01//Ĭ�Ϲ����˸Ƶ��

#include <Windows.h>

#include "UtilsOgreDshow.h"

extern "C"
{

	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>

	static GUIDialogAttribute mAttribute;//�ı���ʾ����
	static GUIImageAttribute mImageAttribute;//ÿ��λ�ô�Ŵ�
	static bool mPrinter=true;//�Ƿ�򿪴���Ч��
	static bool mIsShowTextOver=false;//�Ƿ��Ѿ�������ʾ,�ȴ��������?
	
	static int SetTextBgRect(lua_State* L)//�趨�ı�����λ��
	{
		mAttribute.TextBgLeft=luaL_checkint(L, 1);
		mAttribute.TextBgTop=luaL_checkint(L,2);
		mAttribute.TextBgWidth=luaL_checkint(L,3);
		mAttribute.TextBgHeight=luaL_checkint(L,4);

		return 1;
	}

	static int SetTextBgImage(lua_State* L)//�趨�ı�����
	{
		mAttribute.TextBgImage=std::string(luaL_checkstring(L,1));

		return 1;
	}

	static int SetTextRect(lua_State* L)//�趨����λ��
	{
		mAttribute.TextLeft=luaL_checkint(L, 1);
		mAttribute.TextTop=luaL_checkint(L,2);
		mAttribute.TextWidth=luaL_checkint(L,3);
		mAttribute.TextHeight=luaL_checkint(L,4);

		return 1;
	}

	static int SetRoleNameRect(lua_State* L)//�趨��ɫ����λ��
	{
		mAttribute.RoleNameLeft=luaL_checkint(L, 1);
		mAttribute.RoleNameTop=luaL_checkint(L,2);
		mAttribute.RoleNameWidth=luaL_checkint(L,3);
		mAttribute.RoleNameHeight=luaL_checkint(L,4);

		return 1;
	}

	static int SetUITextColor(lua_State* L)//�����ı���ɫ
	{
		mAttribute.TextRed=luaL_checknumber(L, 1);
		mAttribute.TextGreen=luaL_checknumber(L,2);
		mAttribute.TextBlue=luaL_checknumber(L,3);

		return 1;
	}

	static int SetRoleNameColor(lua_State* L)//���ý�ɫ������ɫ
	{
		mAttribute.RoleNameRed=luaL_checknumber(L, 1);
		mAttribute.RoleNameGreen=luaL_checknumber(L,2);
		mAttribute.RoleNameBlue=luaL_checknumber(L,3);

		return 1;
	}

	static int SetFont(lua_State* L)//��������
	{
		mAttribute.TextFont=std::string(luaL_checkstring(L,1));
		mAttribute.RoleFont=std::string(luaL_checkstring(L,2));

		return 1;
	}

	static int SetDialogAttribute(lua_State* L)//�ύ�ı�
	{
		GUIStage* stage=static_cast<GUIStage*>(GUISystem::getSingletonPtr()->getScene(StageScene));
		if(stage!=NULL)
			stage->setTextDialog(mAttribute);

		return 1;
	}

	static void ShowTextBreakup()//�ı�ֱ��ģʽ�ص�
	{
		GUIStage* stage=static_cast<GUIStage*>(GUISystem::getSingletonPtr()->getScene(StageScene));
		if(stage==NULL)
			return ;

		if(stage->CheckMouseState())
		{

			if (stage->mTimerWork==GUIStage::PrinterWork)//��û��ʾ��Ͱ����?
			{
				stage->showOtherText();
				LuaSystem::getSingletonPtr()->LuaBreakupFun=NULL;
			}
			else if (stage->mTimerWork==GUIStage::NoneWork)
			{
				LuaSystem::getSingletonPtr()->LuaBreakupFun=NULL;
			}
		}
		else if(stage->mTimerWork==GUIStage::NoneWork)//��ʾ����
		{
			LuaSystem::getSingletonPtr()->LuaBreakupFun=NULL;
		}
	}

	static void ShowWaitTextBreakup()//�ı��ȴ�ģʽ�ص�
	{
		GUIStage* stage=static_cast<GUIStage*>(GUISystem::getSingletonPtr()->getScene(StageScene));
		if(stage==NULL)
			return ;

		if(stage->CheckMouseState())
		{

			if (stage->mTimerWork==GUIStage::PrinterWork)//��û��ʾ��Ͱ����?
			{
				stage->showOtherText();
				stage->setCheckMouseDown();
			}
			else
			{
				stage->hideTextCursor();
				LuaSystem::getSingletonPtr()->LuaBreakupFun=NULL;
			}
		}
		else if(stage->mTimerWork==GUIStage::NoneWork && mIsShowTextOver)//��ʾ����,�ȴ������
		{
			stage->showTextCursor(true);
			mIsShowTextOver=false;
		}
	}

	static void ShowImageBreakup()//ͼƬ�ص�
	{
		GUIStage* stage=static_cast<GUIStage*>(GUISystem::getSingletonPtr()->getScene(StageScene));
		if(stage==NULL)
			return ;
		if (stage->mTimerWork==GUIStage::NoneWork)
		{
			LuaSystem::getSingletonPtr()->LuaBreakupFun=NULL;
		}
	}
	
	static void ClearTextBreakup()
	{
		GUIStage* stage=static_cast<GUIStage*>(GUISystem::getSingletonPtr()->getScene(StageScene));
		if(stage==NULL)
			return ;
		if (stage->CheckMouseState() || stage->mIsFastForward)
		{
			stage->hideTextCursor();
			stage->clearText();
			LuaSystem::getSingletonPtr()->LuaBreakupFun=NULL;
		}
	}

	static int SetDialogVisible(lua_State* L)//���öԻ��������,1��ʾ��,0��ʾ��
	{
		GUIStage* stage=static_cast<GUIStage*>(GUISystem::getSingletonPtr()->getScene(StageScene));
		if(stage==NULL)
			return 0;
		if (luaL_checkint(L, 1)==1)
		{
			stage->setTextDialogVisible(true);
		}
		else
		{
			stage->setTextDialogVisible(false);
		}

		LuaSystem::getSingletonPtr()->LuaBreakupFun=ShowImageBreakup;//����ͼƬ�����Ļص�

		return 1;
	}

	static int ShowImageSetting(lua_State* L)//ͼƬ����λ��
	{
		mImageAttribute.LeftLayerX=luaL_checkint(L, 1);
		mImageAttribute.LeftLayerY=luaL_checkint(L, 2);
		mImageAttribute.MidLayerX=luaL_checkint(L, 3);
		mImageAttribute.MidLayerY=luaL_checkint(L, 4);
		mImageAttribute.RightLayerX=luaL_checkint(L, 5);
		mImageAttribute.RightLayerY=luaL_checkint(L, 6);
		mImageAttribute.FadeTime=luaL_checkint(L, 7);

		return 1;
	}

	static int ShowImage (lua_State* L)//��ʾͼƬ
	{

		const char* imageName = luaL_checkstring(L, 1);
		int imageLayer =luaL_checkint(L, 2);
		float time =(float)luaL_checkint(L, 3);

		GUIStage* stage=static_cast<GUIStage*>(GUISystem::getSingletonPtr()->getScene(StageScene));
		if(stage==NULL)
			return 0;

		unsigned int left=0;
		unsigned int top=0;

		switch((GUIImageLayer)imageLayer)
		{
		case LeftLayer:
			{
				left=mImageAttribute.LeftLayerX;
				top=mImageAttribute.LeftLayerY;
				break;
			}
		case MidLayer:
			{
				left=mImageAttribute.MidLayerX;
				top=mImageAttribute.MidLayerY;
				break;
			}
		case RightLayer:
			{
				left=mImageAttribute.RightLayerX;
				top=mImageAttribute.RightLayerY;
				break;
			}
		}

		stage->showImage(imageName,(GUIImageLayer)imageLayer,time,left,top);

		LuaSystem::getSingletonPtr()->LuaBreakupFun=ShowImageBreakup;

		return 1;
	}

	static int ShockImage (lua_State* L)//��ͼƬ
	{
		int imageLayer =luaL_checkint(L,1);
		int durationTime =luaL_checkint(L, 2);
		float amplitudeX =(float)luaL_checknumber(L, 3);
		float amplitudeY =(float)luaL_checknumber(L, 4);

		GUIStage* stage=static_cast<GUIStage*>(GUISystem::getSingletonPtr()->getScene(StageScene));
		if(stage==NULL)
			return 0;

		stage->shockImage((GUIImageLayer)imageLayer,durationTime/1000.0,amplitudeX,amplitudeY);

		LuaSystem::getSingletonPtr()->LuaBreakupFun=ShowImageBreakup;

		return 1;
	}

	static int MoveImage (lua_State* L)//�ƶ�ͼƬ
	{
		int imageLayer =luaL_checkint(L, 1);
		float time =(float)luaL_checkint(L, 2);
		int left =luaL_checkint(L, 3);
		int top =luaL_checkint(L, 4);

		GUIStage* stage=static_cast<GUIStage*>(GUISystem::getSingletonPtr()->getScene(StageScene));
		if(stage==NULL)
			return 0;

		stage->moveImage((GUIImageLayer)imageLayer,time,left,top);

		LuaSystem::getSingletonPtr()->LuaBreakupFun=ShowImageBreakup;

		return 1;
	}

	static int ShowRole(lua_State* L)//��ʾͼƬ
	{
		const char* imageName = luaL_checkstring(L, 1);
		int imageLayer =luaL_checkint(L, 2);

		GUIStage* stage=static_cast<GUIStage*>(GUISystem::getSingletonPtr()->getScene(StageScene));
		if(stage==NULL)
			return 0;

		unsigned int left=0;
		unsigned int top=0;

		switch((GUIImageLayer)imageLayer)
		{
		case LeftLayer:
			{
				left=mImageAttribute.LeftLayerX;
				top=mImageAttribute.LeftLayerY;
				break;
			}
		case MidLayer:
			{
				left=mImageAttribute.MidLayerX;
				top=mImageAttribute.MidLayerY;
				break;
			}
		case RightLayer:
			{
				left=mImageAttribute.RightLayerX;
				top=mImageAttribute.RightLayerY;
				break;
			}
		}

		stage->showImage(imageName,(GUIImageLayer)imageLayer,mImageAttribute.FadeTime,left,top);

		LuaSystem::getSingletonPtr()->LuaBreakupFun=ShowImageBreakup;

		return 1;
	}

	static int ShowText (lua_State* L)//��ʾ�ı�
	{
		const char* text = luaL_checkstring(L, 1);
		int mode=luaL_checkint(L,2);
		
		//���ֽ�ת˫�ֽ�
		DWORD dwNum = MultiByteToWideChar (CP_ACP, 0, text, -1, NULL, 0);
		wchar_t *pwText;
		pwText = new wchar_t[dwNum];

		MultiByteToWideChar (CP_ACP, 0, text, -1, pwText, dwNum);

		GUIStage* stage=static_cast<GUIStage*>(GUISystem::getSingletonPtr()->getScene(StageScene));
		if(stage==NULL)
			return 0;
		if (mPrinter)
		{
			stage->showText(std::wstring(pwText),DefaultShowTextTime);
		}
		else
		{
			stage->showText(std::wstring(pwText),0);
		}

		delete []pwText;
	
		//mode=1 ��ͨģʽ,�ȴ�����Ч�����������ִ��
		//mode=2 �ȴ�ģʽ,�ȴ��û�������
		
		if(!stage->mIsFastForward)//������ǿ��ģʽ
		{
			stage->setCheckMouseDown();

			if(mode==1)
			{
				LuaSystem::getSingletonPtr()->LuaBreakupFun=ShowTextBreakup;
			}
			else if (mode==2)
			{
				mIsShowTextOver=true;//�ȴ�ʱ��ʾ���
				LuaSystem::getSingletonPtr()->LuaBreakupFun=ShowWaitTextBreakup;
			}

		}
		else
		{
			LuaSystem::getSingletonPtr()->LuaBreakupFun=ShowImageBreakup;
		}

		return 1;
	}

	static int ClearText(lua_State* L)//����ı�
	{
		GUIStage* stage=static_cast<GUIStage*>(GUISystem::getSingletonPtr()->getScene(StageScene));
		if(stage==NULL)
			return 0;
		stage->showTextCursor(false);
		stage->setCheckMouseDown();
		LuaSystem::getSingletonPtr()->LuaBreakupFun=ClearTextBreakup;
		return 1;
	}
	
	static int Wait(lua_State* L)//�ȴ�ʱ��
	{
		float time=(float)luaL_checkint(L, 1);

		GUIStage* stage=static_cast<GUIStage*>(GUISystem::getSingletonPtr()->getScene(StageScene));
		if(stage==NULL)
			return 0;
		stage->waitTime(time);
		LuaSystem::getSingletonPtr()->LuaBreakupFun=ShowImageBreakup;
		return 1;
	}

	static int ShowRoleName(lua_State* L)//���ý�ɫ����
	{

		const char* text = luaL_checkstring(L, 1);

		//���ֽ�ת˫�ֽ�
		DWORD dwNum = MultiByteToWideChar (CP_ACP, 0, text, -1, NULL, 0);
		wchar_t *pwText;
		pwText = new wchar_t[dwNum];

		MultiByteToWideChar (CP_ACP, 0, text, -1, pwText, dwNum);

		GUIStage* stage=static_cast<GUIStage*>(GUISystem::getSingletonPtr()->getScene(StageScene));
		if(stage==NULL)
			return 0;
		stage->showRoleName(std::wstring(pwText));

		delete []pwText;

		LuaSystem::getSingletonPtr()->LuaBreakupFun=ShowImageBreakup;

		return 1;
	}

	static int SetText(lua_State* L)//�����ı�����,����Ƕ�ǰ�漸������һ����װ
	{
		mAttribute.TextFont = std::string(luaL_checkstring(L, 1));
		mAttribute.TextLeft=luaL_checkint(L, 2);
		mAttribute.TextTop=luaL_checkint(L,3);
		mAttribute.TextWidth=luaL_checkint(L,4);
		mAttribute.TextHeight=luaL_checkint(L,5);
		bool type=lua_toboolean(L,6);
		if (type)
		{
			mPrinter=true;
		}
		else
		{
			mPrinter=false;
		}

		GUIStage* stage=static_cast<GUIStage*>(GUISystem::getSingletonPtr()->getScene(StageScene));
		if(stage==NULL)
			return 0;
		stage->setTextDialog(mAttribute);

		return 1;
	}

	static int Music(lua_State* L)
	{
		std::string name = std::string(luaL_checkstring(L, 1));
		int time=luaL_checkint(L, 2);
		bool ret=AudioSystem::getSingletonPtr()->playStream(name,true,time);

		if (!ret)
		{
			luaL_error(L, "play music fail:%s,error:%s", name.c_str(),AudioSystem::getSingletonPtr()->getError(true).c_str());
		}
		return 1;
	}

	static int SE(lua_State* L)
	{
		std::string name = std::string(luaL_checkstring(L, 1));
		bool ret=AudioSystem::getSingletonPtr()->playSample(name,false);

		if (!ret)
		{
			luaL_error(L, "play se fail:%s,error:%s", name.c_str(),AudioSystem::getSingletonPtr()->getError(false).c_str());
		}
		return 1;
	}

	static int CM(lua_State* L)
	{
		int time=luaL_checkint(L, 1);
		bool ret=AudioSystem::getSingletonPtr()->stopStream(time);
	
		if (!ret)
		{
			luaL_error(L, "No Music Playing!");
		}
		return 1;
	}

	static int NextScene(lua_State* L)
	{
		std::string name = std::string(luaL_checkstring(L, 1));
		
		LuaSystem::getSingletonPtr()->runScriptFromFile(name,0);

		return 1;
	}

	static int AutoSave(lua_State* L)
	{
		GUIStage* stage=static_cast<GUIStage*>(GUISystem::getSingletonPtr()->getScene(StageScene));
		if(stage==NULL)
			return 0;
		stage->saveData(true);

		return 1;
	}

	static int StoryOver(lua_State* L)
	{
#ifndef SCRIPT_EDITOR
		StateManager::getSingletonPtr()->removeAffixationState();
#endif
		
		return 1;
	}

	static int ButtonLock(lua_State* L)
	{
		GUIStage* stage=static_cast<GUIStage*>(GUISystem::getSingletonPtr()->getScene(StageScene));
		if(stage==NULL)
			return 0;
		stage->setButtonLock(false);

		return 1;
	}

	static int SetShowSupplyButton(lua_State* L)
	{
		int v=luaL_checkint(L, 1);
		GUIStage* stage=static_cast<GUIStage*>(GUISystem::getSingletonPtr()->getScene(StageScene));
		if(stage==NULL)
			return 0;
		if(v==1)
			stage->setIsShowSupplyButton(true);
		else
			stage->setIsShowSupplyButton(false);

		return 1;
	}

	BOOL FindFirstFileExists(LPCTSTR lpPath, DWORD dwFilter)
	{
		WIN32_FIND_DATA fd;
		HANDLE hFind = FindFirstFile(lpPath, &fd);
		BOOL bFilter = (FALSE == dwFilter) ? TRUE : fd.dwFileAttributes & dwFilter;
		BOOL RetValue = ((hFind != INVALID_HANDLE_VALUE) && bFilter) ? TRUE : FALSE;
		FindClose(hFind);
		return RetValue;
	}

	static void PlayMovieBreakup()
	{
		if (OgreUtils::DirectShowManager::getSingleton().isPlayOver())
		{
			OgreUtils::DirectShowManager::getSingleton().DestroyAll();
			LuaSystem::getSingletonPtr()->LuaBreakupFun=NULL;
		}
	}

	static int PlayMovie(lua_State* L)
	{
		const char* name=luaL_checkstring(L, 1);

		DWORD dwNum = MultiByteToWideChar (CP_ACP, 0, name, -1, NULL, 0);
		wchar_t *pwText;
		pwText = new wchar_t[dwNum];

		MultiByteToWideChar (CP_ACP, 0, name, -1, pwText, dwNum);

		if(FindFirstFileExists(pwText, FALSE))
		{
			OgreUtils::DirectShowManager::getSingleton().createDirectshowControl("videotest1",name,1280,720);
		}
		else
		{
			OgreUtils::DirectShowManager::getSingleton().createDirectshowControl("videotest1","../Media/movie/op.wmv",1280,720);
		}

		LuaSystem::getSingletonPtr()->LuaBreakupFun=PlayMovieBreakup;
		return 1;
	}

	static const struct luaL_Reg GUILib[] =
	{
		{"Music",Music},
		{"SE",SE},
		{"CM",CM},

		{"ShowImageSetting",ShowImageSetting},
		{"ShowImage",ShowImage},
		{"ShockImage",ShockImage},
		{"MoveImage",MoveImage},
		{"ShowRole",ShowRole},
		{"ShowText",ShowText},
		{"SetText",SetText},
		{"ShowRoleName",ShowRoleName},
		{"ClearText",ClearText},
		
		{"SetTextBgImage",SetTextBgImage},
		{"SetTextBgRect",SetTextBgRect},
		{"SetTextRect",SetTextRect},
		{"SetRoleNameRect",SetRoleNameRect},
		{"SetUITextColor",SetUITextColor},
		{"SetRoleNameColor",SetRoleNameColor},
		{"SetFont",SetFont},
		{"SetDialogAttribute",SetDialogAttribute},
		{"SetDialogVisible",SetDialogVisible},
		{"Wait",Wait},

		{"NextScene",NextScene},
		{"AutoSave",AutoSave},
		{"ButtonLock",ButtonLock},
		{"StoryOver",StoryOver},
		{"SetShowSupplyButton",SetShowSupplyButton},

		{"PlayMovie",PlayMovie},

		{NULL,NULL}
	};

};