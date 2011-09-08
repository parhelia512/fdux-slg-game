#pragma once

#include <ogre.h>
#include <string>
#include <vector>
#include <map>

#include <ParticleUniverseSystem.h>
#include <ParticleUniverseSystemListener.h>

#include "WeaponGrap.h"

class AnimationBlender;

//��λ��,�ɵ�λ���С��
class UnitGrap//:public ParticleUniverse::ParticleSystemListener 
{
public:
	UnitGrap(std::string unitmesh, std::string unitmat,std::string factiontex,Ogre::SceneNode* node);
	~UnitGrap(void);
	
	enum WeaponType
	{
		NoneWepon,
		MainWepon,
		SecWepon,
		Shield
	};

	enum BoneType
	{
		LeftHand,
		RightHand
	};

	void createWeapon(std::string mesh, std::string mat,WeaponType type);

	void setWeapon(WeaponType type,BoneType bone);

	void setAniGroup(std::string anigroup);

	void setMovePath(std::map<int,Ogre::Vector3>& vectors,std::map<int,Ogre::Quaternion>& quaternions,float MoveSpeed=1.0);

	void setPosition(float x,float z);

	void setHeight();

	void setPositionOffset(float ox,float oy);

	void setAnimation(std::string name,bool loop,bool returnInit);

	void stopTransform();

	void update(unsigned int deltaTime);

	Ogre::SceneNode* mNode;
	Ogre::Entity* mUnitEntity;
	bool mIsCheckHeight;

	WeaponGrap* mMainWeapon;
	WeaponGrap* mSecWeapon;
	WeaponGrap* mShield;
	std::string mLeftHandBoneName;
	std::string mRightHandBoneName;
	std::string mWalkName;
	std::string mAttackName;
	std::string mDeathName;
	std::string mRecoverName;
	std::string mIdleName;
	int mFormationPosition;
	float mOffsetX;
	float mOffsetY;

	AnimationBlender* mAniBlender;//��������
	Ogre::Animation* mNodeAnimation;
	Ogre::AnimationState* mNodeAnimationState;

	bool mReturnInitAni;
	bool mIsAnimationComplete;


	void setParticleVisible(bool visible);
	bool mParticleVisible;

	bool addParticle(std::string id,std::string name);
	void startParticle(std::string id);
	void stopParticle(std::string id);
	void delParticle(std::string id);

	Ogre::SceneNode* mParticleNode;
	typedef std::map<std::string, ParticleUniverse::ParticleSystem*> PUMap;
	PUMap mPUMap;
	typedef std::map<std::string, ParticleUniverse::ParticleSystem*>::iterator PUMapIte;
};