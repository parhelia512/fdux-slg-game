#include "WeaponGrapIG.h"

#include "Core.h"
#include "DataLibrary.h"

#include <OgreTagPoint.h>

WeaponGrapIG::WeaponGrapIG(std::string name)
{
	std::string mesh;
	std::string mat;
	DataLibrary::getSingletonPtr()->getData(std::string("GameData/Weapon/")+name+std::string("/MeshName"),mesh);
	DataLibrary::getSingletonPtr()->getData(std::string("GameData/Weapon/")+name+std::string("/MatName"),mat);

	mWeaponEntity=Core::getSingletonPtr()->mSceneMgr->createEntity(mesh);
	if (mat!="null")
	{
		mWeaponEntity->setMaterialName(mat);
	}
}

WeaponGrapIG::~WeaponGrapIG(void)
{
	Core::getSingletonPtr()->mSceneMgr->destroyEntity(mWeaponEntity);
}

void WeaponGrapIG::attachWeapon( Ogre::Entity* body,std::string boneName )
{
	body->attachObjectToBone(boneName,mWeaponEntity);
	mBoneName=boneName;

	//ParticleUniverse::ParticleSystem* pSys = Core::getSingletonPtr()->createPUSystem(mWeaponEntity->getName()+"_"+Ogre::StringConverter::toString(1),"PUMediaPack/Fireplace_01");
	//Ogre::TagPoint* p= body->attachObjectToBone(boneName,pSys);
	//pSys->setScale(Ogre::Vector3(0.01,0.01,0.01));
	//pSys->setScaleVelocity(0.01);
	//pSys->prepare();
	//pSys->start();

}

void WeaponGrapIG::detachWeapon(Ogre::Entity* body)
{
	body->detachObjectFromBone(mWeaponEntity);
	mBoneName="";
}

std::string WeaponGrapIG::getAttachBoneName()
{
	return mBoneName;
}

void WeaponGrapIG::startFade()
{
	for(int i = 0; i < mWeaponEntity->getNumSubEntities(); ++i)
	{
		Ogre::MaterialPtr p;
		if (Ogre::MaterialManager::getSingletonPtr()->resourceExists(mWeaponEntity->getSubEntity(i)->getMaterial()->getName()+"_Copy"))
		{
			p=Ogre::MaterialManager::getSingletonPtr()->getByName(mWeaponEntity->getSubEntity(i)->getMaterial()->getName()+"_Copy");
		}
		else
		{
			p=mWeaponEntity->getSubEntity(i)->getMaterial()->clone(mWeaponEntity->getSubEntity(i)->getMaterial()->getName()+"_Copy");
		}

		Ogre::Pass *pass = p->getTechnique(0)->getPass(0);
		pass->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA); 
		pass->setDepthCheckEnabled(true); 
		pass->setDepthWriteEnabled(false);
		mWeaponEntity->getSubEntity(i)->setMaterial(p);
	}
}

void WeaponGrapIG::overFade(bool isV)
{
	for(int i = 0; i < mWeaponEntity->getNumSubEntities(); ++i)
	{
		std::string n = mWeaponEntity->getSubEntity(i)->getMaterial()->getName();
		n.replace(n.find("_Copy"),5,"");
		mWeaponEntity->getSubEntity(i)->setMaterialName(n);
		Ogre::MaterialManager::getSingletonPtr()->remove(mWeaponEntity->getSubEntity(i)->getMaterial()->getName()+"_Copy");
	}
	mWeaponEntity->setVisible(isV);
}

void WeaponGrapIG::doFade(float alpha)
{
	for(int i = 0; i < mWeaponEntity->getNumSubEntities(); ++i)
	{
		Ogre::Pass *pass = mWeaponEntity->getSubEntity(i)->getMaterial()->getTechnique(0)->getPass(0);
		pass->setDiffuse(pass->getDiffuse().r, pass->getDiffuse().g, pass->getDiffuse().b, alpha); 
	}
}
