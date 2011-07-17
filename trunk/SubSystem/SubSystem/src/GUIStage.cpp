#include "GUIStage.h"

#include "GUISystem.h"
#include "LuaSystem.h"

#include <ogre.h>

#ifndef SCRIPT_EDITOR
#include "GUISLWindow.h"
#include "DataLibrary.h"
#endif

GUIStage::GUIStage(int width,int height):GUIScene("Stage.layout",width,height),mCheckMouseDown(false),mIsMouseDown(false),mTextX(0),mTextY(0),mIsFastForward(false)
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

	assignWidget(mSaveButton, "SaveButton");
	assignWidget(mLoadButton, "LoadButton");
	assignWidget(mHideButton,"HideButton");
	assignWidget(mSystemButton,"SystemButton");

	mEffectLayer->eventMouseButtonClick+= MyGUI::newDelegate(this, &GUIStage::eventMouseButtonClick);
	mSaveButton->eventMouseButtonClick+=MyGUI::newDelegate(this, &GUIStage::onSave);
	mLoadButton->eventMouseButtonClick+= MyGUI::newDelegate(this, &GUIStage::onLoad);
	mHideButton->eventMouseButtonClick+= MyGUI::newDelegate(this, &GUIStage::onHide);
}

GUIStage::~GUIStage(void)
{

}

void GUIStage::UIInit()
{
	mBackGroundGroup->setSize(mWidth,mHeigth);//���ñ�����С
	mBackGround->setSize(mWidth,mHeigth);
	mBackGroundUniversal->setSize(mWidth,mHeigth);

	mLeftLayerGroup->setSize(mWidth,mHeigth);//�������С
	mMidLayerGroup->setSize(mWidth,mHeigth);//�����д�С
	mRightLayerGroup->setSize(mWidth,mHeigth);//�����Ҵ�С

	mTextBoxBG->setSize(mWidth,mHeigth);//TextBoxBG
	mEffectLayer->setSize(mWidth,mHeigth);//EFLayer

	mBackGround->setAlpha(1);
	mBackGroundUniversal->setImageTexture("");
	mLeftLayer->setAlpha(1);
	mLeftLayerUniversal->setImageTexture("");
	mMidLayer->setAlpha(1);
	mMidLayerUniversal->setImageTexture("");
	mRightLayer->setAlpha(1);
	mRightUniversal->setImageTexture("");
	mRoleName->setAlpha(1);
	mRoleNameUniversal->setCaption("");
	
	hideTextCursor();
#ifndef SCRIPT_EDITOR
	LuaSystem::getSingletonPtr()->executeFunction("AVGInit.lua","AVGInit","1");//���н����ʼ�趨
#endif
}

