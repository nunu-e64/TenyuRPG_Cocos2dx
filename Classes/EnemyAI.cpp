#include "EnemyAI.h"
#include "Actor.h"
#include "Enemy.h"
#include "LogWindow.h"


int CEnemyAI::AttentionMarkerImg[MAX_PLAYER_NUM] = {0};
int CEnemyAI::AttentionBoardImg = 0;
int CEnemyAI::AttentionEffectImg = 0;

int CEnemyAI::GetPlan(const CEnemy* _enemy) { 
	return Planner->GetPlan(_enemy); 
}

int CEnemyAI::GetTarget(const CEnemy* _enemy){
	return Targetter->GetTarget(_enemy);
	
	for (int i=0; i<PLAYER_NUM; i++){
		if (Actor[i]->GetAlive()) break;
		if (i==PLAYER_NUM-1) {
			ERRORDX("'%s':All_Players_Died", _enemy->GetName().c_str());
			return -1;
		}
	}
	
	int target = rand()%PLAYER_NUM;
	while(!Actor[target]->GetAlive()){
		target = rand()%PLAYER_NUM;
	}
	return target;
}

bool CEnemyAI::AddRandomPlanSet(const unsigned int _index, std::vector<std::pair<int, int> > _planList, bool _clear){
	
	if (_clear) RandomPlanSet.clear();

	if (RandomPlanSet.find(_index)==RandomPlanSet.end()){
		RandomPlanSet[_index] = _planList;
		return true;

	}else{
		ErrorDx("Error->SetRandomPlanSet->Already Existed Key(don't override):%d", (int)_index);
		return  false;
	}
}

void CEnemyAI::AddAttention(int _playerIndex, int _value, CLogWindow* _logWindow){
	int attention = between(0, MAX_ATTENTION, Attention[_playerIndex]+_value); 

	if (_logWindow != NULL && attention != Attention[_playerIndex]) {
		char tmp[32];
		mystrcpy(tmp, (_value > 0 ? "上がった" : "下がった"));
		_logWindow->Add("  %sへの%sのアテンションが%dポイント%s！", Actor[_playerIndex]->GetName().c_str(), Planner->GetName().c_str(), abs(attention-Attention[_playerIndex]), tmp);
	}

	Attention[_playerIndex] = attention;
	Targetter->CalcAttentionRank();


	//演出  ダサいので一時ボツ//
		//AttentionEffectCount[_playerIndex] = (_value>0?EFFECT_COUNT:(_value<0?-EFFECT_COUNT:0));

	//myLogf("Attention_P", "EnemyAI:%d", Attention);
}
void CEnemyAI::SetAttention(int _playerIndex, int _value){
	AddAttention(_playerIndex, _value - Attention[_playerIndex]); 	
}

void CEnemyAI::SetAttentionImg(int* _markerImg, int _boardImg, int _effectImg){
	if (_markerImg!=NULL) {
		for (int i=0; i<MAX_PLAYER_NUM; i++){
			AttentionMarkerImg[i] = _markerImg[i];
		}
	}

	AttentionBoardImg = _boardImg;
	AttentionEffectImg = _effectImg;
}

void CEnemyAI::Draw(const CEnemy* _enemy){

	//死んでたらアテンションは0に
	for (int i=0; i<PLAYER_NUM; i++){
		if (Attention[i]!=0 && !Actor[i]->GetAlive()){
			SetAttention(i, 0);
		}
	}

	//アテンションボードとマーカーの描画//////////////////////////////////////////////////

	CVector pos(_enemy->GetRect().Center() + CVector(-60, 50));
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
	DrawCenterGraph(pos.x, pos.y, AttentionBoardImg, true);

	const int YOHAKU_LEFTRIGHT = 21;	//Boardの左端から描画領域左端までの距離（左右対称）
	const int YOHAKU_TOPBOTTOM = 7;		//Boardの上端から描画領域上端までの距離（上下対称）
	CVector boardSize = GetGraphSize(AttentionBoardImg) - CVector(YOHAKU_LEFTRIGHT*2, YOHAKU_TOPBOTTOM*2);
	pos.x -= boardSize.x*0.5;
	pos.y += boardSize.y*0.5;		//AttentionBoardの描画領域の左下座標を取得

	/*for (int i=0; i<MAX_ATTENTION+1; i++){
		DrawLine((int)pos.x-3, (int)(pos.y - (i+0.5)*boardSize.y/(MAX_ATTENTION+1)), (int)(pos.x+boardSize.x+3), (int)(pos.y - (i+0.5)*boardSize.y/(MAX_ATTENTION+1)), GetColor(50,50,50)); 
	}*/	

	int bright = 255;
	//SetDrawBlendMode(DX_BLENDMODE_ALPHA, 230);
	SetDrawBlendMode( DX_BLENDMODE_NOBLEND , 0 ) ;
	for (int i=0; i<PLAYER_NUM; i++){
		bright = choose(Targetter->GetAttentionRank(i)+1, 255, 180, 100);
		SetDrawBright(bright, bright, bright);
		if (Actor[i]->GetAlive()) {
			DrawCenterGraph(pos.x + (i+0.5)*boardSize.x/MAX_PLAYER_NUM, pos.y - (Attention[i]+0.5)*boardSize.y/(MAX_ATTENTION+1), AttentionMarkerImg[i], true);
			
			if (AttentionEffectCount[i] != 0) {
				if (AttentionEffectCount[i] > 0) {
					--AttentionEffectCount[i];
				} else {
					++AttentionEffectCount[i];
				}
				
				int d = 4*mod(AttentionEffectCount[i]/5, 11);
				SetDrawBlendMode( DX_BLENDMODE_ALPHA, 150) ;
				SetDrawBright(255, 255, 255);
				DrawRectRotaGraph((int)(pos.x + (i+0.5)*boardSize.x/MAX_PLAYER_NUM), (int)(pos.y - (Attention[i]+0.5)*boardSize.y/(MAX_ATTENTION+1)), d, 0, (int)GetGraphSize(AttentionEffectImg).y, (int)GetGraphSize(AttentionEffectImg).y, 1, 0, AttentionEffectImg, true, false);
				SetDrawBlendMode( DX_BLENDMODE_NOBLEND , 0 ) ;
			}
		}
	}
	SetDrawBright(255, 255, 255);
	SetDrawBlendMode( DX_BLENDMODE_NOBLEND , 0 ) ;
}