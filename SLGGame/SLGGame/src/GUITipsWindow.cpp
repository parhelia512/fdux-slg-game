#include "GUITipsWindow.h"

#include "StringTable.h"
#include "conversion.h"

#define MAX_TIPS_WIDTH 400
#define FONT_SIZE 20
const int LINE_TEXT_NUM=MAX_TIPS_WIDTH/FONT_SIZE;

GUITipsWindow::GUITipsWindow( MyGUI::Window* window,int Width,int Height ):GUISubWindows(window,Width,Height),mWindow(window)
{
	assignWidget(mTipsTextBox,"TipsTextBox");
	mStringTable=StringTable::getSingletonPtr();
}

GUITipsWindow::~GUITipsWindow()
{

}

bool GUITipsWindow::GridInputEvent( int x,int y )
{
	return false;
}

void GUITipsWindow::showScene( std::string arg )
{

}

void GUITipsWindow::hideScene()
{

}

void GUITipsWindow::FrameEvent()
{

}

void GUITipsWindow::setTips(std::string text)
{	
	std::wstring s;
	UTF8ToUnicode(text,s);
	int textlength=s.length();

	if(textlength<=LINE_TEXT_NUM)
	{
		if (textlength<=2)
			textlength=4;
		mWindow->setSize(textlength*(FONT_SIZE+4),FONT_SIZE*3);
		mTipsTextBox->setCaption(s);
	}
	else
	{
		mWindow->setSize(LINE_TEXT_NUM*(FONT_SIZE-4),FONT_SIZE*(3+(textlength/LINE_TEXT_NUM)));

		for (int i=0;i<textlength/LINE_TEXT_NUM;i++)
		{
			mTipsTextBox->addText(s.substr(0,LINE_TEXT_NUM));
			mTipsTextBox->addText("\n");
			s.erase(0,LINE_TEXT_NUM);
		}
		
		mTipsTextBox->addText(s);
		//int length=mTipsTextBox->getTextLength();
		//mTipsTextBox->eraseText(length-1);
	}
}

void GUITipsWindow::setTipsWindowPosition(int left,int top)
{
	if (mWindow->getWidth()+left>mWidth)
	{
		left-=mWindow->getWidth();
	}
	else if(mWindow->getHeight()+top>mHeigth)
	{
		top-=mWindow->getHeight();
	}

	mWindow->setPosition(left,top);
}

void GUITipsWindow::eventToolTip( MyGUI::Widget* _sender, const MyGUI::ToolTipInfo& _info )
{
	if (_info.type==MyGUI::ToolTipInfo::Show)
	{
		if (_sender->getUserString("Tips")!="")
		{
			setTips(_sender->getUserString("Tips"));
		}
		else
		{
			setTips(mStringTable->getString(std::string("Tips_")+_sender->getName()));
		}
		
		mWindow->setVisible(true);
		setTipsWindowPosition(_info.point.left+20,_info.point.top);
	}
	else if(_info.type==MyGUI::ToolTipInfo::Hide)
	{
		mTipsTextBox->setCaption("");
		mWindow->setVisible(false);
	}
	else if (_info.type==MyGUI::ToolTipInfo::Move)
	{
		setTipsWindowPosition(_info.point.left+20,_info.point.top);
	}
}

void GUITipsWindow::addToolTipEvent( MyGUI::Widget* widget )
{
	widget->eventToolTip.clear();
	widget->eventToolTip += MyGUI::newDelegate(this, &GUITipsWindow::eventToolTip);
}
