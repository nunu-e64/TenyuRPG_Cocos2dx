////多重インクルード防止（インクルードガード）//
#ifndef CMDLIST_H							////
#define CMDLIST_H							////
////////////////////////////////////////////////

//リストで実装したキュー構造

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


////多重インクルード防止（インクルードガード）//
#endif										////
////////////////////////////////////////////////