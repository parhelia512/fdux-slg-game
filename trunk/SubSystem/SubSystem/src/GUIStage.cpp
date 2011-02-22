#include "GUIStage.h"

#include "GUISystem.h"

#include <ogre.h>

GUIStage::GUIStage(int width,int height):GUIScene("Stage.layout"),mCheckMouseDown(false),mIsMouseDown(false),mTextX(0),mTextY(0)
{
	assignWidget(mBackGroundGroup, "BackGroundGroup");
	assignWidget(mBackGround, "BackGround");
	assignWidget(mBackGroundUniversal, "BackGroundUniversal");

	assignWidget(mLeftLayerGroup, "LeftLayerGroup");
	assignWidget(mLeftLayer, "LeftLayer");
	assignWidget(mLeftLayerUniversal, "LeftLayerUniversal");

	assignWidget(mMidLayerGroup, "MidLayerGroup");
	assignWidget(mMidLayer, "MidLayer");
	assignWidget(mMidLayerUniversal, "MidLayerUniversal");

	assignWidget(mRightLayerGroup, "RightLayerGroup");
	assignWidget(mRightLayer, "RightLayer");
	assignWidget(mRightUniversal, "RightLayerUniversal");

	assignWidget(mTextBoxBG, "TextBoxBG");
	assignWidget(mTextBox, "TextBox");
	assignWidget(mRoleName, "RoleName");
	assignWidget(mRoleNameUniversal, "RoleNameUniversal");
	assignWidget(mTextCursor, "TextCursor");

	assignWidget(mEffectLayer, "EffectLayer");

	mBackGroundGroup->setSize(width,height);//���ñ�����С
	mBackGround->setSize(width,height);
	mBackGroundUniversal->setSize(width,height);

	mLeftLayerGroup->setSize(width,height);//�������С
	mMidLayerGroup->setSize(width,height);//�����д�С
	mRightLayerGroup->setSize(width,height);//�����Ҵ�С

	mTextBoxBG->setSize(width,height);//TextBoxBG
	mEffectLayer->setSize(width,height);//EFLayer
	mEffectLayer->eventMouseButtonClick+= MyGUI::newDelegate(this, &GUIStage::eventMouseButtonClick);
}

GUIStage::~GUIStage(void)
{
}

void GUIStage::showScene()
{

}

void GUIStage::hideScene()
{

}

void GUIStage::eventMouseButtonClick(MyGUI::Widget* _sender)
{
	if (mCheckMouseDown)
	{
		mIsMouseDown=true;
		mCheckMouseDown=false;
	}
}


void GUIStage::setCheckMouseDown()
{
	mCheckMouseDown=true;
	mIsMouseDown=false;
}

bool GUIStage::CheckMouseState()
{
	return mIsMouseDown;
}


void GUIStage::setTextDialog( const GUIDialogAttribute& attribute )
{
	mTextBox->setPosition(attribute.TextLeft,attribute.TextTop);
	mTextX=attribute.TextLeft;
	mTextY=attribute.TextTop;

	mTextBox->setSize(attribute.TextWidth,attribute.TextHeight);
	mTextBox->setTextColour(MyGUI::Colour(attribute.TextRed,attribute.TextGreen,attribute.TextBlue));
	mTextBox->setFontName(attribute.TextFont);

	mRoleName->setPosition(attribute.RoleNameLeft,attribute.RoleNameTop);
	mRoleName->setSize(attribute.RoleNameWidth,attribute.RoleNameHeight);
	mRoleName->setTextColour(MyGUI::Colour(attribute.RoleNameRed,attribute.RoleNameGreen,attribute.RoleNameBlue));
	mRoleName->setFontName(attribute.RoleFont);

	mRoleNameUniversal->setPosition(attribute.RoleNameLeft,attribute.RoleNameTop);
	mRoleNameUniversal->setSize(attribute.RoleNameWidth,attribute.RoleNameHeight);
	mRoleNameUniversal->setTextColour(MyGUI::Colour(attribute.RoleNameRed,attribute.RoleNameGreen,attribute.RoleNameBlue));
	mRoleNameUniversal->setFontName(attribute.RoleFont);
}


void GUIStage::setTextDialogVisible( bool visible )
{
	if (visible)
	{
		mSetp=0;
		mSetpDirection=true;
	}
	else
	{
		mSetp=1;
		mSetpDirection=false;
	}

	mTextBoxBG->setAlpha(mSetp);
	mTextBoxBG->setVisible(true);

	mTimerWork=DialogVisible;
	mTickTime=DefaultDialogVisibleTime*1000/100;

	//��ʼ֡����
	mTimer.reset();
	GUISystem::getSingletonPtr()->setFrameUpdateScene(StageScene);
}

