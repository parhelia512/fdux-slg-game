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

	//���ڸ���״̬?
	bool hasAffixationState();
	
	//֡����
	void StateUpdate(unsigned int deltaTime);

	//�ļ����洦��
	void saveState(std::string file);

	//�ļ����봦��
	void loadState(std::string file);

	StateType curState() { return mCurState; }

private:
	GameState* CreateState(StateType type);

	GameState* mBaseState;
	GameState* mAffixationState;

	StateType  mCurState;
	StateType  mNextState;
	std::string mNextStateArg;
};
