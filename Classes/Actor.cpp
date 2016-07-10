#include "Actor.h"
#include "BImgBank.h"
#include "TextBox.h"
#include "LogWindow.h"
#include "BattleCalculator.h"


std::map <std::string, int> CActor::BarImg;

const std::pair<int, int> CActor::ATK_UP  (0, 0);
const std::pair<int, int> CActor::ATK_DOWN(0, 1);
const std::pair<int, int> CActor::DEF_UP  (1, 0);
const std::pair<int, int> CActor::DEF_DOWN(1, 1);
const std::pair<int, int> CActor::SPD_UP  (2, 0);
const std::pair<int, int> CActor::SPD_DOWN(2, 1);
const std::pair<int, int> CActor::MAX_STATUSCHANGER_IMGSIZE(5, 2);
int CActor::StatusChangerImg[5 * 2] = {0};	//上と手動で一致させておかなくてはならない

int CActor::HpFontHandle = -1;

void CActor::FirstSet(int _playernum, int _enemynum, int _index, CTextBox** _textbox, CCmdList* _cmdlist, CLogWindow* _logWindow){
	PLAYER_NUM = _playernum;
	ENEMY_NUM = _enemynum;
	
	ActorIndex = _index;
	Index = ((_index<PLAYER_NUM)? _index: _index-PLAYER_NUM);
	B_TextBox_pp = _textbox;
	LogWindow = _logWindow;
	CmdList = _cmdlist;

	//レベルと成長素子に基づいて各キャラステータスを計算
		Atk   = CBattleCalc::CalcValue(CBattleCalc::ATK, Level, AtkGene);
		Def   = CBattleCalc::CalcValue(CBattleCalc::DEF, Level, DefGene);
		Spd   = CBattleCalc::CalcSpd(Level, SpdGene);
		MaxHp = CBattleCalc::CalcValue(CBattleCalc::MAXHP, Level, MaxHpGene);

	Alive = Visible = (Hp!=0? true:false);
	VisibleStatus = (Alive? VISIBLE:INVISIBLE);
	OldHp = Hp;		//UNDONE:戦闘ごとに全回復させるならここで明示的に=MaxHpとしておく
	
	MaxTimeGauge = 100;	//%
	TimeGauge = rand() % (MaxTimeGauge / 2) + MaxTimeGauge / 2;	//タイムゲージ初期位置はゲージの半分以下でランダム（内部的にはMaxTimeGause/2以上）。バグ防止のため
	if (HpFontHandle == -1) HpFontHandle = CreateFontToHandle(NULL, 10, -1);	//HpBarに表示するHP用のフォント作成

	SetStatus(WAIT, true);
	Mode = PREPARE;

	Target = -1;
	NowTrick = NULL;

	Dx=0; Dy=0;

	FirstSet2();
}

void CActor::SetRect(int _cx, int _cy){
	int width, height;
	GetGraphSize(Img, &width, &height);

	Rect.Left = _cx-width/2;
	Rect.Right = _cx+width/2;
	Rect.Top = _cy-height/2;
	Rect.Bottom = _cy+height/2;
}

/*
void CActor::SetImg(int _img){
	Img = _img;
	SetRect((int)(Rect.Center().x), (int)(Rect.Center().y));
}
*/

bool CActor::SetSystemImg(CBImgBank* _bImgBank){
	
	//静的メンバを何度も初期化するのは良くないが、タイトルに戻った後も初期化されないのは困るし大した負荷ではないと思うので現状放置
		SetBarImg(_bImgBank, HP_BAR);
		SetBarImg(_bImgBank, TIME_BAR1);
		SetBarImg(_bImgBank, TIME_BAR2);
		SetBarImg(_bImgBank, TIME_DEFFENCE);
		SetBarImg(_bImgBank, TIME_WAIT);
		SetBarImg(_bImgBank, TIME_PRAY);
		SetBarImg(_bImgBank, TIME_TRICK);

		_bImgBank->GetImg(STATUS_CHANGER, StatusChangerImg, MAX_STATUSCHANGER_IMGSIZE.first, MAX_STATUSCHANGER_IMGSIZE.second);

	//Playerは魔力カウンタ実装
		SetExtraImg(_bImgBank);

	return true;
}
void CActor::SetBarImg(CBImgBank* _bImgBank, std::string _key) {
	BarImg[_key] = _bImgBank->GetImg(_key.c_str());
}

