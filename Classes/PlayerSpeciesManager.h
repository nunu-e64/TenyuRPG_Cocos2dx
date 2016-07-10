////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
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

	bool CPlayerSpeciesManager::CheckAfterLoad();	//�K�v�ȓǂݍ��ݏ��������ׂĊ������Ă��邩�̊m�F

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

	std::vector <CPlayerSpecies*> MemberList;		//�v���C���[���X�g�B0~MAX_PLAYER_NUM-1	���퓬�ɏo��

	//�P�ꐫ��ۏ�(Singleton)//////////////////////////////////////////
		CPlayerSpeciesManager(){
			CONSTRUCTED;
			PlayerBankLock = false;
		}
		CPlayerSpeciesManager(const CPlayerSpeciesManager& hoge);
		CPlayerSpeciesManager& operator=(const CPlayerSpeciesManager& hoge);
	//////////////////////////////////////////////////////////////////

};
////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#endif										  //
////////////////////////////////////////////////