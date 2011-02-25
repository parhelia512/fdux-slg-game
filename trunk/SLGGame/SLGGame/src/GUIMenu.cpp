#include "GUIMenu.h"

GUIMenu::GUIMenu(int width,int height):GUIScene(width,height,"MainMenu.layout"),mMenuState(NoneState)
{
	assignWidget(mLogoImage,"LogoImage");
	assignWidget(mMenuImage,"BackGround");

	mLogoImage->setSize(width,height);
	mMenuImage->setSize(width,height);
}

GUIMenu::~GUIMenu(void)
{
}

void GUIMenu::showScene( std::string arg )
{
	if (arg=="logo")
	{
		mMenuState=LogoState;
		mMenuImage->setVisible(false);
		mLogoImage->setAlpha(0);
		mLogoImage->setVisible(true);
		mFadeWidget=mLogoImage;
	} 
	else
	{
		mMenuState=MainMenuState;
		mMenuImage->setAlpha(0);
		mMenuImage->setVisible(true);
		mLogoImage->setVisible(false);
		mFadeWidget=mMenuImage;
	}
	
	mSetp=0;
	mSetpDirection=true;//���޵��з���
	mTimerWork=FadeInOutWork;
	mTickTime=2*1000/100;

	mTimer.reset();
	GUISystem::getSingletonPtr()->setFrameUpdateScene(MenuScene);
}

void GUIMenu::hideScene()
{

}

void GUIMenu::FrameEvent()
{
	if(mTickTime!=0)
	{
		if (mTimer.getMilliseconds()>=mTickTime)
		{
			//��λ��ʱ��
			mTimer.reset();

			switch(mMenuState)
			{
			case LogoState:
				{
					FadeInOut();

					if (mTimerWork==NoneWork)
					{
						if (mSetpDirection)
						{
							mMenuState=WaitState;
							mTickTime=3000;
							GUISystem::getSingletonPtr()->setFrameUpdateScene(MenuScene);
						}
						else
						{
							showScene("");//��ʼ��ʾ���˵�
						}
					}

					break;
				}
			case MainMenuState:
				{
					FadeInOut();

					break;
				}
			case WaitState:
				{
					mMenuState=LogoState;
					
					mSetp=1;
					mSetpDirection=false;//���޵��з���
					mTimerWork=FadeInOutWork;
					mTickTime=2*1000/100;

					mTimer.reset();
					GUISystem::getSingletonPtr()->setFrameUpdateScene(MenuScene);
					
					break;
				}
			}
		}
	}
}

