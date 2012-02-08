#include "BattleDeployState.h"

#include "BattleState.h"
#include "CameraContral.h"
#include "Core.h"
#include "InputControl.h"
#include "SquadGrapManager.h"
#include "SquadGraphics.h"
#include "BattleSquadManager.h"
#include "BattleSquad.h"
#include "DataLibrary.h"
#include "Terrain.h"
#include "GUIPUDebug.h"
#include "GUIMenuWindow.h"
#include "GUIBattle.h"
//#include "BattleControlState.h"
#include "GUIDeployWindow.h"
#include "AreaGrap.h"
#include "MapDataManager.h"
#include "GUISquadWindows.h"
#include "TriggerManager.h"
#include "CommonFunction.h"

BattleDeployState::BattleDeployState()
{
	mCameraContral = CameraContral::getSingletonPtr();
	//mCameraContral->resetCamera();
	Core::getSingleton().mInputControl->pushListener(this);
	mMouseX = 640;
	mMouseY = 320;
	//Core::getSingleton().mInputControl->setCameraContral(mCameraContral);

	//DataLibrary::getSingletonPtr()->loadXmlData(DataLibrary::GameData,"../media/mesh/sinbad.xml",true);
	//mSquadGrapManager=new SquadGrapManager(Core::getSingletonPtr()->mSceneMgr);
	//SquadGraphics* s;//=mSquadGrapManager->createSquad("SinbadSquad",1,10,10,SquadGrapManager::North,SquadGrapManager::Loose);
	//GUIPUDebug* puDebug=(GUIPUDebug*)GUISystem::getSingletonPtr()->createScene(PUDebugScene);
	mMenuWindow=static_cast<GUIMenuWindow *>(GUISystem::getSingletonPtr()->createScene(MenuWindowsScene));
	mGUIBattle=static_cast<GUIBattle *>(GUISystem::getSingletonPtr()->getScene(BattleScene));
	//mGUIBattle->setBattleState(mMainState);
	mDeployWindow = static_cast<GUIDeployWindows *>(mGUIBattle->getSubWindow("DeployWindow"));
	mDeployWindow->showScene("");
	mDeployWindow->setDeployState(this);
	mSquadWindow = static_cast<GUISquadWindows *>(mGUIBattle->getSubWindow("SquadWindow"));
	mSquadWindow->setSquad(NULL);
	mSquadManager = BattleSquadManager::getSingletonPtr();
	mSelectSquad = NULL;
	//建立部署小队列表
	BattleSquadManager::BattleSquadIte ite;
	std::vector<std::string> squadlist;
	if(mSquadManager->mSquadList.size() != 0)
	{
		for(ite = mSquadManager->mSquadList.begin(); ite != mSquadManager->mSquadList.end(); ite++)
		{
			if(ite->second->getGridX() < 0)
			{
				squadlist.push_back(ite->second->getName());
				mDeployList.push_back(ite->second);
			}
		}
		mDeployWindow->initList(squadlist);
	}
	else
		mDeployWindow->setAllowConfirm(true);


	mAreaGrap = new AreaGrap(std::string("GameData/BattleData/MapData/Area/DeployArea/CoordList"),"CUBE_BLUE");

	//int k=1;
	//for (int i=8;i<14;i++)
	//{
	//	for (int j=8;j<13;j++)
	//	{
	//		s=mSquadGrapManager->createSquad("SinbadSquad",k,i,j,SquadGrapManager::North,SquadGrapManager::Loose);
	//		k++;
	//	}
	//}
	DataLibrary::getSingleton().saveXmlData(DataLibrary::GameData,"test.xml");
}
BattleDeployState::~BattleDeployState()
{
	mDeployWindow->hideScene();
	mSquadWindow->setSquad(NULL);
	Core::getSingleton().mInputControl->popListener();
	delete mAreaGrap;
}

void BattleDeployState::update(unsigned int deltaTime)
{
	//mSquadGrapManager->update(deltaTime);

	float dx = 0.0f,dy = 0.0f;
	float dt = (float)deltaTime / 5.0f;
	if(mMouseX < 20)
		dx = -dt;
	if(mMouseX > 1260)
		dx = dt;
	if(mMouseY < 20)
		dy = -dt;
	if(mMouseY > 680)
		dy = dt;
	mCameraContral->moveCamera(dx,dy);
}

bool BattleDeployState::keyPressed(const OIS::KeyEvent &arg)
{
	if (arg.key==OIS::KC_ESCAPE )
	{
		if (!mMenuWindow->getVisible())
		{
			mMenuWindow->setAllowSave(false);
			mMenuWindow->showScene("");
			
		}
		else
		{
			mMenuWindow->hideScene();
		}

		return true;
	}
	else
	{
		return false;
	}
}
bool BattleDeployState::keyReleased(const OIS::KeyEvent &arg)
{
	return false;
}

bool BattleDeployState::mouseMoved(const OIS::MouseEvent &arg)
{
	mMouseX = arg.state.X.abs;
	mMouseY = arg.state.Y.abs;
	mGUIBattle->SceneInputEvent(arg.state.X.abs,arg.state.Y.abs);
	return true;
}
bool BattleDeployState::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	int GX,GY;
	Terrain::getSingletonPtr()->coordinateToGrid(arg.state.X.abs,arg.state.Y.abs,GX,GY);
	if(mSelectSquad)
	{
		if(id == OIS::MB_Left)
		{
			MapDataManager* datamanager = MapDataManager::getSingletonPtr();
			if(mAreaGrap->inArea(GX,GY) && datamanager->getPassable(GX,GY,0) && mSquadManager->getBattleSquadAt(GX,GY,false) == NULL)
			{
				std::string id = mSelectSquad->getSquadId();
				SquadGraphics* squadgrap = SquadGrapManager::getSingleton().getSquad(id);
				squadgrap->setGrid(GX,GY);
				mSelectSquad->setGridX(GX);
				mSelectSquad->setGridY(GY);
				char info[64];
				sprintf_s(info,64,"%d,%d",GX,GY);
				mDeployWindow->setDeployInfo(mSelectIndex,std::string(info));
				if(mSquadManager->allDeployed())
					mDeployWindow->setAllowConfirm(true);
			}
		}
		else if(id == OIS::MB_Right)
		{
			int x,y;
			x = mSelectSquad->getGridX();
			y = mSelectSquad->getGridY();
			Direction d = GetDirection(x,y, GX, GY);
			mSelectSquad->setDirection(d);
			std::string grapid = mSelectSquad->getSquadId();
			SquadGraphics* squadgrap = SquadGrapManager::getSingleton().getSquad(grapid);
			squadgrap->setDirection(d,false);
			mSquadWindow->setSquad(mSelectSquad);
		}

	}
	return true;
}
bool BattleDeployState::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	return false;
}

void BattleDeployState::deployConfirm()
{
	if(mSquadManager->allDeployed())
	{
// 		mSquadManager->deployConfirm();
// 		BattleControlState* controlstate = new BattleControlState(true);
// 		mMainState->ChangeState(controlstate);
// 		TriggerManager::getSingleton().finishdeploy();
	}
}

void BattleDeployState::selectIndex(int index)
{
	if (index < mDeployList.size())
	{
		mSelectSquad = mDeployList[index];
		mSquadWindow->setSquad(mSelectSquad);
		mSelectIndex = index;
	}
}