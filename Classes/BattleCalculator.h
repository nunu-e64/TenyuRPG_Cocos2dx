////多重インクルード防止（インクルードガード）//
#ifndef BATTLECALCULATOR_H					////
#define BATTLECALCULATOR_H					////
////////////////////////////////////////////////

class CBattleCalculator{
public:
	CBattleCalculator(){
		CONSTRUCTED;	
	}
	~CBattleCalculator(){
		DESTRUCTED;	
	}
	static enum value_tag {
		MAXHP,
		ATK,
		DEF,
		SPD,
		BASE_TRICK
	};

	static int CalcTrickPower(int _level);
	static int CalcBaseTrickPower(int _level, int _gene);
	static double CalcSpd(int _level, int _gene);
	static int CalcValue(value_tag _key, int _level, int _gene);

	static int CalcGold(int _lv, int _goldgene);
	static int CalcExp(int _lv, int _expgene);
}typedef CBattleCalc;


////多重インクルード防止（インクルードガード）//
#endif										////
////////////////////////////////////////////////