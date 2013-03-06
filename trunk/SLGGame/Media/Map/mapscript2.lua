function initmap()
	local trigerid = MapLib.AddMapTrigger("FinishDeploy","finishdeploy");
	MapLib.ActiveMapTrigger(trigerid);
	ScriptCommonLib.SetString("finishdeploytriger",trigerid);
	
	ScriptCommonLib.PlayMusic("battle1.ogg");
	MapLib.SetCamera(0,4);
end

function finishdeploy()
	local trigerid = ScriptCommonLib.GetString("finishdeploytriger");
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
	BattleLib.AssignAIGroup(2, "Team2Squad_10", "DuxRaider");
	BattleLib.AssignAIGroup(2, "Team2Squad_11", "DuxRaider");
	--����AI����(team,missionname,missiontype,missiontargetarea)
	BattleLib.CreateAIMission(2, "DefendCamp", 0, "CampArea");
	BattleLib.CreateAIMission(3, "AttackDux", 0, "CampArea");
	--ָ��AI��������(team,groupname,missionname)
	BattleLib.AssignAIMission(2, "DuxRaider", "DefendCamp");
	BattleLib.AssignAIMission(3, "Fantasy", "AttackDux");
	
	--��������(missionname,missionstate)
	local mainmission = BattleLib.AddPlayerMission("map2mission1",0);
	ScriptCommonLib.SetInt("mainmission", mainmission);
	local mission1 = BattleLib.AddPlayerMission("map2mission2",0);
	ScriptCommonLib.SetInt("mission1", mission1);
	ScriptCommonLib.SetInt("mission1state", 1);
	
	ScriptCommonLib.SetInt("addedgold", 0);
	ScriptCommonLib.SetInt("battlestory", 0);
	ScriptCommonLib.SetInt("Squad7",0);
end

--���ӱ����𴥷���
function unitdead()
	local squad = ScriptCommonLib.GetTempString("squadid");
	
	if BattleLib.TeamSquadLeft(1) == 0 then
		--ʧ��(storyscript,gold,exp)
		BattleLib.Lost("GameOver.lua","0");
	end
	local faction = SquadLib.GetFaction(squad);
	if faction == 1 then
		BattleLib.AddGold(100);
		local addedgold = ScriptCommonLib.GetInt("addedgold");
		addedgold = addedgold + 100;
		ScriptCommonLib.SetInt("addedgold", addedgold);
		if BattleLib.TeamSquadLeft(2) == 0 then
			BattleLib.AddGold(1800);
			BattleLib.DumpSquadData();
			addedgold = ScriptCommonLib.GetInt("addedgold");
			addedgold = addedgold + 1800;
			local mission1state = ScriptCommonLib.GetInt("mission1state");
			if mission1state == 1 then
				BattleLib.AddGold(900);
				addedgold = addedgold + 900;
			end
			ScriptCommonLib.SetInt("addedgold", addedgold);
			--ʤ��(storyscript,gold,exp)
			BattleLib.Win("cp3.lua", addedgold);
		end
	end
end

function turnstart()
	local turn = ScriptCommonLib.GetTempInt("turn");
	local team = ScriptCommonLib.GetTempInt("team");
	if team == 1 and turn == 1 then
	    BattleLib.Story("cp2_1.lua");
	end
	if team == 1 and turn == 3 then
		MapLib.MoveCameraTo( 2, 11);
		BattleLib.AddStorySquad("Cossa", "Cossa", 2, 11, 0);
		BattleLib.AddStorySquad("WingsLightCavalry", "WingsLightCavalry", 3, 11, 0);
		BattleLib.AddBattleSquad("FantasyHeavyCavalry", "Team3Squad_0", 1, 8, 3, 50);
		BattleLib.AddBattleSquad("FantasyHeavyCavalry", "Team3Squad_1", 1, 9, 3, 50);
		BattleLib.AddBattleSquad("FantasyHeavyCavalry", "Team3Squad_2", 1, 10, 3, 50);
		BattleLib.AssignAIGroup(3, "Team3Squad_0", "Fantasy");
		BattleLib.AssignAIGroup(3, "Team3Squad_1", "Fantasy");
		BattleLib.AssignAIGroup(3, "Team3Squad_2", "Fantasy");
		BattleLib.Story("cp2_2.lua");
	end
end	
		
--�غϽ���������
function turnend()
	local turn = ScriptCommonLib.GetTempInt("turn");
	local team = ScriptCommonLib.GetTempInt("team");
	
	if team == 1 and BattleLib.TeamSquadLeft(2) == 0 then
		BattleLib.DumpSquadData();
		local addedgold = ScriptCommonLib.GetInt("addedgold");
		ScriptCommonLib.SetInt("addedgold", addedgold);
		--ʤ��(storyscript,gold,exp)
		BattleLib.Win("cp3.lua", addedgold);
	end
end

--�뿪���򴥷���
function outarea()
	local squad = ScriptCommonLib.GetTempString("squadid");
	local area = ScriptCommonLib.GetTempString("areaid");
end

--�������򴥷���
function inarea()
	local squad = ScriptCommonLib.GetTempString("squadid");
	local area = ScriptCommonLib.GetTempString("areaid");
	
	if area == "BigCamp" then
		local faction = SquadLib.GetFaction(squad);
		if faction == 1 then
			local mission1state = ScriptCommonLib.GetInt("mission1state");
			if mission1state == 1 then
				local mission1 = ScriptCommonLib.GetInt("mission1");
				--����ɹ�(missionindex, missionstate)
				BattleLib.SetPlayerMission(mission1, 2);
				ScriptCommonLib.SetInt("mission1state", 0);
			end
		end
	end
end