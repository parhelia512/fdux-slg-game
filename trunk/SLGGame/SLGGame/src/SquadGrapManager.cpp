#include "SquadGrapManager.h"

#include "SquadGraphics.h"
#include "DataLibrary.h"

#include "Core.h"
#include "BillboardManager.h"

SquadGrapManager::SquadGrapManager(Ogre::SceneManager* sceneMgr):unitIndex(0)
{
	mBBManager=new BillboardManager(Core::getSingletonPtr()->mCamera);
}

SquadGrapManager::~SquadGrapManager(void)
{
	clear();
	delete BillboardManager::getSingletonPtr();
}

void SquadGrapManager::clear()
{
	for (std::vector<SquadGraphics*>::iterator it=mUnitList.begin();it!=mUnitList.end();it++)
	{
		delete (*it);
	}
	mUnitList.clear();
}

SquadGraphics* SquadGrapManager::createSquad( std::string unitName,std::string datapath,unsigned int id,int x,int y,Direction d,Formation f,int soldierCount)
{
	SquadGraphics* squadGrap=new SquadGraphics(unitName,datapath,Ogre::Vector2(x,y),d,f,id,soldierCount);

	mUnitList.push_back(squadGrap);

	return squadGrap;
}

void SquadGrapManager::destorySquad(unsigned int id)
{
	//�������еĽ�������
	for (std::vector<SquadGraphics*>::iterator it=mUnitList.begin();it!=mUnitList.end();it++)
	{
		if ((*it)->mID==id)
		{
			delete (*it);
			mUnitList.erase(it);
			break;
		}
	}
}

void SquadGrapManager::update(unsigned int deltaTime)
{
	for (std::vector<SquadGraphics*>::iterator it=mUnitList.begin();it!=mUnitList.end();it++)
	{
		(*it)->update(deltaTime);
	}

	mBBManager->update(deltaTime);
}

SquadGraphics* SquadGrapManager::getSquad( unsigned int id )
{
	for (std::vector<SquadGraphics*>::iterator it=mUnitList.begin();it!=mUnitList.end();it++)
	{
		if ((*it)->mID==id)
		{
			return (*it);
		}
	}
	return NULL;
}

void SquadGrapManager::setParticleVisible(bool visible)
{
	for (std::vector<SquadGraphics*>::iterator it=mUnitList.begin();it!=mUnitList.end();it++)
	{
		(*it)->setParticleVisible(visible);
	}
}