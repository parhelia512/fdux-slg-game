function onaffect()
	local eid = ScriptCommonLib.GetTempString("effectid");
	local sid = ScriptCommonLib.GetTempString("squadid");
	local tid = ScriptCommonLib.GetString("triggerid");
	local tid1 = ScriptCommonLib.GetString("triggerid1");
	local mid = ScriptCommonLib.GetString("modifierid");
	
	if mid ~= "" then
		SquadLib.RemoveModifier(sid, mid);
		ScriptCommonLib.SetString("modifierid", "");
	end
	if tid == "" then
		tid = SquadLib.AddSquadTrigger(sid, "OnMeleeAttack", "onmeleeattack");
		SquadLib.ActiveSquadTrigger(sid, tid);
		ScriptCommonLib.SetString("triggerid", tid);
	end
	if tid1 == "" then
		tid1 = SquadLib.AddSquadTrigger(sid, "AfterMeleeAttack", "aftermeleeattack");
		SquadLib.ActiveSquadTrigger(sid, tid1);
		ScriptCommonLib.SetString("triggerid1", tid1);
	end
	ScriptCommonLib.SetString("effectid", eid);
end

function onremove()
	local sid = ScriptCommonLib.GetTempString("squadid");
	local tid = ScriptCommonLib.GetString("triggerid");
	local tid1 = ScriptCommonLib.GetString("triggerid1");
	if tid ~= "" then
		SquadLib.RemoveSquadTrigger(sid, tid);
	end
	if tid1 ~= "" then
		SquadLib.RemoveSquadTrigger(sid, tid1);
	end
end

function canaffect()
	ScriptCommonLib.SetTempInt("affect", 1);
end

function onmeleeattack()
	local sid = ScriptCommonLib.GetTempString("squadid");
	local tsid = ScriptCommonLib.GetTempString("targetsquadid");
	local asdefender = ScriptCommonLib.GetTempInt("asdefender");
	local mid = ScriptCommonLib.GetString("modifierid");
	local eid = ScriptCommonLib.GetString("effectid");
	if mid ~= "" then
		SquadLib.RemoveModifier(sid, mid);
		ScriptCommonLib.SetString("modifierid", "");
	end
	if asdefender == 0 then
		local croodx, croody = SquadLib.GetSquadCoord(tsid);
		local cavap = MapLib.GetGroundAttr(croodx, croody, 3);
		if cavap < 3 then
			local lv = SquadLib.GetEffectLevel(sid, eid);
			mid = SquadLib.ApplyModifier(sid, 0, 1 + 2 * lv, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
			ScriptCommonLib.SetString("modifierid", mid);
		end
	end
end

function aftermeleeattack()
	local mid = ScriptCommonLib.GetString("modifierid");
	if mid ~= "" then
		SquadLib.RemoveModifier(sid, mid);
		ScriptCommonLib.SetString("modifierid", "");
	end
end
