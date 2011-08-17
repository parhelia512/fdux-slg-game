#pragma once

#include <Ogre.h>
#include <ParticleUniverseSystem.h> 

#include <OIS.h>

#include "iisingleton.hpp"

using namespace izayoi;

class GUISystem;

class LuaSystem;

class AudioSystem;

class StateManager;

class DataLibrary;

class InputControl;

class Framerate;

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
	Ogre::Overlay* mDebugOverlay;

	//����ϵͳ
	ParticleUniverse::ParticleSystem* createPUSystem(std::string name,std::string script);
	void destroyPUSystem(ParticleUniverse::ParticleSystem* pu);
	std::vector<ParticleUniverse::ParticleSystem*> mPUSystems;

	//OISϵͳָ��
	OIS::InputManager*	mInputManager;
	OIS::Keyboard* mKeyboard;
	OIS::Mouse*	mMouse;

	//��Ϸ��ϵͳָ��
	InputControl* mInputControl;
	GUISystem* mGUISystem;
	LuaSystem* mLuaSystem;
	AudioSystem* mAudioSystem;

	//��Ϸ������
	StateManager* mStateManager;
	DataLibrary* mDataLibrary;
	Framerate* mFramerate;
	
	//ϵͳ��ʼ��
	bool initialize(bool isFullScene);
	//ϵͳ����
	void run();
	//ϵͳֹͣ
	void stop();
	//������Ⱦ���ڹر�
	bool windowClosing(Ogre::RenderWindow* rw);

private:
	void Uninitialize();

	void RenderingFrame(unsigned int deltaTime);
	
	void initializeOIS();

	void initializeResource();

	void updateStats(void);

	bool testHardwareSupport();

	bool isRun;

	
};
