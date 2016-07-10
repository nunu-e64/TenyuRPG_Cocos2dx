#include "Field.h"
#include "Load.h"
#include "EveManager.h"
#include "ItemManager.h"
#include "AlchemistManager.h"

#include "../Battle/Battle.h"
#include "../Main/MusicManager.h"

CField::CField(){
	CONSTRUCTED;
	Battle = CBattle::GetInstance();
	EveManager = CEveManager::GetInstance();
}

CField::~CField(){
	DESTRUCTED;	

	Map.Init();
	EveManager->Init();
}

bool CField::Init(playdata_tag* _playdata_p, const int _dnum){
	CHECK_TIME_START
	
		//諸変数の初期化
			NowMap=-1;
			GodX = GodY = 0;

			X = Y = 0;		OldX=X; OldY=Y;
			Dir=DOWN; 
			Step=0;			Dx = Dy = 0; 
			Visible=true;
			Alpha = 255;		
			Effect=NONE;
			ImgBackGround = NULL;
			TextAutoPlaySpeed = 1000;

			Mode = MODE_PLAYING;

		//ItemManagerの初期化
			CItemManager::GetInstance()->Init();

		//ShopManagerの初期化
			CShopManager::GetInstance()->Init();
			CAlchemistManager::GetInstance()->Init();

		//MusicManagerの初期化
			CMusicManager::GetInstance()->Init();

		//ログウィンドウの初期化
			FieldLog.Init(50, 50, WINDOW_WIDTH-100, WINDOW_HEIGHT-100, BLACK, 300, 12, WHITE, GRAY);

	//DebugDx("TextBox_Init_Start");

		//メインのテキストボックスとオーバーラップ用テキストボックスの初期化
			TextBox1.Init(60, 370, WINDOW_WIDTH-80*2, 100, 3, 25*2, 16, WHITE, BLACK, TextAutoPlaySpeed, &FieldLog);
			TextWrap1.Init(100, 100, 400, 300, 30, 30 * 2, 14, WHITE, GRAY, TextAutoPlaySpeed, &FieldLog);
			TextBox = &TextBox1;

	//DebugDx("TextBox_Init_End");

		////WorldManagerのポインタ変数に代入
		//	EveManager_p = &EveManager;
		//	FlagSet_p = &FlagSet;
		//	CmdList_p = &CmdList;
		//	DebugDx("OK");
		//	

		
		SetTransColor(0, 0, 0);	//HACK:透過色指定 マゼンダで統一を
			
		//外部テキストのロード
			CLoad SystemLoad;
			CLoad ScenarioLoad;
			CFirstSetCmdManager FirstSetCmdManager;
			CCmdList SystemCmdList;
			
			if ((SystemLoad.LoadAddText(FILE_SYSTEM))
				&&(ScenarioLoad.LoadAddText(FILE_SCENARIO))
				&&(ScenarioLoad.LoadAddText(FILE_EVENT))){
			
				SystemLoad.CommandCopy(&SystemCmdList);
				FirstSetCmdManager.Main(&SystemCmdList, this, &Map, EveManager);

				ScenarioLoad.EventTextCopy(EveManager);	//順序に注意（CmdManager.Mainの後）	
			
			}else{
				return false;
			}
			//DebugDx("Load_Init_End");
	
		SetTransColor(255, 0, 255);	//透過色指定

		//CBattleの初期化
		//DEBUGDX("Battle_Init_Start");
		if (!(Battle->Init())) return false;
		//DEBUGDX("Battle_Init_End");
	
			
		//セーブデータの読み込み
			if (PLAYDATA_NUM>0) PlayData_p = _playdata_p;
			if (!StartSet(_dnum)) return false;
		
		//フィールドメニューの作成
			CreateFieldMenu();


	CHECK_TIME_END("Init_Field")
		
	return true;
}

void CField::CreateFieldMenu() {
	FieldMenu.Init(20, 30, 400, 50);
	FieldMenu.Create(Battle->GetFieldStatusMenuFrontNode("Status"));
}

