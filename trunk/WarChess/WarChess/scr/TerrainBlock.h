#pragma once

#include <Ogre.h>
#include <string>

#include "Common.h"

class TerrainBlock
{
public:
	TerrainBlock(int GX,int GY,float X,float Y,int gridSize,int blockSize,Ogre::SceneManager* sceneManager,Ogre::MeshPtr planeMesh);
	~TerrainBlock(void);

	void setTerrainAsMesh(std::string name, int angle);
	void setTerrainAsPlane(float height);
	void setMeshMaterial(std::string name);
	void setPlaneMaterial(GroundType trrianType, ConerType coner);

	void removeMesh();
	void removeMat();

	void getTileName();//���ظÿ������

	bool isPlane();

	BlockType getBlockType(); //���ص�ǰ�ĵ��ο�����
	BlockType getBlockType(ConerType coner); //���ص�ǰ�ĵ��ο�һ���ǵ�����
	int getBlockMeshIndex(); //���ص�ǰ���ο�ģ��ʹ�õı��
	GroundType getTerrianType(ConerType coner);//���ص�ǰģ��ʹ�õĵ�ͼ��ͼ����
	void setBlockType(BlockType blokType, GroundType terrianType, int index);//���õ��ο�����,�趨��ģ�ͺ����

	void _updateTile(float* pBufferPos , float* pBufferUV , bool bUpdatePos,bool bUpdateUV);//����Terrain��ʼ����ʱ�������ⲿ����

	int mGridX;//���߼������ϵ�X
	int mGridY;//���߼������ϵ�Y

	float mX;//���Ͻ�X����
	float mY;//���Ͻ�Y����

	float mHeight;//�ؿ�߶�
	int mGridSize;//��ͼÿ�п���
	int mBlockSize;//ÿһ��Ĵ�С

private:
	Ogre::SceneNode* mBlockNode;//ģ�ͽڵ�
	Ogre::Entity* mBlockEntity;//���ο�ʵ�����
	Ogre::MeshPtr mPlaneMesh;//��ƽ�������ָ��
	std::string mTileName;//���ο�����
	GroundType mConerTerrian[4];//ÿһ���Ӧ����ͼ����
	Ogre::SceneManager* mSceneManager;//����������

	bool mPlane;//�Ƿ�Ϊƽ��

	int mAngle;//ģ�͵���ת�Ƕ�

	BlockType mBlockType;//��ǰ���ο������
	int mIndex; //ģ��ʹ�õı�ţ�ͬƽ�ؼ���ƴ�ӵķ���
	
	void updatePos(float* pBufferPos);
	void updateUV(float* pBufferUV);
	void WriteConerTexCoords(float* pos, int index, int coner);
};
