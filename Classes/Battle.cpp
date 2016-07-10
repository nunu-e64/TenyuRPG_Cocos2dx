#include "Battle.h"
#include "PlayerSpeciesManager.h"
#include "EnemySpeciesManager.h"
#include "BattleCalculator.h"
#include "TrickManager.h"
#include "Player.h"
#include "Enemy.h"

#include "Load.h"
#include "EveManager.h"
#include "ItemManager.h"


CBattle::CBattle(){
	CONSTRUCTED;	

	Ready = false;

	ACTOR_NUM = 0;
	PLAYER_NUM = 0;
	ENEMY_NUM = 0;
	TextBox = &TextBox1;
	Img_BattleBackGround = NULL;

	PlayerSpeciesManager = CPlayerSpeciesManager::GetInstance();
	EnemySpeciesManager = CEnemySpeciesManager::GetInstance();
	TrickManager = CTrickManager::GetInstance();

	WinCommand[0] = '\0';
	mystrcpy(LoseCommand, "@GameOver");
}


bool CBattle::Init(){	//Field.Init()�ŌĂяo��	//14/06/26
	PlayerSpeciesManager->Clear();
	EnemySpeciesManager->Clear();
	BImgBank.Clear();
	TrickManager->Clear();
	LogWindow.Term();
	
	ACTOR_NUM = 0;
	PLAYER_NUM = 0;
	ENEMY_NUM = 0;
	
	//Load.cpp��ʂ���.rpg�̓ǂݍ���/////////////////////
		//DEBUGDX("BattleLoad_Start");
		//�O���e�L�X�g�̃��[�h
			CLoad bfsLoad;
			CBattleFirstSetCmdManager  bfsCmdManager;
			CCmdList bfsCmdList;
			
			if (bfsLoad.LoadAddText(FILE_B_SYSTEM)){			
				bfsLoad.CommandCopy(&bfsCmdList);
				bfsCmdManager.Main(&bfsCmdList, &BImgBank, PlayerSpeciesManager, EnemySpeciesManager, TrickManager);
			}else{
				return false;
			}
		//DEBUGDX("BattleLoad_End");
	////////////////////////////////////////////////	
	
	//�ǂݍ��݂��K�؂ɍς񂾂��`�F�b�N//////////
		PlayerSpeciesManager->CheckAfterLoad();
		EnemySpeciesManager->CheckAfterLoad();
	///////////////////////////////////////////

	//�퓬�V�X�e���p�摜�̓ǂݍ���////////////////
		TargetMarker.SetImage(BImgBank.GetImg(TARGET_CURSOR));
		CEnemyAI::SetAttentionImg(BImgBank.GetImg(ATTENTION_MARKER, MAX_PLAYER_NUM), BImgBank.GetImg(ATTENTION_BOARD), BImgBank.GetImg(ATTENTION_EFFECT));

	//���C���̃e�L�X�g�{�b�N�X�ƃI�[�o�[���b�v�p�e�L�X�g�{�b�N�X�̏�����
		//DEBUGDX("BattleTextBoxInit_Start");
		TextBox1.Init(60, 370, WINDOW_WIDTH-80*2, 100, 3, 25*2, 16, WHITE, BLACK, 3, NULL);	//�R���X�g���N�^�ɏ�������N�����Ȃ��Ȃ���
		TextWrap1.Init(100, 100, 400, 300, 30, 30 * 2, 14, WHITE, GRAY, 3, NULL);
		TextBox = &TextBox1;
		//DEBUGDX("BattleTextBoxInit_End");
		
	//���O�E�B���h�E�̏�����
		//DEBUGDX("BattleLogWindowInit_Start");
		LogWindow.Init(WINDOW_WIDTH-50, 10, 50, WINDOW_HEIGHT-20, GetColor(30, 30, 30), 200, 12, GetColor(240, 240, 240), GetColor(20, 20, 20), &BImgBank);
		//DEBUGDX("BattleLogWindowInit_End");

	//�p�[�e�B���X�g�̏�����//////////
		PlayerSpeciesManager->SetMemberList();
	//////////////////////////////////

	return true;
}

