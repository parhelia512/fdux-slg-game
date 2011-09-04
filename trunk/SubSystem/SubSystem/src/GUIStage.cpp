#include "GUIStage.h"

#include "GUISystem.h"
#include "LuaSystem.h"

#include "AudioSystem.h"

#include <ogre.h>

#ifndef SCRIPT_EDITOR
#include "GUISLWindow.h"
#include "GUIMenuWindow.h"
#include "DataLibrary.h"
#include "StringTable.h"
#include <boost/format.hpp>
#endif

#define DefaultDialogVisibleTime 500//Ĭ�϶Ի��򽥳�ʱ��
#define DefaultRoleNameVisibleTime 100//Ĭ�Ͻ�ɫ������ʱ��
#define LineCursorFileName "LineCursor.png"//Ĭ���й���ļ���
#define PageCursorFileName "PageCursor.png"//Ĭ�ϻ�ҳ����ļ���

#define SAVE_PATH "..\\save"

GUIStage::GUIStage(int width,int height):GUIScene("Stage.layout",width,height),mCheckMouseDown(false),mIsMouseDown(false),mTextX(0),mTextY(0),mIsFastForward(false),SLWindow(NULL)
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

	assignWidget(mEffectLayerGroup, "EffectLayerGroup");
	assignWidget(mEffectLayer, "EffectLayer");
	assignWidget(mEffectLayerUniversal, "EffectLayerUniversal");

	assignWidget(mHistoryBox, "HistoryBox");
	assignWidget(mHistoryBoxExit, "HistoryBoxExit");

	assignWidget(mInputGroup, "InputGroup");
	assignWidget(mInputLayer, "InputLayer");
	assignWidget(mSaveButton, "SaveButton");
	assignWidget(mLoadButton, "LoadButton");
	assignWidget(mHideButton,"HideButton");
	assignWidget(mSystemButton,"SystemButton");
	assignWidget(mHistoryButton,"HistoryButton");

#ifndef SCRIPT_EDITOR
	mSaveButton->setCaption(StringTable::getSingletonPtr()->getString("SaveButton"));
	mLoadButton->setCaption(StringTable::getSingletonPtr()->getString("LoadButton"));
	mHideButton->setCaption(StringTable::getSingletonPtr()->getString("HideButton"));
	mSystemButton->setCaption(StringTable::getSingletonPtr()->getString("SystemButton"));
	mHistoryButton->setCaption(StringTable::getSingletonPtr()->getString("HistoryButton"));
#endif

	mInputLayer->eventMouseButtonClick+= MyGUI::newDelegate(this, &GUIStage::eventMouseButtonClick);
	mHistoryBoxExit->eventMouseButtonClick+= MyGUI::newDelegate(this, &GUIStage::eventHistoryBoxExit);
	mSaveButton->eventMouseButtonClick+=MyGUI::newDelegate(this, &GUIStage::onSave);
	mLoadButton->eventMouseButtonClick+= MyGUI::newDelegate(this, &GUIStage::onLoad);
	mHideButton->eventMouseButtonClick+= MyGUI::newDelegate(this, &GUIStage::onHide);
	mSystemButton->eventMouseButtonClick+= MyGUI::newDelegate(this, &GUIStage::onSystem);
	mHistoryButton->eventMouseButtonClick+= MyGUI::newDelegate(this, &GUIStage::onHistory);
}

GUIStage::~GUIStage(void)
{
#ifndef SCRIPT_EDITOR
	if (SLWindow!=NULL)
	{
		SLWindow->setCallScene(SLWindow);
	}
	AudioSystem::getSingletonPtr()->stopStream(1000);
#endif
}

void GUIStage::UIInit()
{
	mBackGroundGroup->setSize(mWidth,mHeigth);//���ñ�����С
	mBackGround->setSize(mWidth,mHeigth);
	mBackGroundUniversal->setSize(mWidth,mHeigth);

	mLeftLayerGroup->setSize(mWidth,mHeigth);//�������С
	mMidLayerGroup->setSize(mWidth,mHeigth);//�����д�С
	mRightLayerGroup->setSize(mWidth,mHeigth);//�����Ҵ�С
	mEffectLayerGroup->setSize(mWidth,mHeigth);

	mInputGroup->setSize(mWidth,mHeigth);
	mInputLayer->setSize(mWidth,mHeigth);

	mTextBoxBG->setSize(mWidth,mHeigth);//TextBoxBG
	mEffectLayer->setSize(mWidth,mHeigth);//EFLayer
	mEffectLayerUniversal->setSize(mWidth,mHeigth);//EFLayer

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
	getMouseState();
}

