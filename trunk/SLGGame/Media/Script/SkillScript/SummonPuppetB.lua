function useskill()
	local caster  = ScriptCommonLib.GetTempString("squadid");
	local maxnum = SquadLib.GetUnitMaxNum(caster);
	SquadLib.Animation(caster, 1, "Skill", "none", "mp_seal_05", 0, 1);
	local num = SquadLib.GetUnitNum(caster);
	--local sklv = SquadLib.GetSkillLevel(caster, "SummonPuppetB");
	num = num + 6;-- + sklv * 3;
	if num > maxnum then
		num = maxnum;
	end
	SquadLib.SetUnitNum(caster, num);
	local ep = 8 * num;
	SquadLib.AddExp(caster, ep);
	local eid = SquadLib.ApplyEffect(caster, "Tired");	
	ScriptCommonLib.SetTempInt("castsuccess", 1);
end

function validarea()
	ScriptCommonLib.SetTempInt("validarea", 1);
end

function validtarget()
	ScriptCommonLib.SetTempInt("validtarget", 1);
end