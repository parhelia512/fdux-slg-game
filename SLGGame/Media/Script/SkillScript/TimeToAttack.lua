function useskill()
	local sid = ScriptCommonLib.GetTempString("squadid");

	SquadLib.Animation(sid, 1, "Skill", "none", "none", 0, 1);
	
	local ep = 0;
	local tgtx = ScriptCommonLib.GetTempInt("targetx");
	local tgty = ScriptCommonLib.GetTempInt("targety");
	local sf = SquadLib.GetFaction(sid);
	local croodlist = {
		{tgtx, tgty},
		{tgtx - 1, tgty},
		{tgtx, tgty - 1},
		{tgtx + 1, tgty},
		{tgtx, tgty + 1},
	};
	for i = 1, 5 do
		local tgtsid = BattleLib.GetSquadAt(croodlist[i][1], croodlist[i][2], 1, sf);
		if tgtsid ~= "" then
			local tgtf = SquadLib.GetFaction(tgtsid);
			if tgtf == sf then
				SquadLib.ApplyEffect(tgtsid, "TimeToAttack");
				ep = ep + 20;
			end
		end
	end	
	SquadLib.AddExp(sid, ep);
	ScriptCommonLib.SetTempInt("castsuccess", 1);
end

function validarea()
	ScriptCommonLib.SetTempInt("validarea", 1);
end

function validaffectarea()
	local sid = ScriptCommonLib.GetTempString("squadid");
	local tgtx = ScriptCommonLib.GetTempInt("targetx");
	local tgty = ScriptCommonLib.GetTempInt("targety");
	local sf = SquadLib.GetFaction(sid);
	local tgtsid = BattleLib.GetSquadAt(tgtx, tgty, 1, sf);
	if tgtsid ~= "" then
		local tgtf = SquadLib.GetFaction(tgtsid);
		if tgtf ~= sf then
			ScriptCommonLib.SetTempInt("validaffectarea", 0);
		else
			ScriptCommonLib.SetTempInt("validaffectarea", 1);
		end
	else
		ScriptCommonLib.SetTempInt("validaffectarea", 1);
	end
end

function onaffect()
	local sid = ScriptCommonLib.GetTempString("squadid");
	local eid = ScriptCommonLib.GetTempString("effectid");
	local mid = ScriptCommonLib.GetString("modifierid");
	local tid = ScriptCommonLib.GetString("triggerid");
	if mid ~= "" then
		SquadLib.RemoveModifier(sid, mid);
	end
	mid = SquadLib.ApplyModifier(sid, 1, 0.0, 0.0, 0.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	ScriptCommonLib.SetString("modifierid", mid);
	if tid == "" then
		tid = SquadLib.AddSquadTrigger(sid, "TurnEnd", "onturnend");
		SquadLib.ActiveSquadTrigger(sid, tid);
		ScriptCommonLib.SetString("triggerid", tid);
	end
	ScriptCommonLib.SetString("effectid", eid);
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
	local eid = ScriptCommonLib.GetString("effectid");
	SquadLib.RemoveEffect(sid, eid);
end

function canaffect()
	ScriptCommonLib.SetTempInt("affect", 1);
end