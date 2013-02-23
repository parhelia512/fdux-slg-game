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
	BattleLib.AssignAIGroup(2, "Team2Squad_0", "Ulva");
	BattleLib.AssignAIGroup(2, "Team2Squad_1", "Ulva");
	BattleLib.AssignAIGroup(2, "Team2Squad_2", "Ulva");
	BattleLib.AssignAIGroup(2, "Team2Squad_3", "Ulva");
	BattleLib.AssignAIGroup(2, "Team2Squad_4", "Ulva");
	BattleLib.AssignAIGroup(2, "Team2Squad_5", "Ulva");
	BattleLib.AssignAIGroup(2, "Team2Squad_6", "Ulva");
	BattleLib.AssignAIGroup(2, "Team2Squad_7", "Ulva");
	BattleLib.AssignAIGroup(2, "Team2Squad_8", "Ulva");
	--����AI����(team,missionname,missiontype,missiontargetarea)
	BattleLib.CreateAIMission(2, "Attack", 0, "DeployArea");
	BattleLib.CreateAIMission(2, "AttackUlva", 0, "DeployArea");
	--ָ��AI��������(team,groupname,missionname)
	BattleLib.AssignAIMission(2, "Ulva", "Attack");
	BattleLib.AssignAIMission(3, "Winfred", "AttackUlva");
	
	--��������(missionname,missionstate)
	mainmission = BattleLib.AddPlayerMission("map6mission1",0);
	ScriptCommonLib.SetInt("mainmission", mainmission);
	mission1 = BattleLib.AddPlayerMission("map6mission2",0);
	ScriptCommonLib.SetInt("mission1", mission1);
	ScriptCommonLib.SetInt("mission1state", 1);
	ScriptCommonLib.SetInt("addedgold", 0);
end

--���ӱ����𴥷���
function unitdead()
	squad = ScriptCommonLib.GetTempString("squadid");
	
	if BattleLib.TeamSquadLeft(1) == 0  then
		--ʧ��(storyscript,gold,exp)
		BattleLib.Lost("GameOver.lua","0","0");
	end
	faction = SquadLib.GetFaction(squad);
	if faction == 1 then
		BattleLib.AddGold(100);
		addedgold = ScriptCommonLib.GetInt("addedgold");
		addedgold = addedgold + 100;
		ScriptCommonLib.SetInt("addedgold", addedgold);
	end
	if squad == Elementalist then
	    mission1 = ScriptCommonLib.GetInt("mission1");
		--�������(missionindex, missionstate)
		BattleLib.SetPlayerMission(mission1, 2);
		ScriptCommonLib.SetInt("mission1state", 0);
	end
end

function turnstart()
	turn = ScriptCommonLib.GetTempInt("turn");
	team = ScriptCommonLib.GetTempInt("team");
	if team == 1 and turn == 1 then
	    BattleLib.Story("cp12_1.lua");
	end
	if team == 1 and turn == 2 then
	    BattleLib.Story("cp12_2.lua");
		AVGLib.SetCanSupply("Fay",1);
		AVGLib.SetCanSupply("GuardHeavySpear",1);
		BattleLib.CreateStorySquad ("Fay", "Fay", 17, 18, true);
		BattleLib.CreateStorySquad ("GuardHeavySpear", "GuardHeavySpear", 16, 18, true);
	end
	if team == 1 and turn == 4 then
	    BattleLib.Story("cp12_5.lua");
		BattleLib.AddBattleSquad("James", "Team3Squad_0", 8, 11, 3, 50);
		BattleLib.AddBattleSquad("WinfredHeavyHalberd", "Team3Squad_1", 6, 16, 3, 50);
		BattleLib.AddBattleSquad("WinfredHeavyHalberd", "Team3Squad_2", 8, 10, 3, 50);
		BattleLib.AddBattleSquad("WinfredLightHalberd", "Team3Squad_3", 6, 17, 3, 50);
		BattleLib.AddBattleSquad("WinfredLightHalberd", "Team3Squad_4", 5, 17, 3, 50);
		BattleLib.AddBattleSquad("WinfredLightBow", "Team3Squad_5", 5, 16, 3, 50);
		BattleLib.AddBattleSquad("WinfredLightBow", "Team3Squad_6", 7, 11, 3, 50);
		BattleLib.AddBattleSquad("WinfredLightBow", "Team3Squad_7", 7, 10, 3, 50);
			
		BattleLib.AssignAIGroup(3, "Team2Squad_0", "Winfred");
		BattleLib.AssignAIGroup(3, "Team2Squad_1", "Winfred");
		BattleLib.AssignAIGroup(3, "Team2Squad_2", "Winfred");
		BattleLib.AssignAIGroup(3, "Team2Squad_3", "Winfred");
		BattleLib.AssignAIGroup(3, "Team2Squad_4", "Winfred");
		BattleLib.AssignAIGroup(3, "Team2Squad_5", "Winfred");
		BattleLib.AssignAIGroup(3, "Team2Squad_6", "Winfred");
		BattleLib.AssignAIGroup(3, "Team2Squad_7", "Winfred");
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
		mission1state = ScriptCommonLib.GetInt("mission1state");
		if mission1state == 1 then
			BattleLib.AddGold(700);
			addedgold = addedgold + 700;
		end
		ScriptCommonLib.SetInt("addedgold", addedgold);
		--ʤ��(storyscript,gold,exp)
		BattleLib.Win("cp12.lua", "0", "0");
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
end