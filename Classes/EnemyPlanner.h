////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#ifndef ENEMYPLANNER_H						////
#define ENEMYPLANNER_H						////
////////////////////////////////////////////////

class CActor;
class CEnemy;

class CEnemyPlanner{ //EnemySpecies�̍s���I��AI�p�̃C���^�[�t�F�[�X
public: 
	CEnemyPlanner(std::string _enemyname){
		CONSTRUCTED;
		EnemyName = _enemyname;

		//RandomPlanSet = NUll;
		Actor = NULL;
		PLAYER_NUM = 0;
		ENEMY_NUM = 0;
	}

	~CEnemyPlanner(){
		DESTRUCTED;	
	}

	void BattleReady(const CActor* const* _actorList, const int _playerNum, const int _enemyNum){
		Actor = _actorList;
		PLAYER_NUM = _playerNum;
		ENEMY_NUM = _enemyNum;
	}
	void SetRandomPlanSet(std::map <int, std::vector<std::pair<int, int> > >*_random_plan_set){
		RandomPlanSet = _random_plan_set; 
	}

	std::string GetName() const{return EnemyName;}
	virtual int GetPlan(const CEnemy* _enemy)=0;

protected:
	int CalcRandomPlan(int _randomPlan_key, const CEnemy* _enemy);

	std::map <int, std::vector<std::pair<int, int> > > * RandomPlanSet;

	//�S�A�N�^�[�ւ̃A�N�Z�X���������Ă����i�퓬�J�n���ƂɍX�V�j
		const CActor* const* Actor;
		int PLAYER_NUM;
		int ENEMY_NUM;

private:
	std::string EnemyName;	//��̖��O�B�G���[�o�͗p�ł����Ȃ��B�����ăA�N�Z�X��EnemySpecies����Ȃ���邵�Ăяo������񂪕K�v�Ȏ���this��n���΍ςށB
};


class CEnemyPlanner_DEFAULT: public CEnemyPlanner{
public:
	CEnemyPlanner_DEFAULT(std::string _name):CEnemyPlanner(_name){
		CONSTRUCTED;
	}
	~CEnemyPlanner_DEFAULT(){DESTRUCTED;}

	int GetPlan(const CEnemy* _enemy);	//����
};

class CEnemyPlanner_MYHP: public CEnemyPlanner{
public:
	CEnemyPlanner_MYHP(std::string _name, std::vector<std::string> _argList);
	~CEnemyPlanner_MYHP(){DESTRUCTED;}
	int GetPlan(const CEnemy* _enemy); //����
private:
	std::vector <std::pair<int, int> > planList;
};


class CEnemyPlanner_PLAYERNUM: public CEnemyPlanner{
public:
	CEnemyPlanner_PLAYERNUM(std::string _name, std::vector<std::string> _argList);
	~CEnemyPlanner_PLAYERNUM(){DESTRUCTED;}
	int GetPlan(const CEnemy* _enemy); //����
private:
	int planList[MAX_PLAYER_NUM];
};


////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#endif										////
////////////////////////////////////////////////