////多重インクルード防止（インクルードガード）//
#ifndef ENEMYAI_H						////
#define ENEMYAI_H						////
////////////////////////////////////////////////

class CActor;
class CEnemy;
class CLogWindow;

#include "EnemyPlanner.h"
#include "EnemyTargetter.h"

class CEnemyAI{		//EnemySpeciesが持つ、plannerやTargeterを束ねる
public: 
	CEnemyAI(){
		CONSTRUCTED;
		Actor = NULL;
		PLAYER_NUM = 0;
		ENEMY_NUM = 0;

		for (int i = 0; i<MAX_PLAYER_NUM; i++){
			Attention[i] = 0;
			AttentionEffectCount[i] = 0;
		}

		//DEBUG:アテンション初期値////////////////
			Attention[0] = 5;
			Attention[1] = 5;
			Attention[2] = 5;
		//////////////////////////
	}

	~CEnemyAI(){
		DESTRUCTED;	
	}

	void BattleReady(const CActor* const* _actorList, const int _playerNum, const int _enemyNum){
		Planner->BattleReady(_actorList, _playerNum, _enemyNum);
		Targetter->BattleReady(_actorList, _playerNum, _enemyNum);

		Targetter->SetAttention(Attention);
		myLogf("Attention_P", "EnemyAI:%d", Attention);

		Actor = _actorList;
		PLAYER_NUM = _playerNum;
		ENEMY_NUM = _enemyNum;
	}
	CEnemyPlanner* SetPlanner(CEnemyPlanner* _planner){
		Planner = _planner;
		Planner->SetRandomPlanSet(&RandomPlanSet);
		return _planner;
	}
	CEnemyTargetter* SetTargetter(CEnemyTargetter* _targetter){
		Targetter = _targetter;

		//Targetter->SetAttention(Attention);	ここでアドレスを渡してしまうと戦闘開始時にEnemy生成の際に配列がコピーされてアドレスが変わるため同期が取れなくなる
		//myLogf("Attention_P", "EnemyAI:%d", Attention);

		return _targetter;
	}
	int GetPlan(const CEnemy* _enemy);
	int GetTarget(const CEnemy* _enemy);

	bool AddRandomPlanSet(const unsigned int _index, std::vector<std::pair<int, int> > _planList, bool _clear=false);
	
	void AddAttention(int _playerIndex, int _value, CLogWindow* _logWindow = NULL);
	void SetAttention(int _playerIndex, int _value);

	static void SetAttentionImg(int* _markerImg, int _boardImg, int _effectImg);	
	void Draw(const CEnemy* _enemy);
	

private:

	CEnemyPlanner* Planner;
	CEnemyTargetter* Targetter;

	std::map <int, std::vector<std::pair<int, int> > > RandomPlanSet;
		//RandomPlanSet[index] = (choice, probability)
		//行動選択肢とその発動比を並べたリスト。
		//行動計算の為にAI.plannerとAI.Targetにポインタを渡しておく

	int Attention[MAX_PLAYER_NUM];	//Enemyの各プレイヤーに対する注目度を示す
	int AttentionEffectCount[MAX_PLAYER_NUM];	//アテンションが変化したときのエフェクト用フラグ兼描画カウンタ	変化なし0, UP:+, DOWN:-
	enum{EFFECT_COUNT = 60};

	//全アクターへのアクセスを持たせておく（戦闘開始ごとに更新）（EnemyAIは橋渡しなので実際に持つ必要はない）
		const CActor* const* Actor;
		int PLAYER_NUM;
		int ENEMY_NUM;

	static int AttentionMarkerImg[MAX_PLAYER_NUM];
	static int AttentionBoardImg;
	static int AttentionEffectImg;

};

////多重インクルード防止（インクルードガード）//
#endif										////
////////////////////////////////////////////////