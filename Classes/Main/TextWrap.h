////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#ifndef TEXTWRAP_H							////
#define TEXTWRAP_H							////
////////////////////////////////////////////////

#include "TextBox.h"

class CTextWrap : public CTextBox{	//private�Ȍp���ł͐e�N���X��public�����o�ɒ��ځi�C���X�^���X.�֐�()�̌`�Łj�A�N�Z�X�ł����A�C���X�^���X��e�N���X�Ƃ��Ĉ������Ƃ��ł��Ȃ�
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


////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#endif										////
////////////////////////////////////////////////