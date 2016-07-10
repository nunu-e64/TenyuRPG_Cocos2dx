////多重インクルード防止（インクルードガード）//
#ifndef PLAYERSPECIESMANAGER_H				////
#define PLAYERSPECIESMANAGER_H				////
////////////////////////////////////////////////

#include <map>
#include <string>
#include "Species.h"

class CPlayer;
class CAccessoryItem;

class CPlayerSpeciesManager{
public:
	static CPlayerSpeciesManager* GetInstance(){
		static CPlayerSpeciesManager instance;
		return &instance;
	}
	~CPlayerSpeciesManager(){
		DESTRUCTED;		
	}

	void Clear();

	bool CPlayerSpeciesManager::CheckAfterLoad();	//必要な読み込み処理がすべて完了しているかの確認

	bool CreateSpecies(const char* _name, int _level, int _geneMaxHp, int _geneBaseTrickPower, int _geneAtk, int _geneDef, int _geneSpd, int _img, const trick_tag* _baseTrick);
	bool SetTrickList(const char* _name, std::vector <trick_tag const*> _trickList);
	CPlayerSpecies* GetSpecies(const char* _name);
	CPlayerSpecies* GetSpecies(int _index);

	bool SetMemberList(int _index, const char* _name);
	bool SetMemberList();
	int GetMemberListSize()const{return MemberList.size();}

	void CopyValue(int PLAYER_NUM, CPlayer* _player); 

	void AddExp(int _exp);

	bool SetAccessory(std::string _playerName, int _slot, std::string _accessoryItemName);
	CAccessoryItem* GetAccessory(std::string _playerName, int _slot);

private:
	bool PlayerBankLock;
	std::map <std::string, CPlayerSpecies> PlayerBank;
	CPlayerSpecies DummyPlayer;

	std::vector <CPlayerSpecies*> MemberList;		//プレイヤーリスト。0~MAX_PLAYER_NUM-1	が戦闘に出る

	//単一性を保証(Singleton)//////////////////////////////////////////
		CPlayerSpeciesManager(){
			CONSTRUCTED;
			PlayerBankLock = false;
		}
		CPlayerSpeciesManager(const CPlayerSpeciesManager& hoge);
		CPlayerSpeciesManager& operator=(const CPlayerSpeciesManager& hoge);
	//////////////////////////////////////////////////////////////////

};
////多重インクルード防止（インクルードガード）//
#endif										  //
////////////////////////////////////////////////