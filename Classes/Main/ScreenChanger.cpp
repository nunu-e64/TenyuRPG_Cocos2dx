#include "ScreenChanger.h"



void CScreenChanger::ChangeScreen(const int _pGraph, const int _nGraph, const screenchange_tag _type, int _count, int _option){

	static void (*FuncArray[SCREEN_NUM])(const int, const int, int, int) = {
		&Fade,
		&Bokashi,
		&GuruGuru
	};
	//void (*FuncArray[SCREEN_NUM])(const int, const int, const int) = {&Fade};

		FuncArray[_type](_pGraph, _nGraph, _count, _option);

	/*
	switch(_type){
	case FADE:
		Fade(_pGraph, _nGraph, _count);
		break;
	default:
		ErrorDx("Error->CScreenChanger::ChangeScreen->TypeError");
		return;
	}*/
}


void CScreenChanger::Fade(const int _pGraph, const int _nGraph, int _count, int _option){
	int dGraph = MakeScreen( WINDOW_WIDTH, WINDOW_HEIGHT, FALSE ) ;
	int i = 0;
	
	do{
		GraphBlendBlt( _pGraph, _nGraph, dGraph, (int)(255*i/(double)_count), DX_GRAPH_BLEND_NORMAL);
		DrawGraph(0,0,dGraph,false);
	}while(BasicLoop() && ++i<_count);

}

void CScreenChanger::Bokashi(const int _pGraph, const int _nGraph, int _count, int _option){
	int dGraph = MakeScreen( WINDOW_WIDTH, WINDOW_HEIGHT, FALSE ) ;
	int i = 0;
	
	if (_count%2==1) ++_count;
	double per = 1;

	do{
		//per = i/(double)(_count/2);
		per = sin((PI/2)*i/(double)(_count/2));
		dGraph = MakeScreen( WINDOW_WIDTH-(int)((WINDOW_WIDTH-4)*per), WINDOW_HEIGHT-(int)((WINDOW_HEIGHT-4)*per), FALSE ) ;
		SetDrawScreen(dGraph);
		DrawExtendGraph(0,0,WINDOW_WIDTH-(int)((WINDOW_WIDTH-4)*per),WINDOW_HEIGHT-(int)((WINDOW_HEIGHT-4)*per),_pGraph, false); 
		SetDrawScreen(DX_SCREEN_BACK);
		DrawExtendGraph(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, dGraph,false);
	}while(BasicLoop() && ++i<_count/2);

	i = 0;

	do{
		//per = 1 - i/(double)(_count/2);
		per = sin((PI/2) + (PI/2)*i/(double)(_count/2));
		dGraph = MakeScreen( WINDOW_WIDTH-(int)((WINDOW_WIDTH-4)*per), WINDOW_HEIGHT-(int)((WINDOW_HEIGHT-4)*per), FALSE ) ;
		SetDrawScreen(dGraph);
		DrawExtendGraph(0,0,WINDOW_WIDTH-(int)((WINDOW_WIDTH-4)*per),WINDOW_HEIGHT-(int)((WINDOW_HEIGHT-4)*per), _nGraph, false); 
		SetDrawScreen(DX_SCREEN_BACK);
		DrawExtendGraph(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, dGraph,false);
	}while(BasicLoop() && ++i<_count/2);

}

void CScreenChanger::GuruGuru(const int _pGraph, const int _nGraph, int _count, int _option){
	
	//int column = 8; //WINDOW_WIDTH/80;	
	//int row = 6;	// WINDOW_HEIGHT/80;
	int size = 80;

	CRect box[6*8];

		for (int j=0; j<6*8; j++){
			if (j<8){
				box[j].Left = size * j;
				box[j].Top =  size * 0;
			}else if(j<13){
				box[j].Left = size * 7;
				box[j].Top =  size * (j-7);
			}else if(j<19){
				box[j].Left = size * (19-j);
				box[j].Top =  size * 5;
			}else if(j<23){
				box[j].Left = size * 0;
				box[j].Top =  size * (24-j);
			}else if(j<29){
				box[j].Left = size * (j-23);
				box[j].Top =  size * 1;
			}else if(j<32){
				box[j].Left = size * 6;
				box[j].Top =  size * (j-28);
			}else if(j<37){
				box[j].Left = size * (38-j);
				box[j].Top =  size * 4;
			}else if(j<39){
				box[j].Left = size * 1;
				box[j].Top =  size * (41-j);
			}else if(j<44){
				box[j].Left = size * (j-38);
				box[j].Top =  size * 2;
			}else if(j<48){
				box[j].Left = size * (49-j);
				box[j].Top =  size * 3;
			}

			box[j].Right = box[j].Left + size;
			box[j].Bottom = box[j].Top + size;
		}


	int i=0;
	do{
		DrawGraph(0,0,_pGraph,false);

		if (_option){
			for (int j=0; j<6*8*(double)i/_count; j++){
				DrawRectGraph(box[j].Left, box[j].Top, box[j].Left, box[j].Top, size,size,_nGraph, false, false);
			}
		}else{
			for (int j=6*8-1; j>=6*8*(1-(double)i/_count); j--){
				DrawRectGraph(box[j].Left, box[j].Top, box[j].Left, box[j].Top, size,size,_nGraph, false, false);
			}
		}
	}while(BasicLoop() && ++i<=_count);

}
