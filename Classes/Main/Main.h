////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#ifndef MAIN_H								////
#define MAIN_H								////
////////////////////////////////////////////////

//#include "../Define.h"
#include "../Field/EveObj.h"

class CMain{	//Singleton�p�^�[�����g���P�Ƒ��݂�ۏ�
public:
    // �B��̃A�N�Z�X�o�H
    static CMain* GetInstance() {
    	static CMain instance;  // �B��̃C���X�^���X�i�ÓI�����o�֐�����C���X�^���X�����O���瑶�݂��Ă���CMain::GetInstance�̌`�łǂ�����ł��Ăяo����j
    	return &instance;
    }
	~CMain(){DESTRUCTED;}
	
	//�����o//////////////////////////////////////////////////
	bool Init();
	bool GameLoop();

private:
    // ������R�s�[���֎~����
    CMain(){CONSTRUCTED;}
    CMain(const CMain& rhs);
    CMain& operator=(const CMain& rhs);

	//�����o//////////////////////////////////////////////////
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

////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#endif										////
////////////////////////////////////////////////