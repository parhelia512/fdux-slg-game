function useskill()
	local caster  = ScriptCommonLib.GetTempString("squadid");
	local eid = SquadLib.ApplyEffect(caster, "Defence");
	ScriptCommonLib.SetTempInt("castsuccess", 1);
end

function onaffect()
	local mid = ScriptCommonLib.GetString("modifierid");
	local sid = ScriptCommonLib.GetTempString("squadid");
	local eid = ScriptCommonLib.GetTempString("effectid");
	local tid = ScriptCommonLib.GetString("triggerid");
	local tid1 = ScriptCommonLib.GetString("triggerid1");
	local tid2 = ScriptCommonLib.GetString("triggerid2");
	local tid3 = ScriptCommonLib.GetString("triggerid3");
	if mid ~= "" then
		SquadLib.RemoveModifier(sid, mid);
	end
	local lv = SquadLib.GetSkillLevel(sid, "Defence");
	local ap = SquadLib.GetActionPoint(sid);
	local defence = lv + (ap - 2);
	if defence > 2 + lv * 2 then
		defence = 2 + lv * 2;
	end
	mid = SquadLib.ApplyModifier(sid, 0, 0.0, 0.0, defence, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	ScriptCommonLib.Log("SKILLLOG Defence"..sid.." defence+"..defence);
	ScriptCommonLib.SetString("modifierid", mid);
	if tid == "" then
		tid = SquadLib.AddSquadTrigger(sid, "TurnStart", "onturnstart");
		SquadLib.ActiveSquadTrigger(sid, tid);
		ScriptCommonLib.SetString("triggerid", tid);
	end
	if tid1 == "" then
		tid1 = SquadLib.AddSquadTrigger(sid, "OnUnderRangedAttack", "onunderrangedattack");
		SquadLib.ActiveSquadTrigger(sid, tid1);
		ScriptCommonLib.SetString("triggerid1", tid1);
	end
	if tid2 == "" then
		tid2 = SquadLib.AddSquadTrigger(sid, "OnUnderMagicAttack", "onundermagicattack");
		SquadLib.ActiveSquadTrigger(sid, tid2);
		ScriptCommonLib.SetString("triggerid2", tid2);
	end
	if tid3 == "" then
		tid3 = SquadLib.AddSquadTrigger(sid, "OnMeleeAttack", "onmeleeattack");
		SquadLib.ActiveSquadTrigger(sid, tid3);
		ScriptCommonLib.SetString("triggerid3", tid3);
	end
	ScriptCommonLib.SetString("effectid", eid);
	
	SquadLib.ShowValue1(sid, "Skills_DefendEffect", defence, 1.0, 1.0, 1.0);
end

function onremove()
	local mid = ScriptCommonLib.GetString("modifierid");
	local sid = ScriptCommonLib.GetTempString("squadid");
	local tid = ScriptCommonLib.GetString("triggerid");
	local tid1 = ScriptCommonLib.GetString("triggerid1");
	local tid2 = ScriptCommonLib.GetString("triggerid2");
	local tid3 = ScriptCommonLib.GetString("triggerid3");
	if mid ~= "" then
		SquadLib.RemoveModifier(sid, mid);
	end
	if tid ~= "" then
		SquadLib.RemoveSquadTrigger(sid, tid);
	end
	if tid1 ~= "" then
		SquadLib.RemoveSquadTrigger(sid, tid1);
	end
	if tid2 ~= "" then
		SquadLib.RemoveSquadTrigger(sid, tid2);
	end
	if tid3 ~= "" then
		SquadLib.RemoveSquadTrigger(sid, tid3);
	end
end

function onturnstart()
	local sid = ScriptCommonLib.GetTempString("squadid");
	local eid = ScriptCommonLib.GetString("effectid");
	SquadLib.RemoveEffect(sid, eid);
end

function onunderrangedattack()
	local sid = ScriptCommonLib.GetTempString("squadid");
	local tsid = ScriptCommonLib.GetTempString("targetsquadid");
	local casterlv = SquadLib.GetSquadLevel(sid);
	local targetlv = SquadLib.GetSquadLevel(tsid);
	local ep = 25;
	if targetlv > casterlv then
		ep = ep + (targetlv - casterlv) * 3;
	end
	SquadLib.AddExp(sid, ep);	
end

function onundermagicattack()
	local sid = ScriptCommonLib.GetTempString("squadid");
	local tsid = ScriptCommonLib.GetTempString("targetsquadid");
	local casterlv = SquadLib.GetSquadLevel(sid);
	local targetlv = SquadLib.GetSquadLevel(tsid);
	local ep = 25;
	if targetlv > casterlv then
		ep = ep + (targetlv - casterlv) * 3;
	end
	SquadLib.AddExp(sid, ep);	
end

function onmeleeattack()
	local sid = ScriptCommonLib.GetTempString("squadid");
	local tsid = ScriptCommonLib.GetTempString("targetsquadid");
	local isdefender = ScriptCommonLib.GetTempInt("asdefender");
	if isdefender == 1 then
		local casterlv = SquadLib.GetSquadLevel(sid);
		local targetlv = SquadLib.GetSquadLevel(tsid);
		local ep = 25;
		if targetlv > casterlv then
			ep = ep + (targetlv - casterlv) * 3;
		end
		SquadLib.AddExp(sid, ep);	
	end
end

function validarea()
	ScriptCommonLib.SetTempInt("validarea", 1);
end

function validtarget()
	ScriptCommonLib.SetTempInt("validtarget", 1);
end

function canaffect()
	ScriptCommonLib.SetTempInt("affect", 1);
end