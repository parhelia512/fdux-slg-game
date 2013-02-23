#include "GUIDebugWindow.h"

#include "StringTable.h"

#include "DataLibrary.h"
#include "AVGSquadManager.h"
#include "LuaSystem.h"

GUIDebugWindow::GUIDebugWindow(int Width,int Height):GUIScene("DebugWindow.layout",Width,Height)
{
	assignWidget(mWindow,"DEBUG");
	assignWidget(mAddSquad,"AddSquad");
	assignWidget(mDeleteSquad,"DeleteSquad");
	assignWidget(mRefreshList,"RefreshList");
	assignWidget(mJumpCP,"JumpCP");
	assignWidget(mSquadName,"SquadName");
	assignWidget(mJump,"Jump");
	assignWidget(mSquadLeader,"SquadLeader");
	assignWidget(mSquadList,"SquadList");

	mAddSquad->eventMouseButtonClick+= MyGUI::newDelegate(this, &GUIDebugWindow::onAddSquad);
	mDeleteSquad->eventMouseButtonClick+= MyGUI::newDelegate(this, &GUIDebugWindow::onDeleteSquad);
	mRefreshList->eventMouseButtonClick+= MyGUI::newDelegate(this, &GUIDebugWindow::onRefreshList);
	mJumpCP->eventMouseButtonClick+= MyGUI::newDelegate(this, &GUIDebugWindow::onJumpCP);
}

GUIDebugWindow::~GUIDebugWindow(void)
{
}

void GUIDebugWindow::FrameEvent()
{

}

void GUIDebugWindow::showScene( std::string arg )
{
	mWindow->setVisible(true);
}

void GUIDebugWindow::hideScene()
{
	mWindow->setVisible(false);
}

void GUIDebugWindow::onAddSquad(MyGUI::Widget* _sender)
{
	AVGSquadManager::getSingleton().addNewSquad(mSquadLeader->getCaption(),mSquadName->getCaption());
	onRefreshList(NULL);
}

void GUIDebugWindow::onDeleteSquad(MyGUI::Widget* _sender)
{
	std::string path = "GameData/StoryData/SquadData/";
	DataLibrary::getSingletonPtr()->delNode(path+mSquadName->getCaption());
	onRefreshList(NULL);
}

void GUIDebugWindow::onRefreshList(MyGUI::Widget* _sender)
{
	DataLibrary* datalib = DataLibrary::getSingletonPtr();
	mSquadList->removeAllItems();
	std::string path = "GameData/StoryData/SquadData";
	std::vector<std::string> squadlist = datalib->getChildList(path);
	for (std::vector<std::string>::iterator it=squadlist.begin();it!=squadlist.end();it++)
	{
		mSquadList->addItem((*it));
	}
}

void GUIDebugWindow::onJumpCP(MyGUI::Widget* _sender)
{
	LuaSystem::getSingletonPtr()->runScriptFromFile(mJump->getCaption(),0);
}