int CField::MainLoop(){	//ゲーム中はこのループ内から出ない


	while( BasicLoop() ){
	
		CHECK_TIME_START	


		if (FieldLog.Main()) {	//ログ表示中はキー操作無効（表示非表示切り替えはMain内で判定）

		} else if (CAlchemistManager::GetInstance()->Main() || CShopManager::GetInstance()->Main()) {
		
		}else if (TextBox->Main(&CmdList, &FlagSet)) {	//テキスト表示中はキー操作無効（テキスト送りはTextBox.Mainで判定）
		
		} else if (FieldMenu.Alive) {

			if (!FieldMenu.AccessoryMenuVisible) {

				//処理が複雑化してきたら、状態をCursorだけではなくてenum作って管理させたほうがよい
				CMenuNode* oldCursor = FieldMenu.GetCursor();
				CMenuNode* result;

				if (FieldMenu.Move(result, false)) {
					if (FieldMenu.GetCursor() == oldCursor && FieldMenu.GetCursor()->parent->child == FieldMenu.GetFront()) {	//一番上かつ変化がないのは一番上でX押したときだけ。たぶん。
						FieldMenu.Alive = false;

					} else if (mystrcmp(FieldMenu.GetCursor()->parent->label, "Status")) {		Battle->UpdateFieldPlayerAccesssoryMenu(FieldMenu.GetCursor()->parent);

					} else if (FieldMenu.GetCursor()==oldCursor &&  mystrcmp(FieldMenu.GetCursor()->parent->parent->label, "Status")) {
						FieldMenu.AccessorySlotNum = FieldMenu.GetIndex(result);

						FieldMenu.AccessoryMenuVisible = true;
						if (FieldMenu.AccessoryMenu != NULL) delete FieldMenu.AccessoryMenu;
						FieldMenu.AccessoryMenu = CItemManager::GetInstance()->GetPlayerAccessoryMenu();
					}
				}
				
			} else {
				//装備メニュー

				//装備一覧表示
				CMenuNode* result;
				if (FieldMenu.AccessoryMenu != NULL && FieldMenu.AccessoryMenu->Move(result, true)) {

					if (result != NULL) {
						std::string tmpCmd;

						if (mystrcmp(result->label, REMOVE_EQUIP) || FieldMenu.AccessoryMenu->GetIndex(result)==0) {
							tmpCmd = "@Accessory_Set(" + std::string(FieldMenu.GetCursor()->parent->label) + ", " + std::to_string(FieldMenu.AccessorySlotNum) + ", NULL)";

							//装備名のラベルを書き換える(実際に内部装備が変わるのはコマンド処理の時)
							mystrcpy(FieldMenu.GetCursor()->label, NO_EQUIP);

						} else {
							//Menuは"装備しない"を含むためGetIndexから-1する

							tmpCmd = "@Accessory_Set(" + std::string(FieldMenu.GetCursor()->parent->label) + ", " + std::to_string(FieldMenu.AccessorySlotNum) + ", " + CItemManager::GetInstance()->GetAccessoryItemInBag()[FieldMenu.AccessoryMenu->GetIndex(result) - 1] + ")";

							//装備名のラベルを書き換える(実際に内部装備が変わるのはコマンド処理の時)
							mystrcpy(FieldMenu.GetCursor()->label, CItemManager::GetInstance()->GetAccessoryItemInBag()[FieldMenu.AccessoryMenu->GetIndex(result) - 1].c_str());

						}

						//DEBUGDX(tmpCmd.c_str());
						CmdList.Add(tmpCmd.c_str());

					}

					FieldMenu.AccessoryMenuVisible = false;
					if (FieldMenu.AccessoryMenu != NULL) delete FieldMenu.AccessoryMenu;
					FieldMenu.AccessoryMenu = CItemManager::GetInstance()->GetPlayerAccessoryMenu();

				}

			}

		} else {

			if (OldX != X || OldY != Y) {		//アクションコマンドによる移動判定用
				OldX = X; OldY = Y;
				if (CheckEvent(true)) {
					TextBox->NextPage(&CmdList, &FlagSet);		//足元にテキストが設定してあれば表示
				}

			}else if (CheckHitKeyDown(KEY_INPUT_OK)){
				if (CheckEvent(false)) TextBox->NextPage(&CmdList, &FlagSet);		//目の前のオブジェクトにテキストが設定してあれば表示

			} else if (CheckHitKeyDown(KEY_INPUT_CANCEL)) {
				FieldMenu.Alive = true;

			}else{
				//歩行/////////////////////////////////////////////////////
				int walkspeed = ((CheckHitKey(KEY_INPUT_LSHIFT)||CheckHitKey(KEY_INPUT_RSHIFT))? 4:2);
				#ifndef PRODUCT_MODE
					walkspeed = ((CheckHitKey(KEY_INPUT_LCONTROL)||CheckHitKey(KEY_INPUT_RCONTROL))? MAP_CHIP_SIZE:walkspeed);
				#endif
				
				//PUSHBLOCKを押したときだけWalkからtrueが返る
				if(      CheckHitKey(KEY_INPUT_RIGHT)){	if(Walk(RIGHT, walkspeed)) TextBox->NextPage(&CmdList, &FlagSet);
				}else if(CheckHitKey(KEY_INPUT_LEFT)){	if(Walk(LEFT,  walkspeed)) TextBox->NextPage(&CmdList, &FlagSet);
				}else if(CheckHitKey(KEY_INPUT_DOWN)){	if(Walk(DOWN,  walkspeed)) TextBox->NextPage(&CmdList, &FlagSet);
				}else if(CheckHitKey(KEY_INPUT_UP)){	if(Walk(UP,    walkspeed)) TextBox->NextPage(&CmdList, &FlagSet);
				}
				X = between(0, MAP_SIZE-1, (int)X);
				Y = between(0, MAP_SIZE-1, (int)Y);
				
				if (OldX!=X || OldY!=Y) {		//移動しなかった時は足元チェックしない
					OldX = X; 
					OldY = Y;
					if (CheckEvent(true)){
						TextBox->NextPage(&CmdList, &FlagSet);		//足元にテキストが設定してあれば表示
					}else{
						//戦闘エンカウント判定
						if (Battle->CheckEncount(NowMap, Map.GetMapData(NowMap, X, Y))){
							CmdList.Add("@BattleEncount");
						}
					}
				}
				///////////////////////////////////////////////////////////
				
			}

			////DEBUG：ゲームオーバー＆クリア＆セーブ///////////
				if (CheckHitKey(KEY_INPUT_S) && CheckHitKey(KEY_INPUT_LCONTROL)){
					SaveData(0);
					TextBox->AddStock("0番に上書き保存しました");	TextBox->NextPage(&CmdList, &FlagSet);

				}else if (CheckHitKey(KEY_INPUT_LCONTROL)){
					int tmpdnum=-1;
					if (CheckHitKey(KEY_INPUT_0)){			tmpdnum = 0;
					}else if (CheckHitKey(KEY_INPUT_1)){	tmpdnum = 1;
					}else if (CheckHitKey(KEY_INPUT_2)){	tmpdnum = 2;
					}else if (CheckHitKey(KEY_INPUT_3)){	tmpdnum = 3;						
					}
					if (tmpdnum!=-1){
						while(1){
							//セーブデータ名の自由入力
							char dataname[32];	dataname[0]='\0';
							char inputchar;		char inputmessage[64];
							ClearInputCharBuf() ;
							while(BasicLoop()){
								// 文字入力バッファから文字を取得する
								inputchar = GetInputChar( TRUE ) ;
								// 操作コード以外の文字入力があった場合のみ処理を行う
								if( inputchar == CTRL_CODE_CR && strlen(dataname)>0){		//Enter
									break;
								}else if( inputchar == CTRL_CODE_BS && strlen(dataname)>0){	//BackSpace
									dataname[strlen(dataname)-1]='\0';
								}else{
									switch(inputchar){
									case '.':	case '|':	case '\\':	case '/':	case ':':	case '>':	case '<':	case '?':	case '*':	case '"':	case ' ':	case '%':	//なぜか知らんが%を使用すると応答なしエラーになる	//フォルダに使用できない文字その他の排除
										break;
									default:
										if( inputchar != 0 && inputchar >= CTRL_CODE_CMP && strlen(dataname)<ARRAY_SIZE(dataname)-1){
											dataname[strlen(dataname)+1]='\0';
											dataname[strlen(dataname)]=inputchar;
										}
									}
								}

								sprintf_s(inputmessage, "Input Save Data Name->%s%s", dataname, (strlen(dataname)<ARRAY_SIZE(dataname)-1?"_":""));
								DrawString(0, 0, inputmessage, WHITE);
							}

							//セーブ結果に応じて処理分岐（-1：エラー、0：リトライ、1：成功）
							int saveResult= SaveData(tmpdnum, dataname);
							if (saveResult == 1){
								char tmpmessage[32];			sprintf_s(tmpmessage, "%d番にセーブしました", tmpdnum);
								TextBox->AddStock(tmpmessage);	TextBox->NextPage(&CmdList, &FlagSet);
								break;
							}else if(saveResult == 0){
								char tmpmessage[128];			sprintf_s(tmpmessage, "既に同名のセーブデータが別スロットに存在します。別の名前を入力してしてください。［%s］", dataname);
								TextBox->AddStock(tmpmessage);	//TextBox->NextPage(&CmdList, &FlagSet);
								while(BasicLoop()){
									if( !TextBox->Main(&CmdList, &FlagSet)) {
										break;	//テキストボックスが消されたら再入力画面へ
									}else{
										FieldCmdManager.Main(&CmdList, this, &Map, TextBox, EveManager);
										Draw();
									}
								}
							}else{
								break;
							}
						}
					}

				}else if (CheckHitKey(KEY_INPUT_1)){
					CmdList.Add("@GameOver");
				}else if (CheckHitKey(KEY_INPUT_2)){
					CmdList.Add("@GameClear");
				}else if (CheckHitKeyDown(KEY_INPUT_ESCAPE) || CheckHitKey(KEY_INPUT_3)){
					CmdList.Add("@BackToTitle");
				}else if (CheckHitKeyDown(KEY_INPUT_G)){
					CmdList.Add("@AutoPlay_Set(true,1)");
				} else if (CheckHitKeyDown(KEY_INPUT_I)) {
					CItemManager::GetInstance()->DebugShowAllPlayerItem();
				}else if (CheckHitKeyDown(KEY_INPUT_B)){;
					CmdList.Add("@Battle(bg_01, エネミーA, エネミーB, エネミーC)");
				}
			////DEBUG:ここまで////////////////////////////////////////////
		}

		CHECK_TIME_END("Main_Walk")	

		////デバッグの時にはプレイヤー座標をタイトルバーに表示////////////////////////////////////////
			#ifndef PRODUCT_MODE
				SetTitle("Map_%d Pos_%d:%d Data_%d:%d", NowMap, X, Y, Map.GetMapData(NowMap, X, Y, 0),Map.GetMapData(NowMap, X, Y, 1));
			#endif

		////TextBoxなどによってCmdListに蓄積されたコマンドを処理////////////////////////////////////////
			CHECK_TIME_START	FieldCmdManager.Main(&CmdList, this, &Map, TextBox, EveManager);	CHECK_TIME_END("Command.Main")
			if (Mode != MODE_PLAYING)	return Mode;


		////描画////////////////////////////////////////
			CHECK_TIME_START
			Draw();
			CHECK_TIME_END("Draw")

				
	}
	return MODE_GAMEEND;
}

