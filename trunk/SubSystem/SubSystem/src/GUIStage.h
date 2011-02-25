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

	GUISceneType getType()
	{
		return StageScene;
	}

private:

	void eventMouseButtonClick(MyGUI::Widget* _sender);
	
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

	MyGUI::ImageBox* mLeftLayerGroup;
	MyGUI::ImageBox* mLeftLayer;
	MyGUI::ImageBox* mLeftLayerUniversal;

	MyGUI::ImageBox* mMidLayerGroup;
	MyGUI::ImageBox* mMidLayer;
	MyGUI::ImageBox* mMidLayerUniversal;

	MyGUI::ImageBox* mRightLayerGroup;
	MyGUI::ImageBox* mRightLayer;
	MyGUI::ImageBox* mRightUniversal;

	MyGUI::ImageBox* mTextBoxBG;
	MyGUI::EditBox* mTextBox;
	MyGUI::EditBox* mRoleName;
	MyGUI::EditBox* mRoleNameUniversal;
	MyGUI::ImageBox* mTextCursor;

	MyGUI::ImageBox* mEffectLayer;

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