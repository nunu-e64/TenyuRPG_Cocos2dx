#include "EveManager.h"
#include "Field.h"

void CEveManager::SetMap(unsigned int _mapnum, int _filesize, unsigned char* buf){
	for (int i = 0; i< _filesize; i++){
		EveMap[_mapnum][i%MAP_SIZE][(int)(i/MAP_SIZE)] = buf[i];
	}
}

void CEveManager::Save(FILE *fp){
	for (int i=0; i < MAP_MAX; i++){
		for (unsigned int j=0; j<EveObj[i].size(); j++){
			fwrite(&EveObj[i][j].Name, sizeof(char), sizeof(EveObj[i][j].Name), fp);
			fwrite(&EveObj[i][j].Count, sizeof(int), 1, fp);
			fwrite(&EveObj[i][j].Dx, sizeof(int), 1, fp);
			fwrite(&EveObj[i][j].Dy, sizeof(int), 1, fp);
			fwrite(&EveObj[i][j].Kind, sizeof(char), 1, fp);
			fwrite(&EveObj[i][j].Dir, sizeof(char), 1, fp);
			fwrite(&EveObj[i][j].PicKey, sizeof(char),sizeof( EveObj[i][j].PicKey), fp);
			fwrite(&EveObj[i][j].Alpha, sizeof(unsigned char), 1, fp);
			fwrite(&EveObj[i][j].Visible, sizeof(char), 1, fp);
			
			fwrite(&EveObj[i][j].Effect, sizeof(char), 1, fp);
			fwrite(&EveObj[i][j].EffectNumCmd, sizeof(int), ARRAY_SIZE(EveObj[i][j].EffectNumCmd), fp);
		}
	}
}


void CEveManager::Draw(int _mapnum, int _x, int _y, bool _overdraw, int _dx, int _dy){
	MAP_MAX_CHECK(_mapnum,);		//マップの配列宣言最大数を超えてる場合は読み込めない
	
	int drawx = _x-(WINDOW_WIDTH/MAP_CHIP_SIZE)/2;		//画面左上のマスの世界座標
	int drawy = _y-(WINDOW_HEIGHT/MAP_CHIP_SIZE)/2;
	CEveObj *eveobj_p;
	eveobj_p = &EveObj_dammy;

	for (int ii = -1; ii < WINDOW_WIDTH/MAP_CHIP_SIZE + 2; ii++){
		int i = ii + (int)(_dx/MAP_CHIP_SIZE);
		for (int jj = -1; jj < WINDOW_HEIGHT/MAP_CHIP_SIZE + 1 ; jj++){
			int j = jj + (int)(_dy/MAP_CHIP_SIZE);
			for (int k = (_overdraw ? (objkind_tag::UNDERDRAW_NUM + 1) : 0); k < (_overdraw ? objkind_tag::NUM : objkind_tag::UNDERDRAW_NUM); k++) {
				
				if (GetEveObj(&eveobj_p, _mapnum, mod(drawx+i,MAP_SIZE), mod(drawy+j,MAP_SIZE), k, true)){
					eveobj_p->Draw(-_dx+i*MAP_CHIP_SIZE-MAP_CHIP_SIZE/2, -_dy+j*MAP_CHIP_SIZE);
				}
			}
		}
	}
};