void CField::Draw(bool _screenflip, bool _textshowingstop, int dx, int dy, bool _playeralsoshake){
	
	if (ImgBackGround!=NULL){	//背景一枚絵モードのとき
		CVector picsize = GetGraphSize(ImgBackGround);
		DrawGraph(WINDOW_WIDTH/2-picsize.x/2, WINDOW_HEIGHT/2-picsize.y/2, ImgBackGround, true);
	}else{

		///神システム作りかけ////////////////////////////////////////////////////////////////
		GodX = 0; GodY = 0;
		dx+=GodX*MAP_CHIP_SIZE; dy+=GodY*MAP_CHIP_SIZE;
		/////////////////////////////////////////////////////////////////////////////


		//マップ描画////////////////////////////////////////////////////////////////////////////
		CHECK_TIME_START2	Map.Draw(NowMap, X, Y, dx, dy);			CHECK_TIME_END2("Map.Draw")
		CHECK_TIME_START2	EveManager->Draw(NowMap, X, Y, false, dx, dy);	CHECK_TIME_END2("EveManager->Draw_under")

		//プレイヤー////////////////////////////////////////////////////////////////////////////
			switch(Effect){
			case NONE:
				break;
			case BLINK:
				Alpha = between(0,255,Alpha+EffectNum[3]);
				if (Alpha<=(EffectNum[0]*255/100)) EffectNum[3] = -EffectNum[3];
				if (Alpha>=(EffectNum[1]*255/100)) EffectNum[3] = -EffectNum[3];
				Alpha = between(EffectNum[0]*255/100, EffectNum[1]*255/100, (int)Alpha);
				break;
			}

			SetDrawBlendMode( DX_BLENDMODE_ALPHA, Alpha);
			CVector playerD(Dx,Dy);
				if(!_playeralsoshake){playerD.Add(-GodX*MAP_CHIP_SIZE,-GodY*MAP_CHIP_SIZE);
				}else{				  playerD.Add(-dx,-dy);}
					if(Visible) DrawGraph(playerD.x+WINDOW_WIDTH/2-MAP_CHIP_SIZE/2, playerD.y+WINDOW_HEIGHT/2-MAP_CHIP_SIZE/2, ImgPlayer[Dir*4+mod(Step,4)], true);	//_a.pngで透過情報を読み込み済み
			SetDrawBlendMode( DX_BLENDMODE_NOBLEND , 0 );
		//////////////////////////////////////////////////////////////////////////////////////////
	
		CHECK_TIME_START2	EveManager->Draw(NowMap, X, Y, true, dx, dy);	CHECK_TIME_END2("EveManager->Draw_over")
	}

	//テキストボックス描画//////////////////////////////////////////////////////////////////
	TextBox->Draw(!CmdList.Empty() || _textshowingstop);
	////////////////////////////////////////////////////////////////////////////////////////

	//フィールドメニューの描画////////////////////////////////
	FieldMenu.Draw();
	//////////////////////////////////////////////////////////

	//ログの描画///////////////////////////////////////////////
	FieldLog.Draw();
	///////////////////////////////////////////////////////////

	//ショップの描画///////////////////////////////////////////////
	CShopManager::GetInstance()->Draw();
	CAlchemistManager::GetInstance()->Draw();
	///////////////////////////////////////////////////////////
	
	if (_screenflip)	{BasicLoop();}
}
	
