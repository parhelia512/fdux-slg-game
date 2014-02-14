function useskill()
	local caster  = ScriptCommonLib.GetTempString("squadid");
	local target  = ScriptCommonLib.GetTempString("targetsquadid");
	
	local blv, bid = SquadLib.GetEffectLevelByName(target, "Blind");
	local bilv, biid = SquadLib.GetEffectLevelByName(target, "ImprovedBlind");
	local wlv, wid = SquadLib.GetEffectLevelByName(target, "WeakArmor");
	local wilv, wiid = SquadLib.GetEffectLevelByName(target, "ImprovedWeakArmor");
	local atknum = 40.0;
	local atk  = 10.0 + 4 * blv + 7 * bilv + 4 * wlv + 7 * wilv;
	
	SquadLib.Animation(caster, 1, "Skill", "none", "mp_seal_04", 0, 1);
	SquadLib.PlayParticle(target, 3, "mp_flare", "none", 2500);
	SkillLib.MagicAttack(caster, target, atknum , atk, 30);
	SquadLib.AddExp(caster, atknum);	
	
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