//�t�B�[���h���j���[��Status���ڂɂ����ׂ�MenuNode(Player�����X�g�Ƃ��̎q)������ĕԂ�
CMenuNode* CBattle::GetFieldStatusMenuFrontNode(const char _parentLabel[32]) {
	if (!PlayerSpeciesManager->CheckAfterLoad()) return NULL;

	CMenuNode* parentNode = new CMenuNode(_parentLabel);
	CPlayerSpecies* tmpPlayerSpecies;
	CMenuNode* newNode;
	CMenuNode* prevNode = NULL;

	CMenuNode* newAccessoryNode;
	CMenuNode* prevAccessoryNode = NULL;

	for (int i = 0; i < PlayerSpeciesManager->GetMemberListSize(); i++) {
		tmpPlayerSpecies = PlayerSpeciesManager->GetSpecies(i);
		newNode = new CMenuNode(tmpPlayerSpecies->GetName().c_str());
		if (prevNode != NULL) {
			prevNode->next = newNode;
			newNode->prev = prevNode;
		} else {
			parentNode->child = newNode;
		}
		newNode->parent = parentNode;
		prevNode = newNode;

		//�v���C���[�̑����i�\���B�󃁃j���[�����O�쐬���Ă����B���g�͕\���̍ۂ�Label�������������ē��I�ɍ쐬
		for (int j = 0; j < MAX_ACCESSORY_SLOT; j++) {
			newAccessoryNode = new CMenuNode("");
			newAccessoryNode->parent = newNode;

			if (j == 0) {
				newNode->child = newAccessoryNode;
			} else {
				prevAccessoryNode->next = newAccessoryNode;
				newAccessoryNode->prev = prevAccessoryNode;
			}
			prevAccessoryNode = newAccessoryNode;
		}
		if (prevAccessoryNode != NULL) {
			newNode->child->prev = prevAccessoryNode;
			prevAccessoryNode->next = newNode->child;
		}

	}

	if (prevNode != NULL) {
		parentNode->child->prev = prevNode;
		prevNode->next = parentNode->child;
	}

	return parentNode;
}

//�v���C���[�̍��̑����i���j���[������ĕԂ�FieldMenu�ɘA��������
	
void CBattle::UpdateFieldPlayerAccesssoryMenu(CMenuNode* _playerNodeParent) {

	CMenuNode* _playerNode = _playerNodeParent->child;
	CAccessoryItem* tmpAccessory;
	CMenuNode* accessoryNode;

	while (true){

		accessoryNode = _playerNode->child;

		for (int i = 0; i < MAX_ACCESSORY_SLOT && accessoryNode != NULL; i++) {
			tmpAccessory = PlayerSpeciesManager->GetAccessory(_playerNode->label, i);

			if (tmpAccessory != NULL) {
				mystrcpy(accessoryNode->label, tmpAccessory->Name.c_str());
			} else {
				mystrcpy(accessoryNode->label, NO_EQUIP);
			}

			accessoryNode = accessoryNode->next;
		}

		if (_playerNode->next != _playerNodeParent->child) {
			_playerNode = _playerNode->next;
		} else {
			break;
		}
	}	

}

void UpdatePlayerAccesssoryMenu(CMenuNode* _playerNode) {
	
}


void CBattle::BattleReady(CFlagSet* _flagset_p, CMap* _map_p, CEveManager* _evemanager_p){
	//�퓬���Ƃɍs���J�n������������///////////////////////////////////////////////////	
		
		//Actor�ւ������ݸނ�TextBox,LogWindow�ւ̕R�t��
			ACTOR_NUM = PLAYER_NUM + ENEMY_NUM;
			Actor = new CActor*[ACTOR_NUM];
			for (int i=0; i<ACTOR_NUM; i++){ 
				Actor[i] = ((i<PLAYER_NUM)? (CActor*)&Player[i]: (CActor*)&Enemy[i-PLAYER_NUM]);
				Actor[i]->FirstSet(PLAYER_NUM, ENEMY_NUM, i, &TextBox, &B_CmdList, &LogWindow);
				Actor[i]->SetSystemImg(&BImgBank);
			}

		//�����l�ݒ�
			for (int i=0; i<PLAYER_NUM; i++){
				Player[i].CreateBattleMenu(CItemManager::GetInstance()->GetBattleItemNameList());//���K��AddTrick�̌�ɂ��邱�Ɓi������BattleMenu������Ă��邽�߁j
				Player[i].SetRect(WINDOW_WIDTH/4*(i+1), WINDOW_HEIGHT-200);
			}
			
			for (int i=0; i<ENEMY_NUM; i++){
				Enemy[i].BattleReady(Actor, PLAYER_NUM, ENEMY_NUM);
				Enemy[i].SetRect(WINDOW_WIDTH/4*(i+1), 90);
			}

		//�^�[�Q�b�g�I���}�[�J�[������
			TargetMarker.Init(ACTOR_NUM, PLAYER_NUM, ENEMY_NUM);

		EveManager_p = _evemanager_p;
		FlagSet_p = _flagset_p;
		B_CmdManager.Init(_map_p);

		Ready = true;
}

void CBattle::BattleStart(int* _result, CCmdList* _fieldcmdlist_p){
	if (!Ready){
		ErrorDx("Error->BattleStart->Battle isn't Ready to Start.", __FILE__, __LINE__);
		*_result = -1;
	}else{

		//�J�n���o//////////////////////////////////////////////////////////
			StartEffect();
		///////////////////////////////////////////////////////////////////

		//���C�����[�v����///////////////////////////////////////////////////
			int tmp_result = MainLoop();

		//�I������///////////////////////////////////////////////////
			BattleFinish(tmp_result, _fieldcmdlist_p);
			*_result = tmp_result;
	}
}
void CBattle::BattleSetting(const char* _wincommand, const char* _losecommand){
	mystrcpy(WinCommand, _wincommand);
	mystrcpy(LoseCommand, _losecommand);
}