bool CField::Walk(int _dir, int _walkspeed, bool _eventwalk, bool _walk, int _fade){	
	if(_walk){
		SetMyDir(_dir);
		if(_walkspeed<0) _dir=sys::TurnDir(_dir, 2);
		if (CheckHitKey(KEY_INPUT_A) && !_eventwalk)return false;	//Aを押しながらで方向転換のみ
	}

	if (Map.GetMapData(NowMap, (X+((_dir==RIGHT)? 1: ((_dir==LEFT)? -1: 0)))%MAP_SIZE, (Y+((_dir==DOWN)? 1: ((_dir==UP)? -1: 0)))%MAP_SIZE, 1)) return false;	//障害物の有無を確認
	if (!_eventwalk) if (CheckEvent(false, true)) return true;	//押せるブロック（PUSH_BLOCK）のイベント有無をチェック		//_eventwalkの時は通らないようにした方がいいかも
	if (!EveManager->CheckWalkable(NowMap, (X+((_dir==RIGHT)? 1: ((_dir==LEFT)? -1: 0)))%MAP_SIZE, (Y+((_dir==DOWN)? 1: ((_dir==UP)? -1: 0)))%MAP_SIZE)) return false;	//NPCorBLOCKの有無を確認

	int d=0, oldd=0;	//delta;
	int dx=0, dy=0;
	if (_walkspeed==0) _walkspeed=2;
	if (_walkspeed<0) _walkspeed=-_walkspeed;

	if (_walk) (++Step)%=4;
	int alpha = Alpha;
	if (_fade==1) {Visible = true; Alpha = 0;}

	while(d!=MAP_CHIP_SIZE){
		oldd=d;
		d = between(-MAP_CHIP_SIZE, MAP_CHIP_SIZE, d+_walkspeed);
		if (oldd/(MAP_CHIP_SIZE/2)<1 && d/(MAP_CHIP_SIZE/2)>=1) {
			if(_walk)(++Step)%=4;
		}
		dx = ((_dir==RIGHT)? d: ((_dir==LEFT)? -d: 0));
		dy = ((_dir==DOWN)? d: ((_dir==UP)? -d: 0));
		
		//Draw(true, true, dx, dy);	//140904変更 歩きながらもテキスト表示が進むように。もし不具合が出れば_eventewalkで処理を変える。
		Draw(true, false, dx, dy);

		if (_fade==1)  Alpha = between(0, 255, (int)(alpha*(double)abs(d)/MAP_CHIP_SIZE));
		if (_fade==-1) Alpha = between(0, 255, (int)(alpha*(1-(double)abs(d)/MAP_CHIP_SIZE)));
	}

	if (_fade==-1) {
		Visible = false;
		Alpha = alpha;
	}


	switch(_dir){
	case RIGHT:
		X++;	break;
	case LEFT:
		X--;	break;
	case UP:
		Y--;	break;
	case DOWN:
		Y++;	break;
	}
		
	return false;
}

