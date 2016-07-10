#include "nunuLib.h"

//using namespace nunuLib;


//現在時刻取得関数///////////////////////////////////////////
SYSTEMTIME GetNowSystemTime(){
	SYSTEMTIME st;
	GetSystemTime(&st);
	
	FILETIME ft1;
	FILETIME ft2;
    SystemTimeToFileTime(&st, &ft1);
	
	//a こちらでもOK。動作は確認済み。
	//long long int t = (long long int)(ft1.dwHighDateTime)<<32 | ft1.dwLowDateTime;
	//t += (long long int)9*60*60*1000*1000*10;	//1日・・・24*60*60*1000*1000*10 
	//ft2.dwHighDateTime = t>>32;	// & 0xFFFFFFFF;
	//ft2.dwLowDateTime = (int)t;	
	
	//b
	FileTimeToLocalFileTime(&ft1, &ft2);

	FileTimeToSystemTime(&ft2, &st);
	return st;
}
std::string GetNowSystemTimeString(){
	char currentTime[25] = { 0 };
	SYSTEMTIME st = GetNowSystemTime();
	wsprintf(currentTime, "%04d/%02d/%02d %02d:%02d:%02d %03d",
		st.wYear, st.wMonth, st.wDay,
		st.wHour, st.wMinute, st.wSecond, st.wMilliseconds); 
	std::string tmp = currentTime;
	return tmp;
}
/////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////
////文字列系///////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////
////文字列を数値に変換///////////////////////////////////////
bool mystrtol(const char* str, long int* result, int radix){
	long int num;
	char* end;

	if( str == NULL || result == NULL )return false;
	errno=0;

	num = strtol( str, &end, radix );
	if( errno== ERANGE && (num== LONG_MAX || num== LONG_MIN) ){
		return false;    /* 変換結果が表現できない */
	}else if( str== end ){
		return false;    /* １文字も変換できない */
	}

	*result = num;
	return true;
}
bool mystrtol(const char* str, int* result, int radix){
	long int num;
	bool flag;
	if((flag = mystrtol(str, &num, radix)))	*result = (int)num;
	return flag;
}
bool mystrtod(const char* str, double* result){
	double num;
	char* end;

	if( str == NULL || result == NULL )return false;
	errno=0;

	num = strtod( str, &end);
	if( num== 0 && errno== ERANGE ){
		return false; /*アンダーフロー*/
	}else if( (num== HUGE_VAL || num== -HUGE_VAL) && errno== ERANGE ){
		return false;	/*"結果が表現できない*/
	}

	*result = num;
	return true;
}
bool mystrtod(const char* str, float* result){
	double num;
	bool flag;
	if((flag = mystrtod(str, &num))) *result = (float)num;
	return flag;
}
/////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////
////strlenをsignedに/////////////////////////////////////////
signed int mystrlen(const char* _str){
	return (signed int)strlen(_str);
}
/////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////
///文字列比較用関数//////////////////////////////////////////
bool mystrcmp(const char *String, const char *Words, const char Option){	
	if (String==NULL) return false;
	if (Option=='p'){			//Perfect: 完全一致
		return ((strcmp(String, Words)==0)? true: false);

	}else if (Option=='l'){		//Left: Stringの先頭にWordsが含まれている
		return ((strstr(String, Words)==String)? true: false);

	}else if (Option=='m'){		//Middle: Stringの中にWordsが含まれている
		return ((strstr(String, Words)!=NULL)? true: false);

	}else{
		return mystrcmp(String, Words, 'p');
	}
}
//複数のキーワードでOR判定
bool mystrcmp(const char *String, const char Option, const int arg_num, ...){
	va_list args;
	va_start( args, arg_num);	//arg_numが大きすぎたときの処置方法はないのか？
	
	for (int i=0; i<arg_num; i++){
		if (mystrcmp(String, va_arg(args, char*), Option)){
			va_end(args);
			return true;
		}
	}
	va_end(args);
	return false;
}

