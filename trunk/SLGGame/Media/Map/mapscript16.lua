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
	BattleLib.AssignAIGroup(2, "Team2Squad_0", "Winfred");
	BattleLib.AssignAIGroup(2, "Team2Squad_1", "Winfred");
	BattleLib.AssignAIGroup(2, "Team2Squad_2", "Winfred");
	BattleLib.AssignAIGroup(2, "Team2Squad_3", "Winfred");
	BattleLib.AssignAIGroup(2, "Team2Squad_4", "Winfred");
	BattleLib.AssignAIGroup(2, "Team2Squad_5", "Winfred");

	BattleLib.AssignAIGroup(3, "Team3Squad_0", "Guard");
	BattleLib.AssignAIGroup(3, "Team3Squad_1", "Guard");
	BattleLib.AssignAIGroup(3, "Team3Squad_2", "Guard");
	BattleLib.AssignAIGroup(3, "Team3Squad_3", "Guard");
	BattleLib.AssignAIGroup(3, "Team3Squad_4", "Guard");
	
	--����AI����(team,missionname,missiontype,missiontargetarea)
	BattleLib.CreateAIMission(2, "Attack1", 0, "DeployArea");
	BattleLib.CreateAIMission(3, "Attack2", 0, "DeployArea");
	--ָ��AI��������(team,groupname,missionname)
	BattleLib.AssignAIMission(2,"Winfred", "Attack1");
	BattleLib.AssignAIMission(3,"Guard", "Attack2");
		
	--��������(missionname,missionstate)
	mainmission = BattleLib.AddPlayerMission("map16mission1",0);
	ScriptCommonLib.SetInt("mainmission", mainmission);
	mission1 = BattleLib.AddPlayerMission("map16mission2",0);
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
		if BattleLib.TeamSquadLeft(2) == 0 and BattleLib.TeamSquadLeft(3) == 0 then
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
		BattleLib.Win("cp33.lua", "0", "0");
		
	end
	if faction == 0 then
	   mission1state = ScriptCommonLib.GetInt("mission1state");
		if mission1state == 1 then
			mission1 = ScriptCommonLib.GetInt("mission1");
			--�������(missionindex, missionstate)
			BattleLib.SetPlayerMission(mission1, 2);
			ScriptCommonLib.SetInt("mission1state", 0);
		end
	end
end

function turnstart()
	turn = ScriptCommonLib.GetTempInt("turn");
	team = ScriptCommonLib.GetTempInt("team");
	if team == 1 and turn == 1 then
	    BattleLib.Story("cp32_1.lua");
	end
	if team == 1 and turn == 3 then
		BattleLib.Story("cp32_2.lua");
		AVGLib.SetCanSupply("WingsHeavyInf",1);
		AVGLib.SetCanSupply("WingsCrossBow",1);
		AVGLib.SetCanSupply("Azaria",1);
		AVGLib.SetCanSupply("Caster",1);
		AVGLib.SetCanSupply("Olivia",1);
		AVGLib.SetCanSupply("Cheetah",1);
		BattleLib.CreateStorySquad ("WingsHeavyInf", "WingsHeavyInf", 18, 3, false);
		BattleLib.CreateStorySquad ("WingsCrossBow", "WingsCrossBow", 19, 1, false);
		BattleLib.CreateStorySquad ("Azaria", "Azaria", 19, 2, false);
		BattleLib.CreateStorySquad ("Caster", "Caster", 17, 2, false);
		BattleLib.CreateStorySquad ("Olivia", "Olivia", 19, 3, false);
		BattleLib.CreateStorySquad ("Cheetah", "Cheetah", 18, 2, false);
	end
end	
		
--�غϽ���������
function turnend()
	turn = ScriptCommonLib.GetTempInt("turn");
	team = ScriptCommonLib.GetTempInt("team");
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