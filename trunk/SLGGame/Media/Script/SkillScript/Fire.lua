function useskill()
	local caster  = ScriptCommonLib.GetTempString("squadid");
	local target  = ScriptCommonLib.GetTempString("targetsquadid");
	local atlv, atid = SquadLib.GetEffectLevelByName(caster, "AtkTimeImprove");
	local alv, aid = SquadLib.GetEffectLevelByName(caster, "AtkImprove");
	local atknum = SquadLib.GetActionPoint(caster) * (7 + atlv * 2);
	local atk  = 18 + alv * 4;
	local croodx, croody = SquadLib.GetSquadCoord(target);
	local def = MapLib.GetGroundAttr(croodx, croody, 0);
	if def > 0 then
		atk = atk + def * ( 1.5 + 1.0 * alv);
	end
	
	SquadLib.Animation(caster, 1, "Skill", "none", "mp_seal_05", 0, 1);
	SquadLib.PlayParticle(target, 1, "mp_explosion_02", "none", 2500);
	SkillLib.MagicAttack(caster, target, atknum , atk, 20);
	SquadLib.AddExp(caster, atk * 2.5);	
	
	SquadLib.ApplyEffect(caster, "Tired");
	SquadLib.ApplyEffect(caster, "Tired");
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
	if casterfaction ~= targetfaction then
		ScriptCommonLib.SetTempInt("validtarget", 1);
    else
        ScriptCommonLib.SetTempInt("validtarget", 0);
	end
end
