////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#ifndef BIMGBANK_H						////
#define BIMGBANK_H						////
////////////////////////////////////////////////

#include "../Main/ImgBank.h"

class CBImgBank : public CImgBank{
public:
	CBImgBank(){
		CONSTRUCTED;
	}
	~CBImgBank(){
		DESTRUCTED;
		Clear();
	}

	void Clear();
	void Init();

	void SetBattleBackGround(const char* _key,int _mapnum, int _chipnum=-1);
	int GetBattleBackGround(int _mapnum, int _chipnum=-1);

private:

	std::map <int,int> BattleBackGround[MAP_MAX];

};


////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#endif										////
////////////////////////////////////////////////