void CEveManager::SetEveObj(int _mapnum, int _datanum, int _kind, int _img[CHARA_PIC_NUM], const char* _pickey, char* _name, bool _visible){
	MAP_MAX_CHECK(_mapnum, );
	if (!(_datanum<MAP_DATA_SIZE)){
		ErrorDx("Call Overnum[MAP_DATA_SIZE]:%d", __FILE__, __LINE__, _datanum);
		return;
	}
	CEveObj eveobj;

	eveobj.Mapnum = _mapnum;
	eveobj.Datanum = _datanum;
	eveobj.Visible = _visible;
	eveobj.Kind = (objkind_tag::type)_kind;	
	
	//Img
		if (_kind == objkind_tag::NPC) {
			for(int i=0; i<CHARA_PIC_NUM; i++){
				eveobj.Img[i] = _img[i];
			}
		}else{
			eveobj.Img[0] = _img[0];
		}
		strcpy_s(eveobj.PicKey, _pickey);

	//Name	同Kindで同名は警告

		if(strlen(_name)) {
			if (strlen(_name)>sizeof(eveobj.Name)-1){
				ErrorDx("Error->tooLongName->%s", __FILE__, __LINE__, _name);
				return;
			}
			for (int i=0; i < MAP_MAX; i++){
				for (unsigned int j=0; j<EveObj[i].size(); j++){
					if (mystrcmp(_name, EveObj[i][j].Name)){
						WarningDx("Warning->AlreadyResiteredName->%s", __FILE__, __LINE__, _name);
					}	
				}
			}
			if( sys::PlayerName(_name) ){
				ErrorDx("Error->You can't use \"me\",\"ME\",\"Me\",\"PLAYER\",\"player\",\"Player\" for object name:%s", _name);
			}
			strcpy_s(eveobj.Name, _name);
		
		}else{
			WarningDx("Warning->NoName->datanum:%d", __FILE__, __LINE__, _datanum);	
		
		}
	
	EveObj[_mapnum].push_back(eveobj);
}
void CEveManager::SetPosition(const char* _name, int _dx, int _dy, bool _d){
	CEveObj *eveobj_p = &EveObj_dammy;
	
	if (GetEveObj(&eveobj_p, _name)){	
		eveobj_p->Dx = (_d? eveobj_p->Dx:0)+_dx*MAP_CHIP_SIZE;
		eveobj_p->Dy = (_d? eveobj_p->Dy:0)+_dy*MAP_CHIP_SIZE;
		return;
	}
	ErrorDx("Error->NotFound-SetPosition_name->%s", __FILE__, __LINE__, _name);
}
void CEveManager::SetPosition(const char* _name, const char* _targetname){
	CEveObj *eveobj_p = &EveObj_dammy;

	if (GetEveObj(&eveobj_p, _name)){
		int mapnum0, mapnum1, x0, x1, y0, y1;
		if (GetPos(_name, &mapnum0, &x0, &y0) && GetPos(_targetname, &mapnum1, &x1, &y1)){
			if (mapnum0 != mapnum1){
				ErrorDx("Error->You can't choose EveObj in different map->%s", __FILE__, __LINE__, _targetname);
			}else{
				eveobj_p->Dx += (x1 - x0) * MAP_CHIP_SIZE;
				eveobj_p->Dy += (y1 - y0) * MAP_CHIP_SIZE;
			}
		}
		return;
	}
	ErrorDx("Error->NotFound-SetPosition_name->%s", __FILE__, __LINE__, _name);
}
void CEveManager::SetPosition(const char* _name, int _datanum){
	CEveObj *eveobj_p = &EveObj_dammy;

	if (GetEveObj(&eveobj_p, _name)){
		int mapnum0, x0, x1, y0, y1;
		if (GetPos(_name, &mapnum0, &x0, &y0) && GetPos(eveobj_p->Mapnum, _datanum, &x1, &y1)){
			eveobj_p->Dx += (x1 - x0) * MAP_CHIP_SIZE;
			eveobj_p->Dy += (y1 - y0) * MAP_CHIP_SIZE;
		}
		return;
	}
	ErrorDx("Error->NotFound-SetPosition_name->%s", __FILE__, __LINE__, _name);
}

