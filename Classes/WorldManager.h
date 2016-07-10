////多重インクルード防止（インクルードガード）//
#ifndef WORLDMANAGER_H						////
#define WORLDMANAGER_H						////
////////////////////////////////////////////////


//#include "Map.h"
//#include "TextBox.h"
//#include "TextWrap.h"
//#include "CmdManager.h"
//#include "CmdList.h"
//#include "EveManager.h"

class CWorldManager{
public:
	CWorldManager(){CONSTRUCTED;; ImgBackGround = NULL;}
	~CWorldManager(){DESTRUCTED;;};

	virtual void Draw(bool _screenflip=false, bool _textshowingstop=false, int dx=0, int dy=0, bool _playeralsoshake=false)=0;
	void FadeDraw(int _time, int _img, bool _changeahead, bool _color);
	virtual void ChangeTextMode(bool _box, const char* _eventtext = NULL)=0;

protected:
	
	int ImgBackGround;	//背景一枚絵用
	/*
	CTextBox* TextBox;
	CTextBox TextBox1;
	CTextWrap TextWrap1;

	CEveManager* EveManager_p;
	CFlagSet* FlagSet_p;
	CCmdList* CmdList_p;*/
};

////多重インクルード防止（インクルードガード）//
#endif										////	
////////////////////////////////////////////////