//大文字小文字を区別せずに比較 （strcmpの区別なし版はstristrがある。しかしstrstrの区別なし版は標準で存在しないので一つ一つ手動置換する（strcasestrはwindowsにはないっぽい））
bool mystrcmp2(const char *String, const char *Words, const char Option){	
	if (String==NULL) return false;
	if (Option=='p'){			//Perfect: 完全一致
		for (unsigned int i=0; i<strlen(String)+1; i++){
			if (tolower((unsigned char)String[i]) != tolower((unsigned char)Words[i])) {
				return false;
			}
		}
		return true;

	}else if (Option=='l'){		//Left: Stringの先頭にWordsが含まれている
		for (unsigned int i=0; i<strlen(String)+1; i++){
			if (Words[i] == '\0') {
				return true;
			}else if (tolower((unsigned char)String[i]) != tolower((unsigned char)Words[i])) {
				return false;
			}
		}
		return true;

	}else if (Option=='m'){		//Middle: Stringの中にWordsが含まれている
		char* tmpString = new char[strlen(String)+1]; 
		char* tmpWords  = new char[strlen(Words)+1]; 
		for (unsigned int i=0; i<strlen(String)+1; i++){
			tmpString[i] = tolower((unsigned char)String[i]);
			//myLogf("mystrcmp2", "%c", tmpString[i]);
		}
		for (unsigned int i=0; i<strlen(Words)+1; i++){
			tmpWords[i] = tolower((unsigned char)Words[i]);
			//myLogf("mystrcmp2", "%c", tmpWords[i]);
		}
		bool forReturn = ((strstr(tmpString, tmpWords)!=NULL)? true: false);
		delete [] tmpString;
		delete [] tmpWords;

		return forReturn;

	}else{
		return mystrcmp2(String, Words, 'p');
	}
}
//大文字小文字を気にせずに複数のキーワードでOR判定
bool mystrcmp2(const char *String, const char Option, const int arg_num, ...){
	va_list args;
	va_start( args, arg_num);	//arg_numが大きすぎたときの処置方法はないのか？
	
	for (int i=0; i<arg_num; i++){
		if (mystrcmp2(String, va_arg(args, char*), Option)){
			va_end(args);
			return true;
		}
	}
	va_end(args);
	return false;
}
/////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////
///半角文字正確探索用関数////////////////////////////////////
char* mystrchr(char* string, char character){	//探したい半角文字と、ある全角文字の2Byte目が同じときに誤認してしまう問題を解決（標準関数でないのか・・・？）
	char* p = string;							//※stringのひとつ前が、文頭または半角文字または全角文字の2バイト目の文字になりえる文字であることが保証されているときのみ正常機能	
	
	while ((p=strchr(p, character))!=NULL){

		if (p==string) return p;
		if (_ismbblead(*p)) {
			p++;
			continue;
		}
		
		for (int i=1; (p-i)>=string; i++){
			if (i%2){
				if (_ismbblead(*(p-i))) {
					continue;
				}else{
					return p;	//*pが半角文字と確定
				}
			}else{
				if (_ismbblead(*(p-i))) {
					if ((p-i)==string) return p;	//*pが半角文字と確定
					continue;
				}else{
					break;		//*pが全角文字の2バイト目と確定
				}			
			}
		}
		p++;
	}
	
	return NULL;
}
/////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////
///文字列コピー用関数////////////////////////////////////////
bool mystrcpy(char *String, const char *Source){
	int i=0;
	while(1){
		String[i] = Source[i];
		if (Source[i++]=='\0') break;
	}
	return true;
}
bool mystrcpy(char **String, const char **Source, const int SourceLineSize){
	for (int i=0; i<SourceLineSize; i++){
		mystrcpy(String[i], Source[i]);
	}
	return true;
}
bool mystrcpy(char *String, const char *Source, int size_including_null){
	for (int i=0; i<size_including_null-1; i++){
		String[i] = Source[i];
		if (Source[i]=='\0') return true;
	}
	String[size_including_null-1]='\0';
	return true;
}
/////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////
///文字列の両端から余計な文字を削る関数//////////////////////
bool mystrsmt(char *string, const char* uselesswords){
	int i = 0; int j = 0;
	while(1){	//前から削る
		i = 0;
		while (1){
			if (string[j]==uselesswords[i]){
				j++;
				break;
			}
			if (i==strlen(uselesswords)-1) goto endloop;
			i++;
		}
	}
endloop:
	mystrcpy(string, &string[j]);
	
	j = strlen(string)-1;
	while(1){	//後ろから削る
		i = 0;
		while (1){
			if (string[j]==uselesswords[i]){
				j--;
				break;
			}
			if (i==strlen(uselesswords)-1) goto finish;
			i++;
		}
	}
finish:
	string[j+1]='\0';
	return true;
}
/////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////
///文字列結合用関数////////////////////////////////////////
bool mystrcat(char *string, const char* source){
	char* p;
	p = string+strlen(string);
	
	for (unsigned int i=0; i<strlen(source)+1; i++){
		*p = source[i];
		++p;
	}
	
	return true;
}
/////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////
///文字列置換用関数//////////////////////////////////////////
void mystrrep(char* string, const char* from, const char* to){
	char* p;

	while((p=strstr(string, from))!=NULL){
		char* tmp = new char[strlen(string)];
		*p = '\0';
		p += strlen(from);
		mystrcpy(tmp, p);
		mystrcat(string, to);
		mystrcat(string, tmp);
		delete [] tmp;
	}
}
/////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////
bool mystrgrt(const char* string, const char* target, bool greater){
	int i=0;
	while (string[i]==target[i] && string[i]!='\0' && target[i]!='\0'){
		++i;
	}

	if (greater){
		return (string[i]>target[i]);
	}else{
		return (string[i]<target[i]);
	}
}
/////////////////////////////////////////////////////////////


////文字列系ここまで////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////
////描画系//////////////////////////////////////////////////////////////////////////
CVector GetGraphSize(int GrHandle){
	CVector vec;	
	int w, h;
	GetGraphSize(GrHandle, &w, &h);
	vec.Set(w, h);
	return vec;
}

int LoadGraph(const TCHAR *filename, bool errorcheck)		//DxライブラリのLoadGraphのファイルエラー出力付き版。コード節約が目的。
{	int img = LoadGraph(filename);
	if (errorcheck){
		if(img == -1){ErrorDx("Error->Not Found Image:%s", filename);}
	}
	return img;
}

//DrawGraph
//DrawExtendGraph
//DrawTurnGraph
//DrawCenterGraph

int DrawString(int x, int y, int color, const TCHAR* format, ...){
	va_list args;
	char string[1024];
	int for_return;

	va_start( args, format );
	vsprintf_s( string, format, args );

	if(strlen(string)<1024){
		for_return = DrawString(x,y,string,color);	
	}else{
		ErrorDx("Error->nunuLib:DrawString->too long string:%s", __FILE__, __LINE__, string);
		for_return=-1;
	}
	va_end( args );

	return for_return;	
}
//DrawCenterString Notフォーマット版
int DrawCenterString(int cx, int y, int color, const TCHAR* format, ...){
	va_list args;
	char string[1024];
	int for_return;

	va_start( args, format );
	vsprintf_s( string, format, args );
	va_end( args );

	if(strlen(string)<1024){
		for_return = DrawCenterString((int)cx,(int)y,string,color);	
	}else{
		ErrorDx("Error->nunuLib:DrawCenterString->too long string:%s", __FILE__, __LINE__, string);
		for_return=-1;
	}

	return for_return;	
}
int DrawCenterString(int cx, int y, int color, bool centerY, const TCHAR* format, ...){	//フォーマット版
	va_list args;
	char string[1024];
	int for_return;

	va_start( args, format );
	vsprintf_s( string, format, args );
	va_end( args );

	if(strlen(string)<1024){
		for_return = DrawCenterString((int)cx,(int)y,string,color,centerY);	
	}else{
		ErrorDx("Error->nunuLib:DrawCenterString->too long string:%s", __FILE__, __LINE__, string);
		for_return=-1;
	}

	return for_return;	
}
////描画系ここまで//////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////
////音楽系//////////////////////////////////////////////////////////////////////////
bool CheckSound(int _handle) {

	int result = CheckSoundMem(_handle);
	if (result == 1) {
		return true;
	} else if (result == 0) {
		return false;
	} else {
		WARNINGDX("DXLIB Error was happened. :handle=%d", _handle);
		return false;
	}

}
////音楽系ここまで//////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////
///エラー出力用関数//////////////////////////////////////////
//
//ErrorDxはインライン関数なので、定義はヘッダに記載
//
/////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////
///自作Choose関数////////////////////////////////////////////
int choose(const int target, ...){
	int choice;
	va_list args;
	va_start( args, target);	//targetが大きすぎたときの処置方法はないのか？
	
	if (target<=0){
		ErrorDx("Error->arg[target] should >=1: target=%d", __FILE__, __LINE__, target);
		choice = va_arg(args, int);

	} else {
		for (int i=1; i<=target; i++){
			choice = va_arg(args, int);		//target=1の時、一個目を返す（Not target=0）
		}
	}

	va_end(args);
	return choice;
}
/////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////
///KeyDown判定用関数/////////////////////////////////////////
static nunuLibKey::CKeyManager KeyManager;
bool CheckHitKeyDown(const int KEY_CODE){
	return KeyManager.CheckDown(KEY_CODE);
}
nunuLibKey::CKeyManager::CKeyManager(){
	for (int i=0; i<ARRAY_SIZE(Pressed); i++){
		Pressed[i]=false;
	}
}
bool nunuLibKey::CKeyManager::CheckDown(const int KEY_CODE){		//押した瞬間だけtrueを返す
	//決定キー・・・Z,Enter  キャンセルキー・・・X,BackSpace  それぞれどちらでも可
	switch (KEY_CODE){
		case KEY_INPUT_OK:  {
			bool tmp0 = CheckDown(KEY_INPUT_RETURN);
			bool tmp1 = CheckDown(KEY_INPUT_Z);
			return (tmp0||tmp1);	
		}
		case KEY_INPUT_CANCEL:  {
			bool tmp0 = CheckDown(KEY_INPUT_BACK);
			bool tmp1 = CheckDown(KEY_INPUT_X);
			return (tmp0||tmp1);
		}
		default:{
			break;
		}
	}

	//判定のメイン
	if (CheckHitKey(KEY_CODE)){
		if (!Pressed[KEY_CODE]) {
			Pressed[KEY_CODE] = true;
			return true;
		}
		return false;
	}else{
		Pressed[KEY_CODE]= false;
		return false;
	}

	return false;
}
/////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////
//////フォーマット対応ウィンドウタイトル変更関数/////////////
int SetTitle(const char* format, ...){
	va_list args;
	char title[256];
	int for_return;

	va_start( args, format );
	vsprintf_s( title, format, args );

	if(strlen(title)<256){
		for_return = SetMainWindowText(title);	
	}else{
		ErrorDx("Error->SetTitle->too long title (title should<256):%s", __FILE__, __LINE__, title);
		for_return=-1;
	}
	va_end( args );

	return for_return;	
}
/////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////
////その他///////////////////////////////////////////////////
//between			も同じ
//mod				も同じ	
/////////////////////////////////////////////////////////////