void CEveManager::SetPic(const char* _name, const int _img[CHARA_PIC_NUM], const char* _pickey){	
	CEveObj *eveobj_p = &EveObj_dammy;
	
	if (GetEveObj(&eveobj_p, _name)){
		if (eveobj_p->Kind == objkind_tag::NPC) {
			for(int i=0; i<CHARA_PIC_NUM; i++){
				eveobj_p->Img[i] = _img[i];
			}
		}else{
			eveobj_p->Img[0] = _img[0];
		}
		strcpy_s(eveobj_p->PicKey, _pickey);
		return;
	}	
	ErrorDx("Error->NotFound-SetPic_name->%s", __FILE__, __LINE__, _name);
}
bool CEveManager::GetPos(const char* _name, int* _mapnum, int* _x, int* _y){
	CEveObj *eveobj_p = &EveObj_dammy;
	
	if (GetEveObj(&eveobj_p, _name)){
		int mapnum = eveobj_p->Mapnum;
		int datanum = eveobj_p->Datanum;
		for (int i = 0; i<MAP_SIZE; i++){
			for (int j = 0; j< MAP_SIZE; j++){
				if (datanum == EveMap[mapnum][i][j]){
					*_x = i + (int)(eveobj_p->Dx/MAP_CHIP_SIZE);
					*_y = j + (int)(eveobj_p->Dy/MAP_CHIP_SIZE);
					*_mapnum = mapnum;
					return true;
				}
			}
		}
		ErrorDx("Error->NotFound CEveManager::GetPos datanum name:%s", __FILE__, __LINE__, _name);
		return false;
	}
		
	ErrorDx("Error->NotFound CEveManager::GetPos name->%s", __FILE__, __LINE__, _name);
	return false;
}
bool CEveManager::GetPos(const int _mapnum, const int _datanum, int* _x, int*  _y){
	
		for (int i = 0; i<MAP_SIZE; i++){
			for (int j = 0; j< MAP_SIZE; j++){
				if (_datanum == EveMap[_mapnum][i][j]){
					*_x = i;
					*_y = j;
					return true;
				}
			}
		}
		ErrorDx("Error->NotFound CEveManager::GetPos datanum:%d", __FILE__, __LINE__, _datanum);
		return false;
}
	

bool CEveManager::GetText(char** &_text, int &_count, int _mapnum, int _x, int _y, int _mydir, int _kind){
	CEveObj *eveobj_p = &EveObj_dammy;

	if (GetEveObj(&eveobj_p, _mapnum, _x, _y, _kind) && eveobj_p->Visible && !eveobj_p->Text.empty()){
		unsigned int i;
		_text = new char*[eveobj_p->Text.size()+1];

		for (i=0; i < eveobj_p->Text.size(); i++){
			_text[i] = new char[strlen(eveobj_p->Text[i].text)+1];
			mystrcpy(_text[i], eveobj_p->Text[i].text);
		}

		_text[i] = new char[strlen(EOP)+1];
		mystrcpy(_text[i], EOP);

		_count = ++(eveobj_p->Count);		//テキストを持ちViibleがTrueのものは、PANELでも実際に表示するテキストが存在しなくてもカウントアップ
		if (eveobj_p->Kind == objkind_tag::NPC) eveobj_p->Dir = sys::TurnDir(_mydir, +2);	//NPCなら話しかけたときの向きに応じて向きを変える
		strcpy_s(NowName, eveobj_p->Name);	//調べたオブジェクトの名前はEveManagerに保存しておく。コマンドで対象とする名前を入力する際に使う。

		TmpEffect = eveobj_p->Effect;		//RND_DIRなどのEffectを話しかけたときに一時的に無効化するためにEveManagerに一時保存しておく
		if (TmpEffect>KEEP_NUM){
			eveobj_p->Effect = NONE;
		}

		return true;

	}else{
		return false;
	}
}
void CEveManager::SetText(const char _eventtext[1000][256], const int _line, const char* _name, const int _kind){
	char256 settext;

	if (_kind==-1){
		eventset_tag neworiginalevent;
			strcpy_s(neworiginalevent.Name, _name);
			neworiginalevent.Text.clear();
			
			for (unsigned int i=0; i<OriginalEvent.size(); i++){
				if (mystrcmp(OriginalEvent[i].Name, _name))	{
					ErrorDx("Error->AlreadyResiteredEventName->%s", __FILE__, __LINE__, _name);
					return;
				}
			}
			for (int i=0; i<_line; i++){
				strcpy_s(settext.text, _eventtext[i]);
				neworiginalevent.Text.push_back(settext);
			}

			OriginalEvent.push_back(neworiginalevent);
			return;

	}else{
		CEveObj *eveobj_p = &EveObj_dammy;
		if (GetEveObj(&eveobj_p, _name, _kind)){
			eveobj_p->Text.clear();

			for (int i=0; i<_line; i++){
				if (mystrcmp(_eventtext[i], "@Event(", 'l')){		//@Eventコマンドの時だけ事前登録したマクロを呼び出す処理
					CopyOriginalEvent(&(eveobj_p->Text), _eventtext[i]);

				}else{		//普通はこっち
					strcpy_s(settext.text, _eventtext[i]);
					eveobj_p->Text.push_back(settext);
				}
			}

			return;
		}

		ErrorDx("Error->NotFound-SetText_name->%s", __FILE__, __LINE__, _name);
	}
}

