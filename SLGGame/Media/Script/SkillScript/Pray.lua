function useskill()
	local caster  = ScriptCommonLib.GetTempString("squadid");
	local ap = SquadLib.GetActionPoint(caster);
	local maxnum = SquadLib.GetUnitMaxNum(caster);
	SquadLib.Animation(caster, 1, "Skill", "none", "mp_seal_08", 0, 1);
	local num = SquadLib.GetUnitNum(caster);
	local sklv = SquadLib.GetSkillLevel(caster, "Pray");
	local camp, eid = SquadLib.GetEffectLevelByName(caster, "Camp");
	if ap > 2 + sklv * 2 + camp * 2 then
	 ap = 2 + sklv * 2 + camp * 2;
	end
	num = num + ap * (1 * sklv);
	if num > maxnum then
		num = maxnum;
	end
	SquadLib.SetUnitNum(caster, num);
	local ep = 8 * num;
	SquadLib.AddExp(caster, ep);	
	ScriptCommonLib.SetTempInt("castsuccess", 1);
end

function validarea()
	ScriptCommonLib.SetTempInt("validarea", 1);
end

function validtarget()
	ScriptCommonLib.SetTempInt("validtarget", 1);
end