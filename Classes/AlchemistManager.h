////多重インクルード防止（インクルードガード）//	//pragmaonceと同じ意味だがこちらはコンパイラに依存しない
#ifndef ALCHEMISTMANAGER_H						////
#define ALCHEMISTMANAGER_H						////
////////////////////////////////////////////////

#include "ShopManager.h"

class CAlchemistMenu : public CShopMenu {
public:
	void Move(int _dir);
	void Draw();
	bool Buy();
	
	bool CanBuy();
	bool CanClose();

	std::vector <std::vector < std::pair<std::string, int> > > CurrentMaterialSet;
};

class CAlchemistManager : public CShopManager {
public:
	static CAlchemistManager* GetInstance() {
		static CAlchemistManager instance;
		return &instance;
	}
	~CAlchemistManager() {
		DESTRUCTED;
	}

	void Init();
	bool OpenShop(int _index);

private:
	CAlchemistMenu AlchemistMenuInstance;

	//単一性を保証(Singleton)////////////////////////////////
	CAlchemistManager() {
		CONSTRUCTED;
	}
	CAlchemistManager(const CAlchemistManager& hoge);
	CAlchemistManager& operator=(const CAlchemistManager& hoge);
	/////////////////////////////////////////////////////////

};


////多重インクルード防止（インクルードガード）//
#endif										////
////////////////////////////////////////////////