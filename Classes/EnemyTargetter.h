////多重インクルード防止（インクルードガード）//
#ifndef ENEMYTARGETTER_H					////
#define ENEMYTARGETTER_H					////
////////////////////////////////////////////////

class CActor;
class CEnemy;

class CEnemyTargetter{ //EnemySpeciesのターゲット選択AI用のインターフェース
public: 
	CEnemyTargetter(std::string _enemyname){
		CONSTRUCTED;
		EnemyName = _enemyname;

		Actor = NULL;
		PLAYER_NUM = 0;
		ENEMY_NUM = 0;
		
		for (int i = 0; i<MAX_PLAYER_NUM; i++){
			AttentionRank[i] = 0;
		}
	}

	~CEnemyTargetter(){
		DESTRUCTED;	
	}

	void BattleReady(const CActor* const* _actorList, const int _playerNum, const int _enemyNum){
		Actor = _actorList;
		PLAYER_NUM = _playerNum;
		ENEMY_NUM = _enemyNum;
	}
	void SetAttention(int _attention[]){
		Attention = _attention;
		CalcAttentionRank();

		myLogf("Attention_P", "Targetter:%d", Attention);
	}

	std::string GetName() const{return EnemyName;}
	virtual int GetTarget(const CEnemy* _enemy)=0;
	void CalcAttentionRank();
	int GetAttentionRank(int _key){
		_key = between(0, MAX_PLAYER_NUM, _key);
		return AttentionRank[_key];
	}

protected:

	static const int ATTENTION_RATIO[MAX_PLAYER_NUM];	//EnemyTargetter.cppで定義してます


	//全アクターへのアクセスを持たせておく（戦闘開始ごとに更新）
		const CActor* const* Actor;
		int PLAYER_NUM;
		int ENEMY_NUM;

private:
	std::string EnemyName;	//主の名前。エラー出力用でしかない。だってアクセスはEnemySpeciesからなされるし呼び出し元情報が必要な時はthisを渡せば済む。
	int AttentionRank[MAX_PLAYER_NUM];
	int* Attention;		//本体はCEnemyAIが持つ。SetAttention()でリンクする
};



class CEnemyTargetter_DEFAULT: public CEnemyTargetter{
public:
	CEnemyTargetter_DEFAULT(std::string _name):CEnemyTargetter(_name){
		CONSTRUCTED;
	}
	~CEnemyTargetter_DEFAULT(){DESTRUCTED;}

	int GetTarget(const CEnemy* _enemy);	//共通
};



class CEnemyTargetter_REVERSE: public CEnemyTargetter{
public:
	CEnemyTargetter_REVERSE(std::string _name):CEnemyTargetter(_name){
		CONSTRUCTED;
	};
	~CEnemyTargetter_REVERSE(){DESTRUCTED;}

	int GetTarget(const CEnemy* _enemy); //共通
};


////多重インクルード防止（インクルードガード）//
#endif										////
////////////////////////////////////////////////