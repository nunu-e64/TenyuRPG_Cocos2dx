////多重インクルード防止（インクルードガード）//
#ifndef MAIN_H								////
#define MAIN_H								////
////////////////////////////////////////////////

//#include "../Define.h"
#include "../Field/EveObj.h"

class CMain{	//Singletonパターンを使い単独存在を保証
public:
    // 唯一のアクセス経路
    static CMain* GetInstance() {
    	static CMain instance;  // 唯一のインスタンス（静的メンバ関数･･･インスタンス生成前から存在しておりCMain::GetInstanceの形でどこからでも呼び出せる）
    	return &instance;
    }
	~CMain(){DESTRUCTED;}
	
	//メンバ//////////////////////////////////////////////////
	bool Init();
	bool GameLoop();

private:
    // 生成やコピーを禁止する
    CMain(){CONSTRUCTED;}
    CMain(const CMain& rhs);
    CMain& operator=(const CMain& rhs);

	//メンバ//////////////////////////////////////////////////
	bool Playing(const int _dnum=-1);
	void StartDemo();
	int Title();
	int ChooseData();
	void Opening();
	bool GameOver();
	bool GameClear();
	void Setting();
	
	//playdata_tag PlayData[PLAYDATA_NUM];
	playdata_tag PlayData[1];

};

////多重インクルード防止（インクルードガード）//
#endif										////
////////////////////////////////////////////////