#include "MapLoader.h"

#include "BattleSquadManager.h"
#include "BattleSquad.h"
#include "AVGSquadManager.h"
#include "SquadGrapManager.h"

#include "MapDataManager.h"
#include "Terrain.h"

#include "StringTable.h"
#include "DataLibrary.h"

MapLoader::MapLoader()
{

}
MapLoader::~MapLoader()
{

}

bool MapLoader::loadMapFormFile(std::string mapname)
{
	int mapsize;
	Terrain* terrain = Terrain::getSingletonPtr();
	DataLibrary* datalibrary = DataLibrary::getSingletonPtr();
	std::string path = ".\\..\\Media\\Map\\" + mapname;
	ticpp::Document *doc = new ticpp::Document();
	doc->LoadFile(path,TIXML_ENCODING_UTF8);
	std::string str;
	//�����ͼ���֣����ܺͽű���
	ticpp::Element *element = doc->FirstChildElement("MapName");
	element->GetText(&str);
	datalibrary->setData("GameData/BattleData/MapData/MapName", StringTable::getSingleton().getString(str));
	delete element;

	element = doc->FirstChildElement("MapScript");
	element->GetText(&str);
	datalibrary->setData("GameData/BattleData/MapData/MapScript", str);
	delete element;

	element = doc->FirstChildElement("MapInfo");
	element->GetText(&str);
	datalibrary->setData("GameData/BattleData/MapData/MapInfo", StringTable::getSingleton().getString(str));
	delete element;

	//�����ͼ������Ϣ
	element = doc->FirstChildElement("MapSize");
	element->GetText(&mapsize);
	MapDataManager::getSingleton().mMapSize = mapsize;
	datalibrary->setData("GameData/BattleData/MapData/MapSize", mapsize);
	delete element;

	element = doc->FirstChildElement("MapData");
	element->GetText(&str);
	for(int y = 0; y < mapsize; y++)
	{
		for(int x = 0; x < mapsize; x++)
		{
			int index = (y * mapsize + x);
			char datapathtemp[64];
			sprintf_s(datapathtemp, 64, "GameData/BattleData/MapData/Map/M%d", index);
			std::string datapath = datapathtemp;
			if(str[index * 2] == 'l')
			{
				bool iscliff = false;
				for(int i = y - 1; i < y + 2; i ++)
				{
					for(int j =  x - 1; j < x + 2; j++)
					{
						int u = (i<0)?0:i;
						u = (u >= mapsize)?mapsize-1:u;
						int v = (j<0)?0:j;
						v = (v >= mapsize)?mapsize-1:v;
						int tempindex = u * mapsize + v;
						if(str[tempindex * 2] == 'h' )
						{	
							iscliff = true;
						}
					}
				}
				if(iscliff)
					datalibrary->setData(datapath + "/TerrainType", Cliff);
				else
					datalibrary->setData(datapath + "/TerrainType", LowGround);
			}
			else if(str[index * 2] == 'w')
			{
				datalibrary->setData(datapath + "/TerrainType", Water);
			}
			else if(str[index * 2] == 'h')
			{
				datalibrary->setData(datapath + "/TerrainType", HighGround);
			}
			else if(str[index * 2] == 'r')
			{
				datalibrary->setData(datapath + "/TerrainType", Ramp);
			}

			if(str[index * 2+1] == 'g')
			{
				datalibrary->setData(datapath + "/GroundType", GreenLand);
			}
			else if(str[index * 2+1] == 'd')
			{
				datalibrary->setData(datapath + "/GroundType", Desert);
			}
			else if(str[index * 2+1] == 'w')
			{
				datalibrary->setData(datapath + "/GroundType", Swamp);
			}
			else if(str[index * 2+1] == 's')
			{
				datalibrary->setData(datapath + "/GroundType", Snow);
			}
		}
	}
	delete element;

	element = doc->FirstChildElement("MapObject");
	ticpp::Iterator<ticpp::Element> child;
	for(child = child.begin(element); child != child.end(); child++)
	{
		std::string objname;
		std::string datapath;
		child->GetValue(&objname);
		datapath = std::string("GameData/BattleData/MapData/MapObjModleInfo/") + objname;
		int objx,objy;
		std::string meshname,objtype;
		child->GetAttribute("GridX",&objx);
		child->GetAttribute("GridY",&objy);
		child->GetAttribute("Mesh",&meshname);
		child->GetAttribute("Type",&objtype);
		datalibrary->setData(datapath + "/GridX", objx);
		datalibrary->setData(datapath + "/GridY", objy);
		datalibrary->setData(datapath + "/Mesh", meshname);
		//��Ʒ���ͽű�
		datapath = std::string("GameData/BattleData/MapData/Map/M") + Ogre::StringConverter::toString(objy * mapsize + objx) + std::string("/MapObjType");
		datalibrary->setData(datapath, objtype);
		datalibrary->setData(datapath + "/MapObjModuleId", objname);
	}
	delete element;

	element = doc->FirstChildElement("MapEffect");
	for(child = child.begin(element); child != child.end(); child++)
	{
		std::string particlename;
		std::string datapath;
		child->GetValue(&particlename);
		datapath = std::string("GameData/BattleData/MapData/MapParticleInfo/") + particlename;
		int particlex,particley;
		std::string scriptname;
		child->GetAttribute("GridX",&particlex);
		child->GetAttribute("GridY",&particley);
		child->GetAttribute("Type",&scriptname);
		datalibrary->setData(datapath + "/GridX", particlex);
		datalibrary->setData(datapath + "/GridY", particley);
		datalibrary->setData(datapath + "/Script", scriptname);
	}

	terrain->createTerrain();
	delete element;

	//����������Ϣ
	element = doc->FirstChildElement("MapArea");
	for(child = child.begin(element); child != child.end(); child++)
	{
		std::string areaname;
		std::string datapath;
		child->GetValue(&areaname);
		datapath = std::string("GameData/BattleData/MapData/Area/") + areaname;
		ticpp::Iterator<ticpp::Element> childchild;
		for(childchild = childchild.begin(child.Get()); childchild != childchild.end(); childchild++)
		{
			std::string coordname;
			childchild->GetValue(&coordname);
			int x;
			int y;
			childchild->GetAttribute("X",&x);
			childchild->GetAttribute("Y",&y);
			datalibrary->setData(datapath + std::string("/CoordList/") + coordname + std::string("/X"),x );
			datalibrary->setData(datapath + std::string("/CoordList/") + coordname + std::string("/Y"),y );
		}

	}
	delete element;

	//���������Ϣ
	element = doc->FirstChildElement("MapTeam");
	for(int n = 2; n < 5; n++)
	{
		std::string name = std::string("Team") + Ogre::StringConverter::toString(n);
		std::string factionid;
		ticpp::Element* subelement = element->FirstChildElement(name);
		subelement->GetAttribute("TeamFaction",&factionid);
		datalibrary->setData(std::string("GameData/BattleData/Team/")+ name+ "/FactionId", factionid);
		if(factionid != "none")
		{
			subelement->GetAttribute("TeamType",&factionid);
			datalibrary->setData(std::string("GameData/BattleData/Team/")+ name+ "/Relation", factionid);
		}
		delete subelement;
	}
	std::string playerfactionid;
	datalibrary->getData("GameData/StoryData/Faction",playerfactionid);
	datalibrary->setData("GameData/BattleData/Team/Team1/FactionId",playerfactionid);
	datalibrary->setData("GameData/BattleData/Team/Team1/Relation","player");
	delete element;

	//���벿����Ϣ
	element = doc->FirstChildElement("MapSquad");
	for(child = child.begin(element); child != child.end(); child++)
	{
		std::string teamid;
		std::string datapath;
		child->GetValue(&teamid);
		datapath = std::string("GameData/BattleData/SquadList");
		ticpp::Iterator<ticpp::Element> childchild;
		for(childchild = childchild.begin(child.Get()); childchild != childchild.end(); childchild++)
		{
			std::string squadid;
			std::string squadtype;
			childchild->GetValue(&squadid);
			int x;
			int y;
			Direction d;
			int unitnum;
			childchild->GetAttribute("Type",&squadtype);
			childchild->GetAttribute("GridX",&x);
			childchild->GetAttribute("GridY",&y);
			childchild->GetAttribute("UnitNum",&unitnum);
			childchild->GetAttribute("Direction",&d);
			AVGSquadManager::getSingleton().addSquad(squadid,squadtype, datapath);
			int type;
			Formation f;
			datalibrary->getData(datapath + std::string("/") + squadid + std::string("/Type"), type );
			if(type == SQUAD_NORMAL)
				f = Line;
			else
				f = Loose;
			datalibrary->setData(datapath + std::string("/") + squadid + std::string("/GridX"), x, true );
			datalibrary->setData(datapath + std::string("/") + squadid + std::string("/GridY"), y, true );
			datalibrary->setData(datapath + std::string("/") + squadid + std::string("/UnitNumber"), unitnum, true );
			datalibrary->setData(datapath + std::string("/") + squadid + std::string("/Direction"), d, true );
			datalibrary->setData(datapath + std::string("/") + squadid + std::string("/Formation"), f, true );
			datalibrary->setData(datapath + std::string("/") + squadid + std::string("/TeamId"), teamid, true );
			datalibrary->setData(datapath + std::string("/") + squadid + std::string("/CreateType"), MapSquad, true );
			datalibrary->setData(datapath + std::string("/") + squadid + std::string("/ActionPoint"), 0.0f, true );
		}
	}
	delete element;

	return true;
}
bool MapLoader::loadMapFormSave()
{
	return false;
}