void GUIStage::eventHistoryBoxExit(MyGUI::Widget* _sender)
{
	setHistoryBoxVisible(false);
}

void GUIStage::keyPressed(const OIS::KeyEvent &arg )
{
	if (arg.key==OIS::KC_LCONTROL && (!mHistoryBox->getVisible()))
	{
		mIsFastForward=true;
	}
	else if(arg.key==OIS::KC_SPACE || arg.key==OIS::KC_RETURN)
	{
		getMouseState();
	}
}

void GUIStage::keyReleased(const OIS::KeyEvent &arg )
{
	if (arg.key==OIS::KC_LCONTROL)
	{
		mIsFastForward=false;
	}
}

void GUIStage::getMouseState() 
{
	if (mCheckMouseDown && (!mHistoryBox->getVisible()))
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
	if (mIsFastForward)
	{
		return true;
	}
	else
	{
		return mIsMouseDown;
	}
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

	mHistoryBox->setTextColour(MyGUI::Colour::White);
	mHistoryBox->setFontName(attribute.TextFont);

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
		FadeIn(DefaultDialogVisibleTime,mTextBoxBG);
	}
	else
	{
		FadeOut(DefaultDialogVisibleTime,mTextBoxBG);
	}

	mTimerWork=FadeInOutWork;

	buttonLock(false);
}

void GUIStage::setHistoryBoxVisible( bool visible )
{
	if (visible)
	{
		mInputGroup->setVisible(false);
	}
	else
	{
		mInputGroup->setVisible(true);
	}

	mHistoryBox->setVisible(visible);
	mHistoryBoxExit->setVisible(visible);
	setTextDialogVisible(!visible);
}

