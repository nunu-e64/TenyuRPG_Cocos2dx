#include "ItemManager.h"
#include "Item.h"
#include "Menu.h"

void CItemManager::Init() {
	Clear();
}

void CItemManager::Clear(){

	//イテレータ使ってすべてdeleteしなくちゃいけない 
	std::map <std::string, CItem*>::iterator it = ItemBank.begin();
	while (it != ItemBank.end()) {
		delete (*it).second;
		++it;	//イテレータの指す先を無効化しているがeraseしたわけではないのでイテレータは変動していない＠。ただし保障されていないため危険ではある。
	}
	ItemBank.clear();

	ConsumptionItemBank.clear();
	AccessoryItemBank.clear();
	KeyItemBank.clear();
	MaterialItemBank.clear();

	PlayerItemBag.clear();

}


bool CItemManager::AddItem(CItem* _newItem, const char* _name, item_tag::type _kind, int _ownLimit, int _price, bool _sellable) {

	if (ItemBank.find(_name) == ItemBank.end()) {

		_newItem->Kind = _kind;
		_newItem->Name = _name;
		_newItem->OwnLimit = _ownLimit;
		_newItem->Price = _price;
		
		_newItem->Sellable = _sellable;

		ItemBank[_name] = _newItem;
		//DEBUGDX("newItem:%s:%d", _name, (int)ItemBank[_name]);
		return true;

	} else {
		WARNINGDX("Already Exist Item Name :%s", _name);
		return false;
	}
}

void CItemManager::AddConsumptionItem(const char* _name, int _ownLimit, int _price, bool _sellable, bool _battleUsable, int _waitTime, target_tag::type _target, std::vector<sideEffect_tag> _sideEffectSet) {

	if (ConsumptionItemBank.find(_name) == ConsumptionItemBank.end()) {

		CConsumptionItem* newItem = new CConsumptionItem();

		trick_tag tmpTrick;
		newItem->BattleUsable = _battleUsable;
		newItem->WaitTime = _waitTime;
		newItem->Target = _target;
		newItem->SideEffectSet = _sideEffectSet;

		if (AddItem(newItem, _name, item_tag::CONSUMPTION, _ownLimit, _price, _sellable)) {
			ConsumptionItemBank[_name] = newItem;
			return;
		} else {
			delete newItem;
		}

	} else {
		WARNINGDX("Already Exist Item Name :%s", _name);
		return;
	}

}

void CItemManager::AddAccessoryItem(const char* _name, int _ownLimit, int _price, bool _sellable, std::vector < std::pair<std::string, int> > _materialSet, std::vector<sideEffect_tag> _sideEffectSet) {

	if (AccessoryItemBank.find(_name) == AccessoryItemBank.end()) {

		CAccessoryItem* newItem = new CAccessoryItem();
		newItem->SideEffectSet = _sideEffectSet;

		for (unsigned int i = 0; i < _materialSet.size(); i++) {
			if (!(MaterialItemBank.find(_materialSet[i].first) != MaterialItemBank.end())) {
				WARNINGDX("Not Found Material:%s, ItemName:%s", _materialSet[i].first.c_str(), _name);
				continue;
			}
			if (_materialSet[i].second <= 0) {
				WARNINGDX("Material Num > 0 :%d, ItemName:%s", _materialSet[i].second, _name);
				continue;
			}
			newItem->MaterialSet.push_back(_materialSet[i]);
		}

		if (AddItem(newItem, _name, item_tag::ACCESSORY, _ownLimit, _price, _sellable)) {
			AccessoryItemBank[_name] = newItem;
			return;
		} else {
			delete newItem;
		}

	} else {
		WARNINGDX("Already Exist Item Name :%s", _name);
		return;
	}

}

void CItemManager::AddKeyItem(const char* _name, int _ownLimit, int _price, bool _sellable) {

	if (KeyItemBank.find(_name) == KeyItemBank.end()) {

		CKeyItem* newItem = new CKeyItem();
		if (AddItem(newItem, _name, item_tag::KEY, _ownLimit, _price, _sellable)) {
			KeyItemBank[_name] = newItem;
			return;
		} else {
			delete newItem;
		}

	} else {
		WARNINGDX("Already Exist Item Name :%s", _name);
		return;
	}


}
void CItemManager::AddMaterialItem(const char* _name, int _ownLimit, int _price, bool _sellable) {

	if (MaterialItemBank.find(_name) == MaterialItemBank.end()) {

		CMaterialItem* newItem = new CMaterialItem();
		if (AddItem(newItem, _name, item_tag::MATERIAL, _ownLimit, _price, _sellable)) {
			MaterialItemBank[_name] = newItem;
			return;
		} else {
			delete newItem;
		}

	} else {
		WARNINGDX("Already Exist Item Name :%s", _name);
		return;
	}

}

