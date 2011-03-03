#include "AVGState.h"

#include "LuaSystem.h"
#include "GUIStage.h"

AVGState::AVGState(void)
{
}

AVGState::~AVGState(void)
{
}

void AVGState::initialize( std::string arg )
{
	std::queue<std::string> q;
	split(arg,'.',q);

	if (q.size()==2)
	{
		q.pop();

		//��ʾ�Ի���
		GUIStage* stageScene=static_cast<GUIStage*>(GUISystem::getSingletonPtr()->createScene(StageScene));

		if (q.front()=="lua")//���ļ���ͷ��ȡ
		{
			LuaSystem::getSingletonPtr()->runScriptFromFile(arg,0);
		}
		else if(q.front()=="sav")//���м��ȡ
		{

		}
	}
}

void AVGState::uninitialize()
{

}

void AVGState::update()
{

}

void AVGState::split(const std::string& s, char c,std::queue<std::string>& v) 
{
	std::string::size_type i = 0;
	std::string::size_type j = s.find(c);

	while (j != std::string::npos) {
		v.push(s.substr(i, j-i));
		i = ++j;
		j = s.find(c, j);

		if (j == std::string::npos)
			v.push(s.substr(i, s.length( )));
	}
}