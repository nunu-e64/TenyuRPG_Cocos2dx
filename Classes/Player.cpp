#include "Player.h"
#include "Battle.h"
#include "BattleCalculator.h"
#include "Item.h"

void CPlayer::FirstSet2(){
	if (strlen(BaseTrick.Name)){
		BaseTrick.Power = CBattleCalc::CalcBaseTrickPower(Level, BaseTrickPowerGene);
	}
	NowItem = NULL;
}

void CPlayer::SetExtraImg(CBImgBank* _bImgBank){
	_bImgBank->GetImg(MAGIC_COUNTER, MagicCounterImg, ARRAY_SIZE(MagicCounterImg));
}

void CPlayer::CreateBattleMenu(std::vector <std::string> _battleItemList) {
	
	BattleMenu.Init(10, 360, 100, 130);

	if (strlen(BaseTrick.Name)) {
		BattleMenu.Create("攻撃");
		BattleMenu.Add("","技");
	} else {
		BattleMenu.Create("技");
	}

	BattleMenu.Add("", "祈祷");
	BattleMenu.Add("", "防御");
	BattleMenu.Add("", "道具");
	BattleMenu.Add("", "逃亡");
	BattleMenu.Add("", "待機");
	
	for (unsigned int i=0; i<TrickList.size(); i++){
		BattleMenu.Add("技", TrickList[i]->Name);
	}

	for (unsigned int i = 0; i < _battleItemList.size(); i++) {
		BattleMenu.Add("道具", _battleItemList[i].c_str());
	}	
	
}

void CPlayer::Draw(int _dx, int _dy){
	int dx=_dx+Dx; int dy=_dy+Dy;

	if (Visible) {
		switch(VisibleStatus) {
		case VISIBLE:
			SetDrawBright(255,255,255);
			break;
		case CHANGING:		
			{///死亡演出///////////////////////////////////////////////////
			static std::map<int, int> timeCount;
			if (timeCount.find(ActorIndex) == timeCount.end()) timeCount[ActorIndex] = 0;	//最初の一度だけ初期値代入

			++timeCount[ActorIndex];

			SetDrawBright(250-(timeCount[ActorIndex]*5),250-(timeCount[ActorIndex]*5),250-(timeCount[ActorIndex]*5));
			if (timeCount[ActorIndex] == 40){
				timeCount[ActorIndex] = 0;
				VisibleStatus = INVISIBLE;
			}
			}//////////////////////////////////////////////////////
			break;
		case INVISIBLE:
			SetDrawBright(50,50,50);
			break;
		}
	}

	if (BattleMenu.Alive){
		static std::map<int, int> timeCount;
		if (timeCount.find(ActorIndex) == timeCount.end()) timeCount[ActorIndex] = 0;	//最初の一度だけ初期値代入
		if (timeCount[ActorIndex]==120) timeCount[ActorIndex] = 0;
		++timeCount[ActorIndex];
		if (Visible) DrawGraph(Rect.Left+dx, Rect.Top+dy+(int)(5*sin(timeCount[ActorIndex]*2*PI/120)), Img, true);
	}else{
		if (Visible) DrawGraph(Rect.Left+dx, Rect.Top+dy, Img, true);
	}

	//マジックカウンターの描画//////////////////////////////////
	if (VisibleStatus == VISIBLE){
		SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
		int r = 40;
		double angle = PI/2 + 3*PI/(MAX_MAGIC_COUNT+2);
		for (int i=0; i<MAX_MAGIC_COUNT; i++) {
			if (i < MagicCount) SetDrawBright(255,255,255); 
			else SetDrawBright(100,100,100); 
			DrawCenterGraph(Rect.Center().x + r*cos(angle), Rect.Center().y + r*sin(angle), MagicCounterImg[(i<MagicCount?0:1)], true);
			angle += 2*PI/(MAX_MAGIC_COUNT+2);
		}
	}
	////////////////////////////////////////////////////////////

	SetDrawBright(255,255,255);
	SetDrawBlendMode( DX_BLENDMODE_NOBLEND , 0 ) ;

	Draw_Sub(_dx, _dy);
	BattleMenu.Draw();
}