void GUIStage::addToHistoryBox(std::wstring text)
{
	if(!mCurrentRoleName.empty())
	{
		mHistoryBox->addText(std::wstring(L"#FF0000")+mCurrentRoleName+std::wstring(L"#FFFFFF : "));
		mCurrentRoleName.clear();
	}

	MyGUI::IntPoint p=mHistoryBox->getTextCursorPos();

	int lineTextNum=(mHistoryBox->getWidth()-p.left-30)/25;

	if (lineTextNum>text.length())
	{
		mHistoryBox->addText(text);
	}
	else
	{
		mHistoryBox->addText(text.substr(0,lineTextNum));
		text.erase(0,lineTextNum);

		lineTextNum=(mHistoryBox->getWidth()-30)/25;

		for (int i=0;i<=(text.length()/lineTextNum);i++)
		{
			mHistoryBox->addText("\n");
			mHistoryBox->addText(text.substr(0,lineTextNum));
			text.erase(0,lineTextNum);
		}
	}
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
	addToHistoryBox(text);

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

void GUIStage::showOtherText()
{
	//ֹͣ֡����
	mTickTime=0;
	mTimerWork=NoneWork;
	GUISystem::getSingletonPtr()->setFrameUpdateScene(NoneScene);

	MyGUI::IntPoint p=mTextBox->getTextCursorPos();

	int lineTextNum=(mTextBox->getWidth()-p.left+mTextBox->getLeft())/25;

	if (lineTextNum>mTextBuffer.length())
	{
		mTextBox->addText(mTextBuffer);
	}
	else
	{
		mTextBox->addText(mTextBuffer.substr(0,lineTextNum));
		mTextBuffer.erase(0,lineTextNum);

		lineTextNum=(mTextBox->getWidth())/25;

		for (int i=0;i<=(mTextBuffer.length()/lineTextNum);i++)
		{
			mTextBox->addText("\n");
			mTextBox->addText(mTextBuffer.substr(0,lineTextNum));
			mTextBuffer.erase(0,lineTextNum);
		}
	}

	//while(!mTextBuffer.empty())
	//{
	//	std::wstring subString=mTextBuffer.substr(0,1);
	//	mTextBox->addText(subString);
	//	if (mTextBox->getHScrollPosition()!=0)//�Զ�����
	//	{
	//		int length=mTextBox->getTextLength();
	//		mTextBox->eraseText(length-1);
	//		mTextBox->addText("\n");
	//		mTextBox->addText(subString);
	//	}
	//	mTextBuffer.erase(mTextBuffer.begin());
	//}
	mTextBuffer.clear();
}

void GUIStage::showImage( std::string imageName,GUIImageLayer layer,float time,int left,int top)
{

	//ѡ��ͼ��

	switch(layer)
	{
	case EffectsLayer:
		{
			mEffectLayerGroup->setVisible(true);
			mScrLayer=mEffectLayer;
			mUniversalLayer=mEffectLayerUniversal;
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
			mTimerWork=ClearAllRoleWork;
			
			FadeOut(time,mLeftLayer);
			FadeOut(time,mMidLayer);
			FadeOut(time,mRightLayer);

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
			mUniversalLayer->setImageTexture(imageName);

			mTimerWork=UniversalWork;
			mTextureName=imageName;

			mScrLayer->setAlpha(1);
			mUniversalLayer->setAlpha(0);
			
			FadeOut(time,mScrLayer);
			FadeIn(time,mUniversalLayer);
			GUISystem::getSingletonPtr()->setFrameUpdateScene(StageScene);
		}
		else
		{
			mScrLayer->setImageTexture(imageName);
		}

	}
}

void GUIStage::moveImage(GUIImageLayer layer,float time,int left,int top)
{
	switch(layer)
	{
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
	}

	mTimerWork=MoveWork;
	mUniversalLayer->setPosition(left,top);
	mUniversalLayer->setVisible(false);
	MoveTo(left,top,time,mScrLayer);
	GUISystem::getSingletonPtr()->setFrameUpdateScene(StageScene);

}

void GUIStage::shockImage(GUIImageLayer layer,float durationTime,float amplitudeX,float amplitudeY)
{
	switch(layer)
	{
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
	}

	mUniversalLayer->setVisible(false);
	mShockParam.mImageX=mScrLayer->getLeft();
	mShockParam.mImageY=mScrLayer->getTop();
	mShockParam.mAge=0;
	mShockParam.mDurationTime=durationTime;
	mShockParam.mAmplitudeX=amplitudeX;
	mShockParam.mAmplitudeY=amplitudeY;

	mTimerWork=ShockWork;
	mTickTime=1;
	GUISystem::getSingletonPtr()->setFrameUpdateScene(StageScene);

}

void GUIStage::showRoleName( std::wstring text )
{
	mCurrentRoleName=text;
	mRoleNameUniversal->setCaption(text);
	mRoleNameUniversal->setAlpha(0);
	mRoleName->setAlpha(1);

	mTimerWork=RoleNameWork;

	FadeIn(DefaultRoleNameVisibleTime,mRoleNameUniversal);
	FadeOut(DefaultRoleNameVisibleTime,mRoleName);
	GUISystem::getSingletonPtr()->setFrameUpdateScene(StageScene);
}

void GUIStage::clearText()
{
	mTextBox->setCaption("");
	addToHistoryBox(L"\n");
}

bool GUIStage::isCanFastForward()
{
	if (mTimerWork==FadeInOutWork)
	{
		return false;
	}
	if (!mTextBoxVisible)
	{
		return false;
	}
	else if (SLWindow!=NULL)
	{
		return false;
	}
	else if(mHistoryBox->getVisible())
	{
		return false;
	}

	return true;
}

void GUIStage::fastForward()
{
	if (!isCanFastForward())
	{
		mIsFastForward=false;
		return;
	}

	switch(mTimerWork)
	{
	case PrinterWork:
		{
			showOtherText();
			break;
		}
	case MoveWork:
		{
			StopMoveTo(mScrLayer);
			break;
		}
	case ShockWork:
		{
			mShockParam.mAge =mShockParam.mDurationTime+1;
			break;
		}
	case UniversalWork:
		{
			StopFadeOut(mScrLayer);
			StopFadeIn(mUniversalLayer);
			break;
		}
	case WaitWork:
		{
			mTickTime=1;
			break;
		}
	case RoleNameWork:
		{
			StopFadeOut(mRoleName);
			StopFadeIn(mRoleNameUniversal);
			break;
		}
	case ClearAllRoleWork:
		{
			StopFadeOut(mLeftLayer);
			StopFadeOut(mMidLayer);
			StopFadeOut(mRightLayer);
			break;
		}
	}
}

void GUIStage::FrameEvent()
{
	if (mIsFastForward)
	{
		fastForward();
	}

	if(mTickTime!=0)
	{
		if (mTimer.getMilliseconds()>=mTickTime )
		{
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
				case WaitWork:
					{
						//ֹͣ֡����
						mTickTime=0;
						mTimerWork=NoneWork;
						GUISystem::getSingletonPtr()->setFrameUpdateScene(NoneScene);

						break;
					}
				case ShockWork:
					{
						// ��¼��ǰ�ۼ���ʱ��
						mShockParam.mAge += mTimer.getMilliseconds() / 1000.0;

						// ����ʱ���λ
						if(mShockParam.mAge >mShockParam.mDurationTime)
						{
							mScrLayer->setPosition(mShockParam.mImageX,mShockParam.mImageY);
							mUniversalLayer->setVisible(true);
							//ֹͣ֡����
							mTickTime=0;
							mTimerWork=NoneWork;
							GUISystem::getSingletonPtr()->setFrameUpdateScene(NoneScene);
						}
						else
						{
							// ���չ�ʽ��ƫ����
							
							int x,y=0;

							if (mShockParam.mAmplitudeX!=0)
								x=int(((float)rand())/RAND_MAX * mShockParam.mAmplitudeX - mShockParam.mAmplitudeX);

							if (mShockParam.mAmplitudeY!=0)
								y= int(((float)rand())/RAND_MAX * mShockParam.mAmplitudeY - mShockParam.mAmplitudeY);

							// ����ƫ�Ʋ�����Ч��
							mScrLayer->setPosition(mShockParam.mImageX+x,mShockParam.mImageY+y);
						}

						break;
					}
			}

			//��λ��ʱ��
			mTimer.reset();
		}
	}
}

