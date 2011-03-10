#pragma once
#include "gamestate.h"

class Terrain;
class CameraContral;
class MapDataManager;

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

private:

	MapDataManager *mMapDataManager; //�����ͼ����
	Terrain *mTerrain; //������ͼ
	CameraContral *mCameraContral; //���������

	SquadGrapManager* mUnitGrapManager;
};