bool CBattle::CheckEncount(int _mapnum, int _chipnum){
	std::vector <CEnemySpecies*> tmpparty;

	if (EnemySpeciesManager->CheckEncount(_mapnum, _chipnum, tmpparty)){
		SetEnemy(tmpparty);
		SetBackGround(_mapnum, _chipnum);
		return true;
	}else{
		return false;
	}
}

void CBattle::SetBackGround(const char* _pickey){
	Img_BattleBackGround = BImgBank.GetImg(_pickey);
}
void CBattle::SetBackGround(int _mapnum, int _chipnum){
	Img_BattleBackGround = BImgBank.GetBattleBackGround(_mapnum, _chipnum);
}
void CBattle::SetPlayer(){	//����Ɋ�Â��đI�o
	PLAYER_NUM = min(PlayerSpeciesManager->GetMemberListSize(), MAX_PLAYER_NUM);
	if (PLAYER_NUM==0){
		ErrorDx("Error->Battle::SetPlayer->PLAYER_NUM=0.  SetPartyBattleMember!", __FILE__, __LINE__);
		return;
	}
	Player = new CPlayer[PLAYER_NUM];
	for (int i=0; i<PLAYER_NUM; i++){
		Player[i] = CPlayer(*(PlayerSpeciesManager->GetSpecies(i)));
	}
}
void CBattle::SetPlayer(const int _playerNum, ...){	//�p�[�e�B���R�w��p�i�C�x���g�퓬�j
	PLAYER_NUM=0;
	va_list args;
	va_start( args, _playerNum);
	
	if (_playerNum<=0){
		ErrorDx("Error->arg[playerNum] should >=1: playerNum=%d", __FILE__, __LINE__, _playerNum);
	}else{		
		PLAYER_NUM = _playerNum;
		Player = new CPlayer[PLAYER_NUM];
		for (int i=0; i<PLAYER_NUM; i++){
			Player[i] = CPlayer(*(PlayerSpeciesManager->GetSpecies(va_arg(args, char*))));
		}
		va_end(args);
	}
}
void CBattle::SetEnemy(const int _enemyNum, ...){	//�o�������X�^�[���R�w��p
	va_list args;
	va_start( args, _enemyNum);	//target���傫�������Ƃ��̏��u���@�͂Ȃ��̂��H
	
	if (_enemyNum<=0){
		ErrorDx("Error->arg[enemyNum] should >=1: enemyNum=%d", __FILE__, __LINE__, _enemyNum);
	}else{
		std::vector<std::string> enemyList;
		for (int i=0; i<_enemyNum; i++){
			enemyList.push_back(va_arg(args, char*));
		}
		va_end(args);
	}
}
void CBattle::SetEnemy(std::vector<std::string> _enemyList){
	if (_enemyList.size()==0) ErrorDx("Error->Battle::SetEnemy->enemyNum=0 :%d", __FILE__, __LINE__, _enemyList.size());

	ENEMY_NUM = _enemyList.size();
	Enemy = new CEnemy[ENEMY_NUM];
	for (int i=0; i<ENEMY_NUM; i++){
		Enemy[i] = CEnemy(*(EnemySpeciesManager->GetSpecies(_enemyList[i].c_str())));
	}
}
void CBattle::SetEnemy(std::vector<CEnemySpecies*> _enemyParty){
	if (_enemyParty.size()==0) ErrorDx("Error->Battle::SetEnemy->enemyNum=0 :%d", __FILE__, __LINE__, _enemyParty.size());

	ENEMY_NUM = _enemyParty.size();
	Enemy = new CEnemy[ENEMY_NUM];
	for (int i=0; i<ENEMY_NUM; i++){
		Enemy[i] = CEnemy(*(_enemyParty[i]));
	}
}

void CBattle::StartEffect(){	//�퓬�J�n���o

	char enemyName[256];
	enemyName[0]='\0';
	for (int i=0; i<ENEMY_NUM; i++){
		mystrcat(enemyName, Enemy[i].GetName().c_str());
		mystrcat(enemyName, " ");
	}

	LogWindow.Add("�y�퓬�J�n�z%s", enemyName);

	int x=WINDOW_WIDTH+20;
	int timecount=0;
	CRect obi(0, WINDOW_WIDTH, 230, 230); 

	do{
		SetTitle("%s", enemyName);
		
		Draw();
	
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
		DrawBox(obi.Left, obi.Top, obi.Right, obi.Bottom, BLACK, true);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
		DrawCenterString(x, (int)(obi.Center().y), WHITE, true, "���I�₹����%s�������ꂽ�I", enemyName);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);

		if (timecount<13) {obi.SetHeight(timecount*4);}
		else if (timecount>100) {obi.SetHeight(max(0,obi.Height()-5));}

		if (x>WINDOW_WIDTH/2+160) {x-=10;}
		else if(x<WINDOW_WIDTH/2-160) {x-=18;}
		else{ x-=4;}

		++timecount;
		if (timecount==120) break;


	}while(BasicLoop());

}

