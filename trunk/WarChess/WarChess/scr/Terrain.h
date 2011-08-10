/*
 * �ļ���:Terrain.h
 * ����ʱ��:2010:10:14   16:10
 * ��д��:winsock
 * ����:����ϵͳ,����������ȫ���ĵ��εĹ���
 */

#pragma once

#include <Ogre.h>

#include <map>

#include "Grid.h"
#include "TerrainBlock.h"
#include "Common.h"
#include "TerrainEditor.h"

class TerrainSystem
{
public:
	TerrainSystem(Ogre::SceneManager* sceneManager,Ogre::RenderWindow* renderWindow,Ogre::Camera* camera);
	~TerrainSystem(void);
	
	//��ʼ������ϵͳ,GridSize:�����ܴ�С,BlockSize:ÿ������Ĵ�С
	void initTerrain(int GridSize,int BlockSize);

	//�����ⲿ�ĵ���¼�,ע��˴����ܵ�����Ļ2d������
	void mouseMove(float x,float y);
	void mousePress(float x,float y,MouseButton button);
	void mouseRelease(float x,float y,MouseButton button);

	TerrainEditor* addEditor(EditorID id,GridID gridID);//���һ���༭����ϵͳ����,gridIDָ��ʹ�����������ø�����
	void deleteEditor(EditorID id);//ɾ��һ���༭��
	TerrainEditor* getEditor(EditorID id);//��ȡ�༭��
	void setCurrentEditor(EditorID id);//���õ�ǰ�ı༭��
	void setCurrentGridCursorVisible(bool show);//����ĳ����������

	void updateGridHeight(GridID gridID);//��������ĸ߶�

	//�ṩһ������Ľӿ�,���ڲ�ѯ��ǰ�������һ��,ʹ�ø�����
	bool coordinateToGrid(float x,float y,int* GX,int* GY);
	
	//�ṩһ������Ľӿ�,���ڲ�ѯĳ����������һ��,ʹ�ø�����
	void calculateGrid(float x,float y,int* GX, int* GY );

	//���������ת����ʵ�ʵ���������;
	Ogre::Vector2 getRealXY(int GX,int GY,GridID gridId);

	//��ȡ����Ĵ�С
	int getGridSize();
	float getBlockSize();

	//��ȡĳ��߶�
	float getHeight(int GX,int GY);

	bool isInit();//�Ƿ��ʼ��

	TerrainBlock* mTerrainBlocks[MAX_MAPSIZE][MAX_MAPSIZE];//���ο����ݼ���

	RampManager* mRamp;	

private:

	//ת����ά��Ļx,y���굽����������,����ֵ��ʾ�Ƿ�ɹ�
	bool coordinateToGrid(float x,float y,int* GX,int* GY,Grid* grid);

	//������������һ��
	void calculateGrid(float x,float y,int* GX, int* GY, Grid* grid );

	//������ƽ��
	Ogre::MeshPtr createPlane(int gridSize,int blockSize);

	//��ʼ����ƽ��
	void initPlaneVertex(int gridSize,int blockSize,Ogre::MeshPtr mesh);

	bool mIsInit;//�Ƿ��ʼ��
	Grid* mMGrid;//������ϵͳ
	Grid* mSGrid;//������ϵͳ
	Ogre::SceneManager* mSceneManager;//Ogre����
	Ogre::Camera* mCamera;//Ogre�������
	Ogre::RenderWindow* mWindow;//��Ⱦ����
		

	enum MouseState//���״̬ö��
	{
		MouseRelease,
		MousePress
	};

	MouseState mMouseState;//���״̬

	std::map<TerrainEditor*,GridID> mEditors;//Editor��GridId�Ķ�Ӧ��ϵ

	TerrainEditor* mCurrentEditor;//��ǰ����ı༭��
	Grid* mCurrentGrid;//��ǰ����ı༭����Ӧ������ָ��

	Ogre::SceneNode* mPlaneNode;//ģ�ͽڵ�
	Ogre::Entity* mPlaneEntity;//����ʵ��
	Ogre::MeshPtr mMesh;//��������ָ��

	Ogre::Entity* mWaterEntity;//����ˮ��
	Ogre::SceneNode* mWaterNode;//����ˮ��ڵ�

};
