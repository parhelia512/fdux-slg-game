#pragma once

#include <ogre.h>
#include <string>

//������
class WeaponGrap
{
public:
	WeaponGrap(std::string name);
	~WeaponGrap(void);

	void attachWeapon(Ogre::Entity* body,std::string boneName);

	void detachWeapon(Ogre::Entity* body);

	std::string getAttachBoneName();

private:
	std::string mBoneName;
	Ogre::Entity* mWeaponEntity;
};
