////多重インクルード防止（インクルードガード）//	//pragmaonceと同じ意味だがこちらはコンパイラに依存しない
#ifndef SHOPMANAGER_H						////
#define SHOPMANAGER_H						////
////////////////////////////////////////////////

class CItem;
class CItemManager;

class CShopMenu {
public:
	std::vector	<CItem*> ItemList;	//開店時に作る商品棚
	std::vector<int> Basket;	//数量を記録する買い物かご
	int SumPrice;

	int Cursor;
	bool IsConfirm;
	CItemManager* ItemManager;

	virtual void Move(int _dir);
	virtual void Draw();
	virtual bool Buy();

	virtual bool CanBuy();
	virtual bool CanClose();
};

class CShopManager {
public:
	static CShopManager* GetInstance() {
		static CShopManager instance;
		return &instance;
	}
	~CShopManager() {
		DESTRUCTED;
	}

	virtual void Init();
	virtual bool OpenShop(int _index);
	bool Main();

	bool AddShop(int _index, std::vector<std::string> _itemList);
	std::vector <std::string> GetShop(int _index);	

	bool IsOpen();

	void Draw();

protected:

	std::map<int, std::vector <std::string> > ShopBank;

	int CurrentOpenShopIndex;
	CShopMenu* ShopMenu;

	//単一性を保証(Singleton)////////////////////////////////
	CShopManager() {
		CONSTRUCTED;
	}
	CShopManager(const CShopManager& hoge);
	CShopManager& operator=(const CShopManager& hoge);
	/////////////////////////////////////////////////////////

private :
	CShopMenu ShopMenuInstance;

};

////多重インクルード防止（インクルードガード）//
#endif										////
////////////////////////////////////////////////