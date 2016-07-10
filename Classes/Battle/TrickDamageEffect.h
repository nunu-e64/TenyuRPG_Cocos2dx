////多重インクルード防止（インクルードガード）//
#ifndef TRICKDAMAGEEFFECT_H					////
#define TRICKDAMAGEEFFECT_H					////
////////////////////////////////////////////////

#include <map>
#include <string>
//#include "Battle.h"

class CBattle;
class CBImgBank;
class CTrickDamageEffect{ //戦闘ダメージエフェクト用のインターフェース
public: 
	CTrickDamageEffect(std::string _name){
		CONSTRUCTED;;
		Name = _name;
	}
	~CTrickDamageEffect(){
		DESTRUCTED;
	}
	virtual void DrawDamageEffect(CBattle* _battle, CBImgBank* _bimgbank, CRect _attackerR, CRect _targetR) const =0;
	std::string GetName() const {return Name;}
protected:
	std::string Name;	//エフェクトの名前 例：RED_BOMB, BLUE_BOMB, BIG_SPREAD, SMALL_SPREAD
	//std::string TypeName;	//Typeの名前 例：BOMB, SPREAD
};


class CTrickDamageEffect_BOMB: public CTrickDamageEffect{	//未作成
public:
	CTrickDamageEffect_BOMB(std::string _name, std::vector<std::string>_argList);
	~CTrickDamageEffect_BOMB(){
		DESTRUCTED;
	}
	void DrawDamageEffect(CBattle* _battle, CBImgBank* _bimgbank, CRect _attackerR, CRect _targetR) const;	//共通
private:
	enum{ ARG_NUM=5 };
	int Color;
	int Size;
	int Time;
};

class CTrickDamageEffect_PROTO: public CTrickDamageEffect{
	/*螺旋状に8つの光球が集まって、一つの光球となって相手めがけて飛んでいく	*/
public:
	CTrickDamageEffect_PROTO(std::string _name, std::vector<std::string>_argList);
	~CTrickDamageEffect_PROTO(){
		DESTRUCTED;
	}
	void DrawDamageEffect(CBattle* _battle, CBImgBank* _bimgbank, CRect _attackerR, CRect _targetR) const;	//共通
private:
	enum{ ARG_NUM=3 };
	int Size1;	//収束光球の大きさ
	int Size2;	//打ちだされてヒットする光球の大きさ
	int Radius;  //収束光球の初期配置半径（キャラの中心座標からの距離）
};




////多重インクルード防止（インクルードガード）//
#endif										////
////////////////////////////////////////////////