void MapLoader::loadMapObj()
{
	DataLibrary* datalibrary = DataLibrary::getSingletonPtr();
	Terrain* terrain = Terrain::getSingletonPtr();
	std::string datapath("GameData/BattleData/MapData/MapObjModleInfo");
	std::vector<std::string> childlist;
	childlist = datalibrary->getChildList(datapath);
	if(childlist.size()>0)
	{
		for(int n = 0; n < childlist.size(); n++)
		{
			std::string meshname;
			int x,y;
			datalibrary->getData(datapath + std::string("/") + childlist[n] + std::string("/Mesh"),meshname);
			datalibrary->getData(datapath + std::string("/") +childlist[n] + std::string("/GridX"),x);
			datalibrary->getData(datapath + std::string("/") +childlist[n] + std::string("/GridY"),y);
			int index;
			index = terrain->createMapObj(x,y,meshname);
			datalibrary->setData(datapath + std::string("/") + childlist[n] + std::string("/Index"),index);
		}
	}
	datapath = "GameData/BattleData/MapData/MapParticleInfo";
	childlist = datalibrary->getChildList(datapath);
	if(childlist.size()>0)
	{
		for(int n = 0; n < childlist.size(); n++)
		{
			std::string particlename;
			int x,y;
			datalibrary->getData(datapath + std::string("/") + childlist[n] + std::string("/Script"),particlename);
			datalibrary->getData(datapath + std::string("/") +childlist[n] + std::string("/GridX"),x);
			datalibrary->getData(datapath + std::string("/") +childlist[n] + std::string("/GridY"),y);
			int index;
			index = terrain->createMapParticle(x,y,particlename);
			datalibrary->setData(datapath + std::string("/") + childlist[n] + std::string("/Index"),index);
		}
	}
}

