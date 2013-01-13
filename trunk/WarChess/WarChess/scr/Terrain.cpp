#include "Terrain.h"

#include "BlockEditor.h"
#include "GroundEditor.h"
#include "RampManager.h"

#include <QDebug>

#include "IIRoot.h"

TerrainSystem::TerrainSystem(Ogre::SceneManager* sceneManager,Ogre::RenderWindow* renderWindow,Ogre::Camera* camera):
mMGrid(NULL),mSGrid(NULL),mSceneManager(sceneManager),mWindow(renderWindow),mCamera(camera),mMouseState(MouseRelease),mCurrentEditor(NULL),mCurrentGrid(NULL),mIsInit(false)
{

}

TerrainSystem::~TerrainSystem(void)
{
}

void TerrainSystem::initTerrain(int GridSize,int BlockSize)
{
	//������������
	if (mMGrid==NULL)
	{
		mMGrid=new Grid(mSceneManager,GridSize+1,GridSize+1,BlockSize,Ogre::ColourValue::ZERO,"mainGrid");
		mMGrid->setPosition(Ogre::Vector3::ZERO);
		mMGrid->setCursorVisible(false);
	}

	if (mSGrid==NULL)
	{
		mSGrid=new Grid(mSceneManager,GridSize,GridSize,BlockSize,Ogre::ColourValue::White,"sceGrid");
		mSGrid->setPosition(Ogre::Vector3::ZERO);
		mSGrid->setCursorVisible(false);
	}

	mRamp = new RampManager(BlockSize,GridSize);
	
	//����ƽ��
	mMesh=createPlane(GridSize+1,BlockSize);

	mPlaneNode=mSceneManager->getRootSceneNode()->createChildSceneNode();
	mPlaneEntity = mSceneManager->createEntity("MainPlane");
	mPlaneEntity->setQueryFlags(DEFAULT_QUERY_MARK);
	mPlaneNode->attachObject(mPlaneEntity);
	mPlaneNode->setPosition(0,0,0);

	//������ˮ��
	Ogre::Plane oceanSurface(Ogre::Vector3::UNIT_Y, 0);
	Ogre::MeshManager::getSingleton().createPlane("OceanSurface",
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		oceanSurface,
		(GridSize+ 1) * BlockSize , (GridSize+1) * BlockSize, GridSize, GridSize, true, 1, 1, 1, Ogre::Vector3::UNIT_Z);

	mWaterEntity = mSceneManager->createEntity( "Water", "OceanSurface" );
	mWaterEntity->setQueryFlags(DEFAULT_QUERY_MARK);
	mWaterEntity->setMaterialName("OceanCg");
	mWaterNode = mSceneManager->getRootSceneNode()->createChildSceneNode();
	mWaterNode->attachObject(mWaterEntity);
	mWaterNode->setPosition(0.0f,-1.0f,0.0f);
	
	mIsInit=true;
}

void TerrainSystem::mouseMove(float x,float y)
{
	if (mCurrentGrid!=NULL)
	{
		int gX=0;
		int gY=0;

		if(coordinateToGrid(x,y,&gX,&gY,mCurrentGrid))//��ȡ������ڵ�����
		{	
			mCurrentGrid->setCursorPosition(gX,gY);
		}
	}
}

void TerrainSystem::mousePress(float x,float y,MouseButton button)
{
	if (mMGrid!=NULL && mSGrid!=NULL)
	{
		if (button==LeftButton)
		{
			int gX=0;
			int gY=0;
			
			if (mCurrentEditor!=NULL)
			{
				if(coordinateToGrid(x,y,&gX,&gY,mCurrentGrid))//��ȡ���������
				{	
					mCurrentEditor->setGrid(gX,gY);
				}
			}
		}
	}
}

void TerrainSystem::mouseRelease(float x,float y,MouseButton button)
{

}

bool TerrainSystem::coordinateToGrid(float x,float y,int* GX,int* GY,Grid* grid)
{
	
	Ogre::Vector2 mRenderWindowSize(mWindow->getWidth(),mWindow->getHeight());

	// ���߽�����
	if  (x < 0 || x > mRenderWindowSize.x || y < 0 || y > mRenderWindowSize.y)
		return false;

	//ִ����������,��ȡ������Ľ���
	Ogre::Ray ray = mCamera->getCameraToViewportRay(x / mRenderWindowSize.x, y / mRenderWindowSize.y);
	std::pair<bool, float> result = ray.intersects(grid->getBoundingbox());
	if (result.first)
	{
		Ogre::Vector3 point = ray.getPoint(result.second);

		//��������Ĵ�С,��������һ��
		calculateGrid(point.x,point.z,GX, GY, grid);
		return true;
	}
	else
	{
		return false;
	}

}

bool TerrainSystem::coordinateToGrid( float x,float y,int* GX,int* GY )
{
	return coordinateToGrid(x,y,GX,GY,mSGrid);
}

