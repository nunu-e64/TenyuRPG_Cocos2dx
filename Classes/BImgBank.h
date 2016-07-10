////多重インクルード防止（インクルードガード）//
#ifndef BIMGBANK_H						////
#define BIMGBANK_H						////
////////////////////////////////////////////////

#include "ImgBank.h"

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


////多重インクルード防止（インクルードガード）//
#endif										////
////////////////////////////////////////////////