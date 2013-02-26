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
	BattleLib.AssignAIGroup(2, "Team2Squad_0", "Guard");
	BattleLib.AssignAIGroup(2, "Team2Squad_1", "Guard");
	BattleLib.AssignAIGroup(2, "Team2Squad_2", "Guard");
	BattleLib.AssignAIGroup(2, "Team2Squad_3", "Guard");
	BattleLib.AssignAIGroup(2, "Team2Squad_4", "Guard");
	BattleLib.AssignAIGroup(2, "Team2Squad_5", "Guard");
	BattleLib.AssignAIGroup(2, "Team2Squad_6", "Guard");
	BattleLib.AssignAIGroup(2, "Team2Squad_7", "Guard");
	BattleLib.AssignAIGroup(2, "Team2Squad_8", "Guard");
	BattleLib.AssignAIGroup(2, "Team2Squad_9", "Guard");
	BattleLib.AssignAIGroup(2, "Team2Squad_10", "Guard");
	
	--����AI����(team,missionname,missiontype,missiontargetarea)
	BattleLib.CreateAIMission(2, "Attack1", 0, "KeyArea");
	BattleLib.CreateAIMission(3, "Attack2", 0, "KeyArea");
	--ָ��AI��������(team,groupname,missionname)
	BattleLib.AssignAIMission(2,"Guard", "Attack1");

	--��������(missionname,missionstate)
	mainmission = BattleLib.AddPlayerMission("map20mission1",0);
	ScriptCommonLib.SetInt("mainmission", mainmission);
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
		BattleLib.AddGold(500);
		addedgold = ScriptCommonLib.GetInt("addedgold");
		addedgold = addedgold + 500;
		ScriptCommonLib.SetInt("addedgold", addedgold);
		mission1state = ScriptCommonLib.GetInt("mission1state");
		if BattleLib.TeamSquadLeft(2) == 0 and BattleLib.TeamSquadLeft(3) == 0 then
			BattleLib.AddGold(10000);
			BattleLib.DumpSquadData();
			addedgold = ScriptCommonLib.GetInt("addedgold");
			addedgold = addedgold + 10000;
			ScriptCommonLib.SetInt("addedgold", addedgold);
			--ʤ��(storyscript,gold,exp)
			BattleLib.Win("cp40.lua", "0", "0");
		end
	end
end

function turnstart()
	turn = ScriptCommonLib.GetTempInt("turn");
	team = ScriptCommonLib.GetTempInt("team");
	if team == 1 and turn == 1 then
	    BattleLib.Story("cp39_1.lua");
	end
	if team == 1 and turn == 6 then
		MapLib.MoveCameraTo( 1, 5);
		BattleLib.AddBattleSquad("GuardHeavySpear", "Team2Squad_11", 1, 5, 2, 50);
		BattleLib.AddBattleSquad("GuardHeavySpear", "Team2Squad_12", 1, 7, 2, 50);
		BattleLib.AddBattleSquad("GuardCrossBow", "Team2Squad_13", 0, 5, 2, 50);
		BattleLib.AddBattleSquad("GuardCrossBow", "Team2Squad_14", 0, 6, 2, 50);
		BattleLib.AddBattleSquad("GuardCrossBow", "Team2Squad_15", 0, 7, 2, 50);
		BattleLib.AssignAIGroup(2, "Team3Squad_0", "Guard");
		BattleLib.AssignAIGroup(2, "Team3Squad_1", "Guard");
		BattleLib.AssignAIGroup(2, "Team3Squad_2", "Guard");
		BattleLib.AssignAIGroup(2, "Team3Squad_3", "Guard");
		BattleLib.AssignAIGroup(2, "Team3Squad_3", "Guard");
		BattleLib.Story("cp39_2.lua");
		
	end
	
	if team == 1 and turn == 9 then
		MapLib.MoveCameraTo( 5, 13);
		BattleLib.AddBattleSquad("Datura", "Team3Squad_0", 5, 13, 3, 50);
		BattleLib.AddBattleSquad("Syrin", "Team3Squad_1", 5, 14, 3, 50);
		BattleLib.AddBattleSquad("DarkKnight", "Team3Squad_2", 6, 13, 3, 50);
		BattleLib.AddBattleSquad("DarkKnight", "Team3Squad_3", 4, 13, 3, 50);
		BattleLib.AssignAIGroup(3, "Team3Squad_0", "DarkKnight");
		BattleLib.AssignAIGroup(3, "Team3Squad_1", "DarkKnight");
		BattleLib.AssignAIGroup(3, "Team3Squad_2", "DarkKnight");
		BattleLib.AssignAIGroup(3, "Team3Squad_3", "DarkKnight");
		BattleLib.AssignAIMission(3,"DarkKnight", "Attack2");
		BattleLib.Story("cp39_3.lua");
	end
	
	if team == 1 and turn == 13 then
		MapLib.MoveCameraTo( 10, 8);
	    BattleLib.Story("cp39_4.lua");
		BattleLib.CreateMapParticle(10, 8, "mapfire");
		BattleLib.CreateMapParticle(10, 9, "mapfire");
		BattleLib.CreateMapParticle(10, 9, "mapfire");
	end
	
	if team == 1 and turn == 15 then
	    BattleLib.Story("cp39_5.lua");
		BattleLib.RemoveBattleSquad("Dandelion");
		BattleLib.RemoveBattleSquad("Olivia");
		BattleLib.RemoveBattleSquad("Cossa");
		BattleLib.RemoveBattleSquad("Caster");
		BattleLib.RemoveBattleSquad("WingsLightCavalry");
		BattleLib.RemoveBattleSquad("WingsHeavyInf");
		BattleLib.RemoveBattleSquad("WingsCrossBow");
	end
	
	if team == 2 and turn == 15 then
	    BattleLib.Story("cp39_8.lua");
		BattleLib.AddGold(10000);
		BattleLib.DumpSquadData();
		addedgold = ScriptCommonLib.GetInt("addedgold");
		addedgold = addedgold + 10000;
		ScriptCommonLib.SetInt("addedgold", addedgold);
		--ʤ��(storyscript,gold,exp)
		BattleLib.Win("cp40.lua", "0", "0");
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
	faction = SquadLib.GetFaction(squad);
	if area == "KeyArea" then
		if faction == 1 or faction == 2 then
			BattleLib.Story("cp39_6.lua");
			--ʧ��(storyscript,gold,exp)
			BattleLib.Lost("GameOver.lua","0","0");
	end
end