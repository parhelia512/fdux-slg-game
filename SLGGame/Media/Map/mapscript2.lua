function initmap()
	trigerid = MapLib.AddMapTrigger("FinishDeploy","finishdeploy");
	MapLib.ActiveMapTrigger(trigerid);
	ScriptCommonLib.SetString("finishdeploytriger",trigerid);
	
	ScriptCommonLib.PlayMusic("battle1.ogg");
	MapLib.SetCamera(0,4);
end

function finishdeploy()
	trigerid = ScriptCommonLib.GetString("finishdeploytriger");
	MapLib.RemoveMapTrigger(trigerid);
	--�������ӱ����𴥷���
	trigerid = MapLib.AddMapTrigger("SquadAnnihilated","unitdead");
	MapLib.ActiveMapTrigger(trigerid);
	ScriptCommonLib.SetString("unitdeadtriger",trigerid);
	--�����غϽ���������
	trigerid = MapLib.AddMapTrigger("TurnEnd","turnend");
	MapLib.ActiveMapTrigger(trigerid);
	ScriptCommonLib.SetString("turnendtriger",trigerid);
	--�����غϿ�ʼ������
	trigerid = MapLib.AddMapTrigger("TurnStart","turnstart");
	MapLib.ActiveMapTrigger(trigerid);
	ScriptCommonLib.SetString("turnstarttriger",trigerid);
	--�����������򴥷���
	trigerid = MapLib.AddMapTrigger("GetOutArea","outarea");
	MapLib.ActiveMapTrigger(trigerid);
	ScriptCommonLib.SetString("outareatriger",trigerid);
	--�����뿪���򴥷���
	trigerid = MapLib.AddMapTrigger("GetInArea","inarea");
	MapLib.ActiveMapTrigger(trigerid);
	ScriptCommonLib.SetString("inareatriger",trigerid);
	
	--����AI����(team,squadid,groupname)
	BattleLib.AssignAIGroup(2, "Team2Squad_0", "DuxRaider");
	BattleLib.AssignAIGroup(2, "Team2Squad_1", "DuxRaider");
	BattleLib.AssignAIGroup(2, "Team2Squad_2", "DuxRaider");
	BattleLib.AssignAIGroup(2, "Team2Squad_3", "DuxRaider");
	BattleLib.AssignAIGroup(2, "Team2Squad_4", "DuxRaider");
	BattleLib.AssignAIGroup(2, "Team2Squad_5", "DuxRaider");
	BattleLib.AssignAIGroup(2, "Team2Squad_6", "DuxRaider");
	BattleLib.AssignAIGroup(2, "Team2Squad_7", "DuxRaider");
	BattleLib.AssignAIGroup(2, "Team2Squad_8", "DuxRaider");
	BattleLib.AssignAIGroup(2, "Team2Squad_9", "DuxRaider");
	--����AI����(team,missionname,missiontype,missiontargetarea)
	BattleLib.CreateAIMission(2, "DefendCamp", 0, "CampArea");
	BattleLib.CreateAIMission(3, "AttackDux", 0, "CampArea");
	--ָ��AI��������(team,groupname,missionname)
	BattleLib.AssignAIMission(2, "DuxRaider", "DefendCamp");
	BattleLib.AssignAIMission(3, "Fantasy", "AttackDux");
	
	--��������(missionname,missionstate)
	mainmission = BattleLib.AddPlayerMission("map2mission1",0);
	ScriptCommonLib.SetInt("mainmission", mainmission);
	mission1 = BattleLib.AddPlayerMission("map2mission2",0);
	ScriptCommonLib.SetInt("mission1", mission1);
	ScriptCommonLib.SetInt("mission1state", 1);
	
	ScriptCommonLib.SetInt("addedgold", 0);
	ScriptCommonLib.SetInt("battlestory", 0);
	ScriptCommonLib.SetInt("Squad7",0);
end

--���ӱ����𴥷���
function unitdead()
	squad = ScriptCommonLib.GetTempString("squadid");
	
	if squad == "Cheetah" or squad == "Dandelion" or squad == "Cossa" then
		--ʧ��(storyscript,gold,exp)
		BattleLib.Lost("GameOver.lua","0");
	end
	faction = SquadLib.GetFaction(squad);
	if faction == 1 then
		BattleLib.AddGold(100);
		addedgold = ScriptCommonLib.GetInt("addedgold");
		addedgold = addedgold + 100;
		ScriptCommonLib.SetInt("addedgold", addedgold);
	end
end

function turnstart()
	turn = ScriptCommonLib.GetTempInt("turn");
	team = ScriptCommonLib.GetTempInt("team");
	if team == 1 and turn == 1 then
	    BattleLib.Story("cp2_1.lua");
	end
	if team == 1 and turn == 3 then
	    BattleLib.Story("cp2_2.lua");
		BattleLib.AddBattleSquad("Cossa", "Cossa", 2, 11, 1, 50);
		BattleLib.AddBattleSquad("WingsLightCavalry", "WingsLightCavalry", 3, 11, 1, 50);
		BattleLib.AddBattleSquad("FantasyHeavyCavalry", "Team3Squad_0", 1, 8, 3, 50);
		BattleLib.AddBattleSquad("FantasyHeavyCavalry", "Team3Squad_1", 1, 9, 3, 50);
		BattleLib.AddBattleSquad("FantasyHeavyCavalry", "Team3Squad_2", 1, 10, 3, 50);
		BattleLib.AssignAIGroup(3, "Team3Squad_0", "Fantasy");
		BattleLib.AssignAIGroup(3, "Team3Squad_1", "Fantasy");
		BattleLib.AssignAIGroup(3, "Team3Squad_2", "Fantasy");
	end
end	
		
--�غϽ���������
function turnend()
	turn = ScriptCommonLib.GetTempInt("turn");
	team = ScriptCommonLib.GetTempInt("team");
	
	if team == 1 and BattleLib.TeamSquadLeft(2) == 0 then
		BattleLib.AddGold(1500);
		BattleLib.DumpSquadData();
		addedgold = ScriptCommonLib.GetInt("addedgold");
		addedgold = addedgold + 1500;
		if mission1state == 1 then
			BattleLib.AddGold(500);
			addedgold = addedgold + 500;
		end
		ScriptCommonLib.SetInt("addedgold", addedgold);
		--ʤ��(storyscript,gold,exp)
		BattleLib.Win("cp3.lua", addedgold);
	end
end

--�뿪���򴥷���
function outarea()
	squad = ScriptCommonLib.GetTempString("squadid");
	area = ScriptCommonLib.GetTempString("areaid");
end

--�������򴥷���
function inarea()
	squad = ScriptCommonLib.GetTempString("squadid");
	area = ScriptCommonLib.GetTempString("areaid");
	
	if area == "BigCamp" then
		faction = SquadLib.GetFaction(squad);
		if faction == 1 then
			mission1state = ScriptCommonLib.GetInt("mission1state");
			if mission1state == 1 then
				mission1 = ScriptCommonLib.GetInt("mission1");
				--����ɹ�(missionindex, missionstate)
				BattleLib.SetPlayerMission(mission1, 2);
				ScriptCommonLib.SetInt("mission1state", 0);
			end
		end
	end
end