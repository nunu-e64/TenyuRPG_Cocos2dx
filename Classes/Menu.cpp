#include "Menu.h"
#include "Field/ItemManager.h"

void CMenu::Init(int _x, int _y, int _width, int _height){
	X = _x;
	Y = _y;
	Width = _width;
	Height = _height;
}
void CMenu::Create(CMenuNode* _groupParent) {

	if (_groupParent == NULL) ERRORDX("NULL argument[_groupParent] (Not Initialized this Menu)");

	Clear();
	god = new CMenuNode("NULL");
	god->prev = god;
	god->next = god;
	god->parent = god;

	front = _groupParent;
	front->prev = front;
	front->next = front;
	front->parent = god;
	god->child = front;

	Cursor = front;
	Alive = false;

}

void CMenu::Create(const char _frontlabel[32]){
	
	Create(new CMenuNode(_frontlabel));
		
}

void CMenu::Add(const char _parentlabel[32], CMenuNode* _groupParent) {
	CMenuNode* tmp;

	if (Find(_groupParent->label) != NULL) {
		ERRORDX("Already existed name:%s(delete group)", _groupParent->label);
		Clear(_groupParent);
		return;
	}

	if (!(strlen(_parentlabel))) {
		tmp = god;
	} else {
		tmp = Find(_parentlabel);
	}

	if (tmp == NULL) {
		ERRORDX("NotFoundParentLabel:%s(delete group)", _parentlabel);
		Clear(_groupParent);
		return;
	} else if (tmp->child == NULL) {
		tmp->child = _groupParent;
		tmp->child->parent = tmp;
		tmp->child->next = tmp->child;
		tmp->child->prev = tmp->child;
	} else {
		tmp = tmp->child;
		while (tmp->next != tmp->parent->child) { tmp = tmp->next; }
		tmp->next = _groupParent;
		tmp->next->prev = tmp;
		tmp->next->next = tmp->parent->child;
		tmp->next->parent = tmp->parent;
		tmp->parent->child->prev = tmp->next;
	}

}

void CMenu::Add(const char _parentlabel[32], const char _newlabel[32]){

	if (Find(_newlabel)!=NULL){
		ERRORDX("Already existed name:%s", _newlabel);
		return;
	} else if (strlen(_parentlabel) && Find(_parentlabel) == NULL) {
		ERRORDX("NotFoundParentLabel:%s", _parentlabel);
		return;
	} else {
		Add(_parentlabel, new CMenuNode(_newlabel));
	}
}

void CMenu::Clear(){
	Clear(god);
	Cursor = front = god = NULL;
}
void CMenu::Clear(CMenuNode* _top) {
	CMenuNode* tmp = _top;

	if (tmp == NULL) return;

	if (tmp->parent != NULL) tmp->parent->child = NULL;

	_top->prev->next = NULL;	//リングを切る

	while (tmp != NULL) {
		if (tmp->child != NULL) Clear(tmp->child);
		CMenuNode* deleteNode = tmp;

		tmp = tmp->next;
		delete deleteNode;
	}
}

CMenuNode* CMenu::Find(const char _label[32]){
	return Find(_label, front);
}
CMenuNode* CMenu::Find(const char _label[32], CMenuNode* _top){
	//再帰による全探索。見渡してから潜るため浅いほうが概ね発見が早い。
	
	CMenuNode* tmp = _top;

	do{
		if (mystrcmp(tmp->label, _label)) return tmp;
		tmp = tmp->next;
	}while(tmp != _top);

	CMenuNode* result;	//tmp = _top

	do{	
		if (tmp->child!=NULL){
			if ((result=Find(_label, tmp->child)) != NULL) return result;
		}
		tmp = tmp->next;
	}while(tmp != _top);
	
	return NULL;
}

int CMenu::GetIndex(CMenuNode* _node){
	int index = 0;
	//DEBUGDX(_node->label);
	while(_node != _node->parent->child){
		_node = _node->prev;
		++index;
	}

	return index;
}

