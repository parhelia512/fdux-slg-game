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
	--创建部队被歼灭触发器
	trigerid = MapLib.AddMapTrigger("SquadAnnihilated","unitdead");
	MapLib.ActiveMapTrigger(trigerid);
	ScriptCommonLib.SetString("unitdeadtriger",trigerid);
	--创建回合结束触发器
	trigerid = MapLib.AddMapTrigger("TurnEnd","turnend");
	MapLib.ActiveMapTrigger(trigerid);
	ScriptCommonLib.SetString("turnendtriger",trigerid);
	--创建回合开始触发器
	trigerid = MapLib.AddMapTrigger("TurnStart","turnstart");
	MapLib.ActiveMapTrigger(trigerid);
	ScriptCommonLib.SetString("turnstarttriger",trigerid);
	--创建进入区域触发器
	trigerid = MapLib.AddMapTrigger("GetOutArea","outarea");
	MapLib.ActiveMapTrigger(trigerid);
	ScriptCommonLib.SetString("outareatriger",trigerid);
	--创建离开区域触发器
	trigerid = MapLib.AddMapTrigger("GetInArea","inarea");
	MapLib.ActiveMapTrigger(trigerid);
	ScriptCommonLib.SetString("inareatriger",trigerid);
	
	--创建AI分组(team,squadid,groupname)
	BattleLib.AssignAIGroup(2, "Team2Squad_0", "Noth");
	BattleLib.AssignAIGroup(2, "Team2Squad_1", "Noth");
	BattleLib.AssignAIGroup(2, "Team2Squad_2", "Noth1");
	BattleLib.AssignAIGroup(2, "Team2Squad_3", "Noth");
	BattleLib.AssignAIGroup(2, "Team2Squad_4", "Noth");
	BattleLib.AssignAIGroup(2, "Team2Squad_5", "Noth");
	BattleLib.AssignAIGroup(2, "Team2Squad_6", "Noth");
	BattleLib.AssignAIGroup(2, "Team2Squad_7", "Noth1");
	BattleLib.AssignAIGroup(2, "Team2Squad_8", "Noth1");
	BattleLib.AssignAIGroup(2, "Team2Squad_9", "Noth");
	BattleLib.AssignAIGroup(2, "Team2Squad_10", "Noth");
	--创建AI任务(team,missionname,missiontype,missiontargetarea)
	BattleLib.CreateAIMission(2, "Defend1", 0, "KeyArea1");
	BattleLib.CreateAIMission(2, "Defend2", 0, "KeyArea2");
	--指派AI分组任务(team,groupname,missionname)
	BattleLib.AssignAIMission(2,"Noth1", "Defend1");
	BattleLib.AssignAIMission(2,"Noth", "Defend2");
	
	--创建任务(missionname,missionstate)
	local mainmission = BattleLib.AddPlayerMission("map12mission1",0);
	ScriptCommonLib.SetInt("mainmission", mainmission);
	local mission1 = BattleLib.AddPlayerMission("map12mission2",0);
	ScriptCommonLib.SetInt("mission1", mission1);
	ScriptCommonLib.SetInt("mission1state", 1);
	ScriptCommonLib.SetInt("win", 0);
	ScriptCommonLib.SetInt("addedgold", 0);
end

--部队被歼灭触发器
function unitdead()
	local squad = ScriptCommonLib.GetTempString("squadid");
	local faction = SquadLib.GetFaction(squad);
	
	if BattleLib.TeamSquadLeft(1) == 0  then
		--失败(storyscript,gold,exp)
		BattleLib.Lost("GameOver.lua", "0");
	end
	if faction == 1 then
		BattleLib.AddGold(200);
		local addedgold = ScriptCommonLib.GetInt("addedgold");
		addedgold = addedgold + 200;
		ScriptCommonLib.SetInt("addedgold", addedgold);
	end
end

function turnstart()
	local turn = ScriptCommonLib.GetTempInt("turn");
	local team = ScriptCommonLib.GetTempInt("team");
	local win = ScriptCommonLib.GetInt("win");
		
	if team == 1 and turn == 1 then
		MapLib.MoveCameraTo( 0, 14);
	    BattleLib.Story("cp24_1.lua");
	end
	if win == 1  then
		ScriptCommonLib.SetInt("win", 2);
	end
end	
		
