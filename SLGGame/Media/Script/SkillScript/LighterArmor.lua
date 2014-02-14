function useskill()
	local caster  = ScriptCommonLib.GetTempString("squadid");
	local target  = ScriptCommonLib.GetTempString("targetsquadid");

	SquadLib.Animation(caster, 1, "Skill", "none", "mp_seal_04", 0, 1);
	local sklv = SquadLib.GetSkillLevel(caster, "LighterArmor");
	local improvelv, improve = SquadLib.GetEffectLevelByName(caster, "EnchantAImprove");
	sklv = sklv + improvelv;
	for i = 1, sklv + 1 do
		SquadLib.ApplyEffect(target, "LighterArmor");
	end
	SquadLib.PlayParticle(target, 3, "mp_spell_effect_01", "none", 2500);
	local casterlv = SquadLib.GetSquadLevel(caster);
	local targetlv = SquadLib.GetSquadLevel(target);
	local ep = 50;
	if casterlv > targetlv then
		ep = ep + (casterlv - targetlv) * 5;
	end	
	SquadLib.AddExp(caster, ep);
	ScriptCommonLib.SetTempInt("castsuccess", 1);
end

function validarea()
	ScriptCommonLib.SetTempInt("validarea", 1);
end

function validtarget()
	local caster  = ScriptCommonLib.GetTempString("squadid");
	local target  = ScriptCommonLib.GetTempString("targetsquadid");
	local casterfaction = SquadLib.GetFaction(caster);
	local targetfaction = SquadLib.GetFaction(target);
	if casterfaction == targetfaction then
		local squadtype = SquadLib.GetType(target);
		if squadtype == 0 then
			ScriptCommonLib.SetTempInt("validtarget", 1);
		else
			ScriptCommonLib.SetTempInt("validtarget", 0);
		end
	else
		ScriptCommonLib.SetTempInt("validtarget", 0);
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
	mid = SquadLib.ApplyModifier(sid, 2, 0.0, 0.0, 0.0, 0.0, 2.0, 1.0, 0.0, 0.0, 0.0, 0.0);
	ScriptCommonLib.SetString("modifierid", mid);
	if tid == "" then
		tid = SquadLib.AddSquadTrigger(sid, "TurnStart", "onturnstart");
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

function onturnstart()
	local sid = ScriptCommonLib.GetTempString("squadid");
	local eid = ScriptCommonLib.GetString("effectid");
	local turn = ScriptCommonLib.GetInt("turn");
	local lv = SquadLib.GetEffectLevel(sid, eid);
	if turn > 1 then
		local eid = ScriptCommonLib.GetString("effectid");
		SquadLib.RemoveEffect(sid, eid);
	else
		ScriptCommonLib.SetInt("turn", turn + 1);
	end
end

function canaffect()
	ScriptCommonLib.SetTempInt("affect", 1);
end