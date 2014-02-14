function useskill()
	local caster  = ScriptCommonLib.GetTempString("squadid");
	local target  = ScriptCommonLib.GetTempString("targetsquadid");
	
	local unitnum = SquadLib.GetUnitNum(caster);
	local cost = 5;
	local improvelv, improve = SquadLib.GetEffectLevelByName(caster, "ReligiousnessImprove");
	if improvelv > 0 then
		cost = 3;
	end	
	if unitnum > cost + 1 then	
		SquadLib.Animation(caster, 1, "Skill", "none", "mp_seal_08", 0, 1);
		unitnum = unitnum - cost;
		SquadLib.SetUnitNum(caster, unitnum);
		
		SquadLib.PlayParticle(target, 3, "mp_diabolique", "none", 2500);
		SkillLib.MagicAttack(caster, target, 24 , 30, 20);
	
		local casterlv = SquadLib.GetSquadLevel(caster);
		local targetlv = SquadLib.GetSquadLevel(target);
		local ep = 50;
		if targetlv > casterlv then
			ep = ep + (targetlv - casterlv) * 5;
		end	
		SquadLib.AddExp(caster, ep);
		ScriptCommonLib.SetTempInt("castsuccess", 1);
	else
		SquadLib.ShowValue(sid, "Skills_NotEnoughGuard", 1.0, 1.0, 1.0);
		ScriptCommonLib.SetTempInt("castsuccess", 0);
	end
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