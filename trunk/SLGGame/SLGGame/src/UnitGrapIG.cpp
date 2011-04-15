#include "UnitGrapIG.h"

#include "Core.h"
#include "AnimationBlender.h"
#include "Terrain.h"
#include "DataLibrary.h"

#include <iostream>

#define ALPHA_DELTA_TIME 100

UnitGrapIG::UnitGrapIG(std::string unitName,Ogre::SceneNode* node):
mNode(node),
mMainWeapon(NULL),
mSecWeapon(NULL),
mShield(NULL),
mPUSystem(NULL),
mPUSystemEnd(false),
mIsCheckHeight(false),
mFormationPosition(0),
mAlpha(-1),
mAlphaDeltaTime(0)
{
	std::string meshName;
	std::string matName;
	Ogre::Vector3 scale;
	std::string initAnimation;

	DataLibrary::getSingletonPtr()->getData(std::string("GameData/Unit/")+unitName+std::string("/MeshName"),meshName);
	DataLibrary::getSingletonPtr()->getData(std::string("GameData/Unit/")+unitName+std::string("/MatName"),matName);
	DataLibrary::getSingletonPtr()->getData(std::string("GameData/Unit/")+unitName+std::string("/Scale"),scale);
	DataLibrary::getSingletonPtr()->getData(std::string("GameData/Unit/")+unitName+std::string("/InitAnimation"),initAnimation);
	DataLibrary::getSingletonPtr()->getData(std::string("GameData/Unit/")+unitName+std::string("/LeftHandBone"),mLeftHandBoneName);
	DataLibrary::getSingletonPtr()->getData(std::string("GameData/Unit/")+unitName+std::string("/RightHandBone"),mRightHandBoneName);
	DataLibrary::getSingletonPtr()->getData(std::string("GameData/Unit/")+unitName+std::string("/WalkName"),mWalkName);
	DataLibrary::getSingletonPtr()->getData(std::string("GameData/Unit/")+unitName+std::string("/DeathName"),mDeathName);
	DataLibrary::getSingletonPtr()->getData(std::string("GameData/Unit/")+unitName+std::string("/RecoverName"),mRecoverName);
	DataLibrary::getSingletonPtr()->getData(std::string("GameData/Unit/")+unitName+std::string("/RotationName"),mRotationName);


	mUnitEntity=Core::getSingletonPtr()->mSceneMgr->createEntity(meshName);
	if (matName!="null")
	{
		mUnitEntity->setMaterialName(matName);
	}
	mNode->attachObject(mUnitEntity);

	mAniBlender=new AnimationBlender(mUnitEntity);
	mAniBlender->init(initAnimation);
	mNode->setScale(scale);
}

UnitGrapIG::~UnitGrapIG(void)
{	
	mUnitEntity->detachAllObjectsFromBone();

	if (mMainWeapon!=NULL)
	{
		delete mMainWeapon;
	}

	if (mSecWeapon!=NULL)
	{
		delete mSecWeapon;
	}

	if (mShield!=NULL)
	{
		delete mShield;
	}
	
	if (mPUSystem!=NULL)
	{
		mNode->detachObject(mPUSystem);
		Core::getSingletonPtr()->destroyPUSystem(mPUSystem);
		mPUSystem=NULL;
	}

	mNode->detachAllObjects();
	mNode->getParentSceneNode()->removeAndDestroyChild(mNode->getName());
	Core::getSingletonPtr()->mSceneMgr->destroyEntity(mUnitEntity);
}

