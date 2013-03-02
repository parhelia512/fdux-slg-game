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
	BattleLib.AssignAIGroup(2, "Team2Squad_0", "Guard");
	BattleLib.AssignAIGroup(2, "Team2Squad_1", "Guard");
	BattleLib.AssignAIGroup(2, "Team2Squad_2", "Guard");
	BattleLib.AssignAIGroup(2, "Team2Squad_3", "Guard");
	BattleLib.AssignAIGroup(2, "Team2Squad_4", "Guard");
	
	BattleLib.AssignAIGroup(3, "Team3Squad_0", "Winfred");
	BattleLib.AssignAIGroup(3, "Team3Squad_1", "Winfred");
	BattleLib.AssignAIGroup(3, "Team3Squad_2", "Winfred");
	--����AI����(team,missionname,missiontype,missiontargetarea)
	BattleLib.CreateAIMission(2, "Attack1", 0, "DeployArea");
	BattleLib.CreateAIMission(3, "Attack2", 0, "DeployArea");
	BattleLib.CreateAIMission(3, "Attack3", 0, "TargetArea");
	--ָ��AI��������(team,groupname,missionname)
	BattleLib.AssignAIMission(2,"Noth", "Attack1");
	BattleLib.AssignAIMission(3,"Winfred", "Attack2");
	BattleLib.AssignAIMission(3,"Winfred2", "Attack3");
	
	--��������(missionname,missionstate)
	local mainmission = BattleLib.AddPlayerMission("map15mission1",0);
	ScriptCommonLib.SetInt("mainmission", mainmission);
	local mission1 = BattleLib.AddPlayerMission("map15mission2",0);
	ScriptCommonLib.SetInt("mission1", mission1);
	ScriptCommonLib.SetInt("mission1state", 1);
	ScriptCommonLib.SetInt("addedgold", 0);
end

--���ӱ����𴥷���
function unitdead()
	local squad = ScriptCommonLib.GetTempString("squadid");
	
	if BattleLib.TeamSquadLeft(1) == 0  then
		--ʧ��(storyscript,gold,exp)
		BattleLib.Lost("GameOver.lua", "0");
	end
	local faction = SquadLib.GetFaction(squad);
	if faction == 1 then
		BattleLib.AddGold(300);
		local addedgold = ScriptCommonLib.GetInt("addedgold");
		addedgold = addedgold + 300;
		ScriptCommonLib.SetInt("addedgold", addedgold);
		local mission1state = ScriptCommonLib.GetInt("mission1state");
		if BattleLib.TeamSquadLeft(2) == 0 and BattleLib.TeamSquadLeft(3) == 0 and mission1state == 1 then
			BattleLib.AddGold(5400);
			BattleLib.DumpSquadData();
			addedgold = ScriptCommonLib.GetInt("addedgold");
			addedgold = addedgold + 5400;
			if mission1state == 1 then
				BattleLib.AddGold(3200);
				addedgold = addedgold + 3200;
			end
			ScriptCommonLib.SetInt("addedgold", addedgold);
			--ʤ��(storyscript,gold,exp)
			BattleLib.Win("cp31.lua", addedgold);
		end
	end
end

function turnstart()
	local turn = ScriptCommonLib.GetTempInt("turn");
	local team = ScriptCommonLib.GetTempInt("team");
	if team == 1 and turn == 1 then
	    BattleLib.Story("cp30_1.lua");
	end
	if team == 1 and turn == 3 then
		MapLib.MoveCameraTo( 2, 2);
		AVGLib.SetCanSupply("WingsHeavyInf",1);
		AVGLib.SetCanSupply("WingsCrossBow",1);
		AVGLib.SetCanSupply("Fay",1);
		AVGLib.SetCanSupply("Cheetah",1);
		AVGLib.SetCanSupply("WingsLightCavalry",1);
		BattleLib.AddStorySquad ("WingsHeavyInf", "WingsHeavyInf", 1, 3, 1);
		BattleLib.AddStorySquad ("WingsCrossBow", "WingsCrossBow", 0, 2, 1);
		BattleLib.AddStorySquad ("WingsLightCavalry", "WingsLightCavalry", 2, 3, 1);
		BattleLib.AddStorySquad ("Fay", "Fay", 1, 2, 1);
		BattleLib.AddStorySquad ("Cheetah", "Cheetah", 2, 2, 1);
	    BattleLib.Story("cp30_2.lua");
	end
	if team == 1 and turn == 9 then
		MapLib.MoveCameraTo( 3, 6);
	    BattleLib.AddBattleSquad("James", "Team3Squad_3", 3, 6, 3, 50);
		BattleLib.AddBattleSquad("WinfredLightCavalry", "Team3Squad_4", 1, 5, 3, 50);
		BattleLib.AddBattleSquad("WinfredLightCavalry", "Team3Squad_5", 1, 7, 3, 50);
		BattleLib.AddBattleSquad("WinfredHeavyCavalry", "Team3Squad_6", 2, 5, 3, 50);
		BattleLib.AddBattleSquad("WinfredHeavyCavalry", "Team3Squad_7", 2, 7, 3, 50);
		BattleLib.AddBattleSquad("WinfredHeavyHalberd", "Team3Squad_8", 0, 5, 3, 50);
		BattleLib.AddBattleSquad("WinfredHeavyHalberd", "Team3Squad_9", 0, 7, 3, 50);
		BattleLib.AddBattleSquad("WinfredLightBow", "Team3Squad_10", 0, 4, 3, 50);
		BattleLib.AddBattleSquad("WinfredLightBow", "Team3Squad_11", 0, 8, 3, 50);
		
		BattleLib.AssignAIGroup(3, "Team3Squad_3", "Winfred2");
		BattleLib.AssignAIGroup(3, "Team3Squad_4", "Winfred2");
		BattleLib.AssignAIGroup(3, "Team3Squad_5", "Winfred2");
		BattleLib.AssignAIGroup(3, "Team3Squad_6", "Winfred2");
		BattleLib.AssignAIGroup(3, "Team3Squad_7", "Winfred2");
		BattleLib.AssignAIGroup(3, "Team3Squad_8", "Winfred2");
		BattleLib.AssignAIGroup(3, "Team3Squad_9", "Winfred2");
		BattleLib.AssignAIGroup(3, "Team3Squad_10", "Winfred2");
		BattleLib.AssignAIGroup(3, "Team3Squad_11", "Winfred2");
		
		BattleLib.Story("cp30_5.lua");
		local mission1 = ScriptCommonLib.GetInt("mission1");
		--�������(missionindex, missionstate)
		BattleLib.SetPlayerMission(mission1, 2);
		ScriptCommonLib.SetInt("mission1state", 0);
		local mainmission = BattleLib.AddPlayerMission("map15mission3",0);
		ScriptCommonLib.SetInt("mainmission", mainmission);
	end

end	
		
--�غϽ���������
function turnend()
	local turn = ScriptCommonLib.GetTempInt("turn");
	local team = ScriptCommonLib.GetTempInt("team");
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
	local faction = SquadLib.GetFaction(squad);
	local mission1state = ScriptCommonLib.GetInt("mission1state");
	
	if faction == 0 and area == "TargetArea" and mission1state == 0 then
		BattleLib.AddGold(5400);
		BattleLib.DumpSquadData();
		local addedgold = ScriptCommonLib.GetInt("addedgold");
		addedgold = addedgold + 5400;
		ScriptCommonLib.SetInt("addedgold", addedgold);
		--ʤ��(storyscript,gold,exp)
		BattleLib.Win("cp31.lua",  addedgold);
	end
end