Ogre::Vector2 TerrainSystem::getRealXY(int GX,int GY,GridID gridId)

{
	Grid* grid=NULL;
	switch(gridId)
	{
	case MainGrid:
		{
			grid=mMGrid;
			break;
		}
	case SecGrid:
		{
			grid=mSGrid;
			break;
		}

	}
	int x=GX*grid->mBlockSize-grid->getMaxGrid()+grid->mBlockSize/2;
	int y=GY*grid->mBlockSize-grid->getMaxGrid()+grid->mBlockSize/2;


	return Ogre::Vector2(x,y);
}

TerrainEditor* TerrainSystem::addEditor(EditorID id,GridID gridID)
{
	TerrainEditor* editor=getEditor(id);

	if (editor==NULL)
	{
		switch(id)
		{
		case BlockID:
			{
				editor=new BlockEditor(&mTerrainBlocks,mMGrid,mRamp);
				break;
			}
		case GroundID:
			{
				editor=new GroundEditor(&mTerrainBlocks,mSGrid,mRamp);
				break;
			}
		}

		mEditors[editor]=gridID;
		return editor;
	}
	else
	{
		return NULL;
	}
}

void TerrainSystem::deleteEditor(EditorID id)
{
	TerrainEditor* editor=getEditor(id);
	if (editor!=NULL)
	{
		mEditors.erase(editor);
	}
}

TerrainEditor* TerrainSystem::getEditor(EditorID id)
{
	if (id!=NoneID)
	{
		std::map<TerrainEditor*,GridID>::iterator it;
		
		for (it=mEditors.begin();it!=mEditors.end();it++)
		{
			if (it->first->getEditorId()==id)
			{
				return it->first;
			}
		} 
	}

	return NULL;
}

void TerrainSystem::setCurrentEditor(EditorID id)
{
	mCurrentEditor=getEditor(id);

	setCurrentGridCursorVisible(false);

	if (mCurrentEditor!=NULL)
	{
		GridID gId=mEditors[mCurrentEditor];

		switch(gId)
		{
		case MainGrid:
			{
				mCurrentGrid=mMGrid;
				break;
			}
		case SecGrid:
			{
				mCurrentGrid=mSGrid;
				break;
			}
		}

		setCurrentGridCursorVisible(true);//��ʾ�µĹ��
	}
	else
	{
		mCurrentGrid=NULL;
	}

}

void TerrainSystem::updateGridHeight( GridID gridID )
{

}

Ogre::MeshPtr TerrainSystem::createPlane( int gridSize,int blockSize )
{
	//��ʼ������
	int numVertices = gridSize * gridSize*VERTEX_QUAD;//�ܶ�����
	int numIndex=gridSize*gridSize*VERTEX_PREQUAD;//��������

	//����mesh
	Ogre::MeshPtr mesh;
	mesh =Ogre::MeshManager::getSingleton().createManual("MainPlane",
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	Ogre::SubMesh* subMesh  = mesh->createSubMesh();
	subMesh->useSharedVertices=false;
	subMesh->setMaterialName("TerrainBlock");

	// �����������ݽṹ
	subMesh->vertexData = new Ogre::VertexData();
	subMesh->vertexData->vertexStart = 0;
	subMesh->vertexData->vertexCount = numVertices;
	
	//���������뻺������
	Ogre::VertexDeclaration* vdecl = subMesh->vertexData->vertexDeclaration;
	Ogre::VertexBufferBinding* vbind = subMesh->vertexData->vertexBufferBinding;
	
	//���ö������ݽṹ
	size_t offsetUV = 0;
	vdecl->addElement(VERTEX_POS_BINDING, 0, Ogre::VET_FLOAT3,Ogre::VES_POSITION);//�򶥵����һ��λ��Ԫ��
	for(int i = 0 ; i < TEXTURE_COUNT ; i ++)
	{
		offsetUV += vdecl->addElement (VERTEX_UV_BINDING, offsetUV, Ogre::VET_FLOAT2,  Ogre::VES_TEXTURE_COORDINATES , i).getSize();
	}

	// �����������궥�㻺����
	Ogre::HardwareVertexBufferSharedPtr vbufPos =
		Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
		vdecl->getVertexSize(VERTEX_POS_BINDING),
		numVertices,
		Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY);
	vbind->setBinding(VERTEX_POS_BINDING, vbufPos);


	// �����������궥�㻺����
	Ogre::HardwareVertexBufferSharedPtr vbufUV =
		Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
		vdecl->getVertexSize(VERTEX_UV_BINDING),
		numVertices,
		Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY);
	vbind->setBinding(VERTEX_UV_BINDING, vbufUV);

	// ��������������
	Ogre::HardwareIndexBufferSharedPtr indexBuffer =
		Ogre::HardwareBufferManager::getSingleton().createIndexBuffer(
		Ogre::HardwareIndexBuffer::IT_16BIT ,
		numIndex,
		Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

	initPlaneVertex(gridSize,blockSize,mesh);

	//д��������Ϣ
	// ��������������
	Ogre::ushort* pIdx = (Ogre::ushort*)indexBuffer->lock(Ogre::HardwareBuffer::HBL_DISCARD);
	for(int y = 0 ; y < gridSize ; y ++)
	{
		for(int x = 0 ; x < gridSize ; x ++)
		{
			Ogre::ushort iIndexTopLeft = (x + y * gridSize) * VERTEX_QUAD;
			Ogre::ushort iIndexTopRight = iIndexTopLeft + 1;
			Ogre::ushort iIndexBottomLeft = iIndexTopLeft + 2;
			Ogre::ushort iIndexBottomRight = iIndexTopLeft + 3;
			*pIdx++ = iIndexBottomLeft;
			*pIdx++ = iIndexBottomRight;
			*pIdx++ = iIndexTopLeft;

			*pIdx++ = iIndexBottomRight;
			*pIdx++ = iIndexTopRight;
			*pIdx++ = iIndexTopLeft;
			
		}
	}
	indexBuffer->unlock();
	//����ģ�͵ĵ���������
	subMesh->indexData->indexBuffer = indexBuffer;
	subMesh->indexData->indexStart = 0;
	subMesh->indexData->indexCount =numIndex;
	
	Ogre::AxisAlignedBox meshBounds(-mMGrid->getMaxGrid(),0,-mMGrid->getMaxGrid(),
		gridSize*blockSize-mMGrid->getMaxGrid(),5,gridSize*blockSize-mMGrid->getMaxGrid());
	mesh->_setBounds(meshBounds);

	/*
	Ogre::MeshSerializer ser;
	ser.exportMesh(mesh.getPointer(), "grass.mesh");
	*/

	return mesh;
}

