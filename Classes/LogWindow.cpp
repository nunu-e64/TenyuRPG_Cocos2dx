#include "LogWindow.h"
#include "BImgBank.h"

CLogWindow::CLogWindow(){
	CONSTRUCTED;

	Visible = false;
	Initialized = false;

	LineSpace = 5;
	BoxSpace = 5;

	//
	//StockLine = -1;

	//NowStock = 0;
	//NowTarget = 0;
	//PageChange = false;

	//NewText = -1;
	//Showing = false;
	//ShowingTime = -1;

	//OriginalDir = DOWN;
}

void CLogWindow::Init(int _posx, int _posy, int _width, int _height, int _boxColor, int _stockLine, int _fontSize, int _fontColorMain, int _fontColorSub) {

	if (Initialized) {
		ERRORDX("Already Initialized (do nothing)");
		return;
	}


	PosX = _posx;
	PosY = _posy;
	Width = _width;
	Height = _height;
	BoxColor = _boxColor;

	FontSize = _fontSize;
	FontHandle = CreateFontToHandle(NULL, FontSize, -1);
	FontColorMain = _fontColorMain;
	FontColorSub = _fontColorSub;

	NextLine = 0;
	BackLine = 0;

	//Widthから文字数計算->WordNum
	WordWidth = Width - BoxSpace;	//なるべく箱ギリギリまで文字が入るように右余白は考慮しない
	char tmp[WORD_MAX] = "";
	WordNum = WORD_MAX;
	for (int i = 0; i<WORD_MAX - 1; i++) {
		tmp[i] = 'a';
		tmp[i + 1] = '\0';
		if (GetDrawStringWidthToHandle(tmp, strlen(tmp), FontHandle) > WordWidth) {
			WordNum = i + 1;
			break;
		}
	}

	//HeightとFontSizeから行数計算->LineNum
	StockLineNum = max(1, _stockLine);
	LineNum = min((Height - BoxSpace * 2) / (FontSize + LineSpace), StockLineNum);

	myLog("new LogWindow.Text[%d][%d]...", StockLineNum, WordNum);
	Text = new char*[StockLineNum];
	for (int i = 0; i<StockLineNum; i++) {
		Text[i] = new char[WordNum];
		Text[i][0] = '\0';
	}


	//NowStock = 0;
	//NowTarget = 0;

	//NewText = -1;
	//Showing = false;

	Initialized = true;

}

void CBattleLog::Init(int _smallposx, int _posy, int _smallwidth, int _height, int _boxColor, int _stockLine, int _fontSize, int _fontColorMain, int _fontColorSub, CBImgBank* _bImgBank){

	if (Initialized) {
		ERRORDX("Already Initialized (do nothing)");
		return;
	}
	

	PosX = _smallposx;
	PosY = _posy;
	Width = _smallwidth;
	Height = _height;
	BoxColor = _boxColor;

	FullMode = false;
	PosXFull = 400;
	WidthFull = WINDOW_WIDTH - PosXFull;

	FontSize = _fontSize;
	FontHandle = CreateFontToHandle(NULL, FontSize, -1);
	FontColorMain = _fontColorMain;
	FontColorSub = _fontColorSub;
	_bImgBank->GetImg(LOG_WINDOW_BUTTON, ButtonImg, ARRAY_SIZE(ButtonImg));

	NextLine = 0;
	BackLine = 0;
	
	//Widthから文字数計算->WordNum
		WordWidth = WidthFull - BoxSpace;	//なるべく箱ギリギリまで文字が入るように右余白は考慮しない
		char tmp[WORD_MAX] = "";
		WordNum = WORD_MAX;
		for (int i=0; i<WORD_MAX-1; i++){
			tmp[i]	 = 'a';
			tmp[i+1] = '\0';
			if (GetDrawStringWidthToHandle(tmp, strlen(tmp), FontHandle) > WordWidth){
				WordNum = i+1;
				break;
			}
		}

	//HeightとFontSizeから行数計算->LineNum
		StockLineNum = max(1, _stockLine);
		LineNum = min((Height-BoxSpace*2)/(FontSize+LineSpace), StockLineNum);

	myLog("new LogWindow.Text[%d][%d]...", StockLineNum, WordNum);
	Text = new char* [StockLineNum];
	for (int i=0; i<StockLineNum; i++){
		Text[i] = new char[WordNum];	
		Text[i][0] = '\0';
	}


	//NowStock = 0;
	//NowTarget = 0;

	//NewText = -1;
	//Showing = false;

	Visible = true;
	Initialized = true;

}

