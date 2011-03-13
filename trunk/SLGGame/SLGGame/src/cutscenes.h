#pragma once

class CutScence
{
public:
	CutScence(unsigned int lasttime = 0);
	~CutScence();
	
	void play() {mPaused = false;}
	void pause() {mPaused = true;} 
	void skip();  //������ǰƬ��
	void skipall(); //��������Ƭ��
	bool end(); //Ƭ���Ƿ����
	void update(unsigned int deltaTime);

	void setNextScence(CutScence* next);
	CutScence* getNextScence() { return mNextScence;}
protected:
	CutScence *mNextScence;
	unsigned int mLastTime;
	unsigned int mPassedTime;

	bool mPaused;

	virtual bool endCutScence() = 0;
	virtual void skipCutScence() = 0;
	virtual void updateCutScence(unsigned int deltaTime) = 0;
};