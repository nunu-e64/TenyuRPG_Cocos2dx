////多重インクルード防止（インクルードガード）//
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
	

	//戦闘用
		void ManageAttack(int _attackerActorIndex, int _targetActorIndex, trick_tag const* _trick);
		void InvokeSideEffect(sideEffect_tag _sideEffect, int _invokerActorIndex, int _cursorTargetActorIndex);

		//アテンション関係
		void AddAttention(int _enemyIndex, int _playerIndex, int _value);
		void SetAttention(int _enemyIndex, int _playerIndex, int _value);
	
	//ターゲット選択マーカー
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
			bool EnemySide;	//敵側か味方側か
			int Index;		//敵ごと味方ごとの通し番号
			bool DeadOk; //死人を選択対象とするか

			int ACTOR_NUM;
			int PLAYER_NUM;
			int ENEMY_NUM;
			int Status; //0...ふよふよ 1...確定つなぎ 2...確定演出中
		}TargetMarker;

private:
	//メンバ関数
		void StartEffect();
		int MainLoop();
		void BattleFinish(int &_result, CCmdList* _fieldcmdlist);
		int ResultCheck();

	
	//メンバ変数
		bool Ready;

		//所持インスタンスとポインタ
			CCmdList B_CmdList;				
			CBattleCmdManager B_CmdManager;	
		
			CTextBox* TextBox;
			CTextBox TextBox1;
			CTextWrap TextWrap1;
			CBattleLog LogWindow;
		
			CEveManager* EveManager_p;	//ラップモード時にオリジナルイベント読み込み用
			CFlagSet* FlagSet_p;

		//Actors
			CPlayer* Player;
			CEnemy* Enemy;
			CActor** Actor;
			int ACTOR_NUM;		//戦闘に出ているすべての人数（敵＋味方）
			int PLAYER_NUM;		//戦闘に出ている味方人数
			int ENEMY_NUM;		//戦闘に出ている敵人数
			//vector<CEnemy>


		//グラフィック
			int Img_BattleBackGround;	//戦闘背景
			
		//重要な所持インスタンス
			CBImgBank BImgBank;
			CPlayerSpeciesManager* PlayerSpeciesManager;
			CEnemySpeciesManager* EnemySpeciesManager;
			CTrickManager* TrickManager;	//singleton使う関係上ポインタで保持
			std::queue <CActor*> ActionQueue;	//行動待機リスト

		//戦闘設定
			char WinCommand[256];
			char LoseCommand[256];

	//単一性を保証(Singleton)//////////////////////////////////////////
		CBattle();
		CBattle(const CBattle& hoge);
		CBattle& operator=(const CBattle& hoge);
	//////////////////////////////////////////////////////////////////

};


////多重インクルード防止（インクルードガード）//
#endif										////
////////////////////////////////////////////////