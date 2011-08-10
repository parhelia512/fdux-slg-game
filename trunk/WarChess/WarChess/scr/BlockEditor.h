/*
 * �ļ���:BlockEditor.h
 * ����ʱ��:2010:10:17   9:50
 * ��д��:winsock
 * ����: �̳���TerrainEditor,ʵ���˵��ο�ı༭
 */

#pragma once
#include "terraineditor.h"

class TerrainBlock;

class BlockEditor :
	public TerrainEditor
{
public:
	BlockEditor(TerrainBlock* (*terrainBlocks)[MAX_MAPSIZE][MAX_MAPSIZE],Grid* grid,RampManager* ramp);
	~BlockEditor(void);

	EditorID getEditorId()//���ظñ༭����ID
	{
		return BlockID;
	}

	std::vector<std::string> getEditorTypeList();//���ظñ༭��֧����Щ����
	std::vector<std::string> setEditorType(std::string type);//���õ�ǰ�༭���Ĳ���,����һ��stringList,ָ���ò�����Ҫ��Щ����
	void setGrid(int GX,int GY);//��Ŀǰѡ���ı༭������Ӧ�õ�������

private:
	std::string currType;
};
