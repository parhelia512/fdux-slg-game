function useskill()
	local caster  = ScriptCommonLib.GetTempString("squadid");
	local target  = ScriptCommonLib.GetTempString("targetsquadid");
	local unitnum = SquadLib.GetUnitNum(caster);
	local cost = 3;
	local improvelv, improve = SquadLib.GetEffectLevelByName(caster, "KindnessImprove");
	if improvelv > 0 then
		cost = 2;
	end	
	if unitnum > cost + 1 then
		SquadLib.Animation(caster, 1, "Skill", "none", "mp_seal_08", 0, 1);
		unitnum = unitnum - cost;
		SquadLib.SetUnitNum(caster, unitnum);
		SquadLib.PlayParticle(target, 3, "mp_streak_01", "none", 2500);
		local ep = 0;
		local maxnum = SquadLib.GetUnitMaxNum(target);
		local num = SquadLib.GetUnitNum(target);
		num = num + 12;
		if num > maxnum then
			ep = ep + (maxnum + 12 - num) * 5;
			num = maxnum;
		else
			ep = ep + 50;
		end
		SquadLib.SetUnitNum(target, num);
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