/*
void CActor::AddTrick(trick_tag const* _trick){
	TrickList.push_back(_trick);
}
*/

bool CActor::Main(){

	if (Alive){
		if (TimeGaugeForward()){
			Mode = ACTION;
			return true;
		}
	}		
/*		switch(Mode){
		case PLAN:
		case PREPARE:
			if (TimeGaugeForward()){
				Mode = ACTION;
				return true;
			}
			break;

		case ACTION:
			if (TimeGaugeForward()){
				Mode = PLAN;
				return true;
			}
			break;
	
		default:
			ErrorDx("Error->CActor::Main->unexpected Mode:%d", __FILE__, __LINE__, Mode);
		}
	}*/

	return false;
}

bool CActor::Do(){		//行動待機リスト上位のものから行動していく

	if (Alive){
		bool forReturn = false;
		switch (Mode){
		case BEFORE_PLAN:
			Mode = PLAN;
		case PLAN:
			forReturn = Plan();

			if (forReturn) {
				if (GetStatus(sideEffect_tag::type_tag::HEAL_AFTER_SELECTCOMMAND)) {
					Heal(GetStatus(sideEffect_tag::type_tag::HEAL_AFTER_SELECTCOMMAND));
				}
			}

			break;
		case ACTION:
			if (forReturn = Action()) {
				Mode = BEFORE_PLAN;
				forReturn = false;
			}
			break;
		default:
			ErrorDx("Error->CActor::Do->unexpected Mode:%d", __FILE__, __LINE__, Mode);
			return true;
		}

		//if (forReturn) TimeGaugeForward(); //同時行動時に行動が済んだキャラからゲージが0に戻る	//同時行動はまず起こらないため削除
		return forReturn;

	}else{
		return true;
	}

}

int CActor::GetStatus(int _key){
	if (Status.find(_key) != Status.end()) {
		return Status[_key];
	} else {
		return 0;
		//ERRORDX("NotFound Key:%s", _key);
	}
}
void CActor::SetStatus(int _key, int _value) {
	Status[_key] = _value;
}


int CActor::Damaged(CActor* _attacker, trick_tag const* _trick){
	if (_trick==NULL) {
		ErrorDx("Error->CActor::Damage->_trick==NULL", __FILE__, __LINE__);
		return 0;
	}

	double damage = _trick->Power * _attacker->GetAtk() / (double)GetDef(); //HACK:ダメージ計算式は要検討

	//攻撃側の時間制限付き特殊効果
		for (unsigned int i = 0; i < _attacker->StatusChangerList.size(); i++) {
			switch (_attacker->StatusChangerList[i].StatusKind) {
			case sideEffect_tag::type_tag::ATK:
				damage *= (100 + _attacker->StatusChangerList[i].Power) / 100.0;
				break;
			}
		}

	//被弾側の時間制限付き特殊効果
		for (unsigned int i = 0; i < StatusChangerList.size(); i++) {
			switch (StatusChangerList[i].StatusKind) {
			case sideEffect_tag::type_tag::DEF:
				damage /= (100 + StatusChangerList[i].Power) / 100.0;
				break;
			}
		}

	damage = CalcDamage(damage, _attacker, _trick);
	damage = max(1, damage);	//ダメージ最小値は1

	Hp = between(0, MaxHp, Hp-(int)damage);
	//死亡判定はCheckBarMoveではなくここですべきか？

	return (int)damage;
}

