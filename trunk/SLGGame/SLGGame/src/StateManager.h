#pragma once

#include <string>

#include "GameState.h"

#include "iisingleton.hpp"

using namespace izayoi;

//״̬������,���ڹ������״̬
class StateManager:public IISingleton<StateManager>
{
public:
	StateManager(void);
	~StateManager(void);
	
	enum StateType//״̬ö��
	{
		None,
		Menu,
		AVG,
		Battle,
		Supply
	};

	//�ı���״̬
	void changeState(std::string arg,StateType type);
	
	//��Ӹ���״̬
	void addAffixationState(std::string arg,StateType type);
	
	//�Ƴ�����״̬
	void removeAffixationState();
	
	//֡����
	void StateUpdate(unsigned int deltaTime);

private:
	GameState* CreateState(StateType type);

	GameState* mBaseState;
	GameState* mAffixationState;
};
