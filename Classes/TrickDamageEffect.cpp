#include "TrickDamageEffect.h"
#include "Battle.h"

CTrickDamageEffect_BOMB::CTrickDamageEffect_BOMB(std::string _name, std::vector<std::string>_argList):CTrickDamageEffect(_name){
	CONSTRUCTED;
	int numset[ARG_NUM] = {200,200,200, 10, 60};	//適当

	for (unsigned int i=0; i<_argList.size() && i<ARG_NUM; i++){
		int num;
		if(!( mystrtol(_argList[i].c_str(), &num))){
			ErrorDx("Error->CTriickDamageEffect_BOMB(%s)->Check argument type[%d]", __FILE__, __LINE__, _name.c_str(), i);
		}else{
			numset[i] = num;
		}
	}

	if (_argList.size() < ARG_NUM){
		WarningDx("Warning->TrickEffect_'BOMB'('%s')->ArgumentNum is smaller than needed('%d').", _name.c_str(), ARG_NUM);
	}else if(_argList.size() > ARG_NUM){
		WarningDx("Warning->TrickEffect_'BOMB'('%s')->ArgumentNum is larger than needed('%d').", _name.c_str(), ARG_NUM);
	}

	Color = GetColor(numset[0],numset[1],numset[2]);
	Size = numset[3];
	Time = numset[4];
}

void CTrickDamageEffect_BOMB::DrawDamageEffect(CBattle* _battle, CBImgBank* _bimgbank, CRect _attackerR, CRect _targetR) const{
	//ここに実際のあれこれを書く


}

CTrickDamageEffect_PROTO::CTrickDamageEffect_PROTO(std::string _name, std::vector<std::string>_argList):CTrickDamageEffect(_name){
	CONSTRUCTED;
	int numset[ARG_NUM] = {10, 15, 60};	//適当

	for (unsigned int i=0; i<_argList.size() && i<ARG_NUM; i++){
		int num;
		if(!( mystrtol(_argList[i].c_str(), &num))){
			ERRORDX("'%s':Check argument type[%d]", _name.c_str(), i);
		}else{
			numset[i] = num;
		}
	}

	if (_argList.size() < ARG_NUM){
		WarningDx("Warning->TrickEffect_'PROTO'('%s')->ArgumentNum is smaller than needed('%d').", _name.c_str(), ARG_NUM);
	}else if(_argList.size() > ARG_NUM){
		WarningDx("Warning->TrickEffect_'PROTO'('%s')->ArgumentNum is larger than needed('%d').", _name.c_str(), ARG_NUM);
	}

	Size1 = numset[0];
	Size2 = numset[1];
	Radius = numset[2];
}


void CTrickDamageEffect_PROTO::DrawDamageEffect(CBattle* _battle, CBImgBank* _bimgbank, CRect _attackerR, CRect _targetR) const{
	//ここに実際のあれこれを書く

	//技発動演出//////////////////////////////////////////////////////////////
	int timecount = 0;
	CVector ball[8];
	int a=Radius;
	for (int i=0; i<8; i++){
		ball[i].Set(_attackerR.Center().Add(a*cos(i*PI/4),a*sin(i*PI/4)));		
	}
	int img = _bimgbank->GetImg("EFFECT_BOMB"); 

	do{
		_battle->Draw();
		SetDrawBlendMode(DX_BLENDMODE_ADD, timecount*2);
		for (int i=0; i<8; i++){
			DrawExtendGraph(ball[i].x-Size1, ball[i].y-Size1, ball[i].x+Size1, ball[i].y+Size1, img, false);
		}
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		++timecount;
		a-=1;
		for (int i=0; i<8; i++){
			ball[i].Set(_attackerR.Center().Add(a*cos(timecount*PI/(Radius*2)+i*PI/4),a*sin(timecount*PI/(Radius*2)+i*PI/4)));
		}
	}while(a!=0 && BasicLoop());

	timecount=0;
	a=20;	//光球の移動速度
	CVector vec = _targetR.Center()-_attackerR.Center();
	vec *= 1/vec.GetLength();
	do{
		_battle->Draw();
		SetDrawBlendMode(DX_BLENDMODE_ADD, 120);
		DrawExtendGraph(ball[0].x-Size2, ball[0].y-Size2, ball[0].x+Size2, ball[0].y+Size2, img, false);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		++timecount;
		ball[0].Add(a*vec.x, a*vec.y);
	}while( BasicLoop() && vec.y*(ball[0].y-_targetR.Center().y)<0);
	//////////////////////////////////////////////////////////////////////////

}