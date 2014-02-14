function useskill()
	local caster  = ScriptCommonLib.GetTempString("squadid");
	local target  = ScriptCommonLib.GetTempString("targetsquadid");
	local atlv, atid = SquadLib.GetEffectLevelByName(caster, "AtkTimeImprove");
	local alv, aid = SquadLib.GetEffectLevelByName(caster, "AtkImprove");
	local ap = SquadLib.GetActionPoint(caster);
	local atknum = 30 + atlv * 10;
	local atk  = 40 - (3.0 - 1.1 * alv - 0.2 * ap) * SquadLib.GetMaxActionPoint(target);
	
	SquadLib.Animation(caster, 1, "Skill", "none", "mp_seal_05", 0, 1);
	SquadLib.PlayParticle(target, 1, "mp_woosh_12", "none", 2500);
	SkillLib.MagicAttack(caster, target, atknum , atk, 30);
	SquadLib.AddExp(caster, atk * 2.0);	
	
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