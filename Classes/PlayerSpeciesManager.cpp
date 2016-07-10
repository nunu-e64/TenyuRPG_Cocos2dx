#include "PlayerSpeciesManager.h"
#include "Player.h"
#include "Item.h"

void CPlayerSpeciesManager::Clear(){
	PlayerBank.clear(); 
	PlayerBankLock = false;
	MemberList.clear();

}

bool CPlayerSpeciesManager::CreateSpecies(const char* _name, int _level, int _geneMaxHp, int _geneBaseTrickPower, int _geneAtk, int _geneDef, int _geneSpd, int _img, const trick_tag* _baseTrick) {
	CPlayerSpecies newPlayer;
	newPlayer.SetValue(_name, _level, _geneMaxHp, _geneAtk, _geneDef, _geneSpd);
	newPlayer.Img = _img;

	//普通攻撃をTrickListから中身ごとコピーしてくる。Powerの計算と代入はレベルに左右されるのでPlayerをインスタンス化したときに行う
		bool valueChecker;
		newPlayer.BaseTrickPowerGene = between(1, 5, _geneBaseTrickPower, &valueChecker);
		if (!valueChecker) WARNINGDX("ValueWarning(BaseTrickPowerGene):%d", _geneBaseTrickPower);

		if (_baseTrick != NULL) newPlayer.BaseTrick = *_baseTrick;	//通常技登録
		else newPlayer.BaseTrick.Name[0] = '\0';

	if (PlayerBankLock) {
		WARNINGDX("'%s':PlayerBank is Locked!", _name);
		return false;
	}

	if (PlayerBank.find(_name)==PlayerBank.end()){
		PlayerBank.insert( std::map<std::string, CPlayerSpecies>::value_type( _name, newPlayer) );
		return true;
	}else{
		ErrorDx("Error->Already existed PlayerName:%s", _name);
		return false;
	}
}

bool CPlayerSpeciesManager::SetTrickList(const char* _name, std::vector <trick_tag const*> _trickList){
	if (PlayerBank.find(_name)!=PlayerBank.end()){
		PlayerBank[_name].TrickList = _trickList;
		return true;
	}else{
		ErrorDx("Error->Not Found Player. Name:%s", _name);
		return false;
	}
}

bool CPlayerSpeciesManager::SetMemberList(int _index, const char* _name){
	if (_index<0){
		ErrorDx("Error->SetMemberList->size error:%d", _index);
		return false;
	}else if (_index>=(int)(MemberList.size())){
		MemberList.push_back(GetSpecies(_name));
	}else{
		MemberList[_index] = GetSpecies(_name);
	}
	return true;
}
bool CPlayerSpeciesManager::SetMemberList(){	//パーティメンバーリストの初期化。戦闘に出ないであろうメンバーにも番号は振っておく
	MemberList.clear();
	std::map<std::string, CPlayerSpecies>::iterator it=PlayerBank.begin();
	if (it!=PlayerBank.end()) PlayerBankLock = true;
	while( it != PlayerBank.end()){
		MemberList.push_back(&(*it).second);
		++it;
	}
	return true;
}

CPlayerSpecies* CPlayerSpeciesManager::GetSpecies(const char* _name){
	if (PlayerBank.find(_name)!=PlayerBank.end()){
		PlayerBankLock = true;	//以後PlayerBankの要素アドレスが変わらないように要素の追加を禁じる
		return &PlayerBank[_name];
	}else{
		ErrorDx("Error->PlayerSpeciesManager->GetPlayerSpecies->NotFound:%s", __FILE__, __LINE__, _name);
		return &DummyPlayer;
	}
}
CPlayerSpecies* CPlayerSpeciesManager::GetSpecies(int _index){
	if (_index<0 || _index>=(int)(MemberList.size())){
		ErrorDx("Error->MemberList size error:%d", _index);
		return &DummyPlayer;
	}else{
		PlayerBankLock = true;
		return MemberList[_index];
	}

}


