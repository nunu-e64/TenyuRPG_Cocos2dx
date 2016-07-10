////多重インクルード防止（インクルードガード）//
#ifndef TEXTWRAP_H							////
#define TEXTWRAP_H							////
////////////////////////////////////////////////

#include "TextBox.h"

class CTextWrap : public CTextBox{	//privateな継承では親クラスのpublicメンバに直接（インスタンス.関数()の形で）アクセスできず、インスタンスを親クラスとして扱うこともできない
public:
	CTextWrap(){
		CONSTRUCTED;
	}
	~CTextWrap(){
		DESTRUCTED;
	}
	void Term(CCmdList* _cmdlist);	//Terminate
	void Draw(bool _showingstop=false);

private:
		
};


////多重インクルード防止（インクルードガード）//
#endif										////
////////////////////////////////////////////////