void CItemManager::SetAccessoryEffect(const char* _name, std::vector<sideEffect_tag> _sideEffectSet) {


}
bool CItemManager::IncPlayerItem(std::string _name, int _num) {
	bool result = true;
	
	for (int i = 0; i < _num; i++) {
		result = IncPlayerItem(_name) && result;
	}
	return result;

}
bool CItemManager::IncPlayerItem(std::string _name) {

	if (ItemBank.find(_name) == ItemBank.end()) {
		ERRORDX("Not Found Item. :%s", _name.c_str());
		return false;

	} else if (PlayerItemBag.find(_name) == PlayerItemBag.end()) {
		if (ItemBank[_name]->OwnLimit != 0) {
			PlayerItemBag[_name] = 1;
			//DEBUGDX("%s:%d", _name.c_str(), PlayerItemBag[_name]);
			return true;
		} else {
			return false;
		}

	} else {
		if (PlayerItemBag[_name] + 1 <= ItemBank[_name]->OwnLimit || ItemBank[_name]->OwnLimit == -1) {
			++PlayerItemBag[_name];
			//DEBUGDX("%s:%d", _name.c_str(), PlayerItemBag[_name]);
			return true;
		} else {
			return false;
		}
	}

}

bool CItemManager::DecPlayerItem(std::string _name, int _num) {
	bool result = true;

	for (int i = 0; i < _num; i++) {
		result = DecPlayerItem(_name) && result;
	}
	return result;
}
bool CItemManager::DecPlayerItem(std::string _name) {

	if (ItemBank.find(_name) == ItemBank.end()) {
		ERRORDX("Not Found Item. :%s", _name.c_str());
		return false;

	} else if (PlayerItemBag.find(_name) == PlayerItemBag.end()) {
		return false;

	} else {
		if (PlayerItemBag[_name] > 0) {
			--PlayerItemBag[_name];
			//DEBUGDX("%s:%d", _name.c_str(), PlayerItemBag[_name]);
			return true;
		} else {
			return false;
		}
	}
}

CItem* CItemManager::GetItem(std::string _name) {

	if (ItemBank.find(_name) == ItemBank.end()) {
		ERRORDX("Not Found Item. :%s", _name.c_str());
		return NULL;
	} else {
		return ItemBank[_name];
	}
}
CEffectItem* CItemManager::GetEffectItem(std::string _name) {

	if (ConsumptionItemBank.find(_name) != ConsumptionItemBank.end()) {
		return ConsumptionItemBank[_name];

	} else if (AccessoryItemBank.find(_name) != AccessoryItemBank.end()) {
		return AccessoryItemBank[_name];

	} else {
		ERRORDX("Not Found Item. :%s", _name.c_str());
		return NULL;
	}
}
CConsumptionItem* CItemManager::GetConsumptionItem(std::string _name) {

	if (ConsumptionItemBank.find(_name) == ConsumptionItemBank.end()) {
		ERRORDX("Not Found ConsumptionItem. :%s", _name.c_str());
		return NULL;
	} else {
		return ConsumptionItemBank[_name];
	}
}
CAccessoryItem* CItemManager::GetAccessoryItem(std::string _name) {

	if (AccessoryItemBank.find(_name) == AccessoryItemBank.end()) {
		ERRORDX("Not Found AccessoryItem. :%s", _name.c_str());
		return NULL;
	} else {
		return AccessoryItemBank[_name];
	}
}

CMenu* CItemManager::GetPlayerAccessoryMenu() {	//アイテムバッグの中身と個数リストアップ

	CMenu* accessoryMenu = new CMenu();
	std::string nodeLabel;

	accessoryMenu->Create(REMOVE_EQUIP);
	
	for each (auto item in PlayerItemBag) {
		if (item.second > 0 && GetItem(item.first)->Kind == item_tag::ACCESSORY) {
			nodeLabel = item.first + "   " + std::to_string(item.second);
			accessoryMenu->Add("", nodeLabel.c_str());
		}
	}

	return accessoryMenu;
}

