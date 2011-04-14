#pragma once

#include <string>

#include "GUIScene.h"

class GUISubWindows:public GUIScene
{
public:
	GUISubWindows(MyGUI::Window* window,int Width,int Height):GUIScene(window,Width,Height)
	{

	};
	virtual ~GUISubWindows(){};

	//���������¼�,����ֵ��ʾ�Ƿ�������´���
	virtual bool GridInputEvent(int x,int y)=0;

	virtual std::string getName()=0;
};