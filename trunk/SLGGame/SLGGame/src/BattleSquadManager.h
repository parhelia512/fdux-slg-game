#pragma once

#include "iisingleton.hpp"
using namespace izayoi;

#include <string>
#include <map>
#include <vector>

#include <Ogre.h>

#include "squaddefine.h"
#include "Area.h"

class BattleSquad;
class CutScene;

class BattleSquadManager:public IISingleton<BattleSquadManager>
{
public:
	BattleSquadManager();
	~BattleSquadManager();

	void clear();

	typedef std::map<std::string, BattleSquad*>::iterator BattleSquadIte;
	std::map<std::string, BattleSquad*> mSquadList;

	bool allDeployed();

	BattleSquad* getBattleSquad(std::string id);
	BattleSquad* getBattleSquadAt(int x, int y, int faction, bool visibleonly);

	int getTeamSquadLeft(int team);

	void dumpSquadData();

	//�غ�
	void turnEnd(int team);

	//�ƶ����
public:
	struct MoveNode
	{
		int x;
		int y;
		int lastx;
		int lasty;
		float apleft;
		unsigned int eventflag; //ʹ�÷�ʽδ��
	};
	std::map<int, MoveNode> getMoveArea(BattleSquad* squad);
	std::vector<MoveNode> getMovePath(BattleSquad* squad, int x, int y);
	std::map<int, MoveNode> getFullPath(BattleSquad* squad, Crood tatget);
	void moveSquad(BattleSquad* squad, std::vector<int> pointlist, unsigned int &stopedpoint, unsigned int &eventflag);
	void InterruptMove();

private:
	bool m_moveInterrupt;
	BattleSquad* m_moveSquad;
	std::vector<Ogre::Vector2> m_moveList;
	Ogre::Vector2 m_startPoint;

	//�л�����
public:
	void changeFormation(BattleSquad* squad, int formation, bool costap);

public:
	void setDirection(BattleSquad* squad, int direction);

	//�������
public:
// 	bool useSkillOn(BattleSquad* attacksquad, BattleSquad* targetsquad, std::string skillid);
// 	bool useSkillAt(BattleSquad* attacksquad, int x, int y, std::string skillid);
	struct SkillNode
	{
		int x;
		int y;
		bool validTarget;
		int direction;
	};
	std::vector<SkillNode> getSkillArea(BattleSquad* squad, std::string skillid);
	std::vector<SkillNode> getSkillTargetArea(BattleSquad* squad, std::string skillid, int x, int y);
	void useSkill(BattleSquad* squad, std::string skillid, int x, int y, unsigned int &eventflag);


	//ս����ؽű�����
	bool dealMeleeDamage(BattleSquad* attacksquad, BattleSquad* defenesquad);
	
	bool dealMagicDamage(BattleSquad* attacksquad, BattleSquad* defenesquad, int attacktime, float atk, int randomness);

	bool dealRangedDamage(BattleSquad* attacksquad, BattleSquad* defenesquad);

	void setUnitNum(BattleSquad* squad, int unitnum);

	void rangedCutScene(BattleSquad* attacksquad, int x, int y, UnitType castunit , std::string castparticle, std::string castaction, std::string castsound, int missiletype, std::string missileres, std::string hitparticle,std::string hitsound);
};