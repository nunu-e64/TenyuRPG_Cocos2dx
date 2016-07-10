////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#ifndef ENEMY_H								////
#define ENEMY_H								////
////////////////////////////////////////////////

#include "Actor.h"
#include "Species.h"


class CEnemy : public CActor, public CEnemySpecies{
public:
	
	CEnemy(){
		CONSTRUCTED;
	};
	CEnemy(const CEnemySpecies& obj) : CEnemySpecies(obj){
		CONSTRUCTED;
	};
	~CEnemy(){
		DESTRUCTED;
	}

	void Draw(int _dx=0, int _dy=0);


	void BattleReady(const CActor* const* _actorList, const int _playerNum, const int _enemyNum) {
		AI.BattleReady(_actorList, _playerNum, _enemyNum);
	}

	void AddAttention(int _playerIndex, int _value, CLogWindow* _logWindow=NULL){
		AI.AddAttention(_playerIndex, _value, _logWindow);
	}
	void SetAttention(int _playerIndex, int _value){
		AI.SetAttention(_playerIndex, _value);
	}

	std::vector<std::string> GetDropItemList();		//�퓬�I�����Ƀh���b�v�����A�C�e�������X�g�ɂ��Ď擾


private:
	//�����o�֐�
		bool Plan();
		bool Action();

};



////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#endif										////
////////////////////////////////////////////////
