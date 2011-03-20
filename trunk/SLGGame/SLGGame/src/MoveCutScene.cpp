#include "MoveCutScene.h"

#include "SquadGrapManager.h"
#include "SquadGraphics.h"

#include "Terrain.h"

MoveCutScene::MoveCutScene(unsigned int id,std::vector<Ogre::Vector2>& grids,Ogre::Vector2& currentGrid)
{
	std::map<int,Ogre::Vector3> vectors;
	std::map<int,Ogre::Quaternion> quaternions;
	Ogre::Vector2 current=currentGrid;
	int index=0;

	//·��������㷨
	for(std::vector<Ogre::Vector2>::iterator it=grids.begin();it!=grids.end();it++)
	{
		//ȡ�õ�ǰλ����Ŀ���Ĳ�ֵ
		Ogre::Vector2 diffGrid=(*it)-current;
		
		//ȷ��λ��
		float wx=0,wy=0,height=0;

		if (it+1==grids.end())//���Ϊ���һ���ڵ�
		{
			Terrain::getSingletonPtr()->getWorldCoords((*it).x,(*it).y,wx,wy);//�ƶ������һ���ڵ������
			//height=Terrain::getSingletonPtr()->getHeight(wx,wy);

			Ogre::Vector3 v(wx,height,wy);
			mEndPosition=v;
			vectors[index]=v;
		}
		else
		{
			Terrain::getSingletonPtr()->getWorldCoords(current.x,current.y,wx,wy);
			//height=Terrain::getSingletonPtr()->getHeight(wx,wy);

			Ogre::Vector3 v(wx+diffGrid.x*(TILESIZE/2),height,wy+diffGrid.y*(TILESIZE/2));
			vectors[index]=v;
		}
		
		//ȷ������
		Ogre::Quaternion q;
		if (diffGrid.x==0 && diffGrid.y==-1)//��
		{
			q.FromAngleAxis(Ogre::Degree(180),Ogre::Vector3(0,1,0));
			mDirection=SquadGraphics::North;
		}
		else if (diffGrid.x==0 && diffGrid.y==1)//��
		{
			q.FromAngleAxis(Ogre::Degree(0),Ogre::Vector3(0,1,0));
			mDirection=SquadGraphics::South;
		}
		else if (diffGrid.x==1 && diffGrid.y==0)//��
		{
			q.FromAngleAxis(Ogre::Degree(90),Ogre::Vector3(0,1,0));
			mDirection=SquadGraphics::West;
		}
		else if (diffGrid.x==-1 && diffGrid.y==0)//��
		{
			q.FromAngleAxis(Ogre::Degree(270),Ogre::Vector3(0,1,0));
			mDirection=SquadGraphics::East;
		}
		quaternions[index]=q;
		
		current=(*it);
		index++;
	}

	mSquadGraphics=SquadGrapManager::getSingletonPtr()->getSquad(id);
	mSquadGraphics->setMovePath(vectors,quaternions);

}

MoveCutScene::~MoveCutScene(void)
{
}

bool MoveCutScene::endCutScene()
{
	return mSquadGraphics->isTransformOver();
}

void MoveCutScene::skipCutScene()
{
	mSquadGraphics->stopTransform();
	mSquadGraphics->setGrid(mEndPosition.x,mEndPosition.y);
	mSquadGraphics->setDirection((SquadGraphics::Direction)mDirection,false);
}

void MoveCutScene::updateCutScene( unsigned int deltaTime )
{

}
