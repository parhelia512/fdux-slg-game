function initmap()
	trigerid = MapLib.AddMapTrigger("FinishDeploy","finishdeploy");
	MapLib.ActiveMapTrigger(trigerid);
	ScriptCommonLib.SetString("finishdeploytriger",trigerid);
	
	ScriptCommonLib.PlayMusic("battle2.ogg");
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
	
	BattleLib.AssignAIGroup(3, "Team3Squad_0", "Kareena");
	BattleLib.AssignAIGroup(3, "Team3Squad_1", "Kareena");
	BattleLib.AssignAIGroup(3, "Team3Squad_2", "Kareena");
	
	--����AI����(team,missionname,missiontype,missiontargetarea)
	BattleLib.CreateAIMission(2, "Attack", 0, "DeployArea");
	BattleLib.CreateAIMission(3, "Defend", 0, "KeyArea");
	--ָ��AI��������(team,groupname,missionname)
	BattleLib.AssignAIMission(2,"Ulva", "Attack");
	BattleLib.AssignAIMission(3,"Kareena", "Defend");
	
	--��������(missionname,missionstate)
	mainmission = BattleLib.AddPlayerMission("map8mission1",0);
	ScriptCommonLib.SetInt("mainmission", mainmission);
	mission1 = BattleLib.AddPlayerMission("map8mission2",0);
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
	
	if squad == Kareena then
		ScriptCommonLib.SetInt("mission1state", 0);
		BattleLib.SetPlayerMission(mission1, 2);
	end
	
	faction = SquadLib.GetFaction(squad);
	if faction == 1 then
		BattleLib.AddGold(100);
		addedgold = ScriptCommonLib.GetInt("addedgold");
		addedgold = addedgold + 100;
		ScriptCommonLib.SetInt("addedgold", addedgold);
		if BattleLib.TeamSquadLeft(2) == 0  then
			BattleLib.AddGold(1500);
			BattleLib.DumpSquadData();
			addedgold = ScriptCommonLib.GetInt("addedgold");
			addedgold = addedgold + 1500;
			mission1state = ScriptCommonLib.GetInt("mission1state");
			if mission1state == 1 then
				BattleLib.AddGold(1000);
				addedgold = addedgold + 1000;
			end
			ScriptCommonLib.SetInt("addedgold", addedgold);
			--ʤ��(storyscript,gold,exp)
			BattleLib.Win("cp17.lua", "0", "0");
		end
	end
end

function turnstart()
	turn = ScriptCommonLib.GetTempInt("turn");
	team = ScriptCommonLib.GetTempInt("team");
	if team == 1 and turn == 1 then
	    BattleLib.Story("cp16_1.lua");
	end
	
	if team == 1 and turn == 4 then
	    BattleLib.Story("cp16_5.lua");
		AVGLib.SetCanSupply("GuardHeavySpear",1);
		AVGLib.SetCanSupply("GuardLightBow",1);
		AVGLib.SetCanSupply("GuardLightSword",1);
		AVGLib.SetCanSupply("Fay",1);
		AVGLib.SetCanSupply("Elementalist",1);
		BattleLib.CreateStorySquad ("Fay", "Fay", 12, 14, true);
		BattleLib.CreateStorySquad ("GuardHeavySpear", "GuardHeavySpear", 13, 15, true);
		BattleLib.CreateStorySquad ("GuardLightBow", "GuardHeavySpear", 13, 14, true);
		BattleLib.CreateStorySquad ("GuardLightSword", "GuardHeavySpear", 12, 15, true);
		BattleLib.CreateStorySquad ("Elementalist", "GuardHeavySpear", 14, 15, true);
	end
end	
		
--�غϽ���������
function turnend()
	turn = ScriptCommonLib.GetTempInt("turn");
	team = ScriptCommonLib.GetTempInt("team");
	
	if team == 2 and turn>=10 then
		BattleLib.AddGold(1500);
		BattleLib.DumpSquadData();
		addedgold = ScriptCommonLib.GetInt("addedgold");
		--ʤ��(storyscript,gold,exp)
		BattleLib.Win("cp17.lua", "0", "0");
			if mission1state == 1 then
				BattleLib.AddGold(1000);
				addedgold = addedgold + 1000;
			end
		ScriptCommonLib.SetInt("addedgold", addedgold);	
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