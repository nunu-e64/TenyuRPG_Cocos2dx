#include "EnemyPlanner.h"
#include "Enemy.h"


int CEnemyPlanner::CalcRandomPlan(int _randomPlan_key, const CEnemy* _enemy){

	if (RandomPlanSet==NULL) {
		WARNINGDX("'%s':RandomPlanSet == NULL!(return -1)", _enemy->GetName().c_str());
		return -1;
	} else if(RandomPlanSet->empty()) {
		WARNINGDX("'%s':RandomPlanSet == empty (return -1)", _enemy->GetName().c_str());
		return -1;
	}

	if ((*RandomPlanSet).find(_randomPlan_key)==(*RandomPlanSet).end()) {
		ERRORDX("'%s':Not Found RandomPlan :%d (return -1)", _enemy->GetName().c_str(), _randomPlan_key);
		return -1;

	} else {
		int probability=0;
		int plan = -1;

		for (unsigned int i=0; i<(*RandomPlanSet)[_randomPlan_key].size(); i++){
			probability += (*RandomPlanSet)[_randomPlan_key][i].second;
		    if ((rand()%1000)/(double)1000 * probability < (*RandomPlanSet)[_randomPlan_key][i].second){
				plan = (*RandomPlanSet)[_randomPlan_key][i].first;
			}
		}
	
		return plan;
	}
}

int CEnemyPlanner_DEFAULT::GetPlan(const CEnemy* _enemy){
	if (RandomPlanSet->size()>1) {
		WARNINGDX("%s:RandomPlanSet.size(%d)>1, but EnemyPlanner is still DEFAULT. DEFAULT Planner doesn't use RandomPlan[1~].", _enemy->GetName().c_str(), RandomPlanSet->size());
	} else if(RandomPlanSet->empty()) {
		WARNINGDX("'%s':RandomPlanSet == empty (return -1)", _enemy->GetName().c_str());
		return -1;
	} 

	return CalcRandomPlan(RandomPlanSet->begin()->first, _enemy);
}

CEnemyPlanner_MYHP::CEnemyPlanner_MYHP(std::string _name, std::vector<std::string> _argList):CEnemyPlanner(_name){
	CONSTRUCTED;
	std::vector <int> numset;
	for (unsigned int i=0; i<_argList.size(); i++){
		int num;
		if(!( mystrtol(_argList[i].c_str(), &num))){
			ErrorDx("Error->CEnemyPlanner_MYHP(%s)->Check argument type[%d]", __FILE__, __LINE__, _name.c_str(), i);
			num = -1;
		}
		numset.push_back(num);
	}
	if (numset.size()%2==1) numset.push_back(0);	//引数が奇数の場合、最後尾に0%を追加

	for (unsigned int i=0; i<numset.size()/2; i++){
		planList.push_back( std::pair<int,int>(numset[i*2],numset[i*2+1]));
	}

	//ログ出力
	//for (unsigned int i=0; i<planList.size(); i++){
	//	myLog("%s:MYHP_AIList[%d]=(%d,%d)", _name.c_str(), i, planList[i].first, planList[i].second);
	//}
	//for (unsigned int i=0; i<_randomPlanSet->size(); i++){
	//	for (unsigned int j=0; j<(*_randomPlanSet)[i].size(); j++){
	//		myLog("%s:RandomPlan[%d][%d]=(%d,%d)", _name.c_str(),i,j,(*_randomPlanSet)[i][j].first, (*_randomPlanSet)[i][j].second);
	//	}
	//}
}
int CEnemyPlanner_MYHP::GetPlan(const CEnemy* _enemy){
	//ここに実際のあれこれを書く
	//planList･･･[0~n] first:TrickIndex, second:境界線となるHPパーセント

	int ratio = (int)(100 * (double)_enemy->GetHp()/_enemy->GetMaxHp());
	int plan = -1;

	for (unsigned int i=0; i<planList.size(); i++){
		if (ratio > planList[i].second || i == planList.size()-1){
			plan = CalcRandomPlan(planList[i].first, _enemy);
			return plan;
		}
	}

	return -1;
}

CEnemyPlanner_PLAYERNUM::CEnemyPlanner_PLAYERNUM(std::string _name, std::vector<std::string> _argList):CEnemyPlanner(_name){
	CONSTRUCTED;
	std::vector <int> numset;
	for (unsigned int i=0; i<_argList.size(); i++){
		int num;
		if(!( mystrtol(_argList[i].c_str(), &num))){
			ErrorDx("Error->CEnemyPlanner_PLAYERNUM(%s)->Check argument type[%d]", __FILE__, __LINE__, _name.c_str(), i);
			num = -1;
		}
		numset.push_back(num);
	}

	if ((unsigned int)MAX_PLAYER_NUM > numset.size()){
		WarningDx("Warning->AI_'PLAYERNUM'('%s')->ArgumentNum is smaller than MAX_PLAYER_NUM", _name.c_str());
	}else if((unsigned int)MAX_PLAYER_NUM < numset.size()){
		WarningDx("Warning->AI_'PLAYERNUM'('%s')->ArgumentNum is bigger than MAX_PLAYER_NUM", _name.c_str());
	}

	for (unsigned int i=0; i<MAX_PLAYER_NUM; i++){
		if (i < numset.size()){
			planList[i] = numset[i];
		}else{
			planList[i] = -1;
		}
	}
}
int CEnemyPlanner_PLAYERNUM::GetPlan(const CEnemy* _enemy){
	//ここに実際のあれこれを書く

	int alivePlayerNum = 0;
	for (int i=0; i<PLAYER_NUM+ENEMY_NUM ; i++){
		if (Actor[i]->IsPlayer() && Actor[i]->GetAlive()) ++alivePlayerNum;
	}

	int plan = -1;
	if(alivePlayerNum == 0){
		plan = -1;
	}else{
		plan = CalcRandomPlan(planList[alivePlayerNum-1], _enemy);
	}

	return plan;
}
