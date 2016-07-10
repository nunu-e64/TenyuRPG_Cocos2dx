#include "TalkName.h"


void CTalkName::Init(){
	//左右のテキストネームラベルをすべて削除
		Clear(true);
		Clear(false);

	//テキストネームラベル用画像の読み込み
		ImgLabel[0] = LoadGraph(IMAGE_TALKNAMELABEL[0], true); 
		ImgLabel[1] = LoadGraph(IMAGE_TALKNAMELABEL[1], true);
		ImgLabel[2] = LoadGraph(IMAGE_TALKNAMELABEL[2], true);

}

void CTalkName::Clear(bool _left){
	int side = (_left? 0: 1);
	for(int i=0; i<NAME_NUM; i++){
		Name[side][i][0]='\0';
	}
	NowLeft = !_left;
}

bool CTalkName::Add(bool _left, int _num, ...){

	if (_num>0){
		va_list args;
		va_start(args, _num);	//_numが大きすぎたときの処置方法はないのか？

		int side = (_left? 0: 1);
		NowLeft = _left;

		for (int h=0; h<_num; h++){
			char* tmp = va_arg(args, char*);
			
			for (int i=0; i<NAME_NUM; i++){	
				if (mystrcmp(Name[side][i], tmp)){
					break;
				}else if (Name[side][i][0]=='\0'){
					mystrcpy(Name[side][i], tmp, 32);
					break;
				}else if (i==NAME_NUM-1){
					ErrorDx("Error->CTalkName::Add->Too many names :%s", __FILE__, __LINE__, tmp);
					va_end(args);
					return false;
				}				
			}
		}
		va_end(args);
		return true;

	}else{
		WarningDx("Warning->CTalkName::Add-> arg[_num] should > 0 :%d", _num);
		return false;
	}

}

bool CTalkName::Dec(bool _left, int _num, ...){
	if (_num>0){
		va_list args;
		va_start(args, _num);	//_numが大きすぎたときの処置方法はないのか？

		int side = (_left? 0: 1);

		for (int h=0; h<_num; h++){
			char* tmp = va_arg(args, char*);
			
			for (int i=0; i<NAME_NUM; i++){	
				if (mystrcmp(Name[side][i], tmp)){
					Name[side][i][0]='\0';

					for (int j=i; j<NAME_NUM-1 && Name[side][j+1][0]!='\0'; j++){	
						mystrcpy(Name[side][j], Name[side][j+1]);
						Name[side][j+1][0]='\0';
					}
					break;
				}else if(i==NAME_NUM-1){
					WarningDx("Warning->CTalkName::Dec->Not found name:%s", tmp);
				}
			}
		}
		va_end(args);
		if (Name[side][0][0]=='\0') NowLeft = !_left;

		return true;

	}else{
		WarningDx("Warning->CTalkName::Dec-> arg[_num] should > 0 :%d", _num);
		return false;
	}
}

bool CTalkName::SetNowName(bool _left, char* _name, bool _add){

	int h = (_left? 0: 1);

	if (mystrcmp(Name[h][0], _name)){
		NowLeft = _left;
		return true;
	}
	for (int i=1; i<NAME_NUM && Name[h][i][0]!='\0'; i++){	

		if (mystrcmp(Name[h][i], _name)){
			mystrcpy(Name[h][i], Name[h][0]);
			mystrcpy(Name[h][0], _name);
			NowLeft = _left;
			return true;
		}				

	}

	if(!_add) return false;

	if (Add(_left, 1, _name)){
		return SetNowName(_left, _name, false);
	}else{
		return false;
	}
}

//void SetNowSide(bool _left){NowLeft=_left;};

bool CTalkName::GetVisible(){
	
	for (int h=0; h<SIDE_NUM; h++){
		if (Name[h][0][0]!='\0') {
			return true;
		}
	}
	return false;
}

std::string CTalkName::GetNowName() {
	std::string tmpString = Name[(NowLeft?0:1)][0];
	return tmpString;
}

void CTalkName::Draw(int _left, int _right, int _bottom){	//HACK:座標を読み込んだピクチャではなく、手入力している。Init()と連携させて修正すべき。
	int oldfontsize = GetFontSize();
	
	for (int h=0; h<SIDE_NUM; h++){
		for (int i=NAME_NUM-1; i>=0; i--){
			if (Name[h][i][0]=='\0') continue;
			int alpha = between(0, 255, 255-i*10*255/100);
			SetDrawBlendMode( DX_BLENDMODE_ALPHA, alpha);
			
			int left = (h==0? _left+81*i : _right-(100-81)-81*(i+1));
			if (i==0 && ((h==0&&NowLeft) || (h==1&&!NowLeft))){
				//今話している人の名前
				if (!h){
					DrawGraph(left, _bottom-28, ImgLabel[0], true);
				}else{
					DrawTurnGraph(left-20, _bottom-28, ImgLabel[0], true);
				}
				SetFontSize(14);
				DrawCenterString(left+90/2+1, _bottom-20+1, Name[h][0], BLACK);
				DrawCenterString(left+90/2, _bottom-20, Name[h][0], WHITE);
			}else{
				if (!h){
					DrawGraph(left, _bottom-20, ImgLabel[(i==0?2:1)], true);
				}else{
					DrawTurnGraph(left, _bottom-20, ImgLabel[(i==0?2:1)], true);
				}
				SetFontSize(12);
				DrawCenterString(left+90/2+1, _bottom-15+1, Name[h][i], BLACK);
				DrawCenterString(left+90/2, _bottom-15, Name[h][i], GRAY);
			}
		}
	}
	
	SetFontSize(oldfontsize);
	SetDrawBlendMode( DX_BLENDMODE_NOBLEND , 0 );
}


