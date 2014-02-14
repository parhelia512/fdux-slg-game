function onaffect()
	local tid = ScriptCommonLib.GetString("triggerid");
	local tid1 = ScriptCommonLib.GetString("triggerid1");
	local sid = ScriptCommonLib.GetTempString("squadid");
	local eid = ScriptCommonLib.GetTempString("effectid");
	if tid == "" then
		tid = SquadLib.AddSquadTrigger(sid, "OnCharge", "oncharge");
		SquadLib.ActiveSquadTrigger(sid, tid);
		ScriptCommonLib.SetString("triggerid", tid)
	end
	if tid1 == "" then
		tid1 = SquadLib.AddSquadTrigger(sid, "AfterCharge", "aftercharge");
		SquadLib.ActiveSquadTrigger(sid, tid1);
		ScriptCommonLib.SetString("triggerid1", tid1)
	end
	ScriptCommonLib.SetString("effectid", eid);
end

function onremove()
	local sid = ScriptCommonLib.GetTempString("squadid");
	local tid = ScriptCommonLib.GetString("triggerid");
	local tid1 = ScriptCommonLib.GetString("triggerid1");
	local mid = ScriptCommonLib.GetString("modifierid");
	if tid ~= "" then
		SquadLib.RemoveSquadTrigger(sid, tid);
	end
	if tid1 ~= "" then
		SquadLib.RemoveSquadTrigger(sid, tid1);
	end
	if mid ~= "" then
		SquadLib.RemoveModifier(sid, mid);
	end
end

function canaffect()
	ScriptCommonLib.SetTempInt("affect", 1);
end

function oncharge()
	local mid = ScriptCommonLib.GetString("modifierid");
	local sid = ScriptCommonLib.GetTempString("squadid");
	local eid = ScriptCommonLib.GetString("effectid");
	if mid ~= "" then
		SquadLib.RemoveModifier(sid, mid);
	end
	
	ScriptCommonLib.Log("SKILLLOG Charge"..sid);
	
	local lv = SquadLib.GetEffectLevel(sid, eid);
	mid = SquadLib.ApplyModifier(sid, 0, 1 + lv, 0.0, 0.0, lv, lv, 0.0, 0.0, 0.0, 0.0, 0.0);
	ScriptCommonLib.SetString("modifierid", mid);
	SquadLib.ShowValue(sid, "Skills_Charge", 1.0, 1.0, 1.0);
end

function aftercharge()
	local mid = ScriptCommonLib.GetString("modifierid");
	local sid = ScriptCommonLib.GetTempString("squadid");
	if mid ~= "" then
		SquadLib.RemoveModifier(sid, mid);
		ScriptCommonLib.SetString("modifierid", "");
	end
end