void CLogWindow::Clear(){

	for (int i=0; i<StockLineNum; i++){
		Text[i][0] = '\0';
	}
	NextLine = 0;
}

void CBattleLog::Clear() {
	FullMode = false;
	CLogWindow::Clear();
}

void CLogWindow::Term(){
	if (Initialized){
		myLog("deleting LogWindow.Text[%d][%d]...", StockLineNum, WordNum);
		for (int i=0; i<StockLineNum; i++){
			delete [] Text[i];
		}
		delete [] Text;
	
		Initialized = false;
	}
	/*	
	NowStock = 0;
	NowTarget = 0;

	StockLine = -1;
	StockClear();
		
	Showing = false;
	TextClear();
	Alive = false;
	Visible = false;
	AutoPlay = false;
	AutoPlaySpeed = DefaultAutoPlaySpeed;
*/
}

bool CLogWindow::Add(const char *_format, ...){		//コメント行や空白行はLoadの段階で排除されている
	va_list args;
	char newText[WORD_MAX];
	va_start(args, _format);
	vsprintf_s(newText, _format, args);
	va_end(args);
	
	if (!Initialized) {
		ERRORDX("LogWindow hasn't Initialized yet.(I'll do nothing.)");
		return false;
	}

	int spaceCount = 0;
	for (int i = 0; i < mystrlen(newText); i++) {
		if (newText[i] == ' ') ++spaceCount;
		else break;
	}

	////文字描画時のサイズと文字数の両方で改行の要不要判断
	if (!(mystrlen(newText) < WordNum)) {			//字数オーバー時処理（実際は描画サイズで処理するように上記に書き足したためこちらを使うことはないと思われる）
		char chOverstring[WORD_MAX];
		char chTruestring[WORD_MAX];

		int d = (_ismbblead(newText[WordNum - 2]) ? -1 : 0);	//行末が全角文字の1バイト目だった場合、文字化けするので1バイトずらす
		mystrcpy(chTruestring, newText, WordNum + d);
		Add(chTruestring);
		mystrcpy(chOverstring, newText + WordNum + d - 1 - spaceCount);
		for (int i = 0; i < spaceCount; i++) {
			chOverstring[i] = ' ';
		}
		Add(chOverstring);

	} else if (GetDrawStringWidthToHandle(newText, strlen(newText), FontHandle) > WordWidth) {			//字数オーバー時処理 WordWidthはInit()で定義

		char chOverstring[WORD_MAX];
		char chTruestring[WORD_MAX];
		char tmp[WORD_MAX] = "";
				
		int tmpnum=0;				//一行分の横幅で区切るための位置探し
		while(GetDrawStringWidthToHandle(tmp, strlen(tmp), FontHandle) <= WordWidth && tmpnum < WORD_MAX){
			tmp[tmpnum] = newText[tmpnum];
			tmp[++tmpnum] = '\0';
		}
		--tmpnum;	//tmpnum:\0を含まない適正文字数 true[tmpnum-1]='\0'となるようにする

		if (_ismbblead(newText[tmpnum])) {
			--tmpnum;		//行末が全角文字の1バイト目だった場合、文字化けするので1バイトずらす
		}
				
		mystrcpy(chTruestring, newText, tmpnum);
		Add(chTruestring);
		mystrcpy(chOverstring, newText + tmpnum - 1 - spaceCount);
		for (int i = 0; i < spaceCount; i++) {
			chOverstring[i] = ' ';
		}
		Add(chOverstring);				//オーバーした分を切り取って再度読み込み

	} else {

		mystrcpy(Text[NextLine], newText); 
		NextLine = (++NextLine) % StockLineNum;
	
	}
	
	return true;
}