bool CEveManager::CheckWalkable(int _mapnum, int _x, int _y){
	CEveObj *eveobj_p = &EveObj_dammy;

	for (int k = objkind_tag::WALKABLE_NUM + 1; k < objkind_tag::UNDERDRAW_NUM; k++) {
		if (GetEveObj(&eveobj_p, _mapnum, _x, _y, k)&&(eveobj_p->Visible)){
			return false;
		}
	}
	return true;
};

void CEveManager::SetDir(const char* _name, int _dir){
	CEveObj *eveobj_p = &EveObj_dammy;
	
	if (GetEveObj(&eveobj_p, _name, objkind_tag::NPC)) {
		eveobj_p->Dir = (direction_tag)_dir;
		return;
	}
	ErrorDx("Error->NotFound-SetDir_name(Youcan use @Dir_Set for NPC only)->%s", __FILE__, __LINE__, _name);
}
void CEveManager::SetVisible(const char* _name, bool _visible){
	CEveObj *eveobj_p = &EveObj_dammy;
	
	if (GetEveObj(&eveobj_p, _name)){
		eveobj_p->Visible = _visible;
		if (_visible) eveobj_p->Alpha = max(15, eveobj_p->Alpha);  //見えないのにVisibleがTrueだと事故りそうなのでAlphaを最小15に
		return;
	}
	ErrorDx("Error->NotFound-SetVisible_name->%s", __FILE__, __LINE__, _name);
}
void CEveManager::SetAlpha(const char* _name, unsigned char _alpha){
	CEveObj *eveobj_p = &EveObj_dammy;
	
	if (GetEveObj(&eveobj_p, _name)){
		eveobj_p->Alpha = _alpha;
		return;
	}
	ErrorDx("Error->NotFound-SetAlpha_name->%s", __FILE__, __LINE__, _name);
}
void CEveManager::SetEffect(const char* _name, int _effectname, int _effectnum[]){
	CEveObj *eveobj_p = &EveObj_dammy;
		
	if (GetEveObj(&eveobj_p, _name)){
		if (_effectname==-1) {	//TextBox.Termからの呼び出し
			eveobj_p->Effect = TmpEffect;
			return;
		}

		eveobj_p->Effect = (charaeffect_tag)_effectname;
		if (mystrcmp(_name, JOKER_NAME)) TmpEffect = (charaeffect_tag)_effectname;	//一時保存の実質破棄

		for (int i=0; i<ARRAY_SIZE(eveobj_p->EffectNum); i++){
			eveobj_p->EffectNum[i] = 0;
			eveobj_p->EffectNumCmd[i] = 0;
			if (_effectnum[i]!=-1 && _effectname!=NONE)	eveobj_p->EffectNumCmd[i] = _effectnum[i];
		}

		switch(eveobj_p->Effect){
		case NONE:
			SetAlpha(_name, 255);
			break;
		case BLINK:
			if (eveobj_p->EffectNumCmd[0]<0 || eveobj_p->EffectNumCmd[0]>100 || eveobj_p->EffectNumCmd[1]<0 || eveobj_p->EffectNumCmd[1]>100) {
				ErrorDx("Error->SetEffect-> 0<=BLINK_num<=100", __FILE__, __LINE__);
				goto reset;
			}else{
				eveobj_p->EffectNum[0] = between(0, 100, eveobj_p->EffectNumCmd[0]);
				eveobj_p->EffectNum[1] = between(0, 100, eveobj_p->EffectNumCmd[1]);
				eveobj_p->EffectNum[2] = between(1, 10000, eveobj_p->EffectNumCmd[2]);
				eveobj_p->EffectNum[3] = between(1, 255, ((eveobj_p->EffectNumCmd[1]-eveobj_p->EffectNumCmd[0])*255*2*10) / (eveobj_p->EffectNumCmd[2]*60));
			}
			break;
		case RND_DIR:
			eveobj_p->EffectNum[0] = eveobj_p->EffectNumCmd[0]*60/1000;
			eveobj_p->EffectNum[1] = 0;
			break;
		case WALK:
			break;
		default:
			break;
		}

		return;

reset:
		eveobj_p->Effect = NONE;
		for (int i=0; i<ARRAY_SIZE(eveobj_p->EffectNum); i++){
			eveobj_p->EffectNum[i] = 0;
			eveobj_p->EffectNumCmd[i] = 0;
		}
		return;
	}
	ErrorDx("Error->NotFound-SetEffect_name->%s", __FILE__, __LINE__, _name);
}

