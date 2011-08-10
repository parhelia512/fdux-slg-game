#include "BlockEditor.h"
#include "TerrainBlock.h"

BlockEditor::BlockEditor(TerrainBlock* (*terrainBlocks)[MAX_MAPSIZE][MAX_MAPSIZE],Grid* grid,RampManager* ramp):TerrainEditor(terrainBlocks,grid,ramp),currType("NULL")
{
}

BlockEditor::~BlockEditor(void)
{
}

std::vector<std::string> BlockEditor::setEditorType( std::string type )
{
	currType=type;
	if(currType!="NULL")
	{
	}
	return std::vector<std::string>();
}

void BlockEditor::setGrid( int GX,int GY )
{
	if(currType!="NULL")
	{
		switch(mMode)
		{
		case PutMode:
			{
				if (currType=="PlainBlock")
				{
					((*mTerrainBlocks)[GX][GY])->setTerrainAsPlane(5);
				}
				else
				{
					((*mTerrainBlocks)[GX][GY])->setTerrainAsMesh(currType,0);
				}
				break;
			}
		case RemoveMode:
			{
				((*mTerrainBlocks)[GX][GY])->removeMesh();
				break;
			}
		}
	}
}

std::vector<std::string> BlockEditor::getEditorTypeList()
{
	std::vector<std::string> result;

	/*
	result.push_back("WaterBlcok");//ˮ��
	result.push_back("PlainBlock");//ƽԭ
	result.push_back("HillBlock");//����
	result.push_back("HighLandBlock");//�ߵ�
	result.push_back("UpgradeBlock");//����
	*/
	return result;
}