int CBattle::MainLoop(){	//�퓬���͂��̃��[�v������o�Ȃ�
	int result;

	///�퓬�J�n�����ʔ����i�A�N�Z�T���[�Ȃǁj//////////////////////////
	for (int i = 0; i < PLAYER_NUM; i++) {
		for (int j = 0; j < MAX_ACCESSORY_SLOT; j++) {
			CAccessoryItem* item = PlayerSpeciesManager->GetAccessory(Player[i].GetName(), j);

			if (item == NULL) {
				continue;
			} else {

				LogWindow.Add("%s��%s�̌��ʔ����I", Player[i].GetName().c_str(), item->Name.c_str());
				char tmpCmd[1024];
				sprintf_s(tmpCmd, "@Item_Use(%s, %d, %d)", item->Name.c_str(), Player[i].GetActorIndex(), Player[i].GetActorIndex());
				B_CmdList.Add(tmpCmd);
				DEBUGDX(tmpCmd);
				B_CmdManager.Main(&B_CmdList, this, TextBox);
			}

		}
	}

	B_CmdManager.Main(&B_CmdList, this, TextBox);
	Draw();
	BasicLoop();
	///////////////////////////////////////////////////////////////////

	do{		
		if(!LogWindow.Main() && !TextBox->Main(&B_CmdList, FlagSet_p)){	//���O�\�������邢�̓e�L�X�g�\�����̓L�[���얳���i�e�L�X�g�����TextBox.Main�Ŕ���j

			if ((result=ResultCheck())!=-1){
				B_CmdManager.Main(&B_CmdList, this, TextBox);
				Draw();
				return result;
			}


			if (ActionQueue.empty()){	//�s���ҋ@���X�g���󂩃`�F�b�N
			
				for (int i=0; i<ACTOR_NUM; i++){
					if (Actor[i]->Main()) ActionQueue.push(Actor[i]);		//TimeBar�i�܂��ĕK�v�Ȃ�s���ҋ@���X�g�ɓ����
				}

			}else{
				if (ActionQueue.front()->Do()){
					ActionQueue.pop();
				}else{
					//�s���ҋ@���X�g�퓬�̃L�����̍s�����������Ȃ������Ƃ��ɂ͂Ȃɂ����Ȃ�
				}
			}

		}

		////TextBox�Ȃǂɂ����CmdList�ɒ~�ς��ꂽ�R�}���h������////////////////////////////////////////
			B_CmdManager.Main(&B_CmdList, this, TextBox);

		////�`��////////////////////////////////////////
			Draw();
	}while(BasicLoop());

	return -1;
}

void CBattle::BattleFinish(int &_result, CCmdList* _fieldcmdlist){
	if (!Ready) {WarningDx("Warning->BattleFinish->Battle hasn't started yet.(do nothing return)", __FILE__, __LINE__); return;}
	Ready = false;
	while(!ActionQueue.empty()) {ActionQueue.pop();}
	B_CmdList.Clear();
	////////////////////////////////////////////////////////////
	
	LogWindow.Add("�y�퓬�I���z%s", (_result==WIN?"Win":"Lose"));

	switch (_result){
	case WIN:
		{//�R���p�C���G���[����ׂ̈ɃX�R�[�v�𐧌�
		//���O�o�^�����R�}���h���t�B�[���h�̃R�}���h���X�g�ɑ}��
			if (strlen(WinCommand)) _fieldcmdlist->Add(WinCommand);

		//���ƌo���l�v�Z
			int gold = 0;
			int exp = 0;
			for (int i = 0; i < ENEMY_NUM; i++) {
				gold += CBattleCalc::CalcGold(	Enemy[i].GetLevel(), Enemy[i].GetGoldGene());
				exp	 += CBattleCalc::CalcExp(	Enemy[i].GetLevel(), Enemy[i].GetExpGene());
			}

			CItemManager::GetInstance()->IncGold(gold);
			PlayerSpeciesManager->AddExp(exp);

		//�h���b�v�A�C�e��
			std::vector<std::string> gotItemList;
			for (int i = 0; i < ENEMY_NUM; i++) {
				std::vector <std::string> tmp = Enemy[i].GetDropItemList();
				for (unsigned int i = 0; i < tmp.size(); i++) {
					if (CItemManager::GetInstance()->IncPlayerItem(tmp[i], 1)) {
						gotItemList.push_back(tmp[i]);
					}
				}
			}
		
		//Player���f�[�^�ɕۑ�
			PlayerSpeciesManager->CopyValue(PLAYER_NUM, Player);	//PlayerSpecies�z��œn���������L���X�g�ł̓������z�u�̊֌W��z��ł̎Q�Ƃ�����邽�ߒf�O

		//���U���g��ʏo��
			char resultMessage[2][256];
			sprintf_s(resultMessage[0], "�擾Gold�F%d", gold);
			sprintf_s(resultMessage[1], "�擾Exp�F%d", exp);
			CRect resultScreen(200, WINDOW_WIDTH-200, 100, WINDOW_HEIGHT-100);
			int timecount=0;

			do {
				Draw();

				SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)(200 * min(timecount, 60) / (double)60));
				DrawBox(resultScreen, BLACK, true);
				DrawString(resultScreen.Left + 30, resultScreen.Top + 30, resultMessage[0], WHITE);
				DrawString(resultScreen.Left + 30, resultScreen.Top + 60, resultMessage[1], WHITE);

				for (unsigned int i = 0; i < gotItemList.size(); i++) {
					DrawString(resultScreen.Left + 30, resultScreen.Top + 90 + i * 20, gotItemList[i].c_str(), WHITE);
				}

				if (timecount >= 60) {
					DrawCenterString((int)resultScreen.Center().x, (int)resultScreen.Bottom - 10 + (timecount / 6) % 5, WHITE, "��");
				}
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

				++timecount;
				if (CheckHitKeyDown(KEY_INPUT_OK)) {
					if (timecount < 60) {
						timecount = 60;
					} else {
						break;
					}
				}
			}while(BasicLoop());


		}
		break;
	case LOSE:
		if (strlen(LoseCommand)) _fieldcmdlist->Add(LoseCommand);
		if (mystrcmp(LoseCommand, "@GameOver")) _result = LOSE_NOSCREENCHANGE;	//�s�k���������ʏ�Ȃ�t�B�[���h�ɖ߂��Ȃ�
		break;
	default:
		break;
	}

	while(BasicLoop()){
		if( !TextBox->Main(&B_CmdList, FlagSet_p)) break;
		B_CmdManager.Main(&B_CmdList, this, TextBox);
		Draw();
	}

	Draw();	//����ʂɕ`��i��ʐ؂�ւ����o�p�j

	//////////////////////////////////////////////////////
	//�퓬�֘A�ϐ��̏�����
	delete [] Actor;
	delete [] Player;
	delete [] Enemy;

	ACTOR_NUM = 0;
	ENEMY_NUM = 0;
	PLAYER_NUM = 0;

	WinCommand[0] = '\0';
	mystrcpy(LoseCommand, "@GameOver");

	LogWindow.Clear();

	myLogf("BattleResult", "%s", (_result==WIN?"Win":"Lose"));
}

