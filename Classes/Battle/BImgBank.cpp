#include "BImgBank.h"


void CBImgBank::Clear(){
	for (int i=0; i<MAP_MAX; i++){
		BattleBackGround[i].clear();
		BattleBackGround[i].insert(std::pair<int,int>(-1,0));
	}
	CImgBank::Clear();
}

void CBImgBank::Init(){
	for (int i=0; i<MAP_MAX; i++){
		BattleBackGround[i].insert(std::pair<int,int>(-1,0));
	}
}


void CBImgBank::SetBattleBackGround(const char* _key,int _mapnum, int _chipnum){
	MAP_MAX_CHECK(_mapnum, );
	if (_chipnum<-1 || _chipnum>255){
		ERRORDX("%s:_chipnum should between(-1,255) (return 0):%d", _key, _chipnum);
		return;
	}

	BattleBackGround[_mapnum][_chipnum] = GetImg(_key);
}

int CBImgBank::GetBattleBackGround(int _mapnum, int _chipnum){
	MAP_MAX_CHECK(_mapnum, 0);
	
	if (BattleBackGround[_mapnum].empty()){
		ERRORDX("InternalError->BattleBackGround[_mapnum].size()==0 _mapnum:%d", _mapnum);
		return 0;
	}else if (_chipnum<-1 || _chipnum>255){
		ERRORDX("%d:_chipnum'%d' should between(-1,255) (return 0)", _mapnum, _chipnum);
		return BattleBackGround[_mapnum][-1];
	}

	if (BattleBackGround[_mapnum].find(_chipnum)!=BattleBackGround[_mapnum].end()) {
		return BattleBackGround[_mapnum][_chipnum];
	} else {
		return BattleBackGround[_mapnum][-1];
	}
}