void CEveManager::SetCount(const char* _name, int _count, bool _add){
	CEveObj *eveobj_p = &EveObj_dammy;
	
	if (GetEveObj(&eveobj_p, _name)){
		if(!_add){
			eveobj_p->Count = max(0,_count);
		}else{
			eveobj_p->Count = max(0,eveobj_p->Count+_count);
		}
		return;
	}
	ErrorDx("Error->NotFound-SetCount_name->%s", __FILE__, __LINE__, _name);
}
void CEveManager::Jump(CField* _field, char* _name){
	CEveObj *eveobj_p = &EveObj_dammy;
	
	if (GetEveObj(&eveobj_p, _name, objkind_tag::NPC)) {
		eveobj_p->Dy -= 5;
		for(int i=0; i<5; i++){
			_field->Draw(true, true);			
		}
		eveobj_p->Dy += 5;
		_field->Draw(true, true);
	
	}else{
		ErrorDx("Error->NotFound->@Jump arg[name]->%s", __FILE__, __LINE__, _name);
	}
}
void CEveManager::Walk(CField* _field, char* _name, int _dir, int _walkspeed, bool _walk, int _fade){
	CEveObj *eveobj_p = &EveObj_dammy;
	
	if (GetEveObj(&eveobj_p, _name, (_walk?objkind_tag::NPC:-1), _field->GetNowMap())){
		if (_walk) SetDir(_name, _dir);

		//障害物の有無チェックなしで歩き続ける仕様→スライドの時には障害物の有無チェックする仕様（Command.cppで）
		

		int d=0, oldd=0;	//delta;
		int oldDx=eveobj_p->Dx, oldDy=eveobj_p->Dy;
	
		if (_walk) (++eveobj_p->Step)%=4;
	
		int alpha = eveobj_p->Alpha;
		if (_fade==1) {eveobj_p->Visible = true; eveobj_p->Alpha = 0;}
		
		while(abs(d)!=MAP_CHIP_SIZE){
			oldd=d;
			d = between(-MAP_CHIP_SIZE, MAP_CHIP_SIZE, d+_walkspeed);
			if (abs(oldd)/(MAP_CHIP_SIZE/2)<1 && abs(d)/(MAP_CHIP_SIZE/2)>=1) {
				if (_walk) (++eveobj_p->Step)%=4;
			}
			eveobj_p->Dx = oldDx+((_dir==RIGHT)? d: ((_dir==LEFT)? -d: 0));
			eveobj_p->Dy = oldDy+((_dir==DOWN)? d: ((_dir==UP)? -d: 0));
			
			//_field->Draw(true, true);
			_field->Draw(true, false);

			if (_fade==1) eveobj_p->Alpha = between(0, 255, (int)(alpha*(double)abs(d)/MAP_CHIP_SIZE));
			if (_fade==-1) eveobj_p->Alpha = between(0, 255, (int)(alpha*(1-(double)abs(d)/MAP_CHIP_SIZE)));
		};

		if (_fade==-1) {
			eveobj_p->Visible = false;
			eveobj_p->Alpha = alpha;
		}

	}else{
		ErrorDx("Error->NotFound->@Walk arg[name]->%s", __FILE__, __LINE__, _name);
	}
}


////////private/////////////////////////////////////////////////////////////////////////////////////////////
bool CEveManager::GetEveObj(CEveObj** _eveobj_p, int _mapnum, int _x, int _y, int _kind, bool _forcheck){
	
	MAP_MAX_CHECK(_mapnum,false)
	if (_x<0 || _x>=MAP_SIZE || _y<0 || _y>=MAP_SIZE){
		return false;
	}

	int x=_x, y=_y;
	for (unsigned int i=0; i<EveObj[_mapnum].size(); i++){
		if (!(_kind==-1 || (int)EveObj[_mapnum][i].Kind==_kind)) continue;	//軽量化のため条件文を分割
		if (_forcheck){
			x = _x - EveObj[_mapnum][i].Dx/MAP_CHIP_SIZE;
			y = _y - EveObj[_mapnum][i].Dy/MAP_CHIP_SIZE;
			if (x<0 || x>=MAP_SIZE || y<0 || y>=MAP_SIZE){
				continue;
			}
		}
		if (EveMap[_mapnum][x][y]==0) continue;

		if (EveObj[_mapnum][i].Datanum == EveMap[_mapnum][x][y]){
			*_eveobj_p = &EveObj[_mapnum][i];
			return true;
		}
	}

	return false;
}