bool CLogWindow::Add(char **_newTextArray){
	if (_newTextArray == NULL) return false;
	
	for (int i=0; !mystrcmp(_newTextArray[i], EOP)&&strlen(_newTextArray[i]); i++){
		if (!Add(_newTextArray[i])) return false;
	}
	return true;
}

bool CLogWindow::Main(){

	if (CheckHitKey(KEY_INPUT_UP)) {
		BackLine = min(BackLine+1, StockLineNum-LineNum);
	} else if (CheckHitKey(KEY_INPUT_DOWN)) {
		BackLine = max(BackLine-1, 0);
	}

	return Visible;
}

bool CBattleLog::Main() {

	if (FullMode) {
		CLogWindow::Main();
		if (CheckHitKeyDown(KEY_INPUT_CANCEL)) FullMode = false;
	}

	return FullMode;
}

void CBattleLog::Draw(){

	if(!Visible) return;

	if (CheckHitKeyDown(KEY_INPUT_P)) {	//ウィンドウモード切替（攻撃エフェクト中にもログ確認できるよう、MainではなくDrawでキーチェック）
		SetWindowMode(!FullMode);
		BackLine = 0;
	}

	//ボックス
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, (FullMode?200:100));
		DrawBox((!FullMode?PosX:PosXFull),	 PosY,	 (!FullMode?PosX:PosXFull)+(!FullMode?Width:WidthFull),   PosY+Height, BoxColor, true);
		DrawBox((!FullMode?PosX:PosXFull)+5, PosY+5, (!FullMode?PosX:PosXFull)+(!FullMode?Width:WidthFull)-5, PosY+Height-5, GRAY, false);
	
	//ボタン
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, (FullMode?150:100));
		DrawCenterGraph((!FullMode?PosX:PosXFull), (PosY+Height)/2, ButtonImg[(!FullMode?0:1)], true);

	//テキスト
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, (FullMode?220:200));

		//テキスト描写
		int line;
		int skipCount = 0;	//空行は上詰め
		int drawCount = 0;  //実際に描画した行の数
		char* textForDraw = new char[(FullMode? WordNum: 8)];
		for (int i = 0; i+skipCount < StockLineNum && drawCount < LineNum; i++){
			line = mod ((NextLine - LineNum - BackLine + i + skipCount), StockLineNum);
			if (strlen(Text[line])) {
				mystrcpy(textForDraw, Text[line], (FullMode? WordNum: 5));
				if (!FullMode) sprintf_s(textForDraw, -1, "%s..", textForDraw);
				DrawStringToHandle((!FullMode?PosX:PosXFull)+BoxSpace, PosY + (FontSize+LineSpace)*i+1+BoxSpace, textForDraw, FontColorSub , FontHandle);
				DrawStringToHandle((!FullMode?PosX:PosXFull)+BoxSpace, PosY + (FontSize+LineSpace)*i+BoxSpace  , textForDraw, FontColorMain, FontHandle);
				++drawCount;
			} else {
				++skipCount;
				--i;
			}
		}

		delete [] textForDraw;
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND,-1);
}


CFieldLog::CFieldLog() {
	LineSpace = 10;
	BoxSpace = 25;

	PosMemo = PosMemoOld = -1;
}


