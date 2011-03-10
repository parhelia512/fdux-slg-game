#pragma once

#include <ogre.h>
#include <string>

//������
class WeaponGrap
{
public:
	WeaponGrap(std::string name);
	~WeaponGrap(void);

	std::string name;
	Ogre::Entity* mWeaponEntity;
};
