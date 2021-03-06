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
	
	--创建AI任务(team,missionname,missiontype,missiontargetarea)
	BattleLib.CreateAIMission(2, "Attack1", 0, "KeyArea");
	BattleLib.CreateAIMission(3, "Attack2", 0, "KeyArea");
	--指派AI分组任务(team,groupname,missionname)
	BattleLib.AssignAIMission(2,"Guard", "Attack1");

	--创建任务(missionname,missionstate)
	local mainmission = BattleLib.AddPlayerMission("map20mission1",0);
	ScriptCommonLib.SetInt("mainmission", mainmission);
	ScriptCommonLib.SetInt("addedgold", 0);
end

--部队被歼灭触发器
function unitdead()
	local squad = ScriptCommonLib.GetTempString("squadid");
	
	if BattleLib.TeamSquadLeft(1) == 0  then
		--失败(storyscript,gold,exp)
		BattleLib.Lost("GameOver.lua", "0");
	end
	local faction = SquadLib.GetFaction(squad);
	if faction == 1 then
		BattleLib.AddGold(500);
		local addedgold = ScriptCommonLib.GetInt("addedgold");
		addedgold = addedgold + 500;
		ScriptCommonLib.SetInt("addedgold", addedgold);
		local mission1state = ScriptCommonLib.GetInt("mission1state");
		if BattleLib.TeamSquadLeft(2) == 0 and BattleLib.TeamSquadLeft(3) == 0 then
			BattleLib.AddGold(10000);
			BattleLib.DumpSquadData();
			addedgold = ScriptCommonLib.GetInt("addedgold");
			addedgold = addedgold + 10000;
			ScriptCommonLib.SetInt("addedgold", addedgold);
			--胜利(storyscript,gold,exp)
			BattleLib.Win("cp40.lua", addedgold);
		end
	end
end

function turnstart()
	local turn = ScriptCommonLib.GetTempInt("turn");
	local team = ScriptCommonLib.GetTempInt("team");
	if team == 1 and turn == 1 then
		MapLib.MoveCameraTo( 1, 5);
	    BattleLib.Story("cp39_1.lua");
	end
	if team == 1 and turn == 3 then
		MapLib.MoveCameraTo( 1, 5);
		BattleLib.AddBattleSquad("GuardHeavySpear", "Team2Squad_11", 1, 5, 2, 50);
		BattleLib.AddBattleSquad("GuardHeavySpear", "Team2Squad_12", 1, 7, 2, 50);
		BattleLib.AddBattleSquad("GuardCrossBow", "Team2Squad_13", 0, 5, 2, 50);
		BattleLib.AddBattleSquad("GuardCrossBow", "Team2Squad_14", 0, 6, 2, 50);
		BattleLib.AddBattleSquad("GuardCrossBow", "Team2Squad_15", 0, 7, 2, 50);
		BattleLib.AddBattleSquad("GuardHeavySpear", "Team2Squad_16", 1, 6, 2, 50);
		BattleLib.AddBattleSquad("GuardHeavySpear", "Team2Squad_17", 1, 4, 2, 50);
		BattleLib.AssignAIGroup(2, "Team2Squad_11", "Guard");
		BattleLib.AssignAIGroup(2, "Team2Squad_12", "Guard");
		BattleLib.AssignAIGroup(2, "Team2Squad_13", "Guard");
		BattleLib.AssignAIGroup(2, "Team2Squad_14", "Guard");
		BattleLib.AssignAIGroup(2, "Team2Squad_15", "Guard");
		BattleLib.AssignAIGroup(2, "Team2Squad_16", "Guard");
		BattleLib.AssignAIGroup(2, "Team2Squad_17", "Guard");
		BattleLib.Story("cp39_2.lua");		
	end
	
	if team == 1 and turn == 5 then
		MapLib.MoveCameraTo( 5, 13);
		BattleLib.AddBattleSquad("Datura", "Team3Squad_0", 5, 13, 3, 50);
		BattleLib.AddBattleSquad("Syrin", "Team3Squad_1", 5, 14, 3, 20);
		BattleLib.AddBattleSquad("DarkKnight", "Team3Squad_2", 6, 13, 3, 50);
		BattleLib.AddBattleSquad("DarkKnight", "Team3Squad_3", 4, 13, 3, 50);
		BattleLib.AssignAIGroup(3, "Team3Squad_0", "DarkKnight");
		BattleLib.AssignAIGroup(3, "Team3Squad_1", "DarkKnight");
		BattleLib.AssignAIGroup(3, "Team3Squad_2", "DarkKnight");
		BattleLib.AssignAIGroup(3, "Team3Squad_3", "DarkKnight");
		BattleLib.AssignAIMission(3,"DarkKnight", "Attack2");
		BattleLib.Story("cp39_3.lua");
	end
	
	if team == 1 and turn == 8 then
		MapLib.MoveCameraTo( 10, 8);
	    BattleLib.Story("cp39_4.lua");
		BattleLib.CreateMapParticle(10, 8, "mapfire");
		BattleLib.CreateMapParticle(10, 9, "mapfire");
		BattleLib.CreateMapParticle(10, 9, "mapfire");
	end
	
	if team == 1 and turn == 9 then
	    BattleLib.Story("cp39_5.lua");
	end
	
	if team == 2 and turn == 9 then
	    BattleLib.Story("cp39_8.lua");
		BattleLib.AddGold(20000);
		BattleLib.DumpSquadData();
		local addedgold = ScriptCommonLib.GetInt("addedgold");
		addedgold = addedgold + 20000;
		ScriptCommonLib.SetInt("addedgold", addedgold);
		--胜利(storyscript,gold,exp)
		BattleLib.Win("cp40.lua", addedgold);
	end
end	
		
--回合结束触发器
function turnend()
	local turn = ScriptCommonLib.GetTempInt("turn");
	local team = ScriptCommonLib.GetTempInt("team");
end

--离开区域触发器
function outarea()
	local squad = ScriptCommonLib.GetTempString("squadid");
	local area = ScriptCommonLib.GetTempString("areaid");
end

--进入区域触发器
function inarea()
	local squad = ScriptCommonLib.GetTempString("squadid");
	local area = ScriptCommonLib.GetTempString("areaid");
	local faction = SquadLib.GetFaction(squad);
	if area == "KeyArea" then
		if faction == 1 or faction == 2 then
			BattleLib.Story("cp39_6.lua");
			--失败(storyscript,gold,exp)
			BattleLib.Lost("GameOver.lua","0","0");
		end
	end
end