#pragma once
#include "gamestate.h"

#include <vector>
class SubBattleState;
class SquadGrapManager;

class BattleState :
	public GameState
{
public:

	BattleState(void);
	~BattleState(void);

	void initialize(std::string arg);

	void uninitialize();

	void update(unsigned int deltaTime);

	void ChangeState(SubBattleState* substate); //�л�״̬
	void PushState(SubBattleState* substate); //ѹ��״̬
	void PopState(); //ѹ��״̬

//	void setNextState(int statetype, std::string arg);

private:

	std::vector<SubBattleState*> mSubStateStack; //ս��״̬��ջ
	typedef std::vector<SubBattleState*>::iterator SubStateIte;

	SquadGrapManager* mSquadGrapManager;

// 	bool mIsEnd;
// 	bool mEndTrigger;
// 	int mStateType;
// 	std::string mArg;
};