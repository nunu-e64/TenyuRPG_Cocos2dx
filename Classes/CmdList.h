////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#ifndef CMDLIST_H							////
#define CMDLIST_H							////
////////////////////////////////////////////////

//���X�g�Ŏ��������L���[�\��

class CCmdList{
public:
	
	CCmdList(){
		CONSTRUCTED;
		front = NULL;
		tail = NULL;
	}
	~CCmdList(){
		DESTRUCTED;
		Clear();
	}
	void Add(const char* _format, ...);
	void Get(char* _cmd);
	bool Empty();
	void Clear();

private:
	struct node_tag{
		char command[256];
		node_tag* next;
	};
	
	node_tag* front;
	node_tag* tail;
};


////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#endif										////
////////////////////////////////////////////////