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
class UnitGrap:public ParticleUniverse::ParticleSystemListener 
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

	void setEffect(std::string name);

	bool isEffectOver();

	void stopEffect();

	void setMovePath(std::map<int,Ogre::Vector3>& vectors,std::map<int,Ogre::Quaternion>& quaternions);

	void setPosition(float x,float z);

	void setHeight();

	void setPositionOffset(float ox,float oy);

	void setAnimation(std::string name,bool loop,bool returnInit);

	void stopTransform();

	void setFadeInOut(bool isIn);

	void doFadeInOut(unsigned int deltaTime);

	void update(unsigned int deltaTime);

	void handleParticleSystemEvent (ParticleUniverse::ParticleSystem *particleSystem, ParticleUniverse::ParticleUniverseEvent &particleUniverseEvent);

	Ogre::SceneNode* mNode;
	Ogre::Entity* mUnitEntity;
	bool mIsCheckHeight;
	float mAlpha;
	unsigned int mAlphaDeltaTime;
	bool mFadeInOut;

	WeaponGrap* mMainWeapon;
	WeaponGrap* mSecWeapon;
	WeaponGrap* mShield;
	std::string mLeftHandBoneName;
	std::string mRightHandBoneName;
	std::string mWalkName;
	std::string mDeathName;
	std::string mRecoverName;
	std::string mRotationName;
	std::string mIdleName;
	int mFormationPosition;
	float mOffsetX;
	float mOffsetY;

	AnimationBlender* mAniBlender;//��������
	Ogre::Animation* mNodeAnimation;
	Ogre::AnimationState* mNodeAnimationState;

	ParticleUniverse::ParticleSystem* mPUSystem;
	bool mPUSystemEnd;
	bool mReturnInitAni;
};
