#pragma once

#include <string>

#include "GameState.h"

//״̬������,���ڹ������״̬
class StateManager
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
	void StateUpdate();

private:
	GameState* CreateState(StateType type);

	GameState* mBaseState;
	GameState* mAffixationState;
};
