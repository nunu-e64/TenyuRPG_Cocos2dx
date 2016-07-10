////多重インクルード防止（インクルードガード）//	//pragmaonceと同じ意味だがこちらはコンパイラに依存しない
#ifndef ITEM_H								////
#define ITEM_H								////
////////////////////////////////////////////////

#include "ItemManager.h"

class CItem{	//インターフェースクラス
public:
	CItem() {
		CONSTRUCTED;
		Name = "";
		Kind = CItemManager::item_tag::NOKIND;
		Price = 0;
		OwnLimit = -1;
	}

	std::string Name;
	CItemManager::item_tag::type Kind;
	int OwnLimit;		//所持制限　0で所持不可　-1で無制限
	int Price;			//売却価格
	bool Sellable;

};

class CEffectItem: public CItem {
public :
	std::vector<sideEffect_tag> SideEffectSet;
};


class CConsumptionItem: public CEffectItem {
public:
	bool BattleUsable;	//戦闘中利用可否
	int WaitTime;		//戦闘中使用後の待機時間

	target_tag::type Target;
};

class CAccessoryItem: public CEffectItem {
public:
	std::vector < std::pair<std::string, int> > MaterialSet;	//練成に必要な素材アイテム名と個数
};

class CKeyItem: public CItem {

};

class CMaterialItem: public CItem {

};

////多重インクルード防止（インクルードガード）//
#endif										////
////////////////////////////////////////////////