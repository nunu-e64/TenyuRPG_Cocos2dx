#include "EnemySpeciesManager.h"


void CEnemySpeciesManager::Clear(){

	for(unsigned int i=0; i<EnemyPlannerBank.size(); i++){	
		myLog("deleting EnemyPlannerBank[%d]:%s...", i, EnemyPlannerBank[i]->GetName().c_str());
		delete EnemyPlannerBank[i];	
	}
	for(unsigned int i=0; i<EnemyTargetterBank.size(); i++){	
		myLog("deleting EnemyTargetterBank[%d]:%s...", i, EnemyTargetterBank[i]->GetName().c_str());
		delete EnemyTargetterBank[i];	
	}

	myLog("clearing EnemyPlannerBank...");
	EnemyPlannerBank.clear();

	myLog("clearing EnemyTargetterBank...");
	EnemyTargetterBank.clear();

	myLog("clearing EnemyBank...");
	EnemyBank.clear();
	EnemyBankLock = false;

	std::map <int, std::map<int, encount_tag> >::iterator it = MapEncount.begin();
	while(!MapEncount.empty() && it!=MapEncount.end()){
		(*it).second.clear();
		++it;
	}
	MapEncount.clear();

}

bool CEnemySpeciesManager::CreateSpecies(const char* _name, int _level, int _geneMaxHp, int _geneAtk, int _geneDef, int _geneSpd, int _img) {
	CEnemySpecies newEnemy;
	newEnemy.SetValue(_name, _level, _geneMaxHp, _geneAtk, _geneDef, _geneSpd);
	newEnemy.Img = _img;
	
	if (EnemyBankLock) {
		WARNINGDX("'%s':EnemyBank is Locked!", _name);
		return false;
	}

	if (EnemyBank.find(_name)==EnemyBank.end()){
		
		EnemyBank.insert( std::map<std::string, CEnemySpecies>::value_type( _name, newEnemy) );

		EnemyPlannerBank.push_back( EnemyBank[_name].AI.SetPlanner(
			new CEnemyPlanner_DEFAULT(_name) ) );
		myLog("EnemyPlannerBank.push_back(newPlanner_DEFAULT):%s", _name);

		EnemyTargetterBank.push_back( EnemyBank[_name].AI.SetTargetter(
			new CEnemyTargetter_DEFAULT(_name) ) );

		myLog("EnemyTergetterBank.push_back(newTergetter_DEFAULT):%s", _name);

		return true;

	}else{

		ErrorDx("Error->Already existed EnemyName:%s", _name);
		return false;

	}
}

bool CEnemySpeciesManager::SetTrickList(const char* _name, std::vector <trick_tag const*> _trickList){
	if (EnemyBank.find(_name)!=EnemyBank.end()){
		if (EnemyBank[_name].TrickList.empty()) {
			EnemyBank[_name].TrickList = _trickList;

			std::vector <std::pair<int,int> > tmpRandomPlan;
			for (unsigned int i=0; i<_trickList.size(); i++){
				tmpRandomPlan.push_back(std::pair <int, int> (i, 1));
			}

			AddRandomPlanSet(_name, 0, tmpRandomPlan, true);	

			return true;

		} else {
			ERRORDX("%s:Already Set TrickSet", _name);
			return false;
		}

	}else{
		ErrorDx("Error->%s->Not Found Enemy. Name:%s", __FUNCTION__, _name);
		return false;
	}
}

bool CEnemySpeciesManager::SetDropItemList(const char* _name, std::vector <std::pair<std::string, int> > _dropItemList) {

	if (EnemyBank.find(_name) != EnemyBank.end()) {

		if (EnemyBank[_name].DropItemList.empty()) {
			EnemyBank[_name].DropItemList = _dropItemList;
			return true;

		} else {
			ERRORDX("Enemy[%s]:Already Set DropItemSet", _name);
			return false;
		}

	} else {
		ERRORDX("Not Found Enemy Name:%s", _name);
		return false;
	}
}


