function useskill()
	local caster  = ScriptCommonLib.GetTempString("squadid");
	local target  = ScriptCommonLib.GetTempString("targetsquadid");
	local squadnum = SquadLib.GetUnitNum(caster);
	local form = SquadLib.GetSquadAttr(caster, 3, 0);
	if form < 0 then
		form = 0.0;
	end
	local atk = form * squadnum / 50.0 * 1.5;
	local mid = SquadLib.ApplyModifier(caster, 0, 0.0, atk, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	local x, y = SquadLib.GetSquadCoord(target);
	if x >  -1 then
		re = SkillLib.RangedAttack(caster,target);
		if re > 0 then
			local effectlv, eid = SquadLib.GetEffectLevelByName(caster, "DoubleShoot");
			if effectlv > 0 then
				if SquadLib.GetUnitNum(target) > 0 then
					SkillLib.RangedAttack(caster,target);
				end
			end
			local casterlv = SquadLib.GetSquadLevel(caster);
			local targetlv = SquadLib.GetSquadLevel(target);
			local ep = 50;
			if targetlv > casterlv then
				ep = ep + (targetlv - casterlv) * 5;
			end	
			SquadLib.AddExp(caster, ep);
			ScriptCommonLib.SetTempInt("castsuccess", 1);
		end
	end
	SquadLib.RemoveModifier(caster, mid);
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
	end
end