--回合结束触发器
function turnend()
	local turn = ScriptCommonLib.GetTempInt("turn");
	local team = ScriptCommonLib.GetTempInt("team");
	local win = ScriptCommonLib.GetInt("win");
	
	ScriptCommonLib.Log("MAPLOG turn"..turn.." team"..team.." win"..win);
	
	if team == 1 and turn >= 8 and win == 0 then
		local mission1state = ScriptCommonLib.GetInt("mission1state");
		if mission1state == 1 then
			local mission1 = ScriptCommonLib.GetInt("mission1");
			--完成任务(missionindex, missionstate)
			BattleLib.SetPlayerMission(mission1, 2);
			ScriptCommonLib.SetInt("mission1state", 0);
		end
	end
	
	if team == 1 and win == 2 then
		BattleLib.Story("cp24_5.lua");
		BattleLib.AddGold(4800);
		BattleLib.DumpSquadData();
		local addedgold = ScriptCommonLib.GetInt("addedgold");
		addedgold = addedgold + 4800;
		local mission1state = ScriptCommonLib.GetInt("mission1state");
		if mission1state == 1 then
			BattleLib.AddGold(2400);
			addedgold = addedgold + 2400;
		end
		ScriptCommonLib.SetInt("addedgold", addedgold);
		--胜利(storyscript,gold,exp)
		BattleLib.Win("cp25.lua", addedgold);
	end
end

--离开区域触发器
function outarea()
	local squad = ScriptCommonLib.GetTempString("squadid");
	local area = ScriptCommonLib.GetTempString("areaid");
	
	if area == "TargetArea" then
		local faction = SquadLib.GetFaction(squad);
		ScriptCommonLib.Log("MAPLOG outarea faction"..faction);
	end
end

--进入区域触发器
function inarea()
	local squad = ScriptCommonLib.GetTempString("squadid");
	local area = ScriptCommonLib.GetTempString("areaid");
	
	if area == "TargetArea" then
		local faction = SquadLib.GetFaction(squad);
		ScriptCommonLib.Log("MAPLOG inarea faction"..faction);
		if faction == 0 then
			MapLib.MoveCameraTo( 9, 18);
			ScriptCommonLib.Log("MAPLOG win1");
			ScriptCommonLib.SetInt("win", 1);
			local mission1 = ScriptCommonLib.GetInt("mission1");
			--完成任务(missionindex, missionstate)
			BattleLib.SetPlayerMission(mission1, 1);
			BattleLib.InterruptMove();
			BattleLib.AddBattleSquad("Nothfate", "Team2Squad_11", 9, 18, 2, 50);
			BattleLib.AddBattleSquad("NothHeavyHSword", "Team2Squad_12", 8, 19, 2, 50);
			BattleLib.AddBattleSquad("NothHeavyHSword", "Team2Squad_13", 10, 19, 2, 50);
			BattleLib.AddBattleSquad("NothHeavyHSword", "Team2Squad_14", 1, 12, 2, 50);
			BattleLib.AddBattleSquad("NothHeavyHSword", "Team2Squad_15", 1, 14, 2, 50);
			BattleLib.AddBattleSquad("NothHeavyCrossBow", "Team2Squad_16", 9, 19, 2, 50);
			BattleLib.AddBattleSquad("NothHeavyCrossBow", "Team2Squad_17", 0, 12, 2, 50);
			BattleLib.AddBattleSquad("NothHeavyCrossBow", "Team2Squad_18", 0, 13, 2, 50);
			BattleLib.AddBattleSquad("NothHeavyCrossBow", "Team2Squad_19", 0, 14, 2, 50);
			BattleLib.AssignAIGroup(2, "Team2Squad_11", "Noth");
			BattleLib.AssignAIGroup(2, "Team2Squad_12", "Noth");
			BattleLib.AssignAIGroup(2, "Team2Squad_13", "Noth");
			BattleLib.AssignAIGroup(2, "Team2Squad_14", "Noth");
			BattleLib.AssignAIGroup(2, "Team2Squad_15", "Noth");
			BattleLib.AssignAIGroup(2, "Team2Squad_16", "Noth");
			BattleLib.AssignAIGroup(2, "Team2Squad_17", "Noth");
			BattleLib.AssignAIGroup(2, "Team2Squad_18", "Noth");
			BattleLib.AssignAIGroup(2, "Team2Squad_19", "Noth");
			BattleLib.Story("cp24_2.lua");
		end
	end
end