bool CEnemySpeciesManager::AddRandomPlanSet(const char* _name, unsigned int _index, std::vector<std::pair<int, int> > _planList, bool _defaultPlan){

	if (EnemyBank.find(_name)!=EnemyBank.end()){
		if (EnemyBank[_name].TrickList.empty()) {
			ERRORDX("%s:Set TrickList before AddRandomPlanSet", _name);
			return false;

		} else if (EnemyBank[_name].AI.AddRandomPlanSet(_index, _planList, !_defaultPlan && !EnemyBank[_name].FirstRandomPlanSettingFlag)) {
			if (!_defaultPlan) EnemyBank[_name].FirstRandomPlanSettingFlag = true;
			return true;

		} else {
			ErrorDx("Error->%s.AddRandomPlanSet has failed. :%d", _name, (int)_index);	
			return false;
		}

	}else{
		ERRORDX("Not Found Enemy. Name:%s", _name);
		return false;
	}
}

bool CEnemySpeciesManager::SetEnemyPlanner(std::string _enemyName, std::string _typeName, std::vector<std::string> _argList){	
	bool forReturn = true;
	
	//SetEnemyPlannerは上書き可能
	if (EnemyBank.find(_enemyName)!=EnemyBank.end()){
		
		if (_typeName=="MYHP"){
			EnemyPlannerBank.push_back( EnemyBank[_enemyName].AI.SetPlanner(
				new CEnemyPlanner_MYHP(_enemyName, _argList) ) );

		}else if(_typeName=="PLAYERNUM"){
			EnemyPlannerBank.push_back( EnemyBank[_enemyName].AI.SetPlanner(
				new CEnemyPlanner_PLAYERNUM(_enemyName, _argList) ) );

		}else if (_typeName=="DEFAULT"){
			EnemyPlannerBank.push_back( EnemyBank[_enemyName].AI.SetPlanner(
				new CEnemyPlanner_DEFAULT(_enemyName) ) );

		}else{
			ErrorDx("Error->PlannerTypeName does't match any PlanTypes :%s:%s", _typeName.c_str(), _enemyName.c_str());
			forReturn = false;
		}
	
		if (forReturn) myLog("EnemyPlannerBank.push_back(newPlanner_%s):%s", _typeName.c_str(), _enemyName.c_str());

	}else{
		ErrorDx("Error->%s->Not Found Enemy. Name:%s", __FUNCTION__, _enemyName.c_str());
		forReturn = false;
	}

	return forReturn;
}

bool CEnemySpeciesManager::SetEnemyTargetter(std::string _enemyName, std::string _typeName, std::vector<std::string> _argList){	
	bool forReturn = true;
	
	//SetEnemyTargetterは上書き可能
	if (EnemyBank.find(_enemyName)!=EnemyBank.end()){
		
		if (_typeName=="REVERSE"){
			EnemyTargetterBank.push_back( EnemyBank[_enemyName].AI.SetTargetter(
				new CEnemyTargetter_REVERSE(_enemyName) ) );

		}else if (_typeName=="DEFAULT"){
			EnemyTargetterBank.push_back( EnemyBank[_enemyName].AI.SetTargetter(
				new CEnemyTargetter_DEFAULT(_enemyName) ) );

		}else{
			ErrorDx("Error->TargetterTypeName does't match any TargetTypes :%s:%s", _typeName.c_str(), _enemyName.c_str());
			forReturn = false;
		}
	
		if (forReturn) myLog("EnemyTargetterBank.push_back(newTargetter_%s):%s", _typeName.c_str(), _enemyName.c_str());

	}else{
		ErrorDx("Error->%s->Not Found Enemy. Name:%s", __FUNCTION__, _enemyName.c_str());
		forReturn = false;
	}

	return forReturn;
}


CEnemySpecies* CEnemySpeciesManager::GetSpecies(const char* _name){	
	if (EnemyBank.find(_name)!=EnemyBank.end()){
		EnemyBankLock = true;
		return &EnemyBank[_name];
	}else{
		ErrorDx("EnemySpeciesManager->GetEnemySpecies->NotFound:%s", __FILE__, __LINE__, _name);
		return &Dammy_Enemy;
	}
}