bool CMenu::Move(CMenuNode* &_result, bool _atTip) {
	//決定またはキャンセルキーを押してカーソルが移動するときにTrueを返す。
	//_atTip…Trueなら、メニューの最深部か一番上の階層のときにしかTrueを返さない

	_result = NULL;

	if (CheckHitKeyDown(KEY_INPUT_OK)){
		if (Cursor->child == NULL) {
			_result = Cursor;
			return true;
		}else{
			Cursor = Cursor->child;
			if (!_atTip) {
				_result = Cursor;
				return true;
			}
		}
	
	}else if (CheckHitKeyDown(KEY_INPUT_CANCEL)){
		if (Cursor->parent == NULL || Cursor->parent == god) {
			return true;
		} else {
			Cursor = Cursor->parent;
			if (!_atTip) return true;
		}

	/*
	}else if (CheckHitKeyDown(KEY_INPUT_RIGHT)){
		if (Cursor->next != NULL){
			Cursor = Cursor->next;
		}
	
	}else if (CheckHitKeyDown(KEY_INPUT_LEFT)){
		if (Cursor->prev != NULL){
			Cursor = Cursor->prev;
		}
	
	*/

	}else if (CheckHitKeyDown(KEY_INPUT_DOWN)){
		if (Cursor->next != NULL){
			Cursor = Cursor->next;
		}
	
	}else if (CheckHitKeyDown(KEY_INPUT_UP)){
		if (Cursor->prev != NULL){
			Cursor = Cursor->prev;
		}
	}

	return false;
}


void CBattleMenu::Draw(){
	if (!Alive) return;

	DrawBox(X, Y, X+Width, Y+Height, GetColor(30, 20, 80), true);
	DrawBox(X+5, Y+5, X+Width-5, Y+Height-5, GRAY, false);
	
	//母メニューの文字表示とカーソル表示
	CMenuNode* tmp = GetFront();
	for (int i=0; tmp!=NULL; i++){
		DrawString(X+30, Y+10+i*(1+GetFontSize()), tmp->label, WHITE, BLACK);
		if (Cursor == tmp) DrawString(X+8, Y+10+i*(1+GetFontSize()), "|>", WHITE, BLACK);
		tmp = tmp->next;
		
		if (tmp==GetFront()) break;
	}

	//子メニューの表示とカーソル表示
	if (Cursor->parent->child != tmp) {	//tmp=front
		tmp = Cursor->parent->child;
		CRect childRect(X + Width + 5, X + Width * 4 + 5, Y, Y + Height);
		DrawBox(childRect.Left,		childRect.Top,		childRect.Right,		childRect.Bottom, GetColor(30, 20, 80), true);
		DrawBox(childRect.Left + 5,	childRect.Top + 5,	childRect.Right - 5,	childRect.Bottom - 5, GRAY, false);

		if (mystrcmp(Cursor->parent->label, "道具")) {
			CItemManager* itemManager = CItemManager::GetInstance();
			for (int i = 0; tmp != NULL; i++) {
				char strNum[64];
				sprintf_s(strNum, "%s%d", "*", itemManager->GetPlayerItemNum(tmp->label));

				DrawString(childRect.Left + 30, Y + 10 + i*(1 + GetFontSize()), tmp->label, WHITE, BLACK);
				DrawString(childRect.Right - 2 * (1 + GetFontSize()), Y + 10 + i*(1 + GetFontSize()), strNum, WHITE, BLACK);
				if (Cursor == tmp) DrawString(childRect.Left + 5 + 8, Y + 10 + i*(1 + GetFontSize()), "|>", WHITE, BLACK);
				tmp = tmp->next;

				if (tmp == tmp->parent->child) break;
			}

		} else {
			for (int i = 0; tmp != NULL; i++) {
				DrawString(childRect.Left + 30, Y + 10 + i*(1 + GetFontSize()), tmp->label, WHITE, BLACK);
				if (Cursor == tmp) DrawString(childRect.Left + 8, Y + 10 + i*(1 + GetFontSize()), "|>", WHITE, BLACK);
				tmp = tmp->next;

				if (tmp == tmp->parent->child) break;
			}
		}

	}
}