void CPlayerSpeciesManager::CopyValue(int PLAYER_NUM, CPlayer* _player){

	//戦闘終了後、PlayerBankに保存すべき情報をコピーする

	for (int i=0; i<PLAYER_NUM; i++){
		//PlayerBank[_player[i].GetName()].Hp = _player[i].Hp;
		PlayerBank[_player[i].GetName()].MagicCount = _player[i].MagicCount;
		PlayerBank[_player[i].GetName()].Exp = _player[i].Exp;
	}

}

void CPlayerSpeciesManager::AddExp(int _exp) {

	//HACK: 経験地分配についてはベンチの仲間にも分配するのかなど、要検討
	_exp = max(0, _exp);
	for (unsigned int i = 0; i < MemberList.size(); i++) {
		MemberList[i]->Exp += _exp;
	}

}

bool CPlayerSpeciesManager::CheckAfterLoad(){
	bool forReturn = true;

	std::map <std::string, CPlayerSpecies> ::iterator it = PlayerBank.begin();
	while(it!=PlayerBank.end()){
		if ((*it).second.TrickList.empty()) {
			ERRORDX("%s: Set Trick List for All Player!",(*it).second.GetName().c_str());
			forReturn = false;
		}
		++it;
	}

	return forReturn;
}

bool CPlayerSpeciesManager::SetAccessory(std::string _playerName, int _slot, std::string _accessoryItemName) {

	if (_slot < MAX_ACCESSORY_SLOT) {

		//DEBUGDX("SetAccessory:%s,%d,%s", _playerName.c_str(), _slot, _accessoryItemName.c_str());

		CPlayerSpecies* player = GetSpecies(_playerName.c_str());
		CItemManager* itemManager = CItemManager::GetInstance();

		if (_accessoryItemName.length() == 0){	//装備なしを選択＝装備をはずす
			
			//元々装備していたアイテムをバッグに返す
			if (player->AccessoryList[_slot].length() > 0) {
				itemManager->IncPlayerItem(player->AccessoryList[_slot], 1);
			}
			player->AccessoryList[_slot] = "";

			return true;

		} else if (itemManager->GetAccessoryItem(_accessoryItemName) != NULL) {	//装備アイテムを選択＝装備を交換
			
			if (itemManager->GetPlayerItemNum(_accessoryItemName) > 0) {
				
				std::string oldAcce = player->AccessoryList[_slot];

				//新しく装備するアイテムをバッグからもらう
				itemManager->DecPlayerItem(_accessoryItemName, 1);
				player->AccessoryList[_slot] = _accessoryItemName;

				//元々装備していたアイテムをバッグに返す
				if (oldAcce.length() > 0) {
					itemManager->IncPlayerItem(oldAcce, 1);
				}				

				return true;
			} else {
				ERRORDX("You can't set item which you don't have in bag.:%s", _accessoryItemName.c_str());
				return false;
			}
		} else {
			return false;
		}

	} else {
		ERRORDX("Slot Number:%d is too large. MAX_ACCESSORY_SLOT is %d.", _slot, MAX_ACCESSORY_SLOT);
		return false;
	}
}

CAccessoryItem* CPlayerSpeciesManager::GetAccessory(std::string _playerName, int _slot) {

	if (_slot < MAX_ACCESSORY_SLOT) {

		CPlayerSpecies* player = GetSpecies(_playerName.c_str());
		if (player != &DummyPlayer && !player->AccessoryList[_slot].empty()) {
			return CItemManager::GetInstance()->GetAccessoryItem(player->AccessoryList[_slot]);
		} else {
			return NULL;
		}

	} else {
		ERRORDX("Slot Number:%d is too large. MAX_ACCESSORY_SLOT is %d.", _slot, MAX_ACCESSORY_SLOT);
		return NULL;
	}
}