void GUIStage::onOtherSceneNotify(std::string arg)
{
	if(arg=="LoadSelect")
	{
		load();
	}
	else if(arg=="LoadComplete")
	{
		loadComplete();
		SLWindow=NULL;
	}
	else if (arg=="Return")
	{
		returnScene();
		SLWindow=NULL;
	}
	else if(arg=="FadeInOver")
	{
		switch(mTimerWork)
		{
		case FadeInOutWork:
			{
				buttonLock(true);
				mTimerWork=NoneWork;
				break;
			}
		}

	}
	else if(arg=="FadeOutOver")
	{
		switch(mTimerWork)
		{
		case ClearAllRoleWork:
			{
				mLeftLayer->setImageTexture("");
				mLeftLayer->setAlpha(1.0);
				mMidLayer->setImageTexture("");
				mMidLayer->setAlpha(1.0);
				mRightLayer->setImageTexture("");
				mRightLayer->setAlpha(1.0);
				break;
			}
		case FadeInOutWork:
			{
				buttonLock(true);
				break;
			}
		case RoleNameWork:
			{
				std::string s=mRoleNameUniversal->getCaption();
				mRoleName->setCaption(s);
				mRoleName->setAlpha(1);
				mRoleNameUniversal->setAlpha(0);
				break;
			}
		case UniversalWork:
			{
				mScrLayer->setImageTexture(mTextureName);
				mScrLayer->setAlpha(1);
				mUniversalLayer->setAlpha(0);
				if (mEffectLayerGroup->getVisible())
				{
					mEffectLayerGroup->setVisible(false);
				}
				break;
			}
		}

		mTimerWork=NoneWork;
		GUISystem::getSingletonPtr()->setFrameUpdateScene(NoneScene);
	}
	else if(arg=="MoveToOver")
	{
		mScrLayer->setPosition(mUniversalLayer->getPosition());
		mUniversalLayer->setVisible(true);

		mTimerWork=NoneWork;
		GUISystem::getSingletonPtr()->setFrameUpdateScene(NoneScene);
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

void GUIStage::saveData(bool isAutoSave)
{
#ifndef SCRIPT_EDITOR
	//д�볡������
	DataLibrary::getSingletonPtr()->setData("GameData/StoryData/GameState","AVG");
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
		std::wstring text=mTextBuffer;
		std::wstring temp=mTextBox->getCaption();
		while(!text.empty())
		{
			mTextBox->addText(text.substr(0,1));
			if (mTextBox->getHScrollPosition()!=0)//�Զ�����
			{
				int length=mTextBox->getTextLength();
				mTextBox->eraseText(length-1);
				mTextBox->addText("\n");
				mTextBox->addText(text.substr(0,1));
			}
			text.erase(text.begin());
		}

		DataLibrary::getSingletonPtr()->setData("GameData/StoryData/Text",mTextBox->getOnlyText());
		mTextBox->setCaption(temp);
	}
	else
	{
		DataLibrary::getSingletonPtr()->setData("GameData/StoryData/Text",mTextBox->getOnlyText());
	}

	DataLibrary::getSingletonPtr()->setData("GameData/StoryData/RoleName",mRoleName->getOnlyText());
	DataLibrary::getSingletonPtr()->setData("GameData/StoryData/TextCursorType",mTextCursorType);

	//д��ű�����λ��
	LuaSystem::getSingletonPtr()->saveScriptRuntime();

	//д������
	DataLibrary::getSingletonPtr()->setData("GameData/StoryData/MusicName",AudioSystem::getSingletonPtr()->mStreamName);

	if (isAutoSave)
	{
		//����ʱ���
		SYSTEMTIME systm;
		GetLocalTime(&systm);
		std::string timeString=str(boost::format(" %1%-%2%-%3% %4%:%5%:%6%")%systm.wYear%systm.wMonth%systm.wDay%systm.wHour%systm.wMinute%systm.wSecond);

		DataLibrary::getSingletonPtr()->setData("SystemConfig/Save/Save1",std::string("AutoSave")+timeString);
		DataLibrary::getSingletonPtr()->saveXmlData(DataLibrary::SystemConfig,std::string(SAVE_PATH)+std::string("\\Config.xml"));
		DataLibrary::getSingletonPtr()->saveXmlData(DataLibrary::GameData,std::string(SAVE_PATH)+std::string("\\save1.xml"));
	}
#endif
}

void GUIStage::onSave(MyGUI::Widget* _sender)
{
#ifndef SCRIPT_EDITOR
	saveData(false);

	SLWindow= (GUISLWindow*)GUISystem::getSingletonPtr()->createScene(SLScene);
	SLWindow->setCallScene(this);
	SLWindow->showScene("save");
#endif
}

void GUIStage::onLoad(MyGUI::Widget* _sender)
{
#ifndef SCRIPT_EDITOR
	SLWindow= (GUISLWindow*)GUISystem::getSingletonPtr()->createScene(SLScene);
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

void GUIStage::onHistory( MyGUI::Widget* _sender )
{
	setHistoryBoxVisible(true);
}

void GUIStage::onSystem( MyGUI::Widget* _sender )
{
#ifndef SCRIPT_EDITOR
	GUIMenuWindow* menuWindow =static_cast<GUIMenuWindow *>(GUISystem::getSingletonPtr()->createScene(MenuWindowsScene));
	menuWindow->showScene("");
#endif
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

	mTextBoxVisible=true;
	mTextBoxBG->setVisible(true);
	mTextBoxBG->setAlpha(1);

	//��ȡ����
	
	DataLibrary::getSingletonPtr()->getData("GameData/StoryData/MusicName",text);
	if (text!="none" && text!="")
	{
		AudioSystem::getSingletonPtr()->playStream(text,true,1000);
	}

#endif
}

void GUIStage::loadComplete()
{
	//��ȡ�ű�����λ��
	LuaSystem::getSingletonPtr()->loadScripRuntime();
	returnScene();

}

void GUIStage::returnScene()
{
	if (mTimerWork!=NoneWork)
	{
		if (mTimerWork==PrinterWork)
		{
			mTimer.reset();
			GUISystem::getSingletonPtr()->setFrameUpdateScene(StageScene);
			if (mTextCursor->getVisible())
			{
				hideTextCursor();
			}
		}
		else
		{
			mTimerWork=NoneWork;
		}

	}
}

void GUIStage::buttonLock( bool lock )
{
	mSaveButton->setEnabled(lock);
	mLoadButton->setEnabled(lock);
	mHideButton->setEnabled(lock);
	mSystemButton->setEnabled(lock);
	mHistoryButton->setEnabled(lock);
}


