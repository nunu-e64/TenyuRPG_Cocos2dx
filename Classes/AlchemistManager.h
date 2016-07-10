////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//	//pragmaonce�Ɠ����Ӗ�����������̓R���p�C���Ɉˑ����Ȃ�
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

	//�P�ꐫ��ۏ�(Singleton)////////////////////////////////
	CAlchemistManager() {
		CONSTRUCTED;
	}
	CAlchemistManager(const CAlchemistManager& hoge);
	CAlchemistManager& operator=(const CAlchemistManager& hoge);
	/////////////////////////////////////////////////////////

};


////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#endif										////
////////////////////////////////////////////////