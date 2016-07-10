////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#ifndef ENEMYTARGETTER_H					////
#define ENEMYTARGETTER_H					////
////////////////////////////////////////////////

class CActor;
class CEnemy;

class CEnemyTargetter{ //EnemySpecies�̃^�[�Q�b�g�I��AI�p�̃C���^�[�t�F�[�X
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

	static const int ATTENTION_RATIO[MAX_PLAYER_NUM];	//EnemyTargetter.cpp�Œ�`���Ă܂�


	//�S�A�N�^�[�ւ̃A�N�Z�X���������Ă����i�퓬�J�n���ƂɍX�V�j
		const CActor* const* Actor;
		int PLAYER_NUM;
		int ENEMY_NUM;

private:
	std::string EnemyName;	//��̖��O�B�G���[�o�͗p�ł����Ȃ��B�����ăA�N�Z�X��EnemySpecies����Ȃ���邵�Ăяo������񂪕K�v�Ȏ���this��n���΍ςށB
	int AttentionRank[MAX_PLAYER_NUM];
	int* Attention;		//�{�̂�CEnemyAI�����BSetAttention()�Ń����N����
};



class CEnemyTargetter_DEFAULT: public CEnemyTargetter{
public:
	CEnemyTargetter_DEFAULT(std::string _name):CEnemyTargetter(_name){
		CONSTRUCTED;
	}
	~CEnemyTargetter_DEFAULT(){DESTRUCTED;}

	int GetTarget(const CEnemy* _enemy);	//����
};



class CEnemyTargetter_REVERSE: public CEnemyTargetter{
public:
	CEnemyTargetter_REVERSE(std::string _name):CEnemyTargetter(_name){
		CONSTRUCTED;
	};
	~CEnemyTargetter_REVERSE(){DESTRUCTED;}

	int GetTarget(const CEnemy* _enemy); //����
};


////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#endif										////
////////////////////////////////////////////////