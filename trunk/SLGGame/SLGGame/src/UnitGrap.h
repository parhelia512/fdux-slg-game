#pragma once

#include <ogre.h>
#include <string>
#include <vector>

#include <ParticleUniverseSystem.h> 

#include "WeaponGrap.h"

class AnimationBlender;

//��λ��,�ɵ�λ���С��
class UnitGrap
{
public:
	UnitGrap(std::string meshName,std::string matName,Ogre::SceneNode* node);
	~UnitGrap(void);
	
	enum WeaponType
	{
		NoneWepon,
		MainWepon,
		SecWepon,
		Shield
	};

	void createWeapon(std::string name,WeaponType type);

	void setWeapon(WeaponType type,std::string boneName);

	void setEffect(std::string name);

	void update(unsigned int deltaTime);

	Ogre::SceneNode* mNode;
	Ogre::Entity* mUnitEntity;

	WeaponGrap* mMainWeapon;
	WeaponGrap* mSecWeapon;
	WeaponGrap* mShield;

	AnimationBlender* mAniBlender;//��������
	ParticleUniverse::ParticleSystem* mPUSystem;
};
