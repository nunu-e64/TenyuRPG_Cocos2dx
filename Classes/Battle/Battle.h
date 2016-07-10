////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#ifndef BATTLE_H							////
#define BATTLE_H							////
////////////////////////////////////////////////

#include "BImgBank.h"
#include "LogWindow.h"

#include "../Main/TextBox.h"
#include "../Main/TextWrap.h"
#include "../Main/CmdList.h"
#include "../Main/CmdManager.h"
#include "../Main/WorldManager.h"
#include "../Menu.h"

#include <queue>

class CEveManager;
class CPlayerSpeciesManager;
class CEnemySpeciesManager;
class CTrickManager;

class CActor;
class CPlayer;
class CEnemy;
class CEnemySpecies;

class CBattle : public CWorldManager{
public:
	static CBattle* GetInstance(){
		static CBattle instance;
		return &instance;
	}

	~CBattle(){
		DESTRUCTED;	
	}

	bool Init();
	CMenuNode* GetFieldStatusMenuFrontNode(const char _parentLabel[32]);
	void UpdateFieldPlayerAccesssoryMenu(CMenuNode* _playerNodeParent);
	void UpdatePlayerAccesssoryMenu(CMenuNode* _playerNode);

	void BattleReady(CFlagSet* _flagset_p, CMap* _map_p, CEveManager* _evemanager_p);
	void BattleStart(int* _result, CCmdList* _fieldcmdlist_p);
	void BattleSetting(const char* _wincommand, const char* _losecommand);
	bool CheckEncount(int _mapnum, int _chipnum);
	
	void Draw(bool _screenflip=false, bool _textshowingstop=false, int dx=0, int dy=0, bool _playeralsoshake=false);

	void ChangeTextMode(bool _box, const char* _eventtext = NULL);

	int GetActorNum()const{ return ACTOR_NUM; }

	void SetBackGround(const char* _pickey);
	void SetBackGround(int _mapnum, int _chipnum=-1);

	void SetPlayer();
	void SetPlayer(const int _playerNum, ...);
	void SetEnemy(const int _enemyNum, ...);
	void SetEnemy(std::vector<std::string> _enemyList);
	void SetEnemy(std::vector<CEnemySpecies*> _enemyParty);
	

	//�퓬�p
		void ManageAttack(int _attackerActorIndex, int _targetActorIndex, trick_tag const* _trick);
		void InvokeSideEffect(sideEffect_tag _sideEffect, int _invokerActorIndex, int _cursorTargetActorIndex);

		//�A�e���V�����֌W
		void AddAttention(int _enemyIndex, int _playerIndex, int _value);
		void SetAttention(int _enemyIndex, int _playerIndex, int _value);
	
	//�^�[�Q�b�g�I���}�[�J�[
		class CTargetMarker{
		public:
			CTargetMarker(){
				CONSTRUCTED;	
			}
			~CTargetMarker(){
				DESTRUCTED;	
			}

			void SetImage(int _img);
			void Init(int _actornum, int _playernum, int _enemynum);

			void SetVisible(bool _visible){	Visible = _visible;}
			void SetSide(bool _enemy){		EnemySide = _enemy;}
			bool GetSide(){	return EnemySide;}
			void SetIndex(int _index){		Index = between(0, (EnemySide? ENEMY_NUM: PLAYER_NUM), _index);}
			void SetDeadOk(bool _deadok){	DeadOk = _deadok;}

			void CheckNowIndex(CBattle* _battle);
			void Move(int _dir, CBattle* _battle, int _count=0); 
			void Decide(CBattle* _battle, int _actorIndex);
			void Draw(int dx = 0, int dy = 0);

		private:
			bool Visible;
			int Img;
			int ImgSizeX, ImgSizeY;
			bool EnemySide;	//�G������������
			int Index;		//�G���Ɩ������Ƃ̒ʂ��ԍ�
			bool DeadOk; //���l��I��ΏۂƂ��邩

			int ACTOR_NUM;
			int PLAYER_NUM;
			int ENEMY_NUM;
			int Status; //0...�ӂ�ӂ� 1...�m��Ȃ� 2...�m�艉�o��
		}TargetMarker;

private:
	//�����o�֐�
		void StartEffect();
		int MainLoop();
		void BattleFinish(int &_result, CCmdList* _fieldcmdlist);
		int ResultCheck();

	
	//�����o�ϐ�
		bool Ready;

		//�����C���X�^���X�ƃ|�C���^
			CCmdList B_CmdList;				
			CBattleCmdManager B_CmdManager;	
		
			CTextBox* TextBox;
			CTextBox TextBox1;
			CTextWrap TextWrap1;
			CBattleLog LogWindow;
		
			CEveManager* EveManager_p;	//���b�v���[�h���ɃI���W�i���C�x���g�ǂݍ��ݗp
			CFlagSet* FlagSet_p;

		//Actors
			CPlayer* Player;
			CEnemy* Enemy;
			CActor** Actor;
			int ACTOR_NUM;		//�퓬�ɏo�Ă��邷�ׂĂ̐l���i�G�{�����j
			int PLAYER_NUM;		//�퓬�ɏo�Ă��閡���l��
			int ENEMY_NUM;		//�퓬�ɏo�Ă���G�l��
			//vector<CEnemy>


		//�O���t�B�b�N
			int Img_BattleBackGround;	//�퓬�w�i
			
		//�d�v�ȏ����C���X�^���X
			CBImgBank BImgBank;
			CPlayerSpeciesManager* PlayerSpeciesManager;
			CEnemySpeciesManager* EnemySpeciesManager;
			CTrickManager* TrickManager;	//singleton�g���֌W��|�C���^�ŕێ�
			std::queue <CActor*> ActionQueue;	//�s���ҋ@���X�g

		//�퓬�ݒ�
			char WinCommand[256];
			char LoseCommand[256];

	//�P�ꐫ��ۏ�(Singleton)//////////////////////////////////////////
		CBattle();
		CBattle(const CBattle& hoge);
		CBattle& operator=(const CBattle& hoge);
	//////////////////////////////////////////////////////////////////

};


////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#endif										////
////////////////////////////////////////////////