void GUIStage::waitTime( float time )
{
	mTimerWork=WaitWork;
	mTickTime=time;

	//��ʼ֡����
	mTimer.reset();
	GUISystem::getSingletonPtr()->setFrameUpdateScene(StageScene);
}

void GUIStage::showText( std::wstring text,float delay)
{
	if(delay==0)
	{
		mTextBox->addText(text);
	}
	else
	{
		//����Ǵ���Ч��,��ô�������ֶ�ʱ��
		mTextBuffer=text;
		mTimerWork=PrinterWork;
		mTickTime=delay*1000;

		//��ʼ֡����
		mTimer.reset();
		GUISystem::getSingletonPtr()->setFrameUpdateScene(StageScene);
	}
}

void GUIStage::showImage( std::string imageName,GUIImageLayer layer,float time,int left,int top)
{

	//ѡ��ͼ��

	switch(layer)
	{
	case EffectsLayer:
		{
			mScrLayer=mEffectLayer;
			mUniversalLayer=NULL;
		}
	case LeftLayer:
		{
			mScrLayer=mLeftLayer;
			mUniversalLayer=mLeftLayerUniversal;
			break;
		}
	case MidLayer:
		{
			mScrLayer=mMidLayer;
			mUniversalLayer=mMidLayerUniversal;
			break;
		}
	case RightLayer:
		{
			mScrLayer=mRightLayer;
			mUniversalLayer=mRightUniversal;
			break;
		}
	case BackGroundLayer:
		{
			mScrLayer=mBackGround;
			mUniversalLayer=mBackGroundUniversal;
			break;
		}
	case AllLayer://���Ϊ������е�����
		{
			mSetp=1;
			mTimerWork=ClearAllRoleWork;
			mTickTime=time/100;

			//��ʼ֡����
			mTimer.reset();
			GUISystem::getSingletonPtr()->setFrameUpdateScene(StageScene);
			return;
		}
	}

	//Ԥ�ȼ���,��ȡͼƬ�Ŀ��

	int width,height=0;

	if (!imageName.empty())
	{
		if(!Ogre::TextureManager::getSingleton().resourceExists(imageName))
		{
			Ogre::Image i;
			i.load(imageName,Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
			width=i.getWidth();
			height=i.getHeight();
		}
		else
		{
			width=((Ogre::TexturePtr)Ogre::TextureManager::getSingleton().getByName(imageName))->getWidth();
			height=((Ogre::TexturePtr)Ogre::TextureManager::getSingleton().getByName(imageName))->getHeight();
		}
	}

	if (mScrLayer!=NULL)
	{
		mScrLayer->setPosition(left,top);
		mUniversalLayer->setPosition(left,top);
		if (!imageName.empty())//�������Ϊ�յĻ�...��ô��������С
		{
			mScrLayer->setSize(width,height);
			mUniversalLayer->setSize(width,height);
		}


		if (time!=0)//���Ҫʹ�ý������
		{
			mUniversalLayer->setAlpha(0);
			mUniversalLayer->setImageTexture(imageName);
			mUniversalLayer->setVisible(true);

			mSetp=0;
			mTimerWork=UniversalWork;
			mTickTime=time/100;
			mTextureName=imageName;

			//��ʼ֡����
			mTimer.reset();
			GUISystem::getSingletonPtr()->setFrameUpdateScene(StageScene);
		}
		else
		{
			mScrLayer->setImageTexture(imageName);
		}

	}
}

void GUIStage::showRoleName( std::wstring text )
{
	mRoleNameUniversal->setAlpha(0);
	mRoleNameUniversal->setCaption(text);
	mRoleNameUniversal->setVisible(true);

	mSetp=0;
	mTimerWork=RoleNameWork;
	mTickTime=DefaultRoleNameVisibleTime*1000/100;

	//��ʼ֡����
	mTimer.reset();
	GUISystem::getSingletonPtr()->setFrameUpdateScene(StageScene);
}

void GUIStage::showOtherText()
{
	//ֹͣ֡����
	mTickTime=0;
	mTimerWork=NoneWork;
	GUISystem::getSingletonPtr()->setFrameUpdateScene(NoneScene);

	mTextBox->addText(mTextBuffer);
	mTextBuffer.clear();
}

void GUIStage::clearText()
{
	mTextBox->setCaption("");
}

void GUIStage::FrameEvent()
{
	if(mTickTime!=0)
	{
		if (mTimer.getMilliseconds()>=mTickTime)
		{
			//��λ��ʱ��
			mTimer.reset();

			switch(mTimerWork)
			{
			case PrinterWork:
				{
					if (!mTextBuffer.empty())
					{
						mTextBox->addText(mTextBuffer.substr(0,1));
						if (mTextBox->getHScrollPosition()!=0)//�Զ�����
						{
							int length=mTextBox->getTextLength();
							mTextBox->eraseText(length-1);
							mTextBox->addText("\n");
							mTextBox->addText(mTextBuffer.substr(0,1));
						}
						mTextBuffer.erase(mTextBuffer.begin());
					}
					else
					{
						//ֹͣ֡����
						mTickTime=0;
						mTimerWork=NoneWork;
						GUISystem::getSingletonPtr()->setFrameUpdateScene(NoneScene);
					}
					break;
				}
			case UniversalWork:
				{
					if(mSetp+0.01<=1.0)
					{

						mScrLayer->setAlpha(1.0-mSetp);
						mUniversalLayer->setAlpha(mSetp);

						mSetp+=0.01;
					}
					else
					{
						mScrLayer->setImageTexture(mTextureName);
						mScrLayer->setAlpha(1.0);
						mUniversalLayer->setVisible(false);

						//ֹͣ֡����
						mTickTime=0;
						mTimerWork=NoneWork;
						GUISystem::getSingletonPtr()->setFrameUpdateScene(NoneScene);
					}

					break;
				}
				case DialogVisible:
					{
						if (mSetpDirection)//�ж����䲽����
						{
							if(mSetp+0.01<=1.0)
							{
								mSetp+=0.01;
								
								mTextBoxBG->setAlpha(mSetp);
							}
							else
							{
								//ֹͣ֡����
								mTickTime=0;
								mTimerWork=NoneWork;
								GUISystem::getSingletonPtr()->setFrameUpdateScene(NoneScene);
							}
						}
						else
						{
							if(mSetp-0.01>=0)
							{
								mSetp-=0.01;
								
								mTextBoxBG->setAlpha(mSetp);
							}
							else
							{

								mTextBoxBG->setVisible(false);

								//ֹͣ֡����
								mTickTime=0;
								mTimerWork=NoneWork;
								GUISystem::getSingletonPtr()->setFrameUpdateScene(NoneScene);
							}

						}

						break;
					}
				case WaitWork:
					{
						//ֹͣ֡����
						mTickTime=0;
						mTimerWork=NoneWork;
						GUISystem::getSingletonPtr()->setFrameUpdateScene(NoneScene);

						break;
					}
				case RoleNameWork:
					{
						if(mSetp+0.01<=1.0)
						{
							mRoleName->setAlpha(1.0-mSetp);
							mRoleNameUniversal->setAlpha(mSetp);

							mSetp+=0.01;
						}
						else
						{
							std::string s=mRoleNameUniversal->getCaption();
							mRoleNameUniversal->setVisible(false);
							mRoleName->setCaption(s);
							mRoleName->setAlpha(1);

							//ֹͣ֡����
							mTickTime=0;
							mTimerWork=NoneWork;
							GUISystem::getSingletonPtr()->setFrameUpdateScene(NoneScene);
						}

						break;
					}
				case ClearAllRoleWork:
					{
						if(mSetp-0.01>=0)
						{
							mSetp-=0.01;

							mLeftLayer->setAlpha(mSetp);
							mMidLayer->setAlpha(mSetp);
							mRightLayer->setAlpha(mSetp);

						}
						else
						{
							
							mLeftLayer->setImageTexture("");
							mLeftLayer->setAlpha(1.0);
							mMidLayer->setImageTexture("");
							mMidLayer->setAlpha(1.0);
							mRightLayer->setImageTexture("");
							mRightLayer->setAlpha(1.0);

							//ֹͣ֡����
							mTickTime=0;
							mTimerWork=NoneWork;
							GUISystem::getSingletonPtr()->setFrameUpdateScene(NoneScene);
						}
					}
			}
		}
	}
}

void GUIStage::showTextCursor( bool isLine )
{
	MyGUI::IntPoint p=mTextBox->getTextCursorPos();

	mTextCursor->setPosition(p.left,p.top);
	mTextCursor->setVisible(true);
	mTextCursor->setItemResource ("TextCursorImage");

	if(!isLine)
	{
		mTextCursor->setItemGroup("LineBreak");
	}
	else
	{
		mTextCursor->setItemGroup("PageBreak");
	}

	mTextCursor->setItemName("show");

}

void GUIStage::hideTextCursor()
{
	mTextCursor->setVisible(false);
}