void CField::Jump(){
	Dy=-5;	
	for(int i=0; i<5; i++){
		Draw(true, true);
	}
	Dy=0;
	Draw(true, true);
}

void CField::SetNowMap(int _mapnum) {

	int oldMap = NowMap;
	MAP_MAX_CHECK(_mapnum,);

	NowMap = _mapnum;

	std::string newMusicKey = Map.GetMapMusic(NowMap);
	std::string oldMusicKey = (oldMap != -1? Map.GetMapMusic(oldMap): "");

	//暫定、音楽未設定のマップに入った時には音楽がなくなる。店や戦闘の出入りを考えるとこれが作りやすいため。
	if (newMusicKey != oldMusicKey || oldMap == -1) {

		if (oldMusicKey.length() > 0) {
			CMusicManager::GetInstance()->StopMusic(oldMusicKey);
		}

		if (newMusicKey.length() > 0) {
			CMusicManager::GetInstance()->PlayMusic(newMusicKey);
		}
	}
}

void CField::SetPosition(int _mapnum, int _x, int _y, bool _d){
	
	SetNowMap(_mapnum);
	
	X = (_d? X+_x:_x) % MAP_SIZE;
	Y = (_d? Y+_y:_y) % MAP_SIZE;
}
void CField::SetMyPic(const int _img[CHARA_PIC_NUM], const char* _pickey){
	for(int i=0; i<CHARA_PIC_NUM; i++){
		ImgPlayer[i] = _img[i];
	}
	strcpy_s(PlayerPicKey, _pickey);
}


void CField::ChangeTextMode(bool _box, const char* _eventtext){
	if (_box){
		TextBox = &TextBox1;
	}else{
		TextBox = &TextWrap1;

		if (_eventtext!=NULL){	//EveManager::CopyOriginalEventを汎用性を上げて改善。これでTextWrap1に@EventWrapの内容を渡せた
			std::vector<char256> tmptext;
			EveManager->CopyOriginalEvent(&tmptext, _eventtext);
			for (unsigned int i=0; i<tmptext.size(); i++){
				TextWrap1.AddStock(tmptext[i].text);
			}
			TextBox->NextPage(&CmdList, &FlagSet);
		}
	}
};

