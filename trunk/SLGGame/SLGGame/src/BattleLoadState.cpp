#include "BattleLoadState.h"

#include "Terrain.h"

#include "BattleState.h"
#include "BattleDeployState.h"
#include "LoadScene.h"
#include "BattleSquadManager.h"
#include "DataLibrary.h"
#include "GUIBattle.h"
#include "MapLoader.h"

#include "MapDataManager.h"

BattleLoadState::BattleLoadState( std::string arg)
{
	mState = LOADSCENE;
	mLoadFromMap =true;
	mMapFile = arg;
	mLoadScene = static_cast<LoadScene*>(GUISystem::getSingleton().createScene(LoadingScene));
	mMapLoader = new MapLoader;
}

BattleLoadState::~BattleLoadState()
{
	GUISystem::getSingleton().destoryScene(LoadingScene);
}

void BattleLoadState::update(unsigned int deltaTime)
{
	switch(mState)
	{
	case LOADSCENE:
		mState = LOADTERRAIN;
		mLoadScene->setText("Loading Terrain");
		break;
	case LOADTERRAIN:
		if(mLoadFromMap)
			mMapLoader->loadMapFormFile(mMapFile);
		Core::getSingleton().mRoot->renderOneFrame(0.0f);
		mState = LOADOBJECT;
		mLoadScene->setProgress(25);
		mLoadScene->setText("Loading MapObject");
		break;
	case LOADOBJECT:
		mMapLoader->loadMapObj();
		mState = LOADUNIT;
		mLoadScene->setProgress(50);
		mLoadScene->setText("Loading Unit");
		break;
	case LOADUNIT:
		mMapLoader->initBattleSquad(true);
		mState = LOADGRID;
		mLoadScene->setProgress(75);
		mLoadScene->setText("Creating Grid");
		break;
	case LOADGRID:
		Terrain::getSingleton().createGrid();
		GUIBattle* guibattle=static_cast<GUIBattle *>(GUISystem::getSingleton().createScene(BattleScene));
		BattleDeployState* deploystate = new BattleDeployState;
		guibattle->setBattleState(mMainState);
		//GUISystem::getSingleton().setFrameUpdateScene(BattleScene);
		mLoadScene->setProgress(100);
		mLoadScene->setText("Finish");
		mMainState->ChangeState(deploystate);
		break;
	}
}
