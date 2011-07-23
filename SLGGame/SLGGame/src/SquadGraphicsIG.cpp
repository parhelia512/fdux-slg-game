#include "SquadGraphicsIG.h"

#include <stdlib.h>
#include <time.h>

#include "Core.h"
#include "DataLibrary.h"

#include "UnitGrapIG.h"
#include "AnimationBlender.h"
#include "BillboardManager.h"

#include "Terrain.h"

const Ogre::Vector3 LineVector[5]={Ogre::Vector3(0,0,4),Ogre::Vector3(-4,0,4),Ogre::Vector3(4,0,4),Ogre::Vector3(2,0,-2),Ogre::Vector3(-2,0,-2)};
const Ogre::Vector3 CircularVector[5]={Ogre::Vector3(0,0,3),Ogre::Vector3(-3,0,1),Ogre::Vector3(3,0,1),Ogre::Vector3(2,0,-2),Ogre::Vector3(-2,0,-2)};
const Ogre::Vector3 LooseVector[5]={Ogre::Vector3(0,0,0),Ogre::Vector3(-3,0,3),Ogre::Vector3(3,0,3),Ogre::Vector3(3,0,-3),Ogre::Vector3(-3,0,-3)};

#define MOVE_KEYFRAME_TIME 1.5
#define FORMATION_KEYFRAME_TIME 1
#define RELIEF_MOVE_TIME 1

SquadGraphicsIG::SquadGraphicsIG(std::string squadName,Ogre::Vector2& grid,Direction direction,Formation f,unsigned int index):
mID(index),
mPUSystem(NULL),
mPUSystemEnd(false),
mNodeAnimation(NULL),
mNodeAnimationState(NULL),
mDeathUnit(NULL),
mReliefUnit(NULL),
mSoldierIndex(0),
mFormation(f)
{
	mSceneMgr=Core::getSingletonPtr()->mSceneMgr;
	mNode=mSceneMgr->getRootSceneNode()->createChildSceneNode(squadName+Ogre::StringConverter::toString(index));

	//获取数据
	std::string mainWeaponName;
	std::string secWeaponName;
	std::string shieldName;

	DataLibrary::getSingletonPtr()->getData(std::string("GameData/Squad/")+squadName+std::string("/CommanderUnit"),mCommanderUnitName);
	DataLibrary::getSingletonPtr()->getData(std::string("GameData/Squad/")+squadName+std::string("/CommanderMainWeapon"),mainWeaponName);
	DataLibrary::getSingletonPtr()->getData(std::string("GameData/Squad/")+squadName+std::string("/CommanderSecWeapon"),secWeaponName);
	DataLibrary::getSingletonPtr()->getData(std::string("GameData/Squad/")+squadName+std::string("/CommanderShield"),shieldName);

	//组建单位队伍与组建武器
	mCommanderUnit=new UnitGrapIG(mCommanderUnitName,mNode->createChildSceneNode(mNode->getName()+"_Commander"));
	mCommanderUnit->createWeapon(mainWeaponName,UnitGrapIG::MainWepon);
	mCommanderUnit->createWeapon(secWeaponName,UnitGrapIG::SecWepon);
	mCommanderUnit->createWeapon(shieldName,UnitGrapIG::Shield);

	DataLibrary::getSingletonPtr()->getData(std::string("GameData/Squad/")+squadName+std::string("/SoldierUnit"),mSoldierUnitName);
	DataLibrary::getSingletonPtr()->getData(std::string("GameData/Squad/")+squadName+std::string("/SoldierMainWeapon"),mMainWeaponName);
	DataLibrary::getSingletonPtr()->getData(std::string("GameData/Squad/")+squadName+std::string("/SoldierSecWeapon"),mSecWeaponName);
	DataLibrary::getSingletonPtr()->getData(std::string("GameData/Squad/")+squadName+std::string("/SoldierShield"),mShieldName);
	for (int i=0;i<4;i++)
	{
		createSoldier();
	}
	
	//设置参数
	setGrid(grid.x,grid.y);
	setDirection(direction,false);
	setFormation(f,false);
	setWeaponMode(SquadGraphicsIG::MainWepon);

//	mNode->attachObject(BillboardManager::getSingletonPtr()->getBillboardSet());

}