void CField::SetMyEffect(int _effectname, int _effectnum[]){
		
		if (_effectname==-1) {	//TextBox.Termからの呼び出し
			return;
		}

		Effect = (charaeffect_tag)_effectname;

		for (int i=0; i<ARRAY_SIZE(EffectNum); i++){
			EffectNum[i] = 0;
			EffectNumCmd[i] = 0;
			if (_effectnum[i]!=-1 && _effectname!=NONE)	EffectNumCmd[i] = _effectnum[i];
		}

		switch(Effect){
		case NONE:
			SetMyAlpha(255);
			break;
		case BLINK:
			if (EffectNumCmd[0]<0 || 
				EffectNumCmd[0]>100 || EffectNumCmd[1]<0 || EffectNumCmd[1]>100) {
				ErrorDx("Error->SetMyEffect-> 0<=BLINK_num<=100", __FILE__, __LINE__);
				goto reset;
			}else{
				EffectNum[0] = between(0, 100, EffectNumCmd[0]);
				EffectNum[1] = between(0, 100, EffectNumCmd[1]);
				EffectNum[2] = between(1, 10000, EffectNumCmd[2]);
				EffectNum[3] = between(1, 255, ((EffectNumCmd[1]-EffectNumCmd[0])*255*2*10) / (EffectNumCmd[2]*60));
			}
			break;
		case RND_DIR:
		case WALK:
			ERRORDX("CField::SetMyEffect->You can't set [RND_DIR]or[WALK] for Player ....yet?");
			goto reset;
			break;
		default:
			break;
		}

		return;

reset:
		Effect = NONE;
		for (int i=0; i<ARRAY_SIZE(EffectNum); i++){
			EffectNum[i] = 0;
			EffectNumCmd[i] = 0;
		}
		return;
}

void CField::BattleStart(const char* _pic_bg, std::vector<std::string> _enemyList){	
	//イベントバトル用（背景画像と出現敵を指定した戦闘）
	Battle->SetBackGround(_pic_bg);	//増えてきたらまるごとB_CmdListに投げる
	Battle->SetEnemy(_enemyList);
	BattleStart();
}

void CField::BattleStart(){
	int result;
	CCmdList resultcmdlist;

	Battle->SetPlayer();
	Battle->BattleReady(&FlagSet, &Map, EveManager);
	
	//音楽切り替え
		std::string fieldMusic = Map.GetMapMusic(NowMap, false);
		std::string battleMusic = Map.GetMapMusic(NowMap, true);

		if (battleMusic.length() > 0 && battleMusic != fieldMusic) {
			CMusicManager::GetInstance()->StopMusic(fieldMusic);
			CMusicManager::GetInstance()->PlayMusic(battleMusic);
		}

	//画面切り替え効果（戦闘開始）
		int fieldGraph = MakeScreen(WINDOW_WIDTH, WINDOW_HEIGHT);
			SetDrawScreen(fieldGraph);
			Draw(false,true);
		int battleGraph = MakeScreen(WINDOW_WIDTH, WINDOW_HEIGHT);
			SetDrawScreen(battleGraph);
			Battle->Draw(false,true);
		SetDrawScreen(DX_SCREEN_BACK);
		int blankGraph =  MakeScreen(WINDOW_WIDTH,WINDOW_HEIGHT);
		CScreenChanger::ChangeScreen(fieldGraph, blankGraph,  CScreenChanger::SCREEN_FADE, 10);
		CScreenChanger::ChangeScreen(blankGraph, fieldGraph,  CScreenChanger::SCREEN_FADE, 10);
		CScreenChanger::ChangeScreen(fieldGraph, blankGraph,  CScreenChanger::SCREEN_FADE, 10);
		CScreenChanger::ChangeScreen(blankGraph, fieldGraph,  CScreenChanger::SCREEN_FADE, 10);
		
		switch(rand()%5){
		case 0:
			CScreenChanger::ChangeScreen(fieldGraph, blankGraph, CScreenChanger::SCREEN_GURUGURU, 60, true);
			CScreenChanger::ChangeScreen(blankGraph, battleGraph, CScreenChanger::SCREEN_GURUGURU, 60, false);
			break;
		default:
			CScreenChanger::ChangeScreen(fieldGraph, battleGraph, CScreenChanger::SCREEN_BOKASHI, 60);
			break;
		}

		
	//戦闘開始（戦闘終了まで帰ってこない）
	Battle->BattleStart(&result, &resultcmdlist);
	

	//画面切り替え効果（戦闘終了）
		if (result!=LOSE_NOSCREENCHANGE){
			GetDrawScreenGraph(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, battleGraph) ;
			SetDrawScreen(DX_SCREEN_BACK);
			CScreenChanger::ChangeScreen(battleGraph, blankGraph, CScreenChanger::SCREEN_FADE, 30);
			CScreenChanger::ChangeScreen(blankGraph, fieldGraph,  CScreenChanger::SCREEN_FADE, 30);
		}

	//音楽切り替え
		if (fieldMusic.length() > 0 && battleMusic != fieldMusic) {
			CMusicManager::GetInstance()->StopMusic(battleMusic);
			CMusicManager::GetInstance()->PlayMusic(fieldMusic);
		}

	//戦闘結果コマンドの処理
	FieldCmdManager.Main(&resultcmdlist, this, &Map, TextBox, EveManager);
}

