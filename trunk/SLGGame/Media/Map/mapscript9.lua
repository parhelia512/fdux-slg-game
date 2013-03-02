function initmap()
	local trigerid = MapLib.AddMapTrigger("FinishDeploy","finishdeploy");
	MapLib.ActiveMapTrigger(trigerid);
	ScriptCommonLib.SetString("finishdeploytriger",trigerid);
	
	ScriptCommonLib.PlayMusic("battle3.ogg");
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
	BattleLib.AssignAIGroup(2, "Team2Squad_0", "Noth");
	BattleLib.AssignAIGroup(2, "Team2Squad_1", "Noth");
	BattleLib.AssignAIGroup(2, "Team2Squad_2", "Noth");
	BattleLib.AssignAIGroup(2, "Team2Squad_3", "Noth");
	BattleLib.AssignAIGroup(2, "Team2Squad_4", "Noth");
	BattleLib.AssignAIGroup(2, "Team2Squad_5", "Noth");
	BattleLib.AssignAIGroup(2, "Team2Squad_6", "Noth");
	BattleLib.AssignAIGroup(2, "Team2Squad_7", "Noth");
	BattleLib.AssignAIGroup(2, "Team2Squad_8", "Noth");
	--����AI����(team,missionname,missiontype,missiontargetarea)
	BattleLib.CreateAIMission(2, "Attack", 0, "Moontown");
	--ָ��AI��������(team,groupname,missionname)
	BattleLib.AssignAIMission(2, "Noth", "Attack");
	
	--��������(missionname,missionstate)
	local mainmission = BattleLib.AddPlayerMission("map9mission1",0);
	ScriptCommonLib.SetInt("mainmission", mainmission);
	local mission1 = BattleLib.AddPlayerMission("map9mission2",0);
	local mission2 = BattleLib.AddPlayerMission("map9mission3",0);
	ScriptCommonLib.SetInt("mission1", mission1);
	ScriptCommonLib.SetInt("mission2", mission2);
	ScriptCommonLib.SetInt("mission1state", 0);
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
		BattleLib.AddGold(140);
		local addedgold = ScriptCommonLib.GetInt("addedgold");
		addedgold = addedgold + 140;
		ScriptCommonLib.SetInt("addedgold", addedgold);
	if BattleLib.TeamSquadLeft(2) == 0  then
		local mission1 = ScriptCommonLib.GetInt("mission1");
		ScriptCommonLib.SetInt("mission1state", 1);
		BattleLib.SetPlayerMission(mission1, 1);
		BattleLib.AddGold(3000);
		BattleLib.DumpSquadData();
		local addedgold = ScriptCommonLib.GetInt("addedgold");
		addedgold = addedgold + 3000;
		local mission1state = ScriptCommonLib.GetInt("mission1state");
		local mission2state = ScriptCommonLib.GetInt("mission2state");
		if mission1state == 1 then
			BattleLib.AddGold(1000);
			addedgold = addedgold + 1000;
		end
		if mission2state == 1 then
			BattleLib.AddGold(1000);
			addedgold = addedgold + 1000;
		end
			ScriptCommonLib.SetInt("addedgold", addedgold);
			--ʤ��(storyscript,gold,exp)
			BattleLib.Win("cp20.lua",  addedgold);
		end
	end
end

function turnstart()
	turn = ScriptCommonLib.GetTempInt("turn");
	team = ScriptCommonLib.GetTempInt("team");
end	
		
--�غϽ���������
function turnend()
	turn = ScriptCommonLib.GetTempInt("turn");
	team = ScriptCommonLib.GetTempInt("team");
	
	if team == 1 and turn >= 15 then
		BattleLib.AddGold(3000);
		BattleLib.DumpSquadData();
		local addedgold = ScriptCommonLib.GetInt("addedgold");
		addedgold = addedgold + 3000;
		local mission1state = ScriptCommonLib.GetInt("mission1state");
		local mission2state = ScriptCommonLib.GetInt("mission2state");
		if mission1state == 1 then
			BattleLib.AddGold(1000);
			addedgold = addedgold + 1000;
		end
		if mission2state == 1 then
			BattleLib.AddGold(1000);
			addedgold = addedgold + 1000;
		end
		ScriptCommonLib.SetInt("addedgold", addedgold);
		--ʤ��(storyscript,gold,exp)
		BattleLib.Win("cp20.lua",  addedgold);
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
	
		
	if area == "Moontown" then
		local faction = SquadLib.GetFaction(squad);
		if faction == 1 then
			local mission2state = ScriptCommonLib.GetInt("mission2state");
			if mission2state == 1 then
				local mission2 = ScriptCommonLib.GetInt("mission2");
				--����ɹ�(missionindex, missionstate)
				BattleLib.SetPlayerMission(mission2, 2);
				ScriptCommonLib.SetInt("mission2state", 0);
			end
		end
	end
end