int CItemManager::GetPlayerItemNum(std::string _name) {

	if (ItemBank.find(_name) == ItemBank.end()) {
		WARNINGDX("Not Found Item. :%s", _name.c_str());
		return 0;
	} else if (PlayerItemBag.find(_name) == PlayerItemBag.end()) {
		return 0;
	} else {
		return PlayerItemBag[_name];
	}
}

std::vector <std::string> CItemManager::GetAccessoryItemInBag() {

	std::vector<std::string> itemList;

	for each (auto item in PlayerItemBag) {
		if (item.second > 0 && GetItem(item.first)->Kind == item_tag::ACCESSORY) {
			itemList.push_back(item.first);
		}
	}

	return itemList;
}


std::vector<std::string> CItemManager::GetBattleItemNameList() {	
	std::vector<std::string> itemList;
	
	std::map <std::string, CConsumptionItem*>::iterator it = ConsumptionItemBank.begin();
	while (it != ConsumptionItemBank.end()) {
		if ((*it).second->BattleUsable && GetPlayerItemNum((*it).first) > 0) {
			itemList.push_back((*it).first);
		}
		++it;
	}
	return itemList;
}

bool CItemManager::IncGold(int _gold) {
	if (_gold <= 0) {
		return false;
	} else {
		Gold += _gold;	//HACK: 金額上限が必要ならここで制限する
		return true;
	}
}

bool CItemManager::DecGold(int _gold) {
	
	if (Gold == 0 || _gold <= 0) {
		return false;
	} else {
		Gold -= _gold;
		if (Gold < 0) Gold = 0;
		return true;
	}

}
	

void CItemManager::DebugShowAllItem() {	//DEBUG

	std::string tmp;
	{
		tmp += "--ALL--\n";
		std::map <std::string, CItem*>::iterator it = ItemBank.begin();
		while (it != ItemBank.end()) {
			tmp += (*it).first;
			tmp += "\n";
			++it;	//イテレータの指す先を無効化しているがeraseしたわけではないのでイテレータは変動していない＠。ただし保障されていないため危険ではある。
		}
	}
	{
		tmp += "--Consumption--\n";
		std::map <std::string, CConsumptionItem*>::iterator it = ConsumptionItemBank.begin();
		while (it != ConsumptionItemBank.end()) {
			tmp += (*it).first;
			tmp += "\n";
			++it;	//イテレータの指す先を無効化しているがeraseしたわけではないのでイテレータは変動していない＠。ただし保障されていないため危険ではある。
		}
	}
	{
		tmp += "--Accessory--\n";
		std::map <std::string, CAccessoryItem*>::iterator it = AccessoryItemBank.begin();
		while (it != AccessoryItemBank.end()) {
			tmp += (*it).first;
			tmp += "\n";
			++it;	//イテレータの指す先を無効化しているがeraseしたわけではないのでイテレータは変動していない＠。ただし保障されていないため危険ではある。
		}
	}
	{
		tmp += "--KeyItem--\n";
		std::map <std::string, CKeyItem*>::iterator it = KeyItemBank.begin();
		while (it != KeyItemBank.end()) {
			tmp += (*it).first;
			tmp += "\n";
			++it;	//イテレータの指す先を無効化しているがeraseしたわけではないのでイテレータは変動していない＠。ただし保障されていないため危険ではある。
		}
	}
	{
		tmp += "--MaterialItem--\n";
		std::map <std::string, CMaterialItem*>::iterator it = MaterialItemBank.begin();
		while (it != MaterialItemBank.end()) {
			tmp += (*it).first;
			tmp += "\n";
			++it;	//イテレータの指す先を無効化しているがeraseしたわけではないのでイテレータは変動していない＠。ただし保障されていないため危険ではある。
		}
	}

	DEBUGDX(tmp.c_str());
}

void CItemManager::DebugShowAllPlayerItem() {

	std::string tmp;
	{
		tmp += "\n--PlayerItemPocket--\n";
		std::map <std::string, int>::iterator it = PlayerItemBag.begin();
		while (it != PlayerItemBag.end()) {
			tmp += (*it).first;
			tmp += ":";
			char strNum[1024];
			sprintf_s(strNum, "%d", (*it).second);
			tmp += strNum;
			tmp += "\n";
			++it;	//イテレータの指す先を無効化しているがeraseしたわけではないのでイテレータは変動していない＠。ただし保障されていないため危険ではある。
		}

		char strNum[1024];
		sprintf_s(strNum, "\n\n--PlayerMoney--\n%d\n", Gold);
		tmp += strNum;
	}

	DEBUGDX(tmp.c_str());
}