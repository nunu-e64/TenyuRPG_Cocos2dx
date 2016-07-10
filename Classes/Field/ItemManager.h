////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//	//pragmaonce�Ɠ����Ӗ�����������̓R���p�C���Ɉˑ����Ȃ�
#ifndef ITEMMANAGER_H								////
#define ITEMMANAGER_H								////
////////////////////////////////////////////////


class CItem;
class CEffectItem;
class CConsumptionItem;
class CAccessoryItem;
class CKeyItem;
class CMaterialItem;
class CMenu;

class CItemManager {
public:
	static CItemManager* GetInstance() {
		static CItemManager instance;
		return &instance;
	}

	~CItemManager() {
		DESTRUCTED;
	}

	ENUM(item_tag, NOKIND, CONSUMPTION, ACCESSORY, KEY, MATERIAL);

	void Init();
	void Clear();

	bool AddItem(CItem* _newItem, const char* _name, item_tag::type _kind, int _ownLimit, int _price, bool _sellable);
	void AddConsumptionItem (const char* _name, int _ownLimit, int _price, bool _sellable, bool _battleUsable, int _waitTime, target_tag::type _target, std::vector<sideEffect_tag> _sideEffectSet);
	void AddAccessoryItem(const char* _name, int _ownLimit, int _price, bool _sellable, std::vector < std::pair<std::string, int> > _materialSet, std::vector<sideEffect_tag> _sideEffectSet);
	void AddKeyItem			(const char* _name, int _ownLimit, int _price, bool _sellable);
	void AddMaterialItem	(const char* _name, int _ownLimit, int _price, bool _sellable);
	void SetAccessoryEffect (const char* _name, std::vector<sideEffect_tag> _sideEffectSet);

	CItem* GetItem(std::string _name);
	CEffectItem* GetEffectItem(std::string _name);
	CConsumptionItem* GetConsumptionItem(std::string _name);
	CAccessoryItem* GetAccessoryItem(std::string _name);

	CMenu* GetPlayerAccessoryMenu();

	//�A�C�e���o�b�O�֌W////////////////////////
	bool IncPlayerItem(std::string _name, int _num);	
	bool DecPlayerItem(std::string _name, int _num);
	int GetPlayerItemNum(std::string _name);			//�������Ă���A�C�e�������擾
	std::vector <std::string> GetBattleItemNameList();	//�������Ă��闘�p�\�ȏ���A�C�e���̃��X�g���擾
	std::vector <std::string> GetAccessoryItemInBag();	//�������Ă��鑕���A�C�e���̃��X�g���擾
	////////////////////////////////////////////

	//���z�֌W//////////////////////////////////
	bool IncGold(int _gold); 
	bool DecGold(int _gold); 
	int GetGold() { return Gold; };
	////////////////////////////////////////////


	//DEBUG: ///////////////////////////////////
	void DebugShowAllItem();	 
	void DebugShowAllPlayerItem();
	////////////////////////////////////////////

private:

	bool IncPlayerItem(std::string _name);	//true...�������� false...��������or�G���[
	bool DecPlayerItem(std::string _name);	//true...�������� false...����0or�G���[

	std::map <std::string, CItem*> ItemBank;
	std::map <std::string, CConsumptionItem*> ConsumptionItemBank;
	std::map <std::string, CAccessoryItem*> AccessoryItemBank;
	std::map <std::string, CKeyItem*> KeyItemBank;
	std::map <std::string, CMaterialItem*> MaterialItemBank;

	std::map <std::string, int> PlayerItemBag;	//�����A�C�e���̌�

	int Gold;	//���z


	//�P�ꐫ��ۏ�(Singleton)////////////////////////////////
	CItemManager() {
		CONSTRUCTED;
	}
	CItemManager(const CItemManager& hoge);
	CItemManager& operator=(const CItemManager& hoge);
	/////////////////////////////////////////////////////////


};

////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#endif										////
////////////////////////////////////////////////