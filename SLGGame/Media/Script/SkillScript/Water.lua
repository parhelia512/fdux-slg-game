function useskill()
	local caster  = ScriptCommonLib.GetTempString("squadid");
	local target  = ScriptCommonLib.GetTempString("targetsquadid");
	local atlv, atid = SquadLib.GetEffectLevelByName(caster, "AtkTimeImprove");
	local alv, aid = SquadLib.GetEffectLevelByName(caster, "AtkImprove");
	local atknum = SquadLib.GetActionPoint(caster) * (10 + 3 * atlv);
	local atk  = 10 + alv * 4;
	
	SquadLib.Animation(caster, 1, "Skill", "none", "mp_seal_05", 0, 1);
	SquadLib.PlayParticle(target, 3, "mp_water", "none", 2500);
	SkillLib.MagicAttack(caster, target, atknum , atk, 30);
	SquadLib.AddExp(caster, atknum);	
	
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
