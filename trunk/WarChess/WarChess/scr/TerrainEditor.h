/*
 * �ļ���:TerrainEditor.h
 * ����ʱ��:2010:10:14   15:09
 * ��д��:winsock
 * ����:  ��Ϊ���еĵ���ϵͳ�༭���Ļ���,ʹ���ⲿģ�������ϵͳ���뿪��
 */

#pragma once

#include <vector>
#include <map>
#include <string>
#include "Common.h"
#include "Grid.h"

class TerrainBlock;
class RampManager;

class TerrainEditor
{
public:
	TerrainEditor(TerrainBlock* (*terrainBlocks)[MAX_MAPSIZE][MAX_MAPSIZE],Grid* grid,RampManager* ramp ):mTerrainBlocks(terrainBlocks),mGrid(grid),mRamp(ramp),mMode(PutMode){};//��˼��,����һ�������ָ��,���������TerrainBlock��ָ��
	virtual ~TerrainEditor(void){};
	
	virtual EditorID getEditorId()=0;//���ظñ༭����ID
	virtual std::vector<std::string> getEditorTypeList()=0;//���ظñ༭��֧����Щ����
	virtual std::vector<std::string> setEditorType(std::string type)=0;//���õ�ǰ�༭���Ĳ���,����һ��stringList,ָ���ò�����Ҫ��Щ����
	virtual void setGrid(int GX,int GY)=0;//��Ŀǰѡ���ı༭������Ӧ�õ�������

	void setEditorMode(EditorMode mode)//���ñ༭���Ĳ���ģʽ
	{
		mMode=mode;
	}

	void setEditorArgs(std::string name,std::string value)//���ñ༭����������
	{
		mArgs[name]=value;
	}

protected:
	TerrainBlock* (*mTerrainBlocks)[MAX_MAPSIZE][MAX_MAPSIZE];
	std::map<std::string,std::string> mArgs;
	EditorMode mMode;
	Grid* mGrid;
	RampManager *mRamp;
};
