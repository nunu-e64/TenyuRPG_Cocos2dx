////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//	//pragmaonce�Ɠ����Ӗ�����������̓R���p�C���Ɉˑ����Ȃ�
#ifndef SHOPMANAGER_H						////
#define SHOPMANAGER_H						////
////////////////////////////////////////////////

class CItem;
class CItemManager;

class CShopMenu {
public:
	std::vector	<CItem*> ItemList;	//�J�X���ɍ�鏤�i�I
	std::vector<int> Basket;	//���ʂ��L�^���锃��������
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

	//�P�ꐫ��ۏ�(Singleton)////////////////////////////////
	CShopManager() {
		CONSTRUCTED;
	}
	CShopManager(const CShopManager& hoge);
	CShopManager& operator=(const CShopManager& hoge);
	/////////////////////////////////////////////////////////

private :
	CShopMenu ShopMenuInstance;

};

////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#endif										////
////////////////////////////////////////////////