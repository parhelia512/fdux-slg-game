#pragma once

#include <string>

//��Ϸ״̬����
class GameState
{
public:
	GameState();
	virtual ~GameState();
	
	virtual void initialize(std::string arg)=0;

	virtual void uninitialize()=0;

	virtual void update()=0;
};