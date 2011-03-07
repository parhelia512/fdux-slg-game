#pragma once

#include <vector>

#include "GamePlayer.h"

//�غϿ�����,����غϿ����Լ���ҽ���
class TurnControl
{
public:
	TurnControl(void);
	~TurnControl(void);
	
	//��ʼ���غ�
	void initTurn();

	//�л�����һ�����
	void nextTurn();

	//���浱ǰ�غ�״̬
	void saveTurn();

	//��ȡ�غ�״̬
	void loadTurn();
	
	//��ȡ��ǰ���
	GamePlayer* getCurrencyPlayer();

private:

	int mTurnNum;

	std::vector<GamePlayer*> mPlayerList;

	std::vector<GamePlayer*>::iterator mPlayerIt;

};