void UnitGrapIG::setWeapon( WeaponType type,BoneType bone )
{
	std::string boneName;

	switch(bone)
	{
	case LeftHand:
		{
			boneName=mLeftHandBoneName;
			break;
		}
	case RightHand:
		{
			boneName=mRightHandBoneName;
			break;
		}
	}

	if (mMainWeapon!=NULL)
	{
		if (mMainWeapon->getAttachBoneName()==boneName)
		{
			mMainWeapon->detachWeapon(mUnitEntity);
		}
	}

	if (mSecWeapon!=NULL)
	{
		if (mSecWeapon->getAttachBoneName()==boneName)
		{
			mSecWeapon->detachWeapon(mUnitEntity);
		
		}
	}

	if (mShield!=NULL)
	{
		if (mShield->getAttachBoneName()==boneName)
		{
			mShield->detachWeapon(mUnitEntity);
		}
	}


	switch(type)
	{
	case MainWepon:
		{
			if (mMainWeapon!=NULL)
			{
				mMainWeapon->attachWeapon(mUnitEntity,boneName);
			}
			break;
		}
	case SecWepon:
		{
			if (mSecWeapon!=NULL)
			{
				mSecWeapon->attachWeapon(mUnitEntity,boneName);
			}
			break;
		}
	case Shield:
		{
			if (mShield!=NULL)
			{
				mShield->attachWeapon(mUnitEntity,boneName);
			}
			break;
		}
	}
}

void UnitGrapIG::createWeapon( std::string name,WeaponType type )
{
	if (name!="null")
	{
		switch(type)
		{
		case MainWepon:
			{
				mMainWeapon=new WeaponGrapIG(name);
				break;
			}
		case SecWepon:
			{
				mSecWeapon=new WeaponGrapIG(name);
				break;
			}
		case Shield:
			{
				mShield=new WeaponGrapIG(name);
				break;
			}
		}
	}
}

void UnitGrapIG::setEffect( std::string name )
{
	if (mPUSystem!=NULL)
	{
		mNode->detachObject(mPUSystem);
		Core::getSingletonPtr()->destroyPUSystem(mPUSystem);
		mPUSystem=NULL;
	}

	mPUSystem=Core::getSingletonPtr()->createPUSystem(mNode->getName()+"_PU",name);
	mPUSystem->addParticleSystemListener(this);
	mNode->attachObject(mPUSystem);
	mPUSystem->prepare();
	mPUSystem->start();
}

bool UnitGrapIG::isEffectOver()
{
	return mPUSystemEnd;
}

void UnitGrapIG::handleParticleSystemEvent( ParticleUniverse::ParticleSystem *particleSystem, ParticleUniverse::ParticleUniverseEvent &particleUniverseEvent )
{
	if (particleUniverseEvent.componentType==ParticleUniverse::CT_SYSTEM && particleUniverseEvent.eventType==ParticleUniverse::PU_EVT_NO_PARTICLES_LEFT)
	{
		mPUSystemEnd=true;
	}
}

void UnitGrapIG::stopEffect()
{
	mPUSystem->stop();
}

void UnitGrapIG::setPosition( float x,float z )
{
	mNode->setPosition(x,0,z);
	setHeight();
}

void UnitGrapIG::setHeight()
{
	Ogre::Vector3 lv=mNode->getPosition();
	Ogre::Vector3 v=mNode->convertLocalToWorldPosition(lv);
	lv.y=Terrain::getSingletonPtr()->getHeight(v.x,v.z);
	mNode->setPosition(lv);
}

