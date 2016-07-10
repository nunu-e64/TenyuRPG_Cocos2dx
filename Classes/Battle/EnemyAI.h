////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#ifndef ENEMYAI_H						////
#define ENEMYAI_H						////
////////////////////////////////////////////////

class CActor;
class CEnemy;
class CLogWindow;

#include "EnemyPlanner.h"
#include "EnemyTargetter.h"

class CEnemyAI{		//EnemySpecies�����Aplanner��Targeter�𑩂˂�
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

		//DEBUG:�A�e���V���������l////////////////
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

		//Targetter->SetAttention(Attention);	�����ŃA�h���X��n���Ă��܂��Ɛ퓬�J�n����Enemy�����̍ۂɔz�񂪃R�s�[����ăA�h���X���ς�邽�ߓ��������Ȃ��Ȃ�
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
		//�s���I�����Ƃ��̔��������ׂ����X�g�B
		//�s���v�Z�ׂ̈�AI.planner��AI.Target�Ƀ|�C���^��n���Ă���

	int Attention[MAX_PLAYER_NUM];	//Enemy�̊e�v���C���[�ɑ΂��钍�ړx������
	int AttentionEffectCount[MAX_PLAYER_NUM];	//�A�e���V�������ω������Ƃ��̃G�t�F�N�g�p�t���O���`��J�E���^	�ω��Ȃ�0, UP:+, DOWN:-
	enum{EFFECT_COUNT = 60};

	//�S�A�N�^�[�ւ̃A�N�Z�X���������Ă����i�퓬�J�n���ƂɍX�V�j�iEnemyAI�͋��n���Ȃ̂Ŏ��ۂɎ��K�v�͂Ȃ��j
		const CActor* const* Actor;
		int PLAYER_NUM;
		int ENEMY_NUM;

	static int AttentionMarkerImg[MAX_PLAYER_NUM];
	static int AttentionBoardImg;
	static int AttentionEffectImg;

};

////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#endif										////
////////////////////////////////////////////////