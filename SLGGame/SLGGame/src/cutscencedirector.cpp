#include "cutscencediretor.h"
#include "cutscenes.h"

CutScenceDirector::CutScenceDirector()
{
	mId = 0;
}
CutScenceDirector::~CutScenceDirector()
{

}

int CutScenceDirector::addCutScence(CutScence* cutscence)
{
	if(cutscence!= NULL)
	{
		int id = mId++;
		mCutScence.insert(CutScencePair(id, cutscence));
		return id;
	}
	return -1;
}

void CutScenceDirector::skipCutScence(int id)
{
	CutScenceIte ite;
	ite = mCutScence.find(id);
	if(ite != mCutScence.end())
	{
		ite->second->skipall();
	}
}

void CutScenceDirector::pauseCutScence(int id)
{
	CutScenceIte ite;
	ite = mCutScence.find(id);
	if(ite != mCutScence.end())
	{
		ite->second->pause();
	}
}

void CutScenceDirector::playCutScence(int id)
{
	CutScenceIte ite;
	ite = mCutScence.find(id);
	if(ite != mCutScence.end())
	{
		ite->second->play();
	}
}

void CutScenceDirector::update(unsigned int deltaTime)
{
	CutScenceIte ite;
	for(ite = mCutScence.begin(); ite != mCutScence.end();)
	{
		if(ite->second->end())
		{
			delete ite->second;
			ite = mCutScence.erase(ite);
		}
		else
		{
			ite->second->update(deltaTime);
			ite++;
		}
	}
}