void CFieldMenu::Draw() {
	if (!Alive) return;

	DrawBox(X, Y, X + Width, Y + Height, GetColor(30, 20, 80), true);	//HACK:DrawGraphにせねば
	DrawBox(X + 5, Y + 5, X + Width - 5, Y + Height - 5, GRAY, false);

	//母メニューの文字表示とカーソル表示
	CMenuNode* tmp = GetFront();
	for (int i = 0; tmp != NULL; i++) {
		DrawString(X + 30, Y + 10 + i*(1 + GetFontSize()), tmp->label, WHITE, BLACK);
		if (Cursor == tmp) DrawString(X + 8, Y + 10 + i*(1 + GetFontSize()), "|>", WHITE, BLACK);
		tmp = tmp->next;

		if (tmp == GetFront()) break;
	}

	//子メニューの表示とカーソル表示

/*	if (mystrcmp(Cursor->parent->parent->label, "Status")) {
		tmp = Find("Status")->child->child;
		DrawBox(X + Width, Y + Height + 5, X + Width + Width, Y + Height * 4 + 5, GetColor(30, 20, 80), true);
		DrawBox(X + 5 + Width, Y + Height + 5 + 5, X + Width + Width - 5, Y + Height * 4 + 5 - 5, GRAY, false);
		for (int i = 0; tmp != NULL; i++) {
			DrawString(X + 30, Y + Height + 5 + 10 + i*(1 + GetFontSize()), tmp->label, WHITE, BLACK);
			if (Cursor == tmp) DrawString(X + 8, Y + Height + 5 + 10 + i*(1 + GetFontSize()), "|>", WHITE, BLACK);
			tmp = tmp->next;

			if (tmp == tmp->parent->child) break;
		}

		tmp = Find("Status")->child;
		DrawBox(X, Y + Height + 5, X + Width, Y + Height * 4 + 5, GetColor(30, 20, 80), true);
		DrawBox(X + 5, Y + Height + 5 + 5, X + Width - 5, Y + Height * 4 + 5 - 5, GRAY, false);

		for (int i = 0; tmp != NULL; i++) {
			DrawString(X + 30, Y + Height + 5 + 10 + i*(1 + GetFontSize()), tmp->label, WHITE, BLACK);
			if (Cursor == tmp) DrawString(X + 8, Y + Height + 5 + 10 + i*(1 + GetFontSize()), "|>", WHITE, BLACK);
			tmp = tmp->next;

			if (tmp == tmp->parent->child) break;
		}
		

	} else */
	if (Cursor->parent->child != GetFront()) {	//カーソルがfrontと同じ階にいない→子
		tmp = Cursor->parent->child;
		DrawBox(X, Y + Height + 5, X + Width, Y + Height * 4 + 5, GetColor(30, 20, 80), true);
		DrawBox(X + 5, Y + Height + 5 + 5, X + Width - 5, Y + Height * 4 + 5 - 5, GRAY, false);

		for (int i = 0; tmp != NULL; i++) {
			DrawString(X + 30, Y + Height + 5 + 10 + i*(1 + GetFontSize()), tmp->label, WHITE, BLACK);
			if (Cursor == tmp) DrawString(X + 8, Y + Height + 5 + 10 + i*(1 + GetFontSize()), "|>", WHITE, BLACK);
			tmp = tmp->next;

			if (tmp == tmp->parent->child) break;
		}
	}

	//装備メニューの描画
	if (AccessoryMenuVisible && AccessoryMenu != NULL) {

		tmp = AccessoryMenu->GetFront();
		DrawBox(X+Width , Y + Height, X + Width*2, Y + Height * 4 , GetColor(30, 20, 80), true);

		for (int i = 0; tmp != NULL; i++) {
			DrawString(X +Width + 30, Y + Height + 5 + 10 + i*(1 + GetFontSize()), tmp->label, WHITE, BLACK);
			if (AccessoryMenu->GetCursor() == tmp) DrawString(X + Width + 8, Y + Height + 5 + 10 + i*(1 + GetFontSize()), "|>", WHITE, BLACK);
			tmp = tmp->next;

			if (tmp == tmp->parent->child) break;
		}

	}
}