SquadGraphicsIG::~SquadGraphicsIG(void)
{
	delete mCommanderUnit;
	for (std::vector<UnitGrapIG*>::iterator it=mSoldierUnits.begin();it!=mSoldierUnits.end();it++)
	{
		delete (*it);
	}

	if (mPUSystem!=NULL)
	{
		mNode->detachObject(mPUSystem);
		Core::getSingletonPtr()->destroyPUSystem(mPUSystem);
		mPUSystem=NULL;
	}

	if (mNodeAnimationState!=NULL)
	{
		mSceneMgr->destroyAnimationState(mNode->getName()+"_AniState");
		mSceneMgr->destroyAnimation(mNode->getName()+"_Ani");
	}
	
	mNode->removeAndDestroyAllChildren();
	mNode->getParentSceneNode()->removeAndDestroyChild(mNode->getName());
}

UnitGrapIG* SquadGraphicsIG::createSoldier()
{
	mSoldierIndex++;
	UnitGrapIG* unit=new UnitGrapIG(mSoldierUnitName,mNode->createChildSceneNode(mNode->getName()+"_Soldier"+Ogre::StringConverter::toString(mSoldierIndex)));

	unit->createWeapon(mMainWeaponName,UnitGrapIG::MainWepon);
	unit->createWeapon(mSecWeaponName,UnitGrapIG::SecWepon);
	unit->createWeapon(mShieldName,UnitGrapIG::Shield);

	mSoldierUnits.push_back(unit);

	return unit;
}

void SquadGraphicsIG::setMovePath(std::map<int,Ogre::Vector3>& vectors,std::map<int,Ogre::Quaternion>& quaternions)
{
	mNodeAnimation = mSceneMgr->createAnimation(mNode->getName()+"_Ani", vectors.size()*MOVE_KEYFRAME_TIME);
	mNodeAnimation->setInterpolationMode(Ogre::Animation::IM_LINEAR);
	Ogre::NodeAnimationTrack* track = mNodeAnimation->createNodeTrack(1, mNode);
	
	float timePosition=0;
	Ogre::TransformKeyFrame* kf = track->createNodeKeyFrame(timePosition);
	kf->setTranslate(mNode->getPosition());
	kf->setRotation(mNode->getOrientation());

	std::map<int,Ogre::Vector3>::iterator itr  =  vectors.begin();
	for(  ;  itr !=  vectors.end();  ++itr )
	{
		timePosition+=MOVE_KEYFRAME_TIME;
		kf = track->createNodeKeyFrame(timePosition);

		kf->setTranslate(itr->second);

		std::map<int,Ogre::Quaternion>::iterator itr1;
		itr1 = quaternions.find(itr->first);

		if( itr1 != quaternions.end() )
		{
			kf->setRotation(itr1->second);
		}
	}
	
	mNodeAnimationState = mSceneMgr->createAnimationState(mNode->getName()+"_Ani");

	setCheckUnitHeight(true);
	mNodeAnimationState->setLoop(false);
	mNodeAnimationState->setEnabled(true);

	mCommanderUnit->setAnimation(mCommanderUnit->mWalkName,true,false);
	for (std::vector<UnitGrapIG*>::iterator it=mSoldierUnits.begin();it!=mSoldierUnits.end();it++)
	{
		(*it)->setAnimation((*it)->mWalkName,true,false);
	}
	mReturnInitAni=true;

}

