#pragma once

#include "GUIScene.h"

class GUISLWindow;

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

	void shockImage(GUIImageLayer layer,float durationTime,float amplitudeX,float amplitudeY);//��ͼƬ

	void moveImage(GUIImageLayer layer,float time,int left,int top);

	void showText( std::wstring text,float delay);//��ʾ����

	void changeShowTextOptionTime();

	void clearText();//����ı�

	void showOtherText();//�ڴ���״̬��,ֱ����ʾ���µ��ı�

	void showRoleName( std::wstring text );//��ʾ��ɫ��

	void setTextDialog(const GUIDialogAttribute& attribute);//���öԻ���
	void setTextDialogVisible(bool visible);
	void setButtonLock(bool visible);

	void setCheckMouseDown();//�������Ƿ���

	bool CheckMouseState();//�����Ƿ��������

	void showTextCursor(bool isLine);

	void hideTextCursor();

	void waitTime( float time );

	void onOtherSceneNotify(std::string arg);

	void saveData(bool isAutoSave);

	void setHistoryBoxVisible(bool visible);

	void setIsShowSupplyButton(bool visible);

	void setSupplyTips();

	void eventSupplyTipsFade(MyGUI::Widget* _sender);

	void resetAutoTimer(){mAutoTimer.reset();isClearAutoTimer=true;}

	void addToHistoryBox(std::wstring text);

	GUISceneType getType()
	{
		return StageScene;
	}

	bool mIsFastForward;

	//��ǰ��ʱ���Ĺ���
	enum TimerWork
	{
		NoneWork,
		PrinterWork,//���ֻ�Ч��
		UniversalWork,//����Ч��
		FadeInOutWork,//�Ի�������
		WaitWork,//�ȴ�
		RoleNameWork,//���ֽ���
		ClearAllRoleWork,//������еĽ�ɫͼƬ
		MoveWork,//�ƶ�ͼƬ
		ShockWork//��ͼƬ
	};
	TimerWork mTimerWork;

private:

	MyGUI::Timer mTimer;//��ʱ��
	MyGUI::Timer mAutoTimer;//�Զ��ȴ���ʱ��
	bool isClearAutoTimer;
	float mTickTime;//���δ���ʱ��

	void eventMouseButtonClick(MyGUI::Widget* _sender);
	void eventHistoryBoxExit(MyGUI::Widget* _sender);
	void getMouseState();
	void keyPressed(const OIS::KeyEvent &arg);
	void keyReleased(const OIS::KeyEvent &arg);

	void onSave(MyGUI::Widget* _sender);
	void onLoad(MyGUI::Widget* _sender);
	void onHide(MyGUI::Widget* _sender);
	void onSystem(MyGUI::Widget* _sender);
	void onHistory( MyGUI::Widget* _sender );
	void onAuto( MyGUI::Widget* _sender );
	void onSupply( MyGUI::Widget* _sender );

	void load();

	void loadComplete();

	void returnScene();

	void fastForward();

	void UIInit(std::string arg);

	void buttonLock(bool lock);

	bool isCanFastForward();
	bool isCanAuto();
	bool isCanClick();

	int mTextX;
	int mTextY;

	bool mCheckMouseDown;
	bool mIsMouseDown;
	bool mIsAuto;

	GUISLWindow* SLWindow;

	float mShowTextOptionTime;//�û�������ʾ�ı�ʱ��
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
	bool mIsCanShowTextBox;
	bool mIsCanShowButton;

	MyGUI::EditBox* mHistoryBox;
	MyGUI::ImageBox* mHistoryBoxExit;
	std::wstring mCurrentRoleName;
	std::wstring mHistoryText;
	int mLeftOffect;

	MyGUI::ImageBox* mEffectLayerGroup;
	MyGUI::ImageBox* mEffectLayer;
	MyGUI::ImageBox* mEffectLayerUniversal;

	MyGUI::ImageBox* mInputGroup;
	MyGUI::ImageBox* mInputLayer;
	MyGUI::Button* mSaveButton;
	MyGUI::Button* mLoadButton;
	MyGUI::Button* mHideButton;
	MyGUI::Button* mSystemButton;
	MyGUI::Button* mHistoryButton;
	MyGUI::Button* mAutoButton;
	MyGUI::Button* mSupplyButton;
	bool mIsShowSupplyButton;

	MyGUI::ImageBox* mSupplyTips;
	MyGUI::ControllerFadeAlpha* mTipsFadeController;
	bool mIsSupplyTipsFade;

	struct ShockParam
	{
		int mImageX;
		int mImageY;
		float mDurationTime;//����ʱ��
		float mAge;//��ǰʱ��
		float mAmplitudeX;//���
		float mAmplitudeY;//Ƶ��
	};
	
	ShockParam mShockParam;

	bool mIsMirrorMode;

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