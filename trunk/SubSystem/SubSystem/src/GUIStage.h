#pragma once

#include "GUIScene.h"

class GUIStage:public GUIScene
{
public:
	GUIStage(int width,int height);
	~GUIStage(void);

	void showScene(std::string arg);//��ʾ����
	void hideScene();//���س���

	//֡�¼�	
	void FrameEvent();

	void showImage( std::string imageName,GUIImageLayer layer,float time,int left,int top);//��ͼ������ʾͼƬ

	void showText( std::wstring text,float delay);//��ʾ����

	void clearText();//����ı�

	void showOtherText();//�ڴ���״̬��,ֱ����ʾ���µ��ı�

	void showRoleName( std::wstring text );//��ʾ��ɫ��

	void setTextDialog(const GUIDialogAttribute& attribute);//���öԻ���
	void setTextDialogVisible(bool visible);

	void setCheckMouseDown();//�������Ƿ���

	bool CheckMouseState();//�����Ƿ��������

	void showTextCursor(bool isLine);

	void hideTextCursor();

	void waitTime( float time );

	void onOtherSceneNotify(std::string arg);

	GUISceneType getType()
	{
		return StageScene;
	}

	bool mIsFastForward;

private:

	void eventMouseButtonClick(MyGUI::Widget* _sender);
	void keyPressed(const OIS::KeyEvent &arg);
	void keyReleased(const OIS::KeyEvent &arg);

	void onSave(MyGUI::Widget* _sender);
	void onLoad(MyGUI::Widget* _sender);
	void onHide(MyGUI::Widget* _sender);
	void onSystem(MyGUI::Widget* _sender);

	void load();

	void loadComplete();

	void returnScene();

	void fastForward(bool value);

	void UIInit();
	
	int mTextX;
	int mTextY;

	bool mCheckMouseDown;
	bool mIsMouseDown;

	std::wstring mTextBuffer;//�ı�����

	MyGUI::ImageBox* mUniversalLayer;//��ǰʹ�õĽ����
	MyGUI::ImageBox* mScrLayer;//��ǰ��ִ�н���Ĳ�
	std::string mTextureName;//Ŀ����ͼ��

	MyGUI::ImageBox* mBackGroundGroup;
	MyGUI::ImageBox* mBackGround;
	MyGUI::ImageBox* mBackGroundUniversal;
	std::string mBackGroundImageName;

	MyGUI::ImageBox* mLeftLayerGroup;
	MyGUI::ImageBox* mLeftLayer;
	MyGUI::ImageBox* mLeftLayerUniversal;
	std::string mLeftImageName;

	MyGUI::ImageBox* mMidLayerGroup;
	MyGUI::ImageBox* mMidLayer;
	MyGUI::ImageBox* mMidLayerUniversal;
	std::string mMidImageName;

	MyGUI::ImageBox* mRightLayerGroup;
	MyGUI::ImageBox* mRightLayer;
	MyGUI::ImageBox* mRightUniversal;
	std::string mRightImageName;

	MyGUI::ImageBox* mTextBoxBG;
	MyGUI::EditBox* mTextBox;
	MyGUI::EditBox* mRoleName;
	MyGUI::EditBox* mRoleNameUniversal;
	MyGUI::ImageBox* mTextCursor;
	bool mTextCursorType;
	bool mTextBoxVisible;

	MyGUI::ImageBox* mEffectLayer;

	MyGUI::Button* mSaveButton;
	MyGUI::Button* mLoadButton;
	MyGUI::Button* mHideButton;
	MyGUI::Button* mSystemButton;

};

class GUIStageFactory:
	public GUISceneFactory
{
public:
	GUIStageFactory(){}

	~GUIStageFactory(){}

	GUIScene* CreateScene(int Width,int Height)
	{
		return new GUIStage(Width,Height);
	}
};