bool SquadGraphicsIG::isTransformOver()
{
	if (mNodeAnimationState!=NULL)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void SquadGraphicsIG::stopTransform()
{
	mSceneMgr->destroyAnimationState(mNode->getName()+"_Ani");
	mSceneMgr->destroyAnimation(mNode->getName()+"_Ani");
	mNodeAnimation=NULL;
	mNodeAnimationState=NULL;
	setCheckUnitHeight(false);
}

void SquadGraphicsIG::setAnimation(std::string name,Object object,bool isLoop,bool returnInit)
{
	switch(object)
	{
	case Squad:
		{

		}
	case Commander:
		{
			mCommanderUnit->setAnimation(name,isLoop,returnInit);
			if (object!=Squad)
			{
				break;
			}
		}
	case Soldier:
		{
			for (std::vector<UnitGrapIG*>::iterator it=mSoldierUnits.begin();it!=mSoldierUnits.end();it++)
			{
				(*it)->setAnimation(name,isLoop,returnInit);
			}
			break;
		}
	}
}


void SquadGraphicsIG::setInitAnimation( Object object )
{
	switch(object)
	{
	case Squad:
		{

		}
	case Commander:
		{
			mCommanderUnit->mAniBlender->BackToInit();
			if (object!=Squad)
			{
				break;
			}
		}
	case Soldier:
		{
			for (std::vector<UnitGrapIG*>::iterator it=mSoldierUnits.begin();it!=mSoldierUnits.end();it++)
			{
				(*it)->mAniBlender->BackToInit();
			}
			break;
		}
	}
}


bool SquadGraphicsIG::isAnimationOver(Object object)
{
	if (object==Commander)
	{
		return mCommanderUnit->mAniBlender->complete;
	}
	else
	{
		if (mSoldierUnits.size()!=0)
		{
			return mSoldierUnits.at(0)->mAniBlender->complete;
		}
		else
		{
			return true;
		}
	}
}


void SquadGraphicsIG::setEffect( std::string name,Object object)
{
	switch(object)
	{
	case Squad:
		{
			if (mPUSystem!=NULL)
			{
				mNode->detachObject(mPUSystem);
				Core::getSingletonPtr()->destroyPUSystem(mPUSystem);
				mPUSystem=NULL;
			}

			mPUSystem=Core::getSingletonPtr()->createPUSystem(mNode->getName()+"_PU",name);
			mPUSystem->addParticleSystemListener(this);
			mPUSystem->prepare();
			mNode->attachObject(mPUSystem);
			mPUSystem->start();
			break;
		}
	case Commander:
		{
			mCommanderUnit->setEffect(name);
			break;
		}
	case Soldier:
		{
			for (std::vector<UnitGrapIG*>::iterator it=mSoldierUnits.begin();it!=mSoldierUnits.end();it++)
			{
				(*it)->setEffect(name);
			}
			break;
		}
	}
}

void SquadGraphicsIG::setFormation( Formation f,bool isAnim )
{
	mFormation=f;
	Ogre::Vector3 CommanderVector;
	Ogre::Vector3 SoldierVector[4];

	switch(f)//确定各个位置
	{
	case Line:
		{
			CommanderVector=LineVector[0];

			for (int i=1;i<5;i++)
			{
				SoldierVector[i-1]=LineVector[i];
			}
			break;
		}
	case Circular:
		{
			CommanderVector=CircularVector[0];

			for (int i=1;i<5;i++)
			{
				SoldierVector[i-1]=CircularVector[i];
			}
			break;
		}
	case Loose:
		{
			CommanderVector=LooseVector[0];

			for (int i=1;i<5;i++)
			{
				SoldierVector[i-1]=LooseVector[i];
			}
			break;
		}
	}

	if (isAnim)
	{
		mNodeAnimation = mSceneMgr->createAnimation(mNode->getName()+"_Ani",FORMATION_KEYFRAME_TIME);
		mNodeAnimation->setInterpolationMode(Ogre::Animation::IM_LINEAR);
		Ogre::NodeAnimationTrack* track = mNodeAnimation->createNodeTrack(0,mCommanderUnit->mNode);

		Ogre::TransformKeyFrame* kf = track->createNodeKeyFrame(0);
		kf->setTranslate(mCommanderUnit->mNode->getPosition());
		kf->setRotation(mCommanderUnit->mNode->getOrientation());
		kf->setScale(mCommanderUnit->mNode->getScale());
		Ogre::Quaternion q=mCommanderUnit->mNode->getOrientation();

		kf=track->createNodeKeyFrame(FORMATION_KEYFRAME_TIME);
		Ogre::Vector3 v=CommanderVector;
		Ogre::Vector3 wv=mCommanderUnit->mNode->convertLocalToWorldPosition(v);
		v.y=Terrain::getSingletonPtr()->getHeight(wv.x,wv.z);
		kf->setTranslate(v);
		//Ogre::Quaternion destQ;
		//Ogre::Vector3 lv=mCommanderUnit->mNode->getPosition();
		//destQ.FromAngleAxis(lv.angleBetween(v),Ogre::Vector3(0,1,0));
		kf->setRotation(mCommanderUnit->mNode->getOrientation());
		kf->setScale(mCommanderUnit->mNode->getScale());

	/*	kf=track->createNodeKeyFrame(2);
		kf->setTranslate(v);
		kf->setRotation(mCommanderUnit->mNode->getOrientation());
		kf->setScale(mCommanderUnit->mNode->getScale());*/

		int i=0;
		for (std::vector<UnitGrapIG*>::iterator it=mSoldierUnits.begin();it!=mSoldierUnits.end();it++)
		{
			track = mNodeAnimation->createNodeTrack(i+1,(*it)->mNode);

			Ogre::TransformKeyFrame* kf = track->createNodeKeyFrame(0);
			kf->setTranslate((*it)->mNode->getPosition());
			kf->setRotation((*it)->mNode->getOrientation());
			kf->setScale((*it)->mNode->getScale());
			//Ogre::Quaternion q=(*it)->mNode->getOrientation();

			//kf=track->createNodeKeyFrame(1.5);
			v=SoldierVector[i];
			wv=mCommanderUnit->mNode->convertLocalToWorldPosition(v);
			v.y=Terrain::getSingletonPtr()->getHeight(wv.x,wv.z);
			//kf->setTranslate(v);
			//kf->setRotation((*it)->mNode->getPosition().getRotationTo(v));
			//kf->setScale((*it)->mNode->getScale());

			kf=track->createNodeKeyFrame(FORMATION_KEYFRAME_TIME);
			kf->setTranslate(v);
			kf->setRotation((*it)->mNode->getOrientation());
			kf->setScale((*it)->mNode->getScale());
			(*it)->mFormationPosition=i;
			i++;
		}

		mNodeAnimationState = mSceneMgr->createAnimationState(mNode->getName()+"_Ani");

		setCheckUnitHeight(true);
		mNodeAnimationState->setLoop(false);
		mNodeAnimationState->setEnabled(true);

		mCommanderUnit->setAnimation(mCommanderUnit->mWalkName,true,false);
		for (std::vector<UnitGrapIG*>::iterator it=mSoldierUnits.begin();it!=mSoldierUnits.end();it++)
		{
			(*it)->setAnimation((*it)->mWalkName,true,false);
		}
		mReturnInitAni=true;

	}
	else
	{
		mCommanderUnit->setPosition(CommanderVector.x,CommanderVector.z);
		
		int i=0;
		for (std::vector<UnitGrapIG*>::iterator it=mSoldierUnits.begin();it!=mSoldierUnits.end();it++)
		{
			(*it)->setPosition(SoldierVector[i].x,SoldierVector[i].z);
			(*it)->mFormationPosition=i;
			i++;
		}
	}
}

void SquadGraphicsIG::handleParticleSystemEvent( ParticleUniverse::ParticleSystem *particleSystem, ParticleUniverse::ParticleUniverseEvent &particleUniverseEvent )
{
	if (particleUniverseEvent.componentType==ParticleUniverse::CT_SYSTEM && particleUniverseEvent.eventType==ParticleUniverse::PU_EVT_NO_PARTICLES_LEFT)
	{
		mPUSystemEnd=true;
	}
}

bool SquadGraphicsIG::isEffectOver( Object object )
{
	switch(object)
	{
	case Squad:
		{
			return mPUSystemEnd;
			break;
		}
	case Commander:
		{
			mCommanderUnit->isEffectOver();
			break;
		}
	case Soldier:
		{
			if (mSoldierUnits.size()!=0)
			{
				return mSoldierUnits.at(0)->isEffectOver();
			}
			else
			{
				return true;
			}
			break;
		}
	}

	return true;
}

void SquadGraphicsIG::stopEffect( Object object )
{
	if (mPUSystem!=NULL)
	{
		switch(object)
		{
		case Squad:
			{
				mPUSystem->stop();
				break;
			}
		case Commander:
			{
				mCommanderUnit->stopEffect();
				break;
			}
		case Soldier:
			{
				for (std::vector<UnitGrapIG*>::iterator it=mSoldierUnits.begin();it!=mSoldierUnits.end();it++)
				{
					(*it)->stopEffect();
				}
				break;
			}
		}
	}
}

void SquadGraphicsIG::setGrid(int x,int y)
{
	float wx,wy=0;
	Terrain::getSingletonPtr()->getWorldCoords(x,y,wx,wy);
	
	mNode->setPosition(wx,0,wy);
	for (std::vector<UnitGrapIG*>::iterator it=mSoldierUnits.begin();it!=mSoldierUnits.end();it++)
	{
		(*it)->setPosition(wx,wy);
	}
	mCommanderUnit->setPosition(wx,wy);

}

void SquadGraphicsIG::setScale( Ogre::Vector3 scale,bool isAnim )
{

}

void SquadGraphicsIG::setDirection( Direction d,bool isAnim )
{
	Ogre::Quaternion q;

	switch(d)
	{
	case North:
		{
			q.FromAngleAxis(Ogre::Degree(180),Ogre::Vector3(0,1,0));
			break;
		}
	case South:
		{
			q.FromAngleAxis(Ogre::Degree(360),Ogre::Vector3(0,1,0));
			break;
		}
	case West:
		{
			q.FromAngleAxis(Ogre::Degree(270),Ogre::Vector3(0,1,0));
			break;
		}
	case East:
		{
			q.FromAngleAxis(Ogre::Degree(90),Ogre::Vector3(0,1,0));
			break;
		}
	}

	if (isAnim)
	{
		mNodeAnimation = mSceneMgr->createAnimation(mNode->getName()+"_Ani", 2);
		mNodeAnimation->setInterpolationMode(Ogre::Animation::IM_LINEAR);
		Ogre::NodeAnimationTrack* track = mNodeAnimation->createNodeTrack(1, mNode);

		Ogre::TransformKeyFrame* kf = track->createNodeKeyFrame(0);
		kf->setTranslate(mNode->getPosition());
		kf->setRotation(mNode->getOrientation());

		kf = track->createNodeKeyFrame(2);
		kf->setTranslate(mNode->getPosition());
		kf->setRotation(q);

		mNodeAnimationState = mSceneMgr->createAnimationState(mNode->getName()+"_Ani");
		
		setCheckUnitHeight(true);
		mNodeAnimationState->setLoop(false);
		mNodeAnimationState->setEnabled(true);

		mCommanderUnit->setAnimation(mCommanderUnit->mRotationName,true,false);
		for (std::vector<UnitGrapIG*>::iterator it=mSoldierUnits.begin();it!=mSoldierUnits.end();it++)
		{
			(*it)->setAnimation((*it)->mRotationName,true,false);
		}
		mReturnInitAni=true;
	}
	else
	{
		mNode->setOrientation(q);
	}
}

void SquadGraphicsIG::setCheckUnitHeight( bool enable )
{
	mCommanderUnit->mIsCheckHeight=enable;
	for (std::vector<UnitGrapIG*>::iterator it=mSoldierUnits.begin();it!=mSoldierUnits.end();it++)
	{
		(*it)->mIsCheckHeight=enable;
	}
}

void SquadGraphicsIG::setWeaponMode( WeaponMode mode )
{
	UnitGrapIG::WeaponType type;
	if (mode==MainWepon)
	{
		type=UnitGrapIG::MainWepon;
	}
	else
	{
		type=UnitGrapIG::SecWepon;
	}

	mCommanderUnit->setWeapon(type,UnitGrapIG::RightHand);
	mCommanderUnit->setWeapon(UnitGrapIG::Shield,UnitGrapIG::LeftHand);
	for (std::vector<UnitGrapIG*>::iterator it=mSoldierUnits.begin();it!=mSoldierUnits.end();it++)
	{
		(*it)->setWeapon(type,UnitGrapIG::RightHand);
		(*it)->setWeapon(UnitGrapIG::Shield,UnitGrapIG::LeftHand);
	}
}

void SquadGraphicsIG::setDeath()
{
	if (mSoldierUnits.size()!=0)//还有士兵?
	{
		//随机选择
		int i;
		if (mSoldierUnits.size()!=1)
		{
			i=rand()%(mSoldierUnits.size()-1);
		}
		else
		{
			i=0;
		}

		mDeathUnit=mSoldierUnits.at(i);

		mReliefUnit=NULL;

		if (mDeathUnit->mFormationPosition<2)//如果是前排?
		{
			int findF;//确定士兵队伍里面还有没有替补?
			if (i==0)
			{
				findF=3;
			}
			else
			{
				findF=2;
			}
			
			for (std::vector<UnitGrapIG*>::iterator it=mSoldierUnits.begin();it!=mSoldierUnits.end();it++)
			{
				if ((*it)->mFormationPosition==findF)
				{
					mReliefUnit=(*it);
				}
			}

			if (mReliefUnit!=NULL)//如果找到了替补队员
			{
				mReliefUnit->mFormationPosition=mDeathUnit->mFormationPosition;
			}

		}

	}
	else
	{
		mDeathUnit=mCommanderUnit;
		mReliefUnit=NULL;
	}

	//执行死亡动画
	mDeathUnit->setAnimation(mDeathUnit->mDeathName,false,false);
	mDeathStep=playAni;

}

void SquadGraphicsIG::doDeathStep()
{
	switch(mDeathStep)
	{
	case playAni:
		{
			//等待死亡动画结束
			if (mDeathUnit->mAniBlender->complete)
			{
				mDeathStep=changeMat;
				mDeathUnit->setFadeInOut(false);
			}

			break;
		}
	case changeMat:
		{
			if (mReliefUnit!=NULL)//需要替补?
			{
				mDeathStep=moveUnit;
			}
			else
			{
				mDeathStep=resetAni;//终止
			}
			break;
		}
	case moveUnit:
		{
			mReliefAniName=mReliefUnit->mAniBlender->getSource()->getAnimationName();
			mReliefAniLoop=mReliefUnit->mAniBlender->getSource()->getLoop();
			
			//设置节点动画
			mNodeAnimation = mSceneMgr->createAnimation(mNode->getName()+"_Ani", RELIEF_MOVE_TIME);
			mNodeAnimation->setInterpolationMode(Ogre::Animation::IM_LINEAR);
			Ogre::NodeAnimationTrack* track = mNodeAnimation->createNodeTrack(1, mReliefUnit->mNode);

			Ogre::TransformKeyFrame* kf = track->createNodeKeyFrame(0);
			kf->setTranslate( mReliefUnit->mNode->getPosition());
			kf->setRotation( mReliefUnit->mNode->getOrientation());
			kf->setScale(mReliefUnit->mNode->getScale());

			kf = track->createNodeKeyFrame(RELIEF_MOVE_TIME);
			kf->setTranslate(mDeathUnit->mNode->getPosition());
			kf->setRotation(mReliefUnit->mNode->getOrientation());
			kf->setScale( mReliefUnit->mNode->getScale());

			mNodeAnimationState = mSceneMgr->createAnimationState(mNode->getName()+"_Ani");

			setCheckUnitHeight(true);
			mNodeAnimationState->setLoop(false);
			mNodeAnimationState->setEnabled(true);

			mReturnInitAni=false;

			//设置跑步动画
			mReliefUnit->setAnimation(mReliefUnit->mWalkName,true,false);
			
			mDeathStep=resetAni;

			break;
		}
	case resetAni:
		{
			if (mNodeAnimation==NULL)
			{
				if(mReliefUnit!=NULL)
				{
					//回复原来动画
					mReliefUnit->setAnimation(mReliefAniName,mReliefAniLoop,false);
					mReliefUnit=NULL;
				}
				
				if (mDeathUnit->mAlpha==-1)
				{
					//移除单位
					for (std::vector<UnitGrapIG*>::iterator it=mSoldierUnits.begin();it!=mSoldierUnits.end();it++)
					{
						if ((*it)==mDeathUnit)
						{
							mSoldierUnits.erase(it);
							break;
						}
					}

					if (mDeathUnit==mCommanderUnit)
					{
						mCommanderUnit=NULL;
					}

					delete mDeathUnit;
					mDeathUnit=NULL;
				}
			}
			break;
		}
	}
}

void SquadGraphicsIG::stopDeath()
{
	if (mReliefUnit!=NULL)
	{
		if (mNodeAnimation!=NULL)
		{
			stopTransform();
		}

		mReliefUnit->mNode->setPosition(mDeathUnit->mNode->getPosition());
	}

	for (std::vector<UnitGrapIG*>::iterator it=mSoldierUnits.begin();it!=mSoldierUnits.end();it++)
	{
		if ((*it)==mDeathUnit)
		{
			mSoldierUnits.erase(it);
			break;
		}
	}

	delete mDeathUnit;
	mDeathUnit=NULL;
}

bool SquadGraphicsIG::isDeathOver()
{
	if (mDeathUnit!=NULL)
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool SquadGraphicsIG::isRecoverOver()
{
	bool over=true;
	for (std::vector<UnitGrapIG*>::iterator it=mSoldierUnits.begin();it!=mSoldierUnits.end();it++)
	{
		if((*it)->mAniBlender->getSource()->getAnimationName()==(*it)->mRecoverName)
		{
			over=false;
			break;
		}
	}
	return over;
}

void SquadGraphicsIG::setRecover(int num)
{
	int size=mSoldierUnits.size();
	if (mSoldierUnits.size()+num<5)
	{
		for (int i=0;i<num;i++)
		{
			int findIndex=-1;
			
			for (int j=0;j<mSoldierUnits.size();j++)
			{
				bool flag=false;
				for (std::vector<UnitGrapIG*>::iterator it=mSoldierUnits.begin();it!=mSoldierUnits.end();it++)
				{
					if((*it)->mFormationPosition==j)
					{
						flag=true;
						break;
					}
				}

				if (!flag)//没有找到?
				{
					findIndex=j;
					break;
				}
			}

			UnitGrapIG* unit=createSoldier();
			unit->mFormationPosition=findIndex;
				
			switch(mFormation)
			{
			case Line:
				{
					unit->setPosition(LineVector[unit->mFormationPosition+1].x,LineVector[unit->mFormationPosition+1].z);
					break;
				}
			case Circular:
				{
					unit->setPosition(CircularVector[unit->mFormationPosition+1].x,CircularVector[unit->mFormationPosition+1].z);
					break;
				}
			case Loose:
				{
					unit->setPosition(LooseVector[unit->mFormationPosition+1].x,LooseVector[unit->mFormationPosition+1].z);
					break;
				}
			}

			unit->setFadeInOut(true);
			unit->setAnimation(unit->mRecoverName,false,true);

		}
	}
}


void SquadGraphicsIG::update( unsigned int deltaTime )
{
	if (mDeathUnit!=NULL)
	{
		doDeathStep();
	}
	
	if (mNodeAnimationState!=NULL)
	{
		if (mNodeAnimationState->getTimePosition() >= mNodeAnimationState->getLength())
		{
			stopTransform();
			if (mReturnInitAni)
			{
				setInitAnimation(Squad);
			}
		}
		else
		{
			mNodeAnimationState->addTime(deltaTime/1000.0f);
		}
	}

	if (mCommanderUnit!=NULL)
	{
		mCommanderUnit->update(deltaTime);
	}
	for (std::vector<UnitGrapIG*>::iterator it=mSoldierUnits.begin();it!=mSoldierUnits.end();it++)
	{
		(*it)->update(deltaTime);
	}
}