bool CActor::CheckBarMove(){	//Hpバーの移動終了を確認
	
	if (Alive && Hp==0) Alive = false;

	if (OldHp!=Hp) { //Hpバー減少中
		return false;
	}else{			//Hpバー減少終了
		if (!Alive && VisibleStatus == VISIBLE) {
			LogWindow->Add("%sは倒れた！", GetName().c_str());
			VisibleStatus = CHANGING;
		}

		return (VisibleStatus != CHANGING);		//描画状況が安定してからtrueをかえす
	}
}

void CActor::AddStatusChanger(int _kind, int _powerPercent, int _time) {
	
	if (_powerPercent == 0) return;

	statusChanger_tag tmp;
	tmp.StatusKind = _kind;
	tmp.Power = _powerPercent;
	tmp.Time = (double)_time;
	tmp.Img = 0;

	//ログ出力
		char chtmp[256];
		std::pair <int, int> imgIndex;

		switch (_kind) {
		case sideEffect_tag::type_tag::ATK:
			if (_powerPercent>0) {
				imgIndex = ATK_UP;
				mystrcpy(chtmp, "  %sの攻撃力が%d％上がった！");
			} else {
				imgIndex = ATK_DOWN;
				mystrcpy(chtmp, "  %sの攻撃力が%d％下がった！");
			}
			break;

		case sideEffect_tag::type_tag::DEF:
			if (_powerPercent>0) {
				imgIndex = DEF_UP;
				mystrcpy(chtmp, "  %sの防御力が%d％上がった！");
			} else {
				imgIndex = DEF_DOWN;
				mystrcpy(chtmp, "  %sの防御力が%d％下がった！");
			}
			break;

		case sideEffect_tag::type_tag::SPD:
			if (_powerPercent>0) {
				imgIndex = SPD_UP;
				mystrcpy(chtmp, "  %sの行動速度が%d％上がった！");
			} else {
				imgIndex = SPD_DOWN;
				mystrcpy(chtmp, "  %sの行動速度が%d％下がった！");
			}
			break;

		default:
			ERRORDX("StatusKind doesn't match any status. :%s:%d", Name.c_str(), _kind);
			return;
		}

		tmp.Img = StatusChangerImg[imgIndex.first + MAX_STATUSCHANGER_IMGSIZE.first * imgIndex.second];
		LogWindow->Add(chtmp, Name.c_str(), abs(_powerPercent));
		StatusChangerList.push_back(tmp);

}

void CActor::ChangeValue(int _kind, int _powerPercent){	//永続（ステータスに直接影響）
	
	switch(_kind){
	case sideEffect_tag::type_tag::ATK:
		Atk += (int)((double)Atk*_powerPercent/100);

		if (_powerPercent>0){
			LogWindow->Add("  %sの攻撃力が上がった！", GetName().c_str());
		}else if(_powerPercent<0){
			LogWindow->Add("  %sの攻撃力が下がった！", GetName().c_str());
		}
		break;

	case sideEffect_tag::type_tag::DEF:
		Def += (int)((double)Def*_powerPercent/100);

		if (_powerPercent>0){
			LogWindow->Add("  %sの防御が上がった！", GetName().c_str());
		}else if(_powerPercent<0){
			LogWindow->Add("  %sの防御力が下がった！", GetName().c_str());
		}
		break;

	case sideEffect_tag::type_tag::SET_TIMEGAUGE:
		TimeGauge = max(1, (double)MaxTimeGauge * (1 - _powerPercent / 100));

		LogWindow->Add("  %sのタイムゲージが強制変動！", GetName().c_str());
		break;

	default:
		ERRORDX("StatusKind doesn't match any status. :%s", _kind);
		return;
	}
}

void CActor::Heal(int _percent) {
	int hp = between(Hp, MaxHp, Hp + MaxHp*_percent / 100);

	if (hp > Hp) {
		LogWindow->Add("  %sのHPが%d回復した！", GetName().c_str(), hp-Hp);
		Hp = hp;
	}
}

