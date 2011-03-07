#pragma once

#include "GUICommon.h"
#include "GUISystem.h"
#include <MyGUI.h>

class GUIScene
{
public:
	GUIScene(int Width,int Height,std::string layoutFile):mLayoutName(layoutFile),mWidth(Width),mHeight(Height)
	{
		mWidgetList=MyGUI::LayoutManager::getInstance().loadLayout(mLayoutName);
	}

	virtual ~GUIScene(void)
	{
		MyGUI::LayoutManager::getInstance().unloadLayout(mWidgetList);
		mWidgetList.clear();
	}

	virtual void showScene(std::string arg)=0;//��ʾ����
	virtual void hideScene()=0;//���س���

	virtual void FrameEvent()=0;//֡�¼�

	virtual GUISceneType getType()=0;

	//��ǰ��ʱ���Ĺ���
	enum TimerWork
	{
		NoneWork,
		PrinterWork,//���ֻ�Ч��
		UniversalWork,//����Ч��
		FadeInOutWork,//�Ի�������
		WaitWork,//�ȴ�
		RoleNameWork,//���ֽ���
		ClearAllRoleWork//������еĽ�ɫͼƬ
	};
	TimerWork mTimerWork;

protected:
	template <typename T>
	void assignWidget(T * & _widget, const std::string& _name, bool _throw = true)
	{
		_widget = nullptr;
		for (MyGUI::VectorWidgetPtr::iterator iter = mWidgetList.begin(); iter != mWidgetList.end(); ++iter)
		{
			MyGUI::Widget* find = (*iter)->findWidget( _name);
			if (nullptr != find)
			{
				T* cast = find->castType<T>(false);
				if (nullptr != cast)
				{
					_widget = cast;
				}
				else if (_throw)
				{
					MYGUI_EXCEPT("Error cast : dest type = '" << T::getClassTypeName()
						<< "' source name = '" << find->getName()
						<< "' source type = '" << find->getTypeName() << "' in layout '" << mLayoutName << "'");
				}
				return;

			}
		}
		MYGUI_ASSERT( ! _throw, "widget name '" << _name << "' in layout '" << mLayoutName << "' not found.");
	}
	
	MyGUI::VectorWidgetPtr mWidgetList;
	std::string mLayoutName;

	int mWidth,mHeight;

	MyGUI::Timer mTimer;//��ʱ��
	float mTickTime;//���δ���ʱ��
	float mSetp;//���䲽
	bool mSetpDirection;//���䲽�ķ���,trueΪ������
	MyGUI::Widget* mFadeWidget;

	void FadeInOut()//����
	{
		if (mSetpDirection)//�ж����䲽����
		{
			if(mSetp+0.01f<=1.0)
			{
				mSetp+=0.01f;

				mFadeWidget->setAlpha(mSetp);
			}
			else
			{
				//ֹͣ֡����
				mTickTime=0;
				mTimerWork=NoneWork;
				GUISystem::getSingletonPtr()->setFrameUpdateScene(NoneScene);
			}
		}
		else
		{
			if(mSetp-0.01>=0)
			{
				mSetp-=0.01;

				mFadeWidget->setAlpha(mSetp);
			}
			else
			{

				mFadeWidget->setVisible(false);

				//ֹͣ֡����
				mTickTime=0;
				mTimerWork=NoneWork;
				GUISystem::getSingletonPtr()->setFrameUpdateScene(NoneScene);
			}

		}
	}
};

class GUISceneFactory
{
public:
	GUISceneFactory()
	{
	}

	virtual ~GUISceneFactory(void)
	{
	}

	virtual GUIScene* CreateScene(int Width,int Height)=0;
};