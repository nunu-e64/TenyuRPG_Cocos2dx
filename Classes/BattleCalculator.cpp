#include "BattleCalculator.h"



int CBattleCalculator::CalcTrickPower(int _level) {

	return (_level > 0 ? CalcValue(MAXHP, _level, 3) / 4 : 0);		//UNDONE ‚±‚Ì‚Ö‚ñŽb’è’l

}

int CBattleCalculator::CalcBaseTrickPower(int _level, int _gene) {

	return CalcValue(MAXHP, _level, _gene) / 6;

}

double CBattleCalculator::CalcSpd(int _level, int _gene) {
	bool checker;
	int level = between(1, 9999, _level, &checker);
	if (!checker) WARNINGDX("ErrorLevel:%d gene=%d", _level, _gene);
	
	return BASIC_SPD;
}

int CBattleCalculator::CalcValue(value_tag _key, int _level, int _gene) {

	bool checker;
	int level = between(1, 9999, _level, &checker);
	if (!checker) WARNINGDX("ErrorLevel:%d key=%d, gene=%d", _level, (int)_key, _gene);

	switch (_key) {
	case MAXHP:
		switch (_gene) {
		case 1:
			return (level + 4) * 10;
			break;
		case 2:
			return (level + 4) * 12;
			break;
		case 3:
			return (level + 4) * 14;
			break;
		case 4:
			return (level + 4) * 17;
			break;
		case 5:
			return (level + 4) * 20;
			break;
		default:
			ERRORDX("ErrorGene[MAXHP]: _key=%d,level=%d,_gene=%d (instead:calc by _gene=3)", (int)_key, level, _gene);
			return CalcValue(_key, level, _gene);
			break;
		}
		break;
	case ATK:
	case DEF:
		switch (_gene) {
		case 1:
			return 70 + level;
			break;
		case 2:
			return 80 + level * 2;
			break;
		case 3:
			return 90 + level * 3;
			break;
		default:
			ERRORDX("ErrorGene[ATKorDEF] _key=%d,level=%d,_gene=%d (instead: calc by _gene=2)", (int)_key, level, _gene);
			return CalcValue(_key, level, _gene);
			break;
		}
		break;
	default:
		ERRORDX("Error value_key:%d: (instead: return -1)", (int)_key);
		return -1;
		break;
	}

}

int CBattleCalculator::CalcGold(int _lv, int _goldgene) {
	return max(1, _lv*_goldgene);
}

int CBattleCalculator::CalcExp(int _lv, int _expgene) {
	return max(1, _lv*_expgene);
}