void CFieldLog::Draw() {

	if (!Visible) return;

	//ボックス
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
	DrawBox(PosX, PosY, PosX + Width, PosY + Height, BoxColor, true);
	DrawBox(PosX + 5, PosY + 5, PosX + Width - 5, PosY + Height - 5, GRAY, false);

	//テキスト
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 220);

	//テキスト描写
	int line;
	int skipCount = 0;	//空行は上詰め
	int drawCount = 0;  //実際に描画した行の数
	char* textForDraw = new char[WordNum];
	for (int i = 0; i + skipCount < StockLineNum && drawCount < LineNum; i++) {
		line = mod((NextLine - LineNum - BackLine + i + skipCount), StockLineNum);
		if (strlen(Text[line])) {
			mystrcpy(textForDraw, Text[line], WordNum);
			DrawStringToHandle(PosX + BoxSpace, PosY + (FontSize + LineSpace)*i + 1 + BoxSpace, textForDraw, FontColorSub, FontHandle);
			DrawStringToHandle(PosX + BoxSpace, PosY + (FontSize + LineSpace)*i + BoxSpace, textForDraw, FontColorMain, FontHandle);

			//sprintf_s(textForDraw, WordNum, "%d", line);	//DEBUG
			//DrawStringToHandle(PosX, PosY + (FontSize + LineSpace)*i + BoxSpace, textForDraw, FontColorMain, FontHandle);

			++drawCount;
		} else {
			++skipCount;
			--i;
		}
	}

	delete[] textForDraw;
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, -1);

}

bool CFieldLog::Main() {

	if (Visible) {
		CLogWindow::Main();
		if (CheckHitKeyDown(KEY_INPUT_P) || CheckHitKeyDown(KEY_INPUT_OK) || CheckHitKeyDown(KEY_INPUT_CANCEL)) Visible = false;
	} else {
		if (CheckHitKeyDown(KEY_INPUT_P)) {
			Visible = true;
			BackLine = 0;
		}
	}

	return Visible;
}

void CFieldLog::MemorizeCurrentPos() {	//トークラベルの挿入箇所を記憶しておく
	PosMemoOld = (PosMemo == -1 ? NextLine : PosMemo);
	PosMemo = NextLine;
	//DEBUGDX("Memorize: Memo:Old=%d:%d", PosMemo, PosMemoOld);
}

void CFieldLog::ResetCurrentPos() {
	//DEBUGDX("ResetCurrentPos");
	PosMemoOld = PosMemo = -1;
}

void CFieldLog::InsertToMemoPos(const char* _string) {	//トークラベルの挿入用
	//DEBUGDX("Insert:%s Memo:Old=%d:%d", _string, PosMemo, PosMemoOld);
	if (PosMemoOld != -1) {
		for (int i = NextLine; mod(i, StockLineNum) != PosMemoOld; i--) {
			mystrcpy(Text[mod(i, StockLineNum)], Text[mod(i - 1, StockLineNum)]);
		}
		mystrcpy(Text[PosMemoOld], _string);
		NextLine = (++NextLine) % StockLineNum;
		
		++PosMemo;	//挿入した分、記録時の位置が変わるのでずらしておく
		++PosMemoOld;
	}
}
/*void CTextBox::Draw_Animation(bool _showingstop){
	
	if (Showing){	//テキストアニメーション中
		if (!_showingstop) ShowingTime++;
	}else if (NewText != -1 && !Showing){	//テキストアニメーション前
		ShowingTime = 0;
		Showing = true;
	}else if (NewText == -1 && !Showing){	//テキストアニメーション済み
		for (int i = 0; i<LineNum; i++){
			strcpy_s(chDrawText[i], chText[i]);
		}
	}			
			
	if (Showing && !_showingstop){
		int NowShow = (int)(SHOWING_SPEED*ShowingTime/60)*2;
		int Sumstrlen = 0;

		for (int i = 0; i<NewText; i++){
			strcpy_s(chDrawText[i], chText[i]);
		}

		for (int i = NewText; i<LineNum; i++){
			if (NowShow-Sumstrlen <= 0) {
				strcpy_s(chDrawText[i], "");
			}else{
				strncpy_s(chDrawText[i], chText[i], min(NowShow-Sumstrlen, mystrlen(chText[i])));	//ルビ用@は表示しないのでそこでアニメーションがずれないようにするコード（まだ試してナイ）→//min(NowShow-Sumstrlen+(chText[i][min(NowShow-Sumstrlen, mystrlen(chText[i]))]=='@'?1:0), mystrlen(chText[i]))
			}
			Sumstrlen += strlen(chText[i]);
		} 
		
		if (NowShow > Sumstrlen) {
			Showing = false;
			NewText = -1;
		}
	}
}
*/