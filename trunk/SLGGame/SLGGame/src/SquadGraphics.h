#pragma once

#include <Ogre.h>

#include <string>
#include <vector>

//С�ӵ�ͼ����,����һ��ͼ�λ

class UnitGrap;
class SquadGrapManager;

class SquadGraphics
{
public:
	friend SquadGrapManager;//���޶���λ���������Թ��쵥λ

	~SquadGraphics(void);

	enum WeaponMode
	{
		MainWepon,
		SceWepon
	};

	void setMovePath();

	void setAnimation(std::string name);

	void setEffect(std::string name,std::string position);

	void setGrid(int x,int y);

	void setScale(Ogre::Vector3 scale,bool isAnim);

	void setDirection(Ogre::Vector3 d,bool isAnim);

	void setHealth(int health);

	void setWeaponMode(WeaponMode mode);

private:
	SquadGraphics(std::string unitName,unsigned int index);

	void update(unsigned int deltaTime);

	Ogre::SceneNode* mNode;
	Ogre::SceneManager* mSceneMgr;

	UnitGrap* mCommanderUnit;
	std::vector<UnitGrap*> mSoldierUnits;



};