void CBattle::Draw(bool _screenflip, bool _textshowingstop, int dx, int dy, bool _playeralsoshake){
	
	if (ImgBackGround!=NULL){	//�w�i�ꖇ�G���[�h�̂Ƃ�
		CVector picsize = GetGraphSize(ImgBackGround);
		DrawGraph(WINDOW_WIDTH/2-picsize.x/2, WINDOW_HEIGHT/2-picsize.y/2, ImgBackGround, true);
	}else{

		//�w�i�`��////////////////////////////////////////////////////////////
		SetDrawBright(200, 200, 200);
		DrawExtendGraph(0,0,WINDOW_WIDTH, WINDOW_HEIGHT, Img_BattleBackGround,true);
		SetDrawBright(255,255,255);
		/////////////////////////////////////////////////////////////////////

		//Player��Enemy�̕`��///////////////////////////////////////////////////////////////
		for (int i=0; i<ACTOR_NUM; i++){
			Actor[i]->Draw(dx, dy);
		}

		//�^�[�Q�b�g�}�[�J�[�̕`��//////////////////////////////////////////////
		SetDrawBright(255,255,255);
		TargetMarker.Draw(dx,dy);

	}

	////////////////////////////////////////////////////////////////////////////////////////
	//�e�L�X�g�{�b�N�X�`��//////////////////////////////////////////////////////////////////
	TextBox->Draw(_textshowingstop);
	////////////////////////////////////////////////////////////////////////////////////////
	
	////////////////////////////////////////////////////////////////////////////////////////
	//���O�E�B���h�E�`��//////////////////////////////////////////////////////////////////
	LogWindow.Draw();
	////////////////////////////////////////////////////////////////////////////////////////
	

	///�e�X�g�p//////////////////////////////////////////////////////////////////////////
	SetDrawBlendMode(DX_BLENDMODE_ALPHA , (int)(150+60*sin(2*PI*GetNowCount()/((double)2000))));
	DrawCenterString(WINDOW_WIDTH/2,WINDOW_HEIGHT/2,"�퓬��---W�܂���L�ŏI��",RED);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);
	///////////////////////////////////////////////////////////////////////////////////////

	if (_screenflip) BasicLoop();

}

int CBattle::ResultCheck(){
	//DEBUG:�퓬�I���V���[�g�J�b�g//////////////////////////////
		if (CheckHitKeyDown(KEY_INPUT_W) && ActionQueue.empty()) return WIN;
		if (CheckHitKeyDown(KEY_INPUT_L) && ActionQueue.empty()) return LOSE;
	////////////////////////////////////////////////////////////

	int i;
	for (i=0; i<PLAYER_NUM; i++){
		if (Player[i].GetAlive()) break;
	}
	if (i==PLAYER_NUM) return LOSE;

	for (i=0; i<ENEMY_NUM; i++){
		if (Enemy[i].GetAlive()) break;
	}
	if (i==ENEMY_NUM) return WIN;

	return -1;
}