void TerrainSystem::initPlaneVertex(int gridSize,int blockSize,Ogre::MeshPtr mesh)
{

	// �������㻺����
	Ogre::HardwareVertexBufferSharedPtr vbufPos =mesh->getSubMesh(0)->vertexData->vertexBufferBinding->getBuffer(VERTEX_POS_BINDING);             
	float* pBufferPos = (float*)vbufPos->lock(Ogre::HardwareBuffer::HBL_DISCARD);

	Ogre::HardwareVertexBufferSharedPtr vbufUV = mesh->getSubMesh(0)->vertexData->vertexBufferBinding->getBuffer(VERTEX_UV_BINDING);             
	float* pBufferUV = (float*)vbufUV->lock(Ogre::HardwareBuffer::HBL_DISCARD);

	for (int i=0;i<gridSize;i++)
	{
		for (int j=0;j<gridSize;j++)
		{
			Ogre::Vector2 v=getRealXY(i,j,MainGrid);

			mTerrainBlocks[i][j]=new TerrainBlock(i,j,v.x,v.y,gridSize,blockSize,mSceneManager,mesh);
			mTerrainBlocks[i][j]->_updateTile(pBufferPos,pBufferUV,true,true);
		}
	}

	// ����������
	vbufUV->unlock();
	vbufPos->unlock();

}

bool TerrainSystem::isInit()
{
	return mIsInit;
}

float TerrainSystem::getHeight( int GX,int GY )
{
	return mTerrainBlocks[GX][GY]->mHeight;
}

void TerrainSystem::calculateGrid(float x,float y,int* GX, int* GY, Grid* grid )
{
	// ������ת������ֵ
	x=x+grid->getMaxGrid();
	y=y+grid->getMaxGrid();

	//IIRoot::getSingletonPtr()->log->logMessage(QString("%1,%2").arg(x).arg(y).toStdString());

	(*GX)=x/grid->mBlockSize;
	(*GY)=y/grid->mBlockSize;

	if((*GX)>=grid->mGridSize)
	{
		(*GX)=grid->mGridSize-1;
	}
	else if ((*GX)<0)
	{
		(*GX)=0;
	}

	if((*GY)>=grid->mGridSize)
	{
		(*GY)=grid->mGridSize-1;
	}
	else if ((*GY)<0)
	{
		(*GY)=0;
	}
}

void TerrainSystem::calculateGrid(float x,float y,int* GX, int* GY )
{
	calculateGrid(x,y,GX,GY,mSGrid);
}

int TerrainSystem::getGridSize()
{
	return mSGrid->mGridSize;
}
float TerrainSystem::getBlockSize()
{
	return mSGrid->mBlockSize;
}	
void TerrainSystem::setCurrentGridCursorVisible(bool show)
{
	if (mCurrentGrid!=NULL)//������ǰ�Ĺ��
	{
		mCurrentGrid->setCursorVisible(show);
	}
}
