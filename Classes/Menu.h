////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//	//pragmaonce�Ɠ����Ӗ�����������̓R���p�C���Ɉˑ����Ȃ�
#ifndef MENU_H								////
#define MENU_H								////
////////////////////////////////////////////////

class CMenuNode{	//CMenu�̒��ɒ�`����H
public:
	CMenuNode(const char _label[32]){
		CONSTRUCTED;
		strcpy_s(label, _label);
		parent = NULL;
		child  = NULL;
		prev   = NULL;
		next   = NULL;
	}
	~CMenuNode(){
		DESTRUCTED;
	}
	
	char label[32];
	CMenuNode* parent;
	CMenuNode* child;
	CMenuNode* prev;
	CMenuNode* next;
};

class CMenu{
public:	
	CMenu(){
		CONSTRUCTED;
		Cursor = front = god = NULL;
		Alive = false;
	}
	~CMenu(){
		Clear();
		DESTRUCTED;
	};

	void Clear();
	void Init(int _x, int _y, int _width, int _height);
	void Create(CMenuNode* _groupParent);
	void Create(const char _frontlabel[32]);
	void Add(const char _parentlabel[32], CMenuNode* _groupParent);
	void Add(const char _parentlabel[32], const char _newlabel[32]);

	void SetCursor(CMenuNode* _node){ Cursor = _node; }
	CMenuNode* GetCursor() { return Cursor; }

	//virtual void Draw();

	CMenuNode* GetFront(){ return front;}
	CMenuNode* Find(const char _label[32]);
	int GetIndex(CMenuNode* _node);
	bool Move(CMenuNode* &_result, bool _atTip);

	bool Alive;

protected:
	int X, Y;
	int Width, Height;
	CMenuNode* Cursor;

private:
	CMenuNode* Find(const char _label[32], CMenuNode* _top);
	void Clear(CMenuNode* _top);

	CMenuNode* god;
	CMenuNode* front;
};

class CBattleMenu : public CMenu{
public:
	CBattleMenu(){CONSTRUCTED;}
	~CBattleMenu(){DESTRUCTED;}

	void Draw();
};

class CFieldMenu: public CMenu {
public:
	CFieldMenu() { 
		AccessoryMenu = NULL;
		AccessoryMenuVisible = false;	//�A�C�e���o�b�O�ꗗ�̕\���L��
		AccessorySlotNum = -1;
		CONSTRUCTED; 
	}
	~CFieldMenu() { DESTRUCTED; }

	void Draw();

	CMenu* AccessoryMenu;
	bool AccessoryMenuVisible;
	int AccessorySlotNum;	//���܃`�F�b�N���Ă�v���C���[�̑����X���b�g�C���f�b�N�X
};

////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#endif										////
////////////////////////////////////////////////