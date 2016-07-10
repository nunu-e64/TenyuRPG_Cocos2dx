#include "Mrt.h"


using namespace mrtlib;		//usingディレクティブを使うかどうかはお任せします

namespace mrt{		//usingとの位置関係がこれであってるのかは不明

void Opening(){	//返り値：なし 引数：なし
		
	/*/////////////////////////////////////////////////////////
	「はじめから」プレイするときだけ流れるオープニング演出。
	*//////////////////////////////////////////////////////////

	int timecount = 0;
	int scene = 0;
	int fontsize = 14;
	char* optext[] = {"物語の舞台は島国ソラリシア",
		"この国にはかつて太陽の神と星の神を奉る二神教が存在し",
		"信者の中には信仰を捧げた神から魔力を授かるものもいた",
		"昼の太陽から力を授かる“太陽の魔法使い”と",
		"夜の星から力を授かる“星の魔法使い”である",
		"しかし、急速な発達が進む都市部では信仰は薄れ",
		"ソラリシア王によって新たな宗教が興される",
		"弾圧された旧教徒たちは、格差にあえぐ人々と手を組み反乱の狼煙を上げた",
		"それから30年、この国には今なお争乱の火種がくすぶり続けている",
		"そして今、レジスタンスに新たなリーダーが誕生する",
		"これは、長きにわたる紛争に新たな歴史を刻む、若きリーダーの英雄譚である",
		" ",
		"（ここまですべて嘘）"};
	
	SetFontSize(fontsize);
	while( BasicLoop() && !CheckHitKeyDown(KEY_INPUT_ESCAPE) && !CheckHitKeyDown(KEY_INPUT_OK) && scene != -1){
		
		switch(scene){
		case 0:
			if(timecount>=3300){
				scene = -1;
			}
			for (int i=0; i<11; i++){
				DrawCenterString(WINDOW_WIDTH/2, (int)(WINDOW_HEIGHT-timecount/3) + i*(fontsize*4), WHITE, optext[i]);
			}
			break;
		}
		
		//左Ctrlで4倍速
		if (CheckHitKey(KEY_INPUT_LCONTROL)){ timecount+=4; }else{ timecount++;}
	}
}

int Title(){	//返り値：title_tag  引数：なし	
	/*///////////////////////////////////////
	下記を参考にZキーを押したときに適切な値を返してください
	enum title_tag{
		TITLE_LOADSTART,
		TITLE_FIRSTSTART,
		TITLE_SETTING,
		TITLE_GAMEEND,
		TITLE_NUM,
	};
	////////////////////////////////////////*/

	int target = 0;
	int timecount = 0;
	
	while( BasicLoop() && !CheckHitKeyDown(KEY_INPUT_ESCAPE)){
		
		if (CheckHitKeyDown(KEY_INPUT_DOWN)){
			target = mod(target+1, TITLE_NUM);
		}else if (CheckHitKeyDown(KEY_INPUT_UP)){
			target = mod(target-1, TITLE_NUM);
		}else if (CheckHitKeyDown(KEY_INPUT_OK)){
			return target;
		}

		timecount++;

		DrawCenterString(WINDOW_WIDTH/2, (int)(WINDOW_HEIGHT*1/10), "- 天 佑 -" , WHITE );
		//DEBUG:ゲームメニュー･･･デバッグ用に一時逆転
		DrawCenterString(WINDOW_WIDTH/2, (int)(WINDOW_HEIGHT*5/10), "はじめから", WHITE);
		DrawCenterString(WINDOW_WIDTH/2, (int)(WINDOW_HEIGHT*6/10), "つづきから", WHITE);
		DrawCenterString(WINDOW_WIDTH/2, (int)(WINDOW_HEIGHT*7/10), "　設定　　", WHITE);
		DrawCenterString(WINDOW_WIDTH/2, (int)(WINDOW_HEIGHT*8/10), "ゲーム終了", WHITE);
		
		if (int(timecount/15)%2) DrawCenterString(WINDOW_WIDTH/2, (int)(WINDOW_HEIGHT*(target+5)/10), "|>　　　　　　　", WHITE);
	}

	return TITLE_GAMEEND;
}


int ChooseData(playdata_tag* _playdata){	//返り値：ロードするデータ番号（エラー：-1[backtotitle]）  引数：セーブデータ配列
	
	/*///////////////////////////////////////
	Existがtrueのセーブデータのみ表示してください。（playdata_tagの仕様についてはDefine.h参照）
	セーブデータはPLAYDATA_NUM個あります。（_playdata[0]~_playdata[PLAYDATA_NUM-1]）
	////////////////////////////////////////*/

	int dnum = 0;
	char tmpstring[256];

	while( BasicLoop()){
		sprintf_s(tmpstring, "データ番号：%d データ名：%s", dnum, (_playdata[dnum].Exist?_playdata[dnum].DataName:"No Data")); 
		DrawCenterString(WINDOW_WIDTH/2, (int)(WINDOW_HEIGHT*1/4), "ロードするデータを選択", WHITE);
		DrawCenterString(WINDOW_WIDTH/2, (int)(WINDOW_HEIGHT*2/4), tmpstring, (_playdata[dnum].Exist? WHITE:GRAY));
		DrawCenterString(WINDOW_WIDTH/2, (int)(WINDOW_HEIGHT*3/4), "↑↓:データ選択 Z:決定 Esc:タイトルに戻る", WHITE);

		if (CheckHitKeyDown (KEY_INPUT_DOWN)){
			dnum = mod(dnum+1, PLAYDATA_NUM);
		}else if (CheckHitKeyDown (KEY_INPUT_UP)){
			dnum = mod(dnum-1, PLAYDATA_NUM);
		}

		if (CheckHitKeyDown(KEY_INPUT_OK)){
			if (_playdata[dnum].Exist){
				return dnum;
			}

		}else if (CheckHitKeyDown(KEY_INPUT_ESCAPE)){
			return -1;
		}
	}

	WarningDx("Warning->Unexpected Error Happened", __FILE__, __LINE__);
	return -1;
}

bool Setting(){	//返り値…設定変更(true) 設定終了(false)  引数…いまのところなし
	
	/*///////////////////////////////////////
		詳細未定。仕様はこれから詰める。
		音量上げ下げくらいならこの関数内で処理してもらってもいいかも…？
	////////////////////////////////////////*/

	int fontsize = 10;

	SetFontSize(fontsize);
	while( BasicLoop() && !CheckHitKeyDown(KEY_INPUT_ESCAPE)){
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);	
		DrawCenterString(WINDOW_WIDTH/2, (int)(WINDOW_HEIGHT*2/5), "各種設定（音量調整、テキストスピード調整、デザイン変更(できれば)、あと何？）をできるようにする。Escで終了。", WHITE);
	}

	return false;

}


}	//namespace閉じる