bool CPlayer::Plan(){
	static bool newPlan = true;

	if (newPlan){
		BattleMenu.Alive = true;
		BattleMenu.SetCursor(BattleMenu.GetFront());
		
		NowTrick = NULL;
		NowItem = NULL;
		Target = -1;

		newPlan = false;

	}else{

		if (Target != -1) {	//Targetが-1じゃない＝TargetMarker.DecideなどでTargetが選択済み
			if (NowTrick != NULL) {
				MagicCount -= NowTrick->Cost;	//魔力消費
				MaxTimeGauge = NowTrick->Time;
			
			} else if (NowItem != NULL) {

				MaxTimeGauge = NowItem->WaitTime;
				for (int i = 0; i < ENEMY_NUM; i++) {	//敵アテンション変動
					//CmdList->Add("@Attention_Add(%d,%d,%d)", i, Index, (int)ATTENIOTN_WAIT);
				}
				SetStatus(WAIT, true);

				LogWindow->Add("%sは%sを使った！", Name.c_str(), NowItem->Name.c_str());
				CItemManager::GetInstance()->DecPlayerItem(NowItem->Name, 1);

				//使う処理
				char tmpCmd[1024];
				sprintf_s(tmpCmd, "@Item_Use(%s, %d, %d)", NowItem->Name.c_str(), ActorIndex, Target);
				CmdList->Add(tmpCmd);
				DEBUGDX(tmpCmd);

			} else {
				ERRORDX("Player[%s]:NowTrick=NULL && NowItem=NULL", Name.c_str());
			}
			return  (newPlan = true);
		}

		if (BattleMenu.Alive){		

			CMenuNode* result;
			if (BattleMenu.Move(result, true) && result != NULL) {

				if (mystrcmp(result->parent->label, "技")) {
					unsigned int index = BattleMenu.GetIndex(BattleMenu.GetCursor());
				
					if (index < TrickList.size()) {
						//魔力確認
							if (TrickList[index]->Cost > MagicCount) {	//魔力は足りているか
								LogWindow->Add("%s：魔力が足りない！(必要コスト%d)", Name.c_str(), TrickList[index]->Cost);
								return false;
							} else {
								NowTrick = TrickList[index];
							}

							
						//HACK:選んだ技の対象人数によって処理を変える
							switch(NowTrick->Target){	
							case target_tag::SINGLE_ENEMY:
								CmdList->Add("@Target_Appear(ENEMY,0,false)");
								break;
							case target_tag::SINGLE_FRIEND:
								CmdList->Add("@Target_Appear(PLAYER,0,false)");
								break;
							case target_tag::ALL_ENEMY:
								Target = PLAYER_NUM;
								break;
							case target_tag::ALL_FRIEND:
								Target = 0;
								break;
							case target_tag::ME:
								Target = ActorIndex;
								break;
							default:
								WARNINGDX("NowTrick->TargetType->Not Found. %s", NowTrick->Name);
								return false;
							}
						BattleMenu.Alive = false;

					}else{
						ErrorDx("Error->CPlayer::Plan->bigger action_num(do nothing):%d", __FILE__, __LINE__, index);
						return false;
					}

				}else if (mystrcmp(result->label, "攻撃")){
					NowTrick = &BaseTrick;
					CmdList->Add("@Target_Appear(ENEMY,0,false)");
					BattleMenu.Alive = false;
	
				}else if (mystrcmp(result->label, "待機")){
					MaxTimeGauge = WAIT_TIME;
					for (int i=0; i<ENEMY_NUM; i++) {	//敵アテンション変動
						CmdList->Add("@Attention_Add(%d,%d,%d)", i, Index, (int)ATTENIOTN_WAIT);
					}
					SetStatus(WAIT, true);
					BattleMenu.Alive=false;
					return  (newPlan=true);

				}else if (mystrcmp(result->label, "祈祷")){
					if (MagicCount == MAX_MAGIC_COUNT){
						LogWindow->Add("%s：魔力回復の必要はないようだ", Name.c_str());
						return false;
					} else {
						SetStatus(PRAY, true);
						MaxTimeGauge = PRAY_TIME;

						LogWindow->Add("%sは祈り始めた", Name.c_str());
						for (int i=0; i<ENEMY_NUM; i++) {	//敵アテンション変動
							CmdList->Add("@Attention_Add(%d,%d,%d)", i, Index, (int)ATTENTION_PRAY);
						}
						BattleMenu.Alive=false;
						return  (newPlan=true);
					}

				} else if (mystrcmp(result->label, "防御")) {
					if (DEFFENCE_MC > MagicCount) {	//魔力は足りているか
						LogWindow->Add("%s：魔力が足りない！(必要コスト%d)", Name.c_str(), DEFFENCE_MC);
						return false;
					} else {
						SetStatus(DEFFENCE, true);
						MaxTimeGauge = DEFFENCE_TIME;
						MagicCount -= DEFFENCE_MC;	//魔力消費

						LogWindow->Add("%sは防御に集中している", Name.c_str());
						for (int i = 0; i < ENEMY_NUM; i++) {	//敵アテンション変動
							CmdList->Add("@Attention_Add(%d,%d,%d)", i, Index, (int)ATTENIOTN_DEFFENCE);
						}

						BattleMenu.Alive = false;
						return  (newPlan = true);
					}

				} else if (mystrcmp(result->parent->label, "道具")) {
					NowItem = CItemManager::GetInstance()->GetConsumptionItem(result->label);
					
					if (NowItem != NULL &&  CItemManager::GetInstance()->GetPlayerItemNum(NowItem->Name) > 0) {

						switch (NowItem->Target) {
						case target_tag::SINGLE_ENEMY:
							CmdList->Add("@Target_Appear(ENEMY,0,false)");
							break;
						case target_tag::SINGLE_FRIEND:
							CmdList->Add("@Target_Appear(PLAYER,0,false)");
							break;
						case target_tag::ALL_ENEMY:
							Target = PLAYER_NUM;
							break;
						case target_tag::ALL_FRIEND:
							Target = 0;
							break;
						case target_tag::ME:
							Target = ActorIndex;
							break;
						default:
							WARNINGDX("Item->TargetType->Not Match Any Type. %s", NowItem->Name.c_str());
							return false;
						}

						BattleMenu.Alive = false;

					} else {
						return false;
					}
				}
			}
		}else{

			if (CheckHitKeyDown(KEY_INPUT_RIGHT)){
				CmdList->Add("@Target_Move(RIGHT)");
			}else if (CheckHitKeyDown(KEY_INPUT_LEFT)){
				CmdList->Add("@Target_Move(LEFT)");
			}else if (CheckHitKeyDown(KEY_INPUT_OK)){
				CmdList->Add("@Target_Decide(%d)", ActorIndex);
			}else if (CheckHitKeyDown(KEY_INPUT_CANCEL)){
				CmdList->Add( "@Target_Disappear");
				NowTrick = NULL;
				NowItem = NULL;
				BattleMenu.Alive = true;
			}
		}
	}

	return false;
}

