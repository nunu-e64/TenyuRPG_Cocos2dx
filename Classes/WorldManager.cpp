#include "WorldManager.h"


//@Anten,@Meiten,@BackGround_Change用のフェードインアウト処理
void CWorldManager::FadeDraw(int _time, int _img, bool _changeahead, bool _color){
    int starttime = GetNowCount() ;
	int alpha;

	if (!_color && !_changeahead) {int tmp = _img; _img = ImgBackGround; ImgBackGround = tmp;}

    while(GetNowCount()-starttime<_time){
		if (_changeahead){
			alpha = 255 * (GetNowCount()-starttime)/_time;
		}else{
			alpha = 255 - 255 * (GetNowCount()-starttime)/_time;
		}
		
		SetDrawBlendMode( DX_BLENDMODE_NOBLEND , 0 );	
		Draw(false, true);		//Draw()内でScreenFlipなどはしない

		SetDrawBlendMode( DX_BLENDMODE_ALPHA, alpha);
		if (_color){
			DrawBox(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, _img, true);	//HACK:DrawBox使わない方が軽いらしい
		}else{
			CVector picsize = GetGraphSize(_img);
			DrawGraph(WINDOW_WIDTH/2-picsize.x/2, WINDOW_HEIGHT/2-picsize.y/2, _img, true);
		}

		if(!BasicLoop() ) break;
	}
	
	if (!_color && _changeahead) ImgBackGround = _img;
	SetDrawBlendMode( DX_BLENDMODE_NOBLEND , 0 );	
}

//void CWorldManager::ChangeTextMode(bool _box, const char* _eventtext){
//	if (_box){
//		TextBox = &TextBox1;
//	}else{
//		TextBox = &TextWrap1;
//
//		if (_eventtext!=NULL){	//EveManager::CopyOriginalEventを汎用性を上げて改善。これでTextWrap1に@EventWrapの内容を渡せた
//			std::vector<char256> tmptext;
//			EveManager_p->CopyOriginalEvent(&tmptext, _eventtext);
//			for (unsigned int i=0; i<tmptext.size(); i++){
//				TextWrap1.AddStock(tmptext[i].text);
//			}
//			TextBox->NextPage(CmdList_p, FlagSet_p);
//		}
//	}
//};
