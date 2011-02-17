#pragma once

#include <Ogre.h>

#include <OIS.h>

#include "iisingleton.hpp"

using namespace izayoi;

class GUISystem;

class LuaSystem;

class AudioSystem;

class StateManager;

//��Ϸ������,������Ϸ��ʼ���Լ�����,����˸���ϵͳ��ָ��
class Core:public IISingleton<Core>,public Ogre::WindowEventListener
{
public:
	Core(void);
	~Core(void);

	//Ogerϵͳָ��
	Ogre::LogManager* logMgr;
	Ogre::Log* log;
	Ogre::Root *mRoot;
	Ogre::Camera* mCamera;
	Ogre::SceneManager* mSceneMgr;
	Ogre::RenderWindow* mWindow;

	//OISϵͳָ��
	OIS::InputManager*	mInputManager;
	OIS::Keyboard* mKeyboard;
	OIS::Mouse*	mMouse;

	//��Ϸ��ϵͳָ��
	GUISystem* mGUISystem;
	LuaSystem* mLuaSystem;
	AudioSystem* mAudioSystem;

	//��Ϸ������
	StateManager* mStateManager;
	
	//ϵͳ��ʼ��
	bool initialize();
	//ϵͳ����
	void run();
	//ϵͳֹͣ
	void stop();
	//������Ⱦ���ڹر�
	bool windowClosing(Ogre::RenderWindow* rw);

private:
	void Uninitialize();

	void RenderingFrame();
	
	void initializeOIS();

	void initializeResource();

	bool isRun;
};