void CField::SetBattleResult(const char* _winmessage, const char* _losemessage){
	Battle->BattleSetting(_winmessage, _losemessage);
}


////////private/////////////////////////////////////////////////////////////////////////////////////////////
bool CField::CheckEvent(bool _foot, bool _push){
	char** addtext = NULL;
	bool event_happened=false;
	int count;		//EveManagerからEveObjのCountを受け取るが変更はできない（Count加算はEveManager->GetTextで行われる）

	if (_push){
			if (EveManager->GetText(addtext, count, NowMap, (X+((Dir==RIGHT)?1:((Dir==LEFT)? -1:0)))%MAP_SIZE, (Y+((Dir==DOWN)? 1:((Dir==UP)?-1:0)))%MAP_SIZE, Dir, objkind_tag::PUSHBLOCK)){
				event_happened = true;
			}
	}else if (!_foot){
		for (int k = objkind_tag::WALKABLE_NUM + 1; k < objkind_tag::UNDERDRAW_NUM; k++) {
			if (k == objkind_tag::PUSHBLOCK) continue;
			if (EveManager->GetText(addtext, count, NowMap, (X+((Dir==RIGHT)?1:((Dir==LEFT)? -1:0)))%MAP_SIZE, (Y+((Dir==DOWN)? 1:((Dir==UP)?-1:0)))%MAP_SIZE, Dir, k)){
				event_happened = true;
				break;
			}
		}
	}else{
		for (int k = 0; k < objkind_tag::WALKABLE_NUM; k++){
			if (EveManager->GetText(addtext, count, NowMap, X, Y, Dir, k)){
				event_happened = true;
				break;
			}
		}		
	}


	if (!event_happened) goto finish;	//イベント発生しなければメモリ解放だけして終了
	
	if (! TextBox->AddStock(addtext, Dir, count)) ErrorDx("Error->AddText", __FILE__, __LINE__);
	int i;
	for (i=0; !mystrcmp(addtext[i], EOP) ; i++){
		delete [] addtext[i];
		if (i > 10000) {ErrorDx("CriticalError->NotFound""EOP""", __FILE__, __LINE__); break;}
	}
	delete [] addtext[i];
	delete [] addtext;


finish:
	return event_happened;
}

bool CField::StartSet(const int _dnum){	//PlayDataに格納された読み込みセーブデータを各変数に振り分けて代入
	CCmdList PlayDataCmdList;

	if (_dnum<-1 || _dnum>=PLAYDATA_NUM) {
		ErrorDx("GameStartError->dnum:%d", __FILE__, __LINE__, _dnum);
		return false;
	}

	if (_dnum!=-1 && PlayData_p[_dnum].Exist){
		char bufcmd[256];	
		PlayDataCmdList.Add("@Position_Set(me, %d,%d,%d,%s)", PlayData_p[_dnum].NowMap, PlayData_p[_dnum].X, PlayData_p[_dnum].Y, PlayData_p[_dnum].PlayerPicKey);
		PlayDataCmdList.Add("@Dir_Set(me,%d)", PlayData_p[_dnum].Dir);
		FieldCmdManager.Main(&PlayDataCmdList, this, &Map, TextBox, EveManager);
		
		for (unsigned int i = 0; i < PlayData_p[_dnum].EveObj.size(); i++){
			sprintf_s(bufcmd, "@Position_Set(%s,%d,%d)", PlayData_p[_dnum].EveObj[i].Name, PlayData_p[_dnum].EveObj[i].Dx/MAP_CHIP_SIZE, PlayData_p[_dnum].EveObj[i].Dy/MAP_CHIP_SIZE);	PlayDataCmdList.Add(bufcmd);
			if ( PlayData_p[_dnum].EveObj[i].Kind == objkind_tag::NPC ){
				sprintf_s(bufcmd, "@Dir_Set(%s,%d)", PlayData_p[_dnum].EveObj[i].Name, PlayData_p[_dnum].EveObj[i].Dir);																PlayDataCmdList.Add(bufcmd);
			}
			sprintf_s(bufcmd, "@Count_Set(%s,%d)", PlayData_p[_dnum].EveObj[i].Name, PlayData_p[_dnum].EveObj[i].Count);																PlayDataCmdList.Add(bufcmd);
			sprintf_s(bufcmd, "@Pic_Set(%s,%s)", PlayData_p[_dnum].EveObj[i].Name, PlayData_p[_dnum].EveObj[i].PicKey);																PlayDataCmdList.Add(bufcmd);
			sprintf_s(bufcmd, "@Alpha_Set(%s,%d)", PlayData_p[_dnum].EveObj[i].Name, PlayData_p[_dnum].EveObj[i].Alpha*100/255);														PlayDataCmdList.Add(bufcmd);
			sprintf_s(bufcmd, "@Visible_Set(%s,%d)", PlayData_p[_dnum].EveObj[i].Name, PlayData_p[_dnum].EveObj[i].Visible);															PlayDataCmdList.Add(bufcmd);

			char tmp[256];
			sprintf_s(tmp, "%d",PlayData_p[_dnum].EveObj[i].EffectNumCmd[0]);
			for (int j=1; j<ARRAY_SIZE(PlayData_p[_dnum].EveObj[i].EffectNumCmd); j++){
				sprintf_s(tmp, "%s,%d", tmp, PlayData_p[_dnum].EveObj[i].EffectNumCmd[j]);
			}
			sprintf_s(bufcmd, "@Effect_Set(%s,%d,%s)", PlayData_p[_dnum].EveObj[i].Name, PlayData_p[_dnum].EveObj[i].Effect, tmp);													PlayDataCmdList.Add(bufcmd);

			FieldCmdManager.Main(&PlayDataCmdList, this, &Map, TextBox, EveManager);
		}

		FlagSet = PlayData_p[_dnum].FlagSet;
		
		PlayData_p[_dnum].Exist = false;		//これの必要性よくわかんない14/05/07
	}
	
	if (!CheckHitKeyDown(KEY_INPUT_ESCAPE)){
		Map.CreateMapGraph(NowMap);
		return true;
	}else{
		return false;
	}
}