bool CEveManager::GetEveObj(CEveObj** _eveobj_p, const char* _name, const int _kind, int _mapnum){
	char name[32];
	if (_mapnum!=-1) MAP_MAX_CHECK(_mapnum,false);	

	strcpy_s(name, ((mystrcmp(_name, JOKER_NAME))? NowName: _name));
	
	for (int i=0; i < MAP_MAX; i++){
		if (i!=_mapnum && _mapnum!=-1) continue;
		for (unsigned int j=0; j<EveObj[i].size(); j++){
			if ((_kind==-1 || (int)EveObj[i][j].Kind==_kind) && mystrcmp(name, EveObj[i][j].Name)){
				*_eveobj_p = &EveObj[i][j];
				return true;
			}
		}
	}
	return false;
}

bool CEveManager::CopyOriginalEvent(std::vector<char256> *vectext_p, const char* _eventtext, int _count){
	char *cntx, *cntx2;		//strtok_s用の雑用
	char* string_copy = new char[strlen(_eventtext)+1];
	char *eventname, *chtimes, *args;
	std::vector <char256> arg;
	long int times = 1;
	eventset_tag *originalevent_p = &OriginalEvent_dammy;

	mystrcpy(string_copy, _eventtext);
	
	args = strchr(string_copy, '[');

	if (args!=NULL){	
		*args = '\0';
		args++;
		mystrsmt(args, " )]");
		char256 newarg;
		char* p = strtok_s(args, CMD_SEPARATOR, &cntx2);
		while(p!=NULL){
			strcpy_s(newarg.text, p);
			arg.push_back(newarg);
			p = strtok_s(NULL, CMD_SEPARATOR, &cntx2);
		}
	}
	
	strtok_s(string_copy, "( ", &cntx );
	eventname = strtok_s(NULL, CMD_SEPARATOR, &cntx );
	chtimes = strtok_s(NULL, CMD_SEPARATOR, &cntx );


	if (eventname==NULL) {
		ErrorDx("Error->@Event->NULL arg[name]:%s", _eventtext);
		delete[] string_copy;
		return false;
	}else{
		mystrsmt(eventname, " )");
	}
	if (chtimes!=NULL){
		mystrsmt(chtimes, " )");
	
		if (!mystrtol(chtimes, &times)){
			times = 1;
			ErrorDx("Error->@Event->Check arg[times]:%s", _eventtext);
		}
	}

	if (GetOriginalEvent(&originalevent_p, eventname)){
		for (int i = 0; i < times; i++){
			for (unsigned int j=0; j < originalevent_p->Text.size(); j++){

				if (mystrcmp(originalevent_p->Text[j].text, "@Event(", 'l')){		//@Eventコマンドの時だけ事前登録したマクロを呼び出す処理
					
					if (_count>100){
						ErrorDx("無限ループってこわくね？   :%s", __FILE__, __LINE__, originalevent_p->Text[j].text);
						continue;
					}else{
						CopyOriginalEvent(vectext_p, originalevent_p->Text[j].text, ++_count);
					}

				}else{
					vectext_p->push_back(originalevent_p->Text[j]);
					for (unsigned int k=0; k<arg.size(); k++){
						char str[8];
						sprintf_s(str, "[arg%d%c", k+1, ']');
						mystrrep(vectext_p->back().text, str, arg[k].text);
					}
				}		

			}
		}
	}else{
		ErrorDx("Error->Not Found eventname:%s", _eventtext);
		delete[] string_copy;
		return false;
	}

	delete[] string_copy;
	return true;
}

bool CEveManager::GetOriginalEvent(eventset_tag** _originalevent_p, const char* _name){

	for (unsigned int i=0; i<OriginalEvent.size(); i++){
		if (mystrcmp(OriginalEvent[i].Name, _name)){
			*_originalevent_p = &OriginalEvent[i];
			return true;
		}
	}
	return false;
}