void CActor::Draw_Sub(int _dx, int _dy){
	CVector barPos;
	CVector barSize;

	//HpBar
		barSize = GetGraphSize(BarImg[HP_BAR]);
		barPos = CVector(Rect.Center().x-barSize.x/2, Rect.Center().y + (IsPlayer()? 45: 80));
			  DrawBox((int)(-1+barPos.x+_dx), (int)(-1+barPos.y+_dy), (int)(1+barPos.x+barSize.x+_dx), (int)(1+barPos.y+barSize.y+_dy), BLUE, true);
		DrawRectGraph((int)(barPos.x+_dx)   , (int)(barPos.y+_dy), 0, 0, (int)(barSize.x*OldHp/MaxHp), (int)barSize.y, BarImg[HP_BAR], false, false);

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
		DrawFormatStringToHandle((int)(2+barPos.x+_dx), (int)(barPos.y+_dy), GetColor(250, 200, 50), HpFontHandle,"%d", Hp);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, -1);

	//TimeBar
		const CVector TIME_BAR_SIZE(50, 10);
		barPos.y += barSize.y + 5;

		int img;
		if (Mode == PLAN || GetStatus(WAIT)) {
			img = BarImg[TIME_WAIT];
		} else if (GetStatus(DEFFENCE)) {
			img = BarImg[TIME_DEFFENCE];
		} else if (GetStatus(PRAY)) {
			img = BarImg[TIME_PRAY];
		} else {
			img = BarImg[TIME_TRICK];
		}

		if (Mode==PREPARE && !GetStatus(DEFFENCE)) SetDrawBright(180,180,180);		
	  	DrawBox((int)(-1+barPos.x+_dx), (int)(-1+barPos.y+_dy),(int)(1+barPos.x+TIME_BAR_SIZE.x*MaxTimeGauge/100.0+_dx), (int)(1+barPos.y+TIME_BAR_SIZE.y+_dy), BLUE, true);
		DrawRectGraph((int)(barPos.x+_dx), (int)(barPos.y+_dy), 0, 0, (int)(TIME_BAR_SIZE.x*(MaxTimeGauge-TimeGauge)/100.0), (int)TIME_BAR_SIZE.y, img, false, false);
	
	SetDrawBright(255, 255, 255);
		
	//StatusChangerIcon
		barPos.y += TIME_BAR_SIZE.y + 5;
		for (unsigned int i = 0; i < StatusChangerList.size(); i++) {
			if (StatusChangerList[i].Time > 50) {
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			} else {
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)(130 + 125 * sin((2 * PI)/1000 * (GetNowCount() % 1000))));
			}
			DrawGraph(barPos.x + i*GetGraphSize(StatusChangerList[i].Img).x, barPos.y, StatusChangerList[i].Img, true);
		}
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);


	//OldHpとHpのギャップを埋める
		if (OldHp>Hp) OldHp--;
		else if (OldHp<Hp) OldHp++;

	DrawCenterString((int)(Rect.Center().x), (int)Rect.Center().y, (Mode==PLAN?"PLAN":(Mode==PREPARE?"PREPARE":"ACTION")) , BLACK, true);  //@TEST//

}



bool CActor::TimeGaugeForward(){
	if (TimeGauge==0) {
		TimeGauge = MaxTimeGauge;
		Mode = (mode_tag)((Mode+1) % MODE_NUM);
	}

	//時間制限付き速度変更特殊効果の有無確認
		double spd = Spd;
		for (unsigned int i = 0; i < StatusChangerList.size(); i++) {
			if (StatusChangerList[i].StatusKind == sideEffect_tag::type_tag::SPD) {
				spd *= (100 + StatusChangerList[i].Power) / 100.0;
			}
		}
		spd = max(spd, 0.001);	//速度最小値は0.001

	TimeGauge-=spd;

	//ステータス変動効果の有効時間管理
		for (std::vector<statusChanger_tag>::iterator it = StatusChangerList.begin(); it != StatusChangerList.end();) {
			(*it).Time -= BASIC_SPD;
			if ((*it).Time <= 0) { 
				it = StatusChangerList.erase(it); 
			} else {
				++it;
			}
		}

	if (TimeGauge<=0){
		TimeGauge = 0;
		return true;
	}
	return false;
}