bool CEnemySpeciesManager::SetMapEncount(int _mapnum, int _chipnum, int _encount){

	MAP_MAX_CHECK(_mapnum, false);

	if (_chipnum<0 || _chipnum>255){
		ErrorDx("Error->_chipnum should between(0,255) :%d", __FILE__, __LINE__, _chipnum);
		return false;
	}
	if (_encount<0 || _encount>1000){
		ErrorDx("Error->_encount should between(0,1000) :%d", __FILE__, __LINE__, _encount);
		return false;
	}

	MapEncount[_mapnum][_chipnum].encount = _encount;
	//DebugDx("MapEncount[_mapnum][_chipnum].encount:%d", MapEncount[_mapnum][_chipnum].encount);
	return true;

}
bool CEnemySpeciesManager::AddMapEncountParty(int _mapnum, int _chipnum, int _encount, std::vector<std::string> _party){

	MAP_MAX_CHECK(_mapnum, false);

	if (_chipnum<0 || _chipnum>255){
		ErrorDx("Error->_chipnum should between(0,255) :%d", __FILE__, __LINE__, _chipnum);
		return false;
	}

	encount_tag::party_tag tmp;
	
	for (unsigned int i=0; i<_party.size(); i++){
		tmp.party.push_back(GetSpecies(_party[i].c_str()));
		//DebugDx("tmp.GetName:%s", tmp.party[i]->GetName().c_str());
	}
	tmp.per = _encount;

	MapEncount[_mapnum][_chipnum].partyset.push_back(tmp) ;

	if (MapEncount[_mapnum][_chipnum].encount < 0 || MapEncount[_mapnum][_chipnum].encount >1000){
		MapEncount[_mapnum][_chipnum].encount = 0;
	}
	return true;
}

bool CEnemySpeciesManager::CheckEncount(int _mapnum, int _chipnum, std::vector<CEnemySpecies*> &_party_p){

	MAP_MAX_CHECK(_mapnum, false);
	if (_chipnum<0 || _chipnum>255){
		ErrorDx("Error->_chipnum should between(0,255) :%d", __FILE__, __LINE__, _chipnum);
		return false;
	}

		if (MapEncount.find(_mapnum)==MapEncount.end()) return false;
		if (MapEncount[_mapnum].find(_chipnum)==MapEncount[_mapnum].end()) return false;

	///////////////////////////////////////////////////////////////////////////////////////

	if (MapEncount[_mapnum][_chipnum].encount > rand()%1000){
		//エンカウント決定したので戦う敵パーティを決定

		int probability=0;
		int partynum = -1;
		for (unsigned int i = 0; i < MapEncount[_mapnum][_chipnum].partyset.size(); i++){
			probability += MapEncount[_mapnum][_chipnum].partyset[i].per;
		    if ((rand()%1000)/(double)1000 * probability < MapEncount[_mapnum][_chipnum].partyset[i].per){
				partynum = i;
			}
		}
		//DebugDx("MapEncount[_mapnum][_chipnum].encount:%d",MapEncount[_mapnum][_chipnum].encount);
		//DebugDx("EnemySpeciesManager:EncountPartySetSize:%d",MapEncount[_mapnum][_chipnum].partyset.size());
		//DebugDx("EnemySpeciesManager:EncountPartyNum:%d",partynum);

		if (partynum!=-1){
			_party_p = MapEncount[_mapnum][_chipnum].partyset[partynum].party;
			return true;
		}else{
			return false;
		}

	}else{
		return false;
	}
}

bool CEnemySpeciesManager::CheckAfterLoad(){
	bool forReturn = true;

	std::map <std::string, CEnemySpecies> ::iterator it = EnemyBank.begin();
	while(it!=EnemyBank.end()){
		if ((*it).second.TrickList.empty()) {
			ERRORDX("%s: Set Trick List for All Enemy!",(*it).second.GetName().c_str());
			forReturn = false;
		}
		++it;
	}

	return forReturn;
}


