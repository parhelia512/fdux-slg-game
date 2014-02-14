function useskill()
	local sid = ScriptCommonLib.GetTempString("squadid");
	
	local atlv, atid = SquadLib.GetEffectLevelByName(sid, "AtkTimeImprove");
	local alv, aid = SquadLib.GetEffectLevelByName(sid, "AtkImprove");
	local atknum = 30 + atlv * 10;
	local atk = SquadLib.GetActionPoint(sid) + 8;
	atk  = atk + alv * 4;
	
	SquadLib.Animation(sid, 1, "Skill", "none", "mp_seal_05", 0, 1);
	
	local ep = 0;
	local tgtx = ScriptCommonLib.GetTempInt("targetx");
	local tgty = ScriptCommonLib.GetTempInt("targety");
	local sf = SquadLib.GetFaction(sid);
	local croodlist = {
		{tgtx, tgty},
		{tgtx - 1, tgty},
		{tgtx, tgty - 1},
		{tgtx + 1, tgty},
		{tgtx, tgty + 1},
	};
	for i = 1, 5 do
		local tgtsid = BattleLib.GetSquadAt(croodlist[i][1], croodlist[i][2], 1, sf);
		if tgtsid ~= "" then
			local tgtf = SquadLib.GetFaction(tgtsid);
			if tgtf ~= sf then
				SquadLib.PlayParticle(tgtsid, 1, "mp_flare_07", "none", 2500);
				SkillLib.MagicAttack(sid, tgtsid, atknum , atk, 30);
				ep = ep + 25;
			end
		end
	end	
	SquadLib.ApplyEffect(sid, "Tired");
	SquadLib.ApplyEffect(sid, "Tired");
	
	SquadLib.AddExp(sid, ep);
	ScriptCommonLib.SetTempInt("castsuccess", 1);
end

function validarea()
	ScriptCommonLib.SetTempInt("validarea", 1);
end

function validaffectarea()
	local sid = ScriptCommonLib.GetTempString("squadid");
	local tgtx = ScriptCommonLib.GetTempInt("targetx");
	local tgty = ScriptCommonLib.GetTempInt("targety");
	local sf = SquadLib.GetFaction(sid);
	local tgtsid = BattleLib.GetSquadAt(tgtx, tgty, 1, sf);
	if tgtsid ~= "" then
		local tgtf = SquadLib.GetFaction(tgtsid);
		if tgtf ~= sf then
			ScriptCommonLib.SetTempInt("validaffectarea", 1);
		end
	else 
		ScriptCommonLib.SetTempInt("validaffectarea", 1);
	end
end