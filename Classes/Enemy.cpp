#include "Enemy.h"
#include "Species.h"
#include "LogWindow.h"
#include "TextBox.h"
#include "CmdList.h"



void CEnemy::Draw(int _dx, int _dy){
	int dx=_dx+Dx; int dy=_dy+Dy;

	switch(VisibleStatus) {
	case VISIBLE:
		SetDrawBright(255,255,255);
		break;
	case CHANGING:		
		{/////���S���o//////////////////////////////////////
		static std::map<int, int> timeCount;
		if (timeCount.find(ActorIndex) == timeCount.end()) timeCount[ActorIndex] = 0;	//�ŏ��̈�x���������l���
		timeCount[ActorIndex]++;
		
		SetDrawBlendMode( DX_BLENDMODE_ALPHA , 240-(timeCount[ActorIndex]*8)) ;
		if (timeCount[ActorIndex]==30){
			timeCount[ActorIndex] = 0;
			VisibleStatus = INVISIBLE;
		}
		}//////////////////////////////////////////////////////
		break;
	case INVISIBLE:
		return;
	}

	if (Visible) DrawGraph(Rect.Left+dx, Rect.Top+dy, Img, true);

	SetDrawBright(255,255,255);
	SetDrawBlendMode( DX_BLENDMODE_NOBLEND , 0 ) ;

	//HPBar��TimeGauge�̕`��
		Draw_Sub(_dx, _dy);

	//AttentionCursor�̕`��
		AI.Draw(this);
	
}


bool CEnemy::Plan(){
	

	//�s���̌���
		int actionNum = AI.GetPlan(this);
		if (actionNum>=0 && actionNum<(int)TrickList.size()){
			NowTrick = TrickList[actionNum];
			MaxTimeGauge = NowTrick->Time;
		}else{
			ERRORDX("%s:actionNum<0 OR actionNum >= TrickList.size (do nothing):%d", GetName().c_str(), actionNum);
			return true;
		}

		if (NowTrick==NULL){
			Target = -1;
			ERRORDX("%s:NowTrick=NULL(do nothing)", GetName().c_str());
			return true;
		}


	return true;
}

bool CEnemy::Action(){
	
	
	if (GetStatus(WAIT) || NowTrick==NULL){	//�ҋ@��I�������ꍇ
		SetStatus(WAIT, false);
	} else {
		//Tatget�̑I���ƍs��
			Target = AI.GetTarget(this); 
			CmdList->Add("@Damage(%d,%d,%d,NORMAL)", ActorIndex, Target, NowTrick);
			LogWindow->Add("%s��%s�I", Name.c_str(), NowTrick->Name);

		//�s���㏈��
			NowTrick = NULL;
			Target = -1;
	
	}

	return true;
}

std::vector<std::string> CEnemy::GetDropItemList() {

	std::vector <std::string> dropItemList;
	for (unsigned int i = 0; i < DropItemList.size(); i++) {
		if (DropItemList[i].second > rand() % 100) {
			dropItemList.push_back(DropItemList[i].first);
		}
	}

	return dropItemList;

}