void CBattle::ManageAttack(int _attackerActorIndex, int _targetActorIndex, trick_tag const* _trick){
	bool noDamage = !(_trick->Power > 0);

	//�G���[�h�~����
		_attackerActorIndex = between(0, ACTOR_NUM-1, _attackerActorIndex); 
		_targetActorIndex   = between(0, ACTOR_NUM-1, _targetActorIndex); 

	//�^�[�Q�b�g���m�肵Actor�ԍ���vector���X�g��	//���l�͏��O
		std::vector <int> targetList;	//�^�[�Q�b�g��Actor�ԍ�	
		switch (_trick->Target) {
		case target_tag::ME:
		case target_tag::SINGLE_ENEMY:
		case target_tag::SINGLE_FRIEND:
			if (Actor[_targetActorIndex]->GetAlive()) targetList.push_back(_targetActorIndex);
			break;
		case target_tag::ALL_ENEMY:
			for (int i = (Actor[_attackerActorIndex]->IsPlayer() ? PLAYER_NUM : 0); i < (Actor[_attackerActorIndex]->IsPlayer() ? ACTOR_NUM : PLAYER_NUM); i++) {
				if (Actor[i]->GetAlive()) {	 //�����m�F	
					targetList.push_back(i);
				}	
			}
			break;
		case target_tag::ALL_FRIEND:
			for (int i = (!Actor[_attackerActorIndex]->IsPlayer() ? PLAYER_NUM : 0); i < (!Actor[_attackerActorIndex]->IsPlayer() ? ACTOR_NUM : PLAYER_NUM); i++) {
				if (Actor[i]->GetAlive()) {	 //�����m�F	
					targetList.push_back(i);
				}
			}
			break;
		default:
			WARNINGDX("_trick->TargetType->Not Found. %s", _trick->Name);
			break;
		}
	
	//�U���Ώۂ����Ȃ��ꍇ
		if (targetList.empty() && !noDamage) {
			LogWindow.Add("  �������U���͊O�ꂽ�I");
			return;
		}

	//�Z�̎�ނɉ������`��G�t�F�N�g����
		if (_trick->DamageEffectIndex!=-1) TrickManager->DrawEffect(_trick->DamageEffectIndex, this, &BImgBank, Actor[_attackerActorIndex]->GetRect(), Actor[targetList[0]]->GetRect());


		if (!noDamage) {
			//�Ԃ̒���
				int timecount = 0;
				do { Draw(); if (++timecount == 10) { break; } } while (BasicLoop());

			//���ۂ̃_���[�W�v�Z
				int* damage = new int[targetList.size()];

				for (unsigned int i = 0; i < targetList.size(); i++) {
					damage[i] = Actor[targetList[i]]->Damaged(Actor[_attackerActorIndex], _trick);
				}

			//���O�E�B���h�E�ɏo��
				for (int i = 0; i < (int)(targetList.size()); i++) {
					LogWindow.Add("  %s��%d�̃_���[�W�I", Actor[targetList[i]]->GetName().c_str(), damage[i]);
				}


			//�_���[�W�l�\�����o//////////////////////////////////////////////////////////////
				timecount = 0;
				bool* oldVisible = new bool[targetList.size()];
				CVector* charPos = new CVector[targetList.size()];

				for (unsigned int i = 0; i < targetList.size(); i++) {
					oldVisible[i] = Actor[targetList[i]]->GetVisible();
					charPos[i] = CVector(Actor[targetList[i]]->GetRect().Center().x, Actor[targetList[i]]->GetRect().Top);
				}

				do {
					Draw();

					for (unsigned int i = 0; i < targetList.size(); i++) {
						DrawCenterString((int)(charPos[i].x), (int)(charPos[i].y - 5 * sin(min(timecount, 10)*(PI / 2) / 10)), WHITE, "%d", damage[i]);

						if (timecount % 6 == 0) Actor[targetList[i]]->SetVisible(oldVisible[i] && true);
						if (timecount % 6 == 3) Actor[targetList[i]]->SetVisible(false);
					}
					if (++timecount > 40) {
						for (unsigned int i = 0; i < targetList.size(); i++) {
							Actor[targetList[i]]->SetVisible(oldVisible[i]);
						}
						break;
					}

				} while (BasicLoop());

				delete[] damage;
				delete[] oldVisible;
				delete[] charPos;
			////////////////////////////////////////////////////////////////////////////
		


			//�U���҂�Player�ōU���Ώۂ�Enemy�������ꍇ�A�e���V�������ϓ�////
				if (!noDamage && Actor[_attackerActorIndex]->IsPlayer()) {
					for (int i=0; i<(int)(targetList.size()); i++){	
						if (!Actor[targetList[i]]->IsPlayer()) Enemy[Actor[targetList[i]]->GetIndex()].AddAttention(_attackerActorIndex, ATTENTION_DAMAGE);	
					}
				}


			//HP�o�[������҂�///////////////////////////////////////////////
				std::vector <bool> hpBarMoved(targetList.size());
				unsigned int i;
				bool allOk;
				while(true){
					Draw();
					if (!BasicLoop()) break;

					allOk = true;
					for (i=0; i<targetList.size(); i++){
						if (!hpBarMoved[i]) {
							if (!(hpBarMoved[i] = Actor[targetList[i]]->CheckBarMove())) allOk = false;
						}
					}
					if (allOk) break;
				}

			//HACK:���S���b�Z�[�W�͂��ׂĂ�HP�o�[�ړ����I����Ă���܂Ƃ߂ďo��ׂ��B
			////////////////////////////////////////////////////////////////

			//�T�C�h�G�t�F�N�gHEAL_AFTER_ATTCK�ɂ���////////////////////
				if (Actor[_attackerActorIndex]->GetStatus(sideEffect_tag::type_tag::HEAL_AFTER_ATTACK)) {
					Actor[_attackerActorIndex]->Heal(Actor[_attackerActorIndex]->GetStatus(sideEffect_tag::type_tag::HEAL_AFTER_ATTACK));
				}
			////////////////////////////////////////////////////////////////


		}

	//�T�C�h�G�t�F�N�g�̗L���m�F�Ɣ���//////////////////////////////////////////////////////////
		for (unsigned int i=0; i<_trick->SideEffect.size(); i++){
			InvokeSideEffect(_trick->SideEffect[i], _attackerActorIndex, _targetActorIndex);
		}
	//�T�C�h�G�t�F�N�g�̏��������܂�////////////////////////////////////////////////////////////////////////////////////////////
	
}