int CField::SaveData(int _dnum, const char _dataname[32]){	//-1：エラー、0：リトライ、1：成功
	char filename[256];
	FILE *fp;
	
	//セーブデータ番号が適正かチェック
		if (_dnum<0 || _dnum>=PLAYDATA_NUM) {
			ErrorDx("Error->SaveDataNumber too small or big:%d", __FILE__, __LINE__, _dnum);
			return -1;
		}
	
	//セーブフォルダをリネーム又は新規作成（_datanameが空文字列のときはセーブ番号だけを頼りに上書き保存する）
		char newdirname[256];
		if (strlen(_dataname) > 0){
			char olddirname[256];
			sprintf_s(olddirname, "%s/%s", DIR_SAVE, PlayData_p[_dnum].DataName);
			sprintf_s(newdirname, "%s/%s", DIR_SAVE, _dataname); 

			//セーブフォルダをリネーム
			if (rename(olddirname, newdirname) != 0){
				//リネームに失敗したので、既存フォルダがないと判断し新規作成
				if (_mkdir(newdirname) !=0 ){
					//フォルダ作成に失敗＝同名のフォルダが存在していた
					return 0;
				}
			}
			strcpy_s(PlayData_p[_dnum].DataName, _dataname);
		}else{
			sprintf_s(newdirname, "%s/%s", DIR_SAVE, PlayData_p[_dnum].DataName);
			_mkdir(newdirname);
		}
		
	//セーブデータ名一覧の保存（既存のものもすべて上書き）
		fopen_s(&fp, FILE_DATANAME, "w" );
		for (int i = 0; i < PLAYDATA_NUM; i++){
			fputs(PlayData_p[i].DataName, fp);		
			fputs("\n", fp);
		}
		fclose(fp);

		
	///セーブデータ番号に基づいてセーブファイルを開く/////////////////////////////////////////////////////////////////////////////////////
		for (int i=0; i<3; i++){
			switch(i){
			case 0:
				sprintf_s(filename, "%s/%s/pos.dat", DIR_SAVE, PlayData_p[_dnum].DataName);
				break;
			case 1:
				sprintf_s(filename, "%s/%s/flg.dat", DIR_SAVE, PlayData_p[_dnum].DataName);
				break;
			case 2:
				sprintf_s(filename, "%s/%s/eve.dat", DIR_SAVE, PlayData_p[_dnum].DataName);
				break;
			default:
				//UNDONE:ロード機能あとはアイテム、キャラステータス、金、と？ 
				break;
			}

			//ファイルを開く
			fopen_s(&fp, filename, "wb" );

			////ファイルに書き込んで保存/////////////////////////////////////////////////////////////////////////////
			switch(i){
			case 0:
				fwrite(&NowMap, sizeof(NowMap), 1, fp);
				fwrite(&X, sizeof(char), 1, fp);
				fwrite(&Y, sizeof(char), 1, fp);
				fwrite(&Dir, sizeof(char), 1, fp);
				fwrite(&PlayerPicKey, sizeof(char), sizeof(PlayerPicKey), fp);
				break;
			case 1:
				for(unsigned int i=0; i<FlagSet.Flag.size(); i++){
					fwrite(&FlagSet.Flag[i].Key, sizeof(char), sizeof(FlagSet.Flag[i].Key), fp);
					fwrite(&FlagSet.Flag[i].Num, sizeof(FlagSet.Flag[i].Num), 1, fp);
				}
				break;
			case 2:
				EveManager->Save(fp);
				break;
			}
			/////////////////////////////////////////////////////////////////////////////////
			fclose(fp);
		}
	///////////////////////////////////////////////////////////////////////////////////////////
	
	return 1;
}

