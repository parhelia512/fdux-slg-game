function onaffect()
	local sid = ScriptCommonLib.GetTempString("squadid");
	local eid = ScriptCommonLib.GetTempString("effectid");
	local mid = ScriptCommonLib.GetString("modifierid");
	local tid = ScriptCommonLib.GetString("triggerid");

	if mid ~= "" then
		SquadLib.RemoveModifier(sid, mid);
	end
	
	local def = 0;
	local armor = SquadLib.GetEquip(sid, 2);
	if armor ~= "none" then
		local attr = SquadLib.GetEquipAttr(2, armor, 2);
		local def = 0;
		if attr > 2 then
			def = (attr - 2) / 2;
		end
		if def > 4 then
			def = 4;
		end
	end
	
	mid = SquadLib.ApplyModifier(sid, 2, 0.0, 0.0, -def, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	ScriptCommonLib.SetString("modifierid", mid);
	if tid == "" then
		tid = SquadLib.AddSquadTrigger(sid, "TurnEnd", "onturnend");
		SquadLib.ActiveSquadTrigger(sid, tid);
		ScriptCommonLib.SetString("triggerid", tid);
	end
	ScriptCommonLib.SetString("effectid", eid);

	ScriptCommonLib.SetInt("turn", 0);
end

function onremove()
	local sid = ScriptCommonLib.GetTempString("squadid");
	local mid = ScriptCommonLib.GetString("modifierid");
	local tid = ScriptCommonLib.GetString("triggerid");
	if mid ~= "" then
		SquadLib.RemoveModifier(sid, mid);
	end
	if tid ~= "" then
		SquadLib.RemoveSquadTrigger(sid, tid);
	end
end

function onturnend()
	local sid = ScriptCommonLib.GetTempString("squadid");
	local turn = ScriptCommonLib.GetInt("turn");
	if turn == 1 then
		local eid = ScriptCommonLib.GetString("effectid");
		SquadLib.RemoveEffect(sid, eid);
	else
		ScriptCommonLib.SetInt("turn", turn + 1);
	end
end

function canaffect()
	ScriptCommonLib.SetTempInt("affect", 1);
end