void CBattle::InvokeSideEffect(sideEffect_tag _sideEffect, int _invokerActorIndex, int _cursorTargetActorIndex){

	//�G���[�h�~����
	_invokerActorIndex = between(0, ACTOR_NUM - 1, _invokerActorIndex);
	_cursorTargetActorIndex = between(0, ACTOR_NUM - 1, _cursorTargetActorIndex);

	//�^�[�Q�b�g���m�肵Actor�ԍ���vector���X�g��
	std::vector <int> effectTargetList;	//�^�[�Q�b�g��Actor�ԍ�	
	effectTargetList.clear();

	switch (_sideEffect.EffectTarget) {
	case target_tag::ME:
		effectTargetList.push_back(_invokerActorIndex);
		break;
	case target_tag::SINGLE_ENEMY:	//�J�[�\���I������Actor
	case target_tag::SINGLE_FRIEND:	//�J�[�\���I������Actor
		effectTargetList.push_back(_cursorTargetActorIndex);
		break;
	case target_tag::ALL_ENEMY:	//�G�S��
		for (int i = (Actor[_invokerActorIndex]->IsPlayer() ? PLAYER_NUM : 0); i<(Actor[_invokerActorIndex]->IsPlayer() ? ACTOR_NUM : PLAYER_NUM); i++) {
			if (Actor[i]->GetAlive()) {
				effectTargetList.push_back(i);
			}
		}
		break;
	case target_tag::ALL_FRIEND:	//�����S��
		for (int i = (Actor[_invokerActorIndex]->IsPlayer() ? 0 : PLAYER_NUM); i<(Actor[_invokerActorIndex]->IsPlayer() ? PLAYER_NUM : ACTOR_NUM); i++) {
			if (Actor[i]->GetAlive()) {
				effectTargetList.push_back(i);
			}
		}
		break;
	default:
		WARNINGDX(">SideEffectTargetType->Not Match. :%d", _sideEffect.EffectTarget);
		break;
	}

	//���ʔ�������̂̂��^�[�Q�b�g���Ƃɏ�����������
	if (_sideEffect.Incidence > rand() % 100) {
		for (unsigned int j = 0; j < effectTargetList.size(); j++) {
			switch (_sideEffect.EffectType) {
			case sideEffect_tag::type_tag::ATK:
			case sideEffect_tag::type_tag::DEF:
			case sideEffect_tag::type_tag::SPD:
				Actor[effectTargetList[j]]->AddStatusChanger(_sideEffect.EffectType, _sideEffect.Power, _sideEffect.Time);
				break;
			case sideEffect_tag::type_tag::HEAL:
				Actor[effectTargetList[j]]->Heal(_sideEffect.Power);
				break;
			case sideEffect_tag::type_tag::MPHEAL:
				if (Actor[effectTargetList[j]]->IsPlayer()) Player[Actor[effectTargetList[j]]->GetIndex()].MpHeal(_sideEffect.Power);
				break;
			case sideEffect_tag::type_tag::ATTENTION:
				if (!Actor[effectTargetList[j]]->IsPlayer()) Enemy[Actor[effectTargetList[j]]->GetIndex()].AddAttention(_invokerActorIndex, _sideEffect.Power, &LogWindow);
				break;
			case sideEffect_tag::type_tag::SET_TIMEGAUGE:
				Actor[effectTargetList[j]]->ChangeValue(_sideEffect.EffectType, _sideEffect.Power);
				break;
			case sideEffect_tag::type_tag::HEAL_AFTER_ATTACK:
			case sideEffect_tag::type_tag::HEAL_AFTER_SELECTCOMMAND:
				Actor[effectTargetList[j]]->SetStatus(_sideEffect.EffectType, _sideEffect.Power);
				break;
			default:
				WARNINGDX("_trick->SideEffectType->Not Match. :%d", _sideEffect.EffectType);
			}
		}
	}
}
void CBattle::AddAttention(int _enemyIndex, int _playerIndex, int _value) {
	NUM_MAX_CHECK(_enemyIndex, ENEMY_NUM, );
	NUM_MAX_CHECK(_playerIndex, PLAYER_NUM, );
	Enemy[_enemyIndex].AddAttention(_playerIndex, _value);
}

