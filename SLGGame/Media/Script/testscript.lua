function initmap()
	trigerid = TriggerLib.AddTrigger("FinishDeploy","finishdeploy");
	TriggerLib.ActiveTrigger(trigerid);
	ScriptCommonLib.SetString("finishdeploytriger",trigerid);
	ScriptCommonLib.PlayMusic("battle1.wav");
end

function finishdeploy()
	trigerid = ScriptCommonLib.GetString("finishdeploytriger");
	TriggerLib.DisableTrigger(trigerid);
	trigerid = TriggerLib.AddTrigger("UnitDead","unitdead");
	TriggerLib.ActiveTrigger(trigerid);
	ScriptCommonLib.SetString("unitdeadtriger",trigerid);
	trigerid = TriggerLib.AddTrigger("TurnEnd","turnend");
	TriggerLib.ActiveTrigger(trigerid);
	ScriptCommonLib.SetString("turnendtriger",trigerid);
end

function unitdead()
	
end

function turnend()
	turn = ScriptCommonLib.GetInt("trun");
	team = ScriptCommonLib.GetInt("team");
end