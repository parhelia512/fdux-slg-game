#pragma once

#include "iisingleton.hpp"

#include "GUICommon.h"

#include <bass.h>

#include <MyGUI.h>

using namespace izayoi;

class AudioSystem:public IISingleton<AudioSystem> 
{
public:
	AudioSystem();
	~AudioSystem(void);

	bool init();

	bool playStream(std::string name,bool isLoop,int time);

	bool stopStream(int time);

	bool playSample(std::string name);

	int getErrorCode();

	void FrameUpdate();
private:
	MyGUI::Timer mTimer;
	HSTREAM mStream;//��Ƶ��
	HSAMPLE mSample;//��Ч
	int mVol;
	bool mInOut;//���뻹�ǽ���,trueΪin,falseΪout
	int mTickTime;//��ʱ������ʱ��
};