void MapLoader::initBattleSquad(bool loadfrommap)
{
	BattleSquadManager* battlesuqadmanager = BattleSquadManager::getSingletonPtr();
	DataLibrary* datalib =DataLibrary::getSingletonPtr();
	SquadGrapManager* suqadgrapmanager = SquadGrapManager::getSingletonPtr();
	AVGSquadManager* avgsquadmanager = AVGSquadManager::getSingletonPtr();
	creatSquadGrapAtPath(std::string("GameData/BattleData/SquadList"));
	if(loadfrommap)
	{
		std::vector<std::string> childlist;
		childlist = datalib->getChildList(std::string("GameData/StoryData/SquadData/EnableSquad"));
		if(childlist.size()>0)
		{
			std::vector<std::string>::iterator ite;
			for(ite = childlist.begin(); ite != childlist.end(); ite++)
			{
				std::string datapath = std::string("GameData/BattleData/SquadList/") + (*ite);
				datalib->copyNode(std::string("GameData/StoryData/SquadData/EnableSquad/") + (*ite),datapath, true);
				int type;
				Formation f;
				datalib->getData(datapath + std::string("/Type"), type );
				if(type == SQUAD_NORMAL)
					f = Line;
				else
					f = Loose;
				datalib->setData(datapath +std::string("/TeamId"), std::string("Team1"), true );
				datalib->setData(datapath +std::string("/Grapid"),battlesuqadmanager->mCurid, true);
				datalib->setData(datapath +std::string("/CreateType"), StroySquad, true );
				datalib->setData(datapath +std::string("/Direction"), North, true );
				datalib->setData(datapath +std::string("/Formation"), f, true );
				datalib->setData(datapath +std::string("/ActionPoint"), 0.0f, true );
				BattleSquad* battlesquad = new BattleSquad((*ite),battlesuqadmanager->mCurid,-1,0); 
				suqadgrapmanager->createSquad((*ite), datapath, battlesuqadmanager->mCurid, -1, 0,North,Line,battlesquad->getUnitGrapNum());
				battlesuqadmanager->mDeployList.push_back(battlesquad);
				battlesuqadmanager->mCurid ++;
			}
		}
	}
}

void MapLoader::creatSquadGrapAtPath(std::string path)
{
	BattleSquadManager* battlesuqadmanager = BattleSquadManager::getSingletonPtr();
	DataLibrary* datalib =DataLibrary::getSingletonPtr();
	SquadGrapManager* suqadgrapmanager = SquadGrapManager::getSingletonPtr();
	AVGSquadManager* avgsquadmanager = AVGSquadManager::getSingletonPtr();
	std::vector<std::string> childlist;
	childlist = datalib->getChildList(path);
	if(childlist.size()>0)
	{
		std::vector<std::string>::iterator ite;
		for(ite = childlist.begin(); ite != childlist.end(); ite++)
		{
			std::string datapath = path + std::string("/") + (*ite);
			int x,y;
			Direction d;
			Formation f;
			datalib->getData(datapath + "/GridX",x);
			datalib->getData(datapath + "/GridY",y);
			datalib->getData(datapath + "/Direction",d);
			datalib->getData(datapath + "/Formation",f);
			BattleSquad* battlesquad =  new BattleSquad((*ite),battlesuqadmanager->mCurid,x,y); 
			suqadgrapmanager->createSquad((*ite), datapath, battlesuqadmanager->mCurid,x,y,d,f, battlesquad->getUnitGrapNum());
			datalib->setData(datapath +std::string("/Grapid"),battlesuqadmanager->mCurid);
			battlesuqadmanager->mSquadList.push_back(battlesquad);
			battlesuqadmanager->mCurid ++;
		}
	}
}