bool CPlayer::Action(){
	
	if (GetStatus(PRAY)) {
		MagicCount = min(MagicCount+PRAY_RECOVERY_MC, MAX_MAGIC_COUNT);
		SetStatus(PRAY, false);
		LogWindow->Add("%sは祈りを捧げ魔力を回復した！", Name.c_str());

	} else if (GetStatus(DEFFENCE)) {
		SetStatus(DEFFENCE, false);

	} else if (GetStatus(WAIT) || NowTrick==NULL){	//待機を選択orアイテムを使用した場合
		SetStatus(WAIT, false);
		
	} else {  //技の使用
		CmdList->Add("@Damage(%d,%d,%d,NORMAL)", ActorIndex, Target, NowTrick);
		LogWindow->Add("%sの%s！", Name.c_str(), NowTrick->Name);


		////技使用後処理
		//	NowTrick = NULL;
		//	Target = -1;
	}

	return true;
}

double CPlayer::CalcDamage(double _damage, CActor* _attacker, trick_tag const* _trick){

	if (GetStatus(DEFFENCE)) {
		_damage *= MAGIC_DEFFENCE_RATE;	//魔法防御
	} else {
		_damage -= _damage * MagicCount / MAX_MAGIC_COUNT * (1 - MAX_MAGIC_COUNTER_DAMAGE_RATE);  //魔力によるダメージ減少
	}
	return _damage;
}

void CPlayer::MpHeal(int _count) {
	int mc= between(MagicCount, MAX_MAGIC_COUNT, MagicCount + _count);

	if (mc > MagicCount) {
		LogWindow->Add("  %sの魔力が%dポイント回復した！", GetName().c_str(), mc - MagicCount);
		MagicCount = mc;
	}
}
