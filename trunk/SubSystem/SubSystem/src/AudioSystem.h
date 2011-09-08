#pragma once

#include "iisingleton.hpp"

#include "GUICommon.h"

#include <audiere.h>

#include <MyGUI.h>

using namespace izayoi;

class AudioSystem:public IISingleton<AudioSystem> 
{
public:
	AudioSystem();
	~AudioSystem(void);

	bool init();

	void changeVolume();

	bool playStream(std::string name,bool isLoop,int time);

	bool stopStream(int time);

	bool playSample(std::string name,bool isLoop);

	bool stopSample();

	std::string getError(bool isStreamError);

	void FrameUpdate();

	std::string mStreamName;
private:
	audiere::AudioDevicePtr mDevice;
	audiere::OutputStreamPtr mStream;
	audiere::OutputStreamPtr mSample;
	float mStreamVol;
	float mSampleVol;
	//MyGUI::Timer mTimer;
	//HSTREAM mStream;//��Ƶ��
	//HSAMPLE mSample;//��Ч
	//int mVol;
	//bool mInOut;//���뻹�ǽ���,trueΪin,falseΪout
	//int mTickTime;//��ʱ������ʱ��
	//std::string mNextMusic;
	//int mNextTime;
	//bool mNextLoop;
};