void UnitGrapIG::setFadeInOut( bool isIn )
{
	//��������
	Ogre::Pass *pass=NULL;
	for(int i = 0; i < mUnitEntity->getNumSubEntities(); ++i)
	{
		Ogre::MaterialPtr p;
		if (Ogre::MaterialManager::getSingletonPtr()->resourceExists(mUnitEntity->getSubEntity(i)->getMaterial()->getName()+"_Copy"))
		{
			p=Ogre::MaterialManager::getSingletonPtr()->getByName(mUnitEntity->getSubEntity(i)->getMaterial()->getName()+"_Copy");
		}
		else
		{
			p=mUnitEntity->getSubEntity(i)->getMaterial()->clone(mUnitEntity->getSubEntity(i)->getMaterial()->getName()+"_Copy");
		}

		pass = p->getTechnique(0)->getPass(0);
		pass->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA); 
		pass->setDepthCheckEnabled(true); 
		pass->setDepthWriteEnabled(false);
		mUnitEntity->getSubEntity(i)->setMaterial(p);
	}

	if (mMainWeapon!=NULL)
	{
		mMainWeapon->startFade();
	}

	if (mSecWeapon!=NULL)
	{
		mSecWeapon->startFade();
	}
	
	if (mShield!=NULL)
	{
		mShield->startFade();
	}

	if (isIn)
	{
		mAlpha=0;
		mFadeInOut=true;
	}
	else
	{
		mAlpha=1;
		mFadeInOut=false;
	}

	pass->setDiffuse(pass->getDiffuse().r, pass->getDiffuse().g, pass->getDiffuse().b, mAlpha); 
}

void UnitGrapIG::doFadeInOut(unsigned int deltaTime )
{
	mAlphaDeltaTime+=deltaTime;
	if (mAlphaDeltaTime>=ALPHA_DELTA_TIME)
	{
		mAlphaDeltaTime=0;
		
		if (mFadeInOut)
		{
			mAlpha+=0.1;
		}
		else
		{
			mAlpha-=0.1;
		}
	
		for(int i = 0; i < mUnitEntity->getNumSubEntities(); ++i)
		{
			if (mAlpha>=1 || mAlpha <=0)
			{
				std::string n = mUnitEntity->getSubEntity(i)->getMaterial()->getName();
				n.replace(n.find("_Copy"),5,"");
				mUnitEntity->getSubEntity(i)->setMaterialName(n);
				Ogre::MaterialManager::getSingletonPtr()->remove(mUnitEntity->getSubEntity(i)->getMaterial()->getName()+"_Copy");
				if( mAlpha <=0)
				{
					mUnitEntity->setVisible(false);
				}
			}
			else
			{
				Ogre::Pass *pass = mUnitEntity->getSubEntity(i)->getMaterial()->getTechnique(0)->getPass(0);
				pass->setDiffuse(pass->getDiffuse().r, pass->getDiffuse().g, pass->getDiffuse().b, mAlpha); 
			}
		}

		if (mAlpha>=1 || mAlpha <=0)
		{
			mAlpha=-1;
			if (mMainWeapon!=NULL)
			{
				if( mAlpha <=0)
				{
					mMainWeapon->overFade(false);
				}
				else
				{
					mMainWeapon->overFade(true);
				}
			}

			if (mSecWeapon!=NULL)
			{
				if( mAlpha <=0)
				{
					mSecWeapon->overFade(false);
				}
				else
				{
					mSecWeapon->overFade(true);
				}
			}

			if (mShield!=NULL)
			{
				if( mAlpha <=0)
				{
					mSecWeapon->overFade(false);
				}
				else
				{
					mSecWeapon->overFade(true);
				}
			}
		}
		else
		{
			if (mMainWeapon!=NULL)
			{
				mMainWeapon->doFade(mAlpha);
			}

			if (mSecWeapon!=NULL)
			{
				mSecWeapon->doFade(mAlpha);
			}

			if (mShield!=NULL)
			{
				mShield->doFade(mAlpha);
			}
		}
	}
}

void UnitGrapIG::setAnimation( std::string name,bool loop,bool returnInit )
{
	mAniBlender->blend(name,AnimationBlender::BlendWhileAnimating,0.2,loop);
	mReturnInitAni=returnInit;
}

void UnitGrapIG::update( unsigned int deltaTime )
{
	mAniBlender->addTime(deltaTime/1000.0f);
	
	if (mReturnInitAni && mAniBlender->complete)
	{
		mAniBlender->BackToInit();
		mReturnInitAni=false;
	}

	if (mIsCheckHeight)
	{
		setHeight();
	}

	if(mAlpha!=-1)
	{
		doFadeInOut(deltaTime);
	}
}