void CBattle::SetAttention(int _enemyIndex, int _playerIndex, int _value) {
	NUM_MAX_CHECK(_enemyIndex, ENEMY_NUM, );
	NUM_MAX_CHECK(_playerIndex, PLAYER_NUM, );
	Enemy[_enemyIndex].SetAttention(_playerIndex, _value);
}

//////////////////////////////////////////////////////////////
void CBattle::CTargetMarker::SetImage(int _img){
	Img = _img;
	GetGraphSize(Img, &ImgSizeX, &ImgSizeY);
}
void CBattle::CTargetMarker::Init(int _actornum, int _playernum, int _enemynum){
	ACTOR_NUM = _actornum;
	PLAYER_NUM = _playernum;
	ENEMY_NUM = _enemynum;
	Visible = false;
	EnemySide = true;
	Index = 0;
	Status = 0;
}

void CBattle::CTargetMarker::CheckNowIndex(CBattle* _battle){
	if (!DeadOk && !_battle->Actor[Index+(EnemySide?PLAYER_NUM:0)]->GetAlive()) {
		Move(RIGHT, _battle);
	}
}

void CBattle::CTargetMarker::Draw(int dx, int dy){
	if (Visible){
		SetDrawBlendMode(DX_BLENDMODE_ALPHA , 255);
		static int timecount=0;
		timecount++;

		if (Status==0){
			if (timecount==60) timecount=0;
			dy += (int)(5*sin((timecount)*2*PI/60));
		}else if(Status==1){
			timecount=0;
			Status=2;
		}else if(Status==2){
			if (timecount>30){ 
				Status=0; Visible=false; 
				timecount=0;
			}
		}
		
		if (!(Status==2 && timecount%10<6)){
			if(EnemySide){
				DrawGraph(WINDOW_WIDTH/4*(Index+1)-ImgSizeX/2+dx, 20+dy, Img, true);
			}else{
				DrawGraph(WINDOW_WIDTH/4*(Index+1)-ImgSizeX/2+dx, WINDOW_HEIGHT-290+dy, Img, true);			
			}
		}

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);
	}
}

void CBattle::CTargetMarker::Move(int _dir, CBattle* _battle, int _count){
	if (Status==1) return;	//�o�O�\�h
	switch (_dir){
	case RIGHT:
		Index = mod(Index+1,(EnemySide? ENEMY_NUM: PLAYER_NUM));
		break;
	case LEFT:
		Index = mod(Index-1,(EnemySide? ENEMY_NUM: PLAYER_NUM));
		break;
	default:
		break;
	}

	if (!DeadOk && !_battle->Actor[Index+(EnemySide?PLAYER_NUM:0)]->GetAlive()) {
		if (_count >= (EnemySide? ENEMY_NUM: PLAYER_NUM)) {	//�������[�v�h�~
			ERRORDX("All targets are Dead. INFINITY LOOP! (MoveCancel)");
			return;
		} else {
			Move(_dir, _battle, ++_count);
		}
	}
}

void CBattle::CTargetMarker::Decide(CBattle* _battle, int _actorIndex){
	int actorIndex = between(0, ACTOR_NUM-1, _actorIndex); 

	if (!DeadOk && !_battle->Actor[Index + (EnemySide?PLAYER_NUM:0)]->GetAlive()){
		return;
	}else{
		_battle->Actor[actorIndex]->SetTarget(Index + (EnemySide?PLAYER_NUM:0));
		Status = 1;
		do {
			_battle->Draw();	//�J�[�\���m��_�ŉ��o
		}while(BasicLoop()&&Status!=0);
	}

}

void CBattle::ChangeTextMode(bool _box, const char* _eventtext){
	if (_box){
		TextBox = &TextBox1;
	}else{
		TextBox = &TextWrap1;

		if (_eventtext!=NULL){	//EveManager::CopyOriginalEvent��ėp�����グ�ĉ��P�B�����TextWrap1��@EventWrap�̓��e��n����
			std::vector<char256> tmptext;
			EveManager_p->CopyOriginalEvent(&tmptext, _eventtext);
			for (unsigned int i=0; i<tmptext.size(); i++){
				TextWrap1.AddStock(tmptext[i].text);
			}
			TextBox->NextPage(&B_CmdList, FlagSet_p);
		}
	}
};
