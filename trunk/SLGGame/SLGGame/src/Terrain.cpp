#include "Terrain.h"

#include "MapDataManager.h"

Terrain::Terrain()
{

}

Terrain::~Terrain()
{

}

bool Terrain::createTerrain(MapDataManager *data)
{
	mMapData = data;
	int terrainszie = mMapData->getMapSize() + 9;

	//�����ƹ�
	Core::getSingleton().mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5f, 0.5f, 0.5f));
	Core::getSingleton().mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
	mLight = Core::getSingleton().mSceneMgr->createLight("TerrainLight");
	mLight->setType(Ogre::Light::LightTypes::LT_DIRECTIONAL);
	mLight->setPosition(-500.0f,500.0f, 500.0f);
	mLight->setDirection(1.0f, -1.0f, -1.0f);
	mLight->setCastShadows(true);
	mLight->setDiffuseColour(Ogre::ColourValue(1.0f, 1.0f,1.0f));

	//��������Mesh
	mTerrainNode = Core::getSingleton().mSceneMgr->getRootSceneNode()->createChildSceneNode("TerrainNode");

	int numVertices = terrainszie * terrainszie * VERTEX_QUAD;
	int numIndex = terrainszie * terrainszie * VERTEX_PREQUAD;
	mTerrainMesh = Ogre::MeshManager::getSingleton().createManual("TerrianMesh",
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	Ogre::SubMesh* subMesh  = mTerrainMesh->createSubMesh();
	subMesh->useSharedVertices=false;
	subMesh->setMaterialName("TerrainTile");

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
	vdecl->addElement(VERTEX_NOM_BINDING, 0, Ogre::VET_FLOAT3,Ogre::VES_NORMAL);
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

	Ogre::HardwareVertexBufferSharedPtr vbufNOM =
		Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
		vdecl->getVertexSize(VERTEX_NOM_BINDING),
		numVertices,
		Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY);
	vbind->setBinding(VERTEX_NOM_BINDING, vbufNOM);

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

	//��������
	float* pBufferPos = (float*)vbufPos->lock(Ogre::HardwareBuffer::HBL_DISCARD);
	float* pBufferUV = (float*)vbufUV->lock(Ogre::HardwareBuffer::HBL_DISCARD);
	float* pBufferNom = (float*)vbufNOM->lock(Ogre::HardwareBuffer::HBL_DISCARD);

	float startpos = - terrainszie * TILESIZE / 2;
	for(int y = 0 ; y < terrainszie; y ++)
	{
		for(int x = 0 ; x < terrainszie; x ++)
		{
			createTile(x, y, startpos + x * TILESIZE, startpos + y * TILESIZE, pBufferPos, pBufferUV, pBufferNom);
			pBufferPos += 3 * VERTEX_QUAD ;
			pBufferNom += 3 * VERTEX_QUAD ;
			pBufferUV += 2 * VERTEX_QUAD * 4;
		}
	}

	vbufNOM->unlock();
	vbufUV->unlock();
	vbufPos->unlock();

	//д��������Ϣ
	// ��������������
	Ogre::ushort* pIdx = (Ogre::ushort*)indexBuffer->lock(Ogre::HardwareBuffer::HBL_DISCARD);
	for(int y = 0 ; y < terrainszie ; y ++)
	{
		for(int x = 0 ; x < terrainszie ; x ++)
		{
			Ogre::ushort iIndexTopLeft = (x + y * terrainszie) * VERTEX_QUAD;
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

	Ogre::AxisAlignedBox meshBounds(startpos,0,startpos,
		-startpos,5,-startpos);
	mTerrainMesh->_setBounds(meshBounds);

	mTerrainEntity = Core::getSingleton().mSceneMgr->createEntity("TerrianMesh");
	mTerrainNode->attachObject(mTerrainEntity);
	mTerrainNode->setPosition(0,0,0);

	//����ˮ��
	mWaterNode = Core::getSingleton().mSceneMgr->getRootSceneNode()->createChildSceneNode("WaterNode");
	mWaterObject = Core::getSingleton().mSceneMgr->createManualObject("WaterObject");

	mWaterObject->begin("OceanCg",Ogre::RenderOperation::OT_TRIANGLE_LIST);
	startpos += TILESIZE/2;
	for(int y = 0; y < terrainszie; y++)
		for(int x = 0; x < terrainszie; x++)
		{
			if(mMapData->getTerrainType(x -3, y -3 ) == Water)
			{
				mWaterObject->position(startpos + x * TILESIZE, 0.0f, startpos + y * TILESIZE);
				mWaterObject->colour(1.0f,1.0f,1.0f);
				mWaterObject->position(startpos + (x+1) * TILESIZE, 0.0f, startpos + (y+1) * TILESIZE);
				mWaterObject->colour(1.0f,1.0f,1.0f);
				mWaterObject->position(startpos + (x+1) * TILESIZE, 0.0f, startpos + y * TILESIZE);
				mWaterObject->colour(1.0f,1.0f,1.0f);
				mWaterObject->position(startpos + (x+1) * TILESIZE, 0.0f, startpos + (y+1) * TILESIZE);
				mWaterObject->colour(1.0f,1.0f,1.0f);
				mWaterObject->position(startpos + x * TILESIZE, 0.0f, startpos + y * TILESIZE);
				mWaterObject->colour(1.0f,1.0f,1.0f);
				mWaterObject->position(startpos + x * TILESIZE, 0.0f, startpos + (y+1) * TILESIZE);
				mWaterObject->colour(1.0f,1.0f,1.0f);
			}
		}
	mWaterObject->end();

	mWaterNode->attachObject(mWaterObject);
	mWaterNode->setPosition(0,WATERHEIGHT,0);
	return true;
}

void Terrain::destoryTerrian()
{
	
}

void Terrain::getWorldCoords(int x, int y, float &wx, float &wy)
{
	int s = mMapData->getMapSize();
	wx = ((float)x - (float)s / 2.0f + 0.5f) * TILESIZE ;
	wy = ((float)y - (float)s / 2.0f + 0.5f) * TILESIZE;
}

float Terrain::getHeight(float x, float y)
{
	return 0.0f;
}

void Terrain::createTile(int x, int y,float sx, float sy, float *posbuffer, float *uvbuffer, float *nombuffer)
{
	//ȷ�����ο�����
	TerrainType tt[VERTEX_QUAD];
	tt[TOPLEFT] = mMapData->getTerrainType(x -4, y -4 );
	tt[TOPRIGHT] = mMapData->getTerrainType(x -3 , y -4 );
	tt[BOTTOMLEFT] = mMapData->getTerrainType(x -4 , y -3 );
	tt[BOTTOMRIGHT] = mMapData->getTerrainType(x -3, y -3 );
	int terrainindex[6] = {0,0,0,0,0,0};
	terrainindex[tt[TOPLEFT]] += 2;
	terrainindex[tt[TOPRIGHT]] += 1;
	terrainindex[tt[BOTTOMLEFT]] += 8;
	terrainindex[tt[BOTTOMRIGHT]] += 4;
	float height = WATERDEEP;
	if(terrainindex[LowGround] + terrainindex[Cliff] == 15)
	{
		height = PLANEHEIGHT;
	}
	if(terrainindex[HighGround] == 15)
	{
		height = HEIGHGROUNDHEIGHT;
	}

	//ȷ���ر���ͼ
	GroundType gt[VERTEX_QUAD];
	gt[TOPLEFT] = mMapData->getGroundType(x -4 , y -4);
	gt[TOPRIGHT] = mMapData->getGroundType(x -3 , y -4 );
	gt[BOTTOMLEFT] = mMapData->getGroundType(x -4 , y -3 );
	gt[BOTTOMRIGHT] = mMapData->getGroundType(x -3, y -3 );
	int groundindex[4] = {0,0,0,0};
	groundindex[gt[TOPLEFT]] +=8;
	groundindex[gt[TOPRIGHT]] += 4;
	groundindex[gt[BOTTOMLEFT]] += 2;
	groundindex[gt[BOTTOMRIGHT]] += 1;

	//���Ƶ��ο�λ��
	//����
	(*posbuffer) = sx;
	posbuffer++;
	(*posbuffer)= height;
	posbuffer++;
	(*posbuffer) = sy;
	//����
	posbuffer++;
	(*posbuffer) = sx + TILESIZE;
	posbuffer++;
	(*posbuffer)= height;
	posbuffer++;
	(*posbuffer) = sy;
	//����
	posbuffer++;
	(*posbuffer) = sx;
	posbuffer++;
	(*posbuffer)= height;
	posbuffer++;
	(*posbuffer) = sy + TILESIZE;
	//����
	posbuffer++;
	(*posbuffer) = sx + TILESIZE;
	posbuffer++;
	(*posbuffer)= height;
	posbuffer++;
	(*posbuffer) = sy + TILESIZE;

	//����
	(*nombuffer) = 0.0f;
	nombuffer++;
	(*nombuffer)= 1.0f;
	nombuffer++;
	(*nombuffer) = 0.0f;
	//����
	nombuffer++;
	(*nombuffer) = 0.0f;
	nombuffer++;
	(*nombuffer)= 1.0f;
	nombuffer++;
	(*nombuffer) = 0.0f;
	//����
	nombuffer++;
	(*nombuffer) = 0.0f;
	nombuffer++;
	(*nombuffer)= 1.0f;
	nombuffer++;
	(*nombuffer) = 0.0f;
	//����
	nombuffer++;
	(*nombuffer) = 0.0f;
	nombuffer++;
	(*nombuffer)= 1.0f;
	nombuffer++;
	(*nombuffer) = 0.0f;
	

	//���Ƶ��ο�UV
	bool bottomlayer = false;
	for(int n = 0; n < TEXTURE_COUNT; n++)
	{
		if(groundindex[n] > 0 && !bottomlayer)
		{
			bottomlayer = true;
			groundindex[n] = 15;
		}
		float u,v;
		u = (groundindex[n] % 4) * TERRAINTEXTILESIZE / TERRAINTEXSIZE;
		v = (groundindex[n] / 4) * TERRAINTEXTILESIZE / TERRAINTEXSIZE;
		*uvbuffer = u;
		*(uvbuffer + 1) = v;
		*(uvbuffer + 8) = u + TERRAINTEXTILESIZE / TERRAINTEXSIZE;
		*(uvbuffer + 9) = v;
		*(uvbuffer + 16) = u;
		*(uvbuffer + 17) = v + TERRAINTEXTILESIZE / TERRAINTEXSIZE;
		*(uvbuffer + 24)= u + TERRAINTEXTILESIZE / TERRAINTEXSIZE;
		*(uvbuffer + 25) = v + TERRAINTEXTILESIZE / TERRAINTEXSIZE;

		uvbuffer += 2;
	}

	//������ģ��(���������µ�����Ӷ�����) 
	bool usemesh = false;
	int meshindex;
	std::string meshName;
	std::string materialName;
	if(terrainindex[HighGround] > 0 && terrainindex[HighGround] < 15)
	{
		usemesh = true;
		meshName = "Cliff";
		materialName = "Cliff";
		meshindex = terrainindex[HighGround];
	}
	else if(terrainindex[Water] > 0 && terrainindex[Water] < 15)
	{
		usemesh = true;
		meshName = "Bank";
		materialName = "Water";
		meshindex = terrainindex[Water];
	}
	if(usemesh)
	{
		switch(mMapData->getGroundType(x -4 , y -4))
		{
		case GreenLand:
			materialName =  materialName + "Mat1";
			break;
		case Desert:
			materialName =  materialName + "Mat2";
			break;
		case Swamp:
			materialName =  materialName + "Mat3";
			break;
		case Snow:
			materialName =  materialName + "Mat4";
			break;
		}
		int angle;
		switch(meshindex)
		{
		case 1:
			meshName = meshName + "1";
			angle = 0;
			break;
		case 2:
			meshName = meshName + "1";
			angle = 90;
			break;
		case 3:
			meshName = meshName + "2";
			angle = 0;
			break;
		case 4:
			meshName = meshName + "1";
			angle = -90;
			break;
		case 5:
			meshName = meshName + "2";
			angle = -90;
			break;
		case 6:
			meshName = meshName + "4";
			angle = -90;
			break;
		case 7:
			meshName = meshName + "3";
			angle = 0;
			break;
		case 8:
			meshName = meshName + "1";
			angle = 180;
			break;
		case 9:
			meshName = meshName + "4";
			angle = 0;
			break;
		case 10:
			meshName = meshName + "2";
			angle = 90;
			break;
		case 11:
			meshName = meshName + "3";
			angle = 90;
			break;
		case 12:
			meshName = meshName + "2";
			angle = 180;
			break;
		case 13:
			meshName = meshName + "3";
			angle = -90;
			break;
		case 14:
			meshName = meshName + "3";
			angle = 180;
			break;
		}
		meshName = meshName + randMesh(x, y);
		stTileEntityData* entitydata = new stTileEntityData;
		entitydata->mTileEntity = Core::getSingleton().mSceneMgr->createEntity(meshName + ".mesh");
		entitydata->mTileEntity->setMaterialName(materialName);
		entitydata->mTileNode = mTerrainNode->createChildSceneNode();
		entitydata->mTileNode->attachObject(entitydata->mTileEntity);
		entitydata->mTileNode->setPosition(sx + TILESIZE /2 ,PLANEHEIGHT,sy + TILESIZE /2);
		entitydata->mTileNode->yaw(Ogre::Degree(angle));
		mTileEntityVector.push_back(entitydata);
	}
}

std::string Terrain::randMesh(int x, int y)
{
	char temp[8];
	int num = abs(x + y)%2 +1;
	sprintf_s(temp, 8, "_%d", num);
	return temp;
}