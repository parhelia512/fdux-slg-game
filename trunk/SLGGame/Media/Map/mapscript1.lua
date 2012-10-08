function initmap()
	trigerid = MapLib.AddMapTrigger("FinishDeploy","finishdeploy");
	MapLib.ActiveMapTrigger(trigerid);
	ScriptCommonLib.SetString("finishdeploytriger",trigerid);
	ScriptCommonLib.PlayMusic("battle1.mp3");
	MapLib.SetCamera(6,2);
end

function finishdeploy()
	trigerid = ScriptCommonLib.GetString("finishdeploytriger");
	MapLib.RemoveMapTrigger(trigerid);
	trigerid = MapLib.AddMapTrigger("UnitDead","unitdead");
	MapLib.ActiveMapTrigger(trigerid);
	ScriptCommonLib.SetString("unitdeadtriger",trigerid);
	trigerid = MapLib.AddMapTrigger("TurnEnd","turnend");
	MapLib.ActiveMapTrigger(trigerid);
	ScriptCommonLib.SetString("turnendtriger",trigerid);
end

function unitdead()
	squad = ScriptCommonLib.GetTempString("squad");
	if squad == "Cheetah" or squad == "Dandelion" or squad == "Olivia" then
		BattleLib.ChangeState(1,"GameOver.lua");
	elseif BattleLib.TeamSquadLeft(2) == 0 then
		BattleLib.DumpSquadData();
		BattleLib.ChangeState(1,"Chapter3.lua");
	end
end

function turnend()
	turn = ScriptCommonLib.GetTempInt("trun");
	team = ScriptCommonLib.GetTempInt("team");
end