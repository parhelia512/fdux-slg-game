#pragma once

#include "cutscene.h"
#include "squaddefine.h"

class SquadGraphics;

class DirectionCutScene:public CutScene
{
public:

	DirectionCutScene(unsigned int id,Direction direction);
	~DirectionCutScene(void);

protected:
	void startCutScence();
	bool endCutScene();
	void skipCutScene();
	void updateCutScene(unsigned int deltaTime);

	SquadGraphics* mSquadGraphics;
	Direction mDirection;
};