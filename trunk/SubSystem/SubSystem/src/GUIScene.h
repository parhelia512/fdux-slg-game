#pragma once

#include "GUICommon.h"
#include <MyGUI.h>

class GUIScene
{
public:
	GUIScene(){}

	virtual ~GUIScene(void){}

	virtual void showScene()=0;//��ʾ����
	virtual void hideScene()=0;//���س���

	virtual void FrameEvent()=0;//֡�¼�

	virtual GUISceneType getType()=0;

	MyGUI::VectorWidgetPtr getWidgetVector()
	{
		return mWidgetVector;
	}

protected:
	MyGUI::VectorWidgetPtr mWidgetVector;
};
