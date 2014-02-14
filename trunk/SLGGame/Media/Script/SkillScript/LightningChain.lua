function useskill()
	local caster  = ScriptCommonLib.GetTempString("squadid");
	local target  = ScriptCommonLib.GetTempString("targetsquadid");
	local atknum = SquadLib.GetUnitNum(target);
	local atk = SquadLib.GetActionPoint(caster) * 3;
	local atlv, atid = SquadLib.GetEffectLevelByName(caster, "AtkTimeImprove");
	local alv, aid = SquadLib.GetEffectLevelByName(caster, "AtkImprove");
	atknum = atknum * (1.0 + (0.4 * atlv));
	atk = atk + alv * 4;
	SquadLib.Animation(caster, 1, "Skill", "none", "mp_seal_05", 0, 1);
	SquadLib.PlayParticle(target, 3, "mp_lightning", "none", 2500);
	SkillLib.MagicAttack(caster, target, atknum , atk, 15);
	SquadLib.AddExp(caster, atknum * 1.5);	
	
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