void GUIStage::showScene(std::string arg)
{
	UIInit();
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

void GUIStage::keyPressed(const OIS::KeyEvent &arg )
{
	if (arg.key==OIS::KC_LCONTROL)
	{
		fastForward(true);
		
	}
}

void GUIStage::keyReleased(const OIS::KeyEvent &arg )
{
	if (arg.key==OIS::KC_LCONTROL)
	{
		fastForward(false);
	}
}

void GUIStage::fastForward( bool value )
{
	mIsFastForward=value;
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
	mTextBoxBG->setPosition(attribute.TextBgLeft,attribute.TextBgTop);
	mTextBoxBG->setSize(attribute.TextBgWidth,attribute.TextBgHeight);
	mTextBoxBG->setImageTexture(attribute.TextBgImage);

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
	mTextBoxVisible=visible;
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
	mFadeWidget=mTextBoxBG;

	mTimerWork=FadeInOutWork;
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
			mLeftImageName=imageName;
			break;
		}
	case MidLayer:
		{
			mScrLayer=mMidLayer;
			mUniversalLayer=mMidLayerUniversal;
			mMidImageName=imageName;
			break;
		}
	case RightLayer:
		{
			mScrLayer=mRightLayer;
			mUniversalLayer=mRightUniversal;
			mRightImageName=imageName;
			break;
		}
	case BackGroundLayer:
		{
			mScrLayer=mBackGround;
			mUniversalLayer=mBackGroundUniversal;
			mBackGroundImageName=imageName;
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
		if (mTimer.getMilliseconds()>=mTickTime || mIsFastForward)
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
				case FadeInOutWork:
					{
						FadeInOut();//���û���ĵ��뵭������

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

	int l=mTextBoxBG->getLeft();
	int t=mTextBoxBG->getTop();

	mTextCursor->setPosition(p.left-l,p.top-t);
	mTextCursor->setVisible(true);
	mTextCursor->setItemResource ("TextCursorImage");
	mTextCursorType=isLine;

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

void GUIStage::onSave(MyGUI::Widget* _sender)
{
#ifndef SCRIPT_EDITOR
	//д�볡������
	//��¼ͼ��
	DataLibrary::getSingletonPtr()->setData("GameData/StoryData/BackGroundName",mBackGroundImageName);
	DataLibrary::getSingletonPtr()->setData("GameData/StoryData/BackGroundPosition",Ogre::Vector3(mBackGround->getLeft(),mBackGround->getTop(),0));
	DataLibrary::getSingletonPtr()->setData("GameData/StoryData/BackGroundSize",Ogre::Vector3(mBackGround->getWidth(),mBackGround->getHeight(),0));

	DataLibrary::getSingletonPtr()->setData("GameData/StoryData/LeftName",mLeftImageName);
	DataLibrary::getSingletonPtr()->setData("GameData/StoryData/LeftPosition",Ogre::Vector3(mLeftLayer->getLeft(),mLeftLayer->getTop(),0));
	DataLibrary::getSingletonPtr()->setData("GameData/StoryData/LeftSize",Ogre::Vector3(mLeftLayer->getWidth(),mLeftLayer->getHeight(),0));

	DataLibrary::getSingletonPtr()->setData("GameData/StoryData/MidName",mMidImageName);
	DataLibrary::getSingletonPtr()->setData("GameData/StoryData/MidPosition",Ogre::Vector3(mMidLayer->getLeft(),mMidLayer->getTop(),0));
	DataLibrary::getSingletonPtr()->setData("GameData/StoryData/MidSize",Ogre::Vector3(mMidLayer->getWidth(),mMidLayer->getHeight(),0));

	DataLibrary::getSingletonPtr()->setData("GameData/StoryData/RightName",mRightImageName);
	DataLibrary::getSingletonPtr()->setData("GameData/StoryData/RightPosition",Ogre::Vector3(mRightLayer->getLeft(),mRightLayer->getTop(),0));
	DataLibrary::getSingletonPtr()->setData("GameData/StoryData/RightSize",Ogre::Vector3(mRightLayer->getWidth(),mRightLayer->getHeight(),0));

	//��¼�ı�
	if (mTimerWork==PrinterWork)
	{
		DataLibrary::getSingletonPtr()->setData("GameData/StoryData/Text",mTextBox->getOnlyText()+mTextBuffer);
	}
	DataLibrary::getSingletonPtr()->setData("GameData/StoryData/Text",mTextBox->getOnlyText());
	DataLibrary::getSingletonPtr()->setData("GameData/StoryData/RoleName",mRoleName->getOnlyText());
	DataLibrary::getSingletonPtr()->setData("GameData/StoryData/TextCursorType",mTextCursorType);
	
	//д��ű�����λ��
	LuaSystem::getSingletonPtr()->saveScriptRuntime();

	GUISLWindow* SLWindow= (GUISLWindow*)GUISystem::getSingletonPtr()->createScene(SLScene);
	SLWindow->showScene("save");
#endif
}

void GUIStage::onLoad(MyGUI::Widget* _sender)
{
#ifndef SCRIPT_EDITOR
	GUISLWindow* SLWindow= (GUISLWindow*)GUISystem::getSingletonPtr()->createScene(SLScene);
	SLWindow->setCallScene(this);
	SLWindow->showScene("load");
#endif
}

void GUIStage::onHide( MyGUI::Widget* _sender )
{
	if (mTextBoxVisible)
	{
		setTextDialogVisible(false);
	}
	else
	{
		setTextDialogVisible(true);
	}
	
}

void GUIStage::onSystem( MyGUI::Widget* _sender )
{

}

void GUIStage::load()
{
#ifndef SCRIPT_EDITOR
	//��ȡ��������
	//��ȡͼ��
	Ogre::Vector3 LayerPosition;
	Ogre::Vector3 LayerSize;
	std::string ImageName;

	UIInit();

	DataLibrary::getSingletonPtr()->getData("GameData/StoryData/BackGroundName",ImageName);
	DataLibrary::getSingletonPtr()->getData("GameData/StoryData/BackGroundPosition",LayerPosition);
	DataLibrary::getSingletonPtr()->getData("GameData/StoryData/BackGroundSize",LayerSize);
	mBackGroundImageName=ImageName;
	mBackGround->setImageTexture(ImageName);
	mBackGround->setPosition(LayerPosition.x,LayerPosition.y);
	mBackGround->setSize(LayerSize.x,LayerSize.y);


	DataLibrary::getSingletonPtr()->getData("GameData/StoryData/LeftName",ImageName);
	DataLibrary::getSingletonPtr()->getData("GameData/StoryData/LeftPosition",LayerPosition);
	DataLibrary::getSingletonPtr()->getData("GameData/StoryData/LeftSize",LayerSize);
	mLeftImageName=ImageName;
	mLeftLayer->setImageTexture(ImageName);
	mLeftLayer->setPosition(LayerPosition.x,LayerPosition.y);
	mLeftLayer->setSize(LayerSize.x,LayerSize.y);

	DataLibrary::getSingletonPtr()->getData("GameData/StoryData/MidName",ImageName);
	DataLibrary::getSingletonPtr()->getData("GameData/StoryData/MidPosition",LayerPosition);
	DataLibrary::getSingletonPtr()->getData("GameData/StoryData/MidSize",LayerSize);
	mMidImageName=ImageName;
	mMidLayer->setImageTexture(ImageName);
	mMidLayer->setPosition(LayerPosition.x,LayerPosition.y);
	mMidLayer->setSize(LayerSize.x,LayerSize.y);

	DataLibrary::getSingletonPtr()->getData("GameData/StoryData/RightName",ImageName);
	DataLibrary::getSingletonPtr()->getData("GameData/StoryData/RightPosition",LayerPosition);
	DataLibrary::getSingletonPtr()->getData("GameData/StoryData/RightSize",LayerSize);
	mRightImageName=ImageName;
	mRightLayer->setImageTexture(ImageName);
	mRightLayer->setPosition(LayerPosition.x,LayerPosition.y);
	mRightLayer->setSize(LayerSize.x,LayerSize.y);

	//�����ı�
	mTextBuffer.clear();
	std::string text;
	DataLibrary::getSingletonPtr()->getData("GameData/StoryData/Text",text);
	mTextBox->setCaption(text);
	DataLibrary::getSingletonPtr()->getData("GameData/StoryData/RoleName",text);
	mRoleName->setCaption(text);
	int type;
	DataLibrary::getSingletonPtr()->getData("GameData/StoryData/TextCursorType",type);
	//showTextCursor(type);

	//��ȡ�ű�����λ��
	LuaSystem::getSingletonPtr()->loadScripRuntime();
#endif
}

void GUIStage::onOtherSceneNotify(std::string arg)
{
	if(arg=="LoadComplete")
	{
		load();
	}
}
