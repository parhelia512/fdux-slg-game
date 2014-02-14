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
	local lv = SquadLib.GetSkillLevel(sid, "Inspire");
	for i = 1, 5 do
		local tgtsid = BattleLib.GetSquadAt(croodlist[i][1], croodlist[i][2], 1, sf);
		if tgtsid ~= "" then
			local tgtf = SquadLib.GetFaction(tgtsid);
			if tgtf == sf then
				if lv > 1 then
					SquadLib.ApplyEffect(tgtsid, "ImprovedInspired");
				else
					SquadLib.ApplyEffect(tgtsid, "Inspired");
				end
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
		if tgtf == sf then
			ScriptCommonLib.SetTempInt("validaffectarea", 1);
		end
	end
end

function onaffect()
	local sid = ScriptCommonLib.GetTempString("squadid");
	local eid = ScriptCommonLib.GetTempString("effectid");
	local mid = ScriptCommonLib.GetString("modifierid");
	local tid = ScriptCommonLib.GetString("triggerid");
	--pid = ScriptCommonLib.GetString("particleid");
	if mid ~= "" then
		SquadLib.RemoveModifier(sid, mid);
	end
	local lv = SquadLib.GetEffectLevel(sid, eid);
	mid = SquadLib.ApplyModifier(sid, 1, 2, 2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	ScriptCommonLib.SetString("modifierid", mid);
	if tid == "" then
		tid = SquadLib.AddSquadTrigger(sid, "TurnStart", "onturnstart");
		SquadLib.ActiveSquadTrigger(sid, tid);
		ScriptCommonLib.SetString("triggerid", tid);
	end
	ScriptCommonLib.SetString("effectid", eid);
	--if pid == "" then
	--	pid = SquadLib.AddParticle(sid, "mp_spell_effect_06", 3);
	--end
	--ScriptCommonLib.SetString("particleid", pid);
	
	SquadLib.ShowValue1(sid, "Skills_Inspire", 2, 1.0, 1.0, 1.0);
	
	ScriptCommonLib.SetInt("turn", 0);
end

function onremove()
	local sid = ScriptCommonLib.GetTempString("squadid");
	local mid = ScriptCommonLib.GetString("modifierid");
	local tid = ScriptCommonLib.GetString("triggerid");
	--pid = ScriptCommonLib.GetString("particleid");
	if mid ~= "" then
		SquadLib.RemoveModifier(sid, mid);
	end
	if tid ~= "" then
		SquadLib.RemoveSquadTrigger(sid, tid);
	end
	--if pid ~= "" then
	--	SquadLib.DelParticle(sid, pid);
	--end
end

function onturnstart()
	local sid = ScriptCommonLib.GetTempString("squadid");
	local turn = ScriptCommonLib.GetInt("turn");
	if turn == 2 then
		local eid = ScriptCommonLib.GetString("effectid");
		SquadLib.RemoveEffect(sid, eid);
	else
		ScriptCommonLib.SetInt("turn", turn + 1);
	end
end

function canaffect()
	ScriptCommonLib.SetTempInt("affect", 1);
end