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
	BattleLib.AssignAIGroup(2, "Team2Squad_0", "Unknow");
	BattleLib.AssignAIGroup(2, "Team2Squad_1", "Unknow");
	BattleLib.AssignAIGroup(2, "Team2Squad_2", "Unknow");
	BattleLib.AssignAIGroup(2, "Team2Squad_3", "Unknow");
	BattleLib.AssignAIGroup(2, "Team2Squad_4", "Unknow");
	BattleLib.AssignAIGroup(2, "Team2Squad_5", "Unknow");
	--����AI����(team,missionname,missiontype,missiontargetarea)
	BattleLib.CreateAIMission(2, "Defend", 0, "KeyArea");
	--ָ��AI��������(team,groupname,missionname)
	BattleLib.AssignAIMission(2, "Unknow", "Defend");
	
	--��������(missionname,missionstate)
	local mainmission = BattleLib.AddPlayerMission("map4mission1",0);
	ScriptCommonLib.SetInt("mainmission", mainmission);
	local mission1 = BattleLib.AddPlayerMission("map4mission2",0);
	local mission2 = BattleLib.AddPlayerMission("map4mission3",0);
	ScriptCommonLib.SetInt("mission1", mission1);
	ScriptCommonLib.SetInt("mission2", mission2);
	ScriptCommonLib.SetInt("mission1state", 1);
	ScriptCommonLib.SetInt("mission2state", 1);
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
		BattleLib.AddGold(120);
		local addedgold = ScriptCommonLib.GetInt("addedgold");
		addedgold = addedgold + 120;
		ScriptCommonLib.SetInt("addedgold", addedgold);
		if BattleLib.TeamSquadLeft(2) == 0 then
			BattleLib.AddGold(2150);
			BattleLib.DumpSquadData();
			addedgold = ScriptCommonLib.GetInt("addedgold");
			addedgold = addedgold + 2150;
			local mission1state = ScriptCommonLib.GetInt("mission1state");
			local mission2state = ScriptCommonLib.GetInt("mission2state");
			if mission1state == 1 then
				BattleLib.AddGold(800);
				addedgold = addedgold + 800;
			end
			if mission2state == 1 then
				BattleLib.AddGold(800);
				addedgold = addedgold + 800;
			end
			ScriptCommonLib.SetInt("addedgold", addedgold);
			--ʤ��(storyscript,gold,exp)
			BattleLib.Win("cp9.lua", addedgold);
		end
	end
	if faction == 0 then
        ScriptCommonLib.SetInt("mission1state", 0);
		local mission1 = ScriptCommonLib.GetInt("mission1");
		BattleLib.SetPlayerMission(mission1, 2);
	end
end

function turnstart()
	local turn = ScriptCommonLib.GetTempInt("turn");
	local team = ScriptCommonLib.GetTempInt("team");
	if team == 1 and turn == 1 then
	    BattleLib.Story("cp8_1.lua");
	end
end	
		
--�غϽ���������
function turnend()
	local turn = ScriptCommonLib.GetTempInt("turn");
	local team = ScriptCommonLib.GetTempInt("team");
	
	if team == 1 and BattleLib.TeamSquadLeft(2) == 0 then
		BattleLib.DumpSquadData();
		local addedgold = ScriptCommonLib.GetInt("addedgold");
		--ʤ��(storyscript,gold,exp)
		BattleLib.Win("cp9.lua", addedgold);
	end
	if team == 1 and turn > 7 then
	    local mission2 = ScriptCommonLib.GetInt("mission2");
		BattleLib.SetPlayerMission(mission2, 2);
	    ScriptCommonLib.SetInt("mission2state", 0);
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