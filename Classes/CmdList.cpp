#include "CmdList.h"

////////////////////////////////////////////////////////////////////////////////////////
///コマンドリスト///////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////


//最後尾に命令を追加
void CCmdList::Add(const char* _format, ...){
	if( strlen(_format)>256 ){
		ERRORDX("CmdList::Add 字数オーバー[255]：%s", _format);		//本体はvsnprintfを使って明示的にサイズを示してやることで防止すべき
		return;
	}

	char newFormat[512];
	mystrcpy(newFormat, _format);

	//外部読込みされた文字としての'％'を書式誤認しないように'％％'に書き換える
	for (int i=0; i<sizeof(newFormat)-1 && newFormat[i]!='\0'; i++){
		if (newFormat[i]=='%' && (newFormat[i+1]==',' || newFormat[i+1]==' ' || newFormat[i+1]==')')) {
			for (int j=strlen(newFormat)+1; j<sizeof(newFormat)-1 && j!=i+1; j--){
				newFormat[j] = newFormat[j-1];
			}
			newFormat[i+1]='%';
			++i;	//二重チェックしないようにひとつ余分に前に進める
		}
	}

	//DEBUGDX("_format=%s", newFormat);

	va_list args;
	char newText[1024];
	va_start(args, _format);
	vsnprintf_s(newText, sizeof(newText), newFormat, args);	
	va_end(args);

	//DEBUGDX("newText=%s", newText);

	if( strlen(newText)>256 ){
		ERRORDX("CmdList::Add 字数オーバー[255]：%s", newText);
		return;
	}

	node_tag* p = new node_tag;
	mystrcpy(p->command, newText);
	p->next = NULL;

	if (tail!=NULL)	tail->next = p;
	tail = p;

	if (front==NULL) front = p;
	
	va_end(args);
}


//先頭行の命令を返す
void CCmdList::Get(char* _cmd){
	node_tag* p = front;

	mystrcpy(_cmd, p->command);
	front = p->next;	
	if (p==tail) tail = NULL;

	delete p;	//コマンドリストから削除
}
bool CCmdList::Empty(){
	return (front == NULL);
}


void CCmdList::Clear(){
	
	node_tag* p=front;
	node_tag* tmp;

	/* 連結リストの要素を削除 */
	while( p != NULL ){
		tmp = p->next;
		delete p;
		p = tmp; 
	}
	
	front = NULL;
	tail = NULL;
}