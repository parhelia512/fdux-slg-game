function useskill()
	local caster  = ScriptCommonLib.GetTempString("squadid");
	local ap = SquadLib.GetActionPoint(caster);
	local maxnum = SquadLib.GetUnitMaxNum(caster);
	local num = SquadLib.GetUnitNum(caster);
	local curnum = num;
	local sklv = SquadLib.GetSkillLevel(caster, "Rest");
	local camp, eid = SquadLib.GetEffectLevelByName(caster, "Camp");
	if ap > 4 + camp * 2 then
	 ap = 4 + camp * 2;
	end
	num = num + (ap + camp * 2) * (1 + sklv);
	ScriptCommonLib.Log("SKILLLOG Rest sklv"..sklv.." camp"..camp);
	if num > maxnum then
		num = maxnum;
	end
	SquadLib.SetUnitNum(caster, num);
	local ep = 5 * (num - curnum);
	SquadLib.AddExp(caster, ep);	
	ScriptCommonLib.SetTempInt("castsuccess", 1);
end

function validarea()
	ScriptCommonLib.SetTempInt("validarea", 1);
end

function validtarget()
	ScriptCommonLib.SetTempInt("validtarget", 1);
end