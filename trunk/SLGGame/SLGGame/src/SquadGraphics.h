#pragma once

#include <Ogre.h>
#include <ParticleUniverseSystem.h>
#include <ParticleUniverseSystemListener.h>

#include <string>
#include <vector>

//С�ӵ�ͼ����,����һ��ͼ�λ

class UnitGrap;
class SquadGrapManager;

class SquadGraphics:public ParticleUniverse::ParticleSystemListener 
{
public:
	friend SquadGrapManager;//���޶���λ���������Թ��쵥λ

	~SquadGraphics(void);

	enum WeaponMode
	{
		MainWepon,
		SceWepon
	};

	enum Object
	{
		Commander,
		Soldier,
		Squad
	};

	enum Direction
	{
		North,
		South,
		West,
		East
	};

	enum Formation
	{
		Line,
		Circular,
		Loose
	};

	void setMovePath(std::map<int,Ogre::Vector3>& vectors,std::map<int,Ogre::Quaternion>& quaternions);
	void setDirection(Direction d,bool isAnim);
	void setFormation(Formation f,bool isAnim);
	bool isTransformOver();

	void setAnimation(std::string name,Object object,bool isLoop);
	bool isAnimationOver(Object object);
	void setInitAnimation(Object object);

	void setEffect(std::string name,Object object);
	bool isEffectOver(Object object);
	void stopEffect(Object object);

	void setGrid(int x,int y);

	void setScale(Ogre::Vector3 scale,bool isAnim);

	void setHealth(int health);

	void setWeaponMode(WeaponMode mode);

private:
	SquadGraphics(std::string unitName,Ogre::Vector2& grid,Direction direction,Formation f,unsigned int index);

	void update(unsigned int deltaTime);

	void handleParticleSystemEvent (ParticleUniverse::ParticleSystem *particleSystem, ParticleUniverse::ParticleUniverseEvent &particleUniverseEvent);

	void setCheckUnitHeight(bool enable);

	unsigned int mID;

	Ogre::SceneNode* mNode;
	Ogre::SceneManager* mSceneMgr;

	UnitGrap* mCommanderUnit;
	std::vector<UnitGrap*> mSoldierUnits;
	
	ParticleUniverse::ParticleSystem* mPUSystem;
	bool mPUSystemEnd;

	Ogre::Animation* mNodeAnimation;
	Ogre::AnimationState* mNodeAnimationState;

};
