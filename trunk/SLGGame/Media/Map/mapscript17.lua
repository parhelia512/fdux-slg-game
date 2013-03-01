function initmap()
	local trigerid = MapLib.AddMapTrigger("FinishDeploy","finishdeploy");
	MapLib.ActiveMapTrigger(trigerid);
	ScriptCommonLib.SetString("finishdeploytriger",trigerid);
	
	ScriptCommonLib.PlayMusic("battle2.ogg");
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
	BattleLib.AssignAIGroup(2, "Team2Squad_0", "WinfredRider");
	BattleLib.AssignAIGroup(2, "Team2Squad_1", "WinfredRider");
	BattleLib.AssignAIGroup(2, "Team2Squad_2", "WinfredRider");
	BattleLib.AssignAIGroup(2, "Team2Squad_3", "WinfredRider");
	BattleLib.AssignAIGroup(2, "Team2Squad_4", "WinfredRider");
	BattleLib.AssignAIGroup(2, "Team2Squad_5", "WinfredRider");
	BattleLib.AssignAIGroup(2, "Team2Squad_6", "WinfredRider");
	BattleLib.AssignAIGroup(2, "Team2Squad_7", "WinfredRider");
	BattleLib.AssignAIGroup(2, "Team2Squad_8", "Winfred");
	BattleLib.AssignAIGroup(2, "Team2Squad_9", "Winfred");
	BattleLib.AssignAIGroup(2, "Team2Squad_10", "Winfred");
	BattleLib.AssignAIGroup(2, "Team2Squad_11", "Winfred");
	BattleLib.AssignAIGroup(2, "Team2Squad_12", "Winfred");
	BattleLib.AssignAIGroup(2, "Team2Squad_13", "Winfred");
	BattleLib.AssignAIGroup(2, "Team2Squad_14", "Winfred");
	BattleLib.AssignAIGroup(2, "Team2Squad_15", "Winfred");
	

	--����AI����(team,missionname,missiontype,missiontargetarea)
	BattleLib.CreateAIMission(2, "Attack1", 0, "DeployArea");
	BattleLib.CreateAIMission(2, "Attack2", 0, "KeyArea1");
	BattleLib.CreateAIMission(2, "Run", 0, "KeyArea2");

	--ָ��AI��������(team,groupname,missionname)
	BattleLib.AssignAIMission(3,"WinfredRider", "Attack1");
	BattleLib.AssignAIMission(3,"Winfred", "Attack2");
	
	--��������(missionname,missionstate)
	local mainmission = BattleLib.AddPlayerMission("map17mission1",0);
	ScriptCommonLib.SetInt("mainmission", mainmission);
	local mission1 = BattleLib.AddPlayerMission("map17mission2",0);
	ScriptCommonLib.SetInt("mission1", mission1);
	ScriptCommonLib.SetInt("mission1state", 0);
	ScriptCommonLib.SetInt("addedgold", 0);
end

--���ӱ����𴥷���
function unitdead()
	local squad = ScriptCommonLib.GetTempString("squadid");
	
	if BattleLib.TeamSquadLeft(1) == 0  then
		--ʧ��(storyscript,gold,exp)
		BattleLib.Lost("GameOver.lua","0","0");
	end
	local faction = SquadLib.GetFaction(squad);
	if faction == 1 then
		BattleLib.AddGold(350);
		local addedgold = ScriptCommonLib.GetInt("addedgold");
		addedgold = addedgold + 350;
		ScriptCommonLib.SetInt("addedgold", addedgold);
		local mission1state = ScriptCommonLib.GetInt("mission1state");
		mission1state = mission1state + 1;
		if mission1state >= 8 then
			local mission1 = ScriptCommonLib.GetInt("mission1");
			--�������(missionindex, missionstate)
			BattleLib.SetPlayerMission(mission1, 1);
		end
		ScriptCommonLib.SetInt("mission1state", mission1state);
		if squad == "Team2Squad_3" then
			BattleLib.Story("cp34_5.lua");
			BattleLib.AddGold(6500);
			BattleLib.DumpSquadData();
			addedgold = ScriptCommonLib.GetInt("addedgold");
			addedgold = addedgold + 6500;
			if mission1state >= 8 then
				BattleLib.AddGold(3900);
				addedgold = addedgold + 3900;
			end
			ScriptCommonLib.SetInt("addedgold", addedgold);
			--ʤ��(storyscript,gold,exp)
			BattleLib.Win("cp35.lua", "0", "0");
		end
	end
end

function turnstart()
	local turn = ScriptCommonLib.GetTempInt("turn");
	local team = ScriptCommonLib.GetTempInt("team");
	if team == 1 and turn == 1 then
	    BattleLib.Story("cp34_1.lua");
	end
end	
		
--�غϽ���������
function turnend()
	local turn = ScriptCommonLib.GetTempInt("turn");
	local team = ScriptCommonLib.GetTempInt("team");

	if team == 1 and turn == 1 then
	    BattleLib.Story("cp34_2.lua");
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
end