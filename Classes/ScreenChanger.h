////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#ifndef SCREENCHANGER_H					////
#define SCREENCHANGER_H					////
////////////////////////////////////////////////

class CScreenChanger{
public:
	CScreenChanger(){
		CONSTRUCTED;
	}
	~CScreenChanger(){
		DESTRUCTED;
	}
	static enum screenchange_tag{
		SCREEN_FADE,
		SCREEN_BOKASHI,
		SCREEN_GURUGURU,
		SCREEN_NUM
	};

	static void ChangeScreen(const int _pGraph, const int _nGraph, const screenchange_tag _type, int _count, int _option=-1);


private:
	static void Fade(const int _pGraph, const int _nGraph, int _count, int _option);
	static void Bokashi(const int _pGraph, const int _nGraph, int _count, int _option);
	static void GuruGuru(const int _pGraph, const int _nGraph, int _count, int _option);
};


////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#endif										////
////////////////////////////////////////////////