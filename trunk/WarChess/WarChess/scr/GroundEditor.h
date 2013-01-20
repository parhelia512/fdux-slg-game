/*
 * �ļ���:BlockEditor.h
 * ����ʱ��:2010:10:17   9:50
 * ��д��:winsock
 * ����: �̳���TerrainEditor,ʵ���˵ر�ı༭
 */

#pragma once
#include "terraineditor.h"



class GroundEditor :
	public TerrainEditor
{
public:
	GroundEditor(TerrainBlock* (*terrainBlocks)[MAX_MAPSIZE][MAX_MAPSIZE],Grid* grid, RampManager* ramp);
	~GroundEditor(void);

	EditorID getEditorId()//���ظñ༭����ID
	{
		return GroundID;
	}

	std::vector<std::string> getEditorTypeList();//���ظñ༭��֧����Щ����
	std::vector<std::string> setEditorType(std::string type);//���õ�ǰ�༭���Ĳ���,����һ��stringList,ָ���ò�����Ҫ��Щ����
	void setGrid(int GX,int GY);//��Ŀǰѡ���ı༭������Ӧ�õ�������
	GroundType getTileTerrianType(int GX,int GZ);

private:
	GroundType currType;
	BlockType currBlockType;

	void planish(int GX,int GZ);//��ƽ���ο�
	void setTile(int GX,int GZ, BlockType blockType, GroundType trrianType);//���õ���ʵ�ʵ���
	void changeBlockMesh(int GX,int GZ, BlockType blockType, GroundType trrianType,int index); //�޸ĵ��ε�ģ����
	BlockType getTileBlockType(int GX,int GZ);
	void changeHighGroundPlane(int GX,int GZ,GroundType trrianType );
	GroundType isHighGroundPlane(int GX,int GZ);
	void setRamp(int GX,int GZ);
};
