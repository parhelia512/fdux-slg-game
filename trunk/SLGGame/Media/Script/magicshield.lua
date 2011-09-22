function useskill()
	attacker  = ScriptCommonLib.GetString("skillcaster");
	unitnum = SkillLib.GetUnitNum(attacker);
	if unitnum > 3 then
		unitnum = unitnum - 2;
		SkillLib.SetUnitNum(attacker, unitnum);
		defender  = ScriptCommonLib.GetString("skilltarget");
		defenderpath = ScriptCommonLib.GetString("skilltargetpath");
		id = SkillLib.ApplyEffect(defenderpath,"MagicShield");
		ScriptCommonLib.SetString("Effectid",id);
		SkillLib.Action(attacker,1,"mp_seal_08","Skill","magicSkill1.mp3");
		ScriptCommonLib.SetInt("skillcast",1);
	end
end

function onaffect()
	squadpath = ScriptCommonLib.GetString("affectsquadpath");
	id = SkillLib.ApplyParticle(squadpath,3,"mp_spell_effect_06");
	ScriptCommonLib.SetString("Particleid",id);
	id = SkillLib.ApplyModifier(squadpath,1,0.0,0.0,2.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0);
	ScriptCommonLib.SetString("Modifierid",id);
	ScriptCommonLib.SetInt("LastTurn",0);
	trigerid = TriggerLib.AddSquadTrigger(squadpath,"TurnEnd","onturnend");
	TriggerLib.ActiveSquadTrigger(squadpath,trigerid);
	ScriptCommonLib.SetString("TurnEndTrigger",trigerid);
end

function onremove()
	squadpath = ScriptCommonLib.GetString("affectsquadpath");
	id = ScriptCommonLib.GetString("Particleid");
	SkillLib.RemoveParticle(squadpath,id);
	id = ScriptCommonLib.GetString("Modifierid");
	SkillLib.RemoveModifier(squadpath,id);
	id = ScriptCommonLib.GetString("TurnEndTrigger");
	TriggerLib.RemoveSquadTrigger(squadpath,id);
end

function onturnend()
	turn = ScriptCommonLib.GetInt("LastTurn");
	turn = turn + 1;
	ScriptCommonLib.SetInt("LastTurn",turn);
	if turn == 3 then
		squadpath = ScriptCommonLib.GetString("affectsquadpath");
		id = ScriptCommonLib.GetString("effectid");
		SkillLib.RemoveEffect(squadpath, id);
	end
end