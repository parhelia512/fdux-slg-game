#pragma once

#include <string>

#include "squaddefine.h"

class BattleSquad
{
public:
	BattleSquad(std::string id, int grapid,int x, int y);
	~BattleSquad();

	std::string getId() {return mId;}

	std::string getPath();

	void setCrood(int x, int y);
	void getCrood(int* x, int* y);

	void setDirection(Direction d);
	Direction getDirection();

	void setFormation(Formation f);
	Formation getFormation();

	std::string getSquadName();
	std::string getFactionId();
	int getGrapId() { return mGrapId; }

	float getAttr(AttrType attrtype, AttrCalcType calctype, Direction direction);

	bool applyModifier(AttrModifier* modifier,std::string &id);
	bool clearModifier(std::string id);

	int getUnitGrapNum();

	void newTurn();
	int getTeam();

	bool viewbyTeam(int team);

	float getActionPoint();
	float getActionPointCost(int type);

	SquadType getType();
private:
	int mX;
	int mY;
	int mGrapId;
	int mTeam;
	std::string mId;

	float mCovert;
	float mDetection;
	bool mViewbyFaction[4];
};