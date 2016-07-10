////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#ifndef FIELD_H								////
#define FIELD_H								////
////////////////////////////////////////////////

#include "Map.h"

#include "TextBox.h"
#include "TextWrap.h"
#include "CmdManager.h"
#include "CmdList.h"
#include "WorldManager.h"
#include "ScreenChanger.h"

#include "Menu.h"

class CEveManager;
class CBattle;
class CShopManager;


class CField : public CWorldManager{
public:
	CField();
	~CField();
	
	bool Init(playdata_tag* _playdata_p, const int _dnum);
	int MainLoop();
	
	void Draw(bool _screenflip=false, bool _textshowingstop=false, int dx=0, int dy=0, bool _playeralsoshake=false);
	
	bool Walk(int _dir, int _walkspeed=0, bool _eventwalk=false, bool _walk=true, int _fade=0);
	void Jump();

	inline int GetNowMap(){return NowMap;}
	void SetNowMap(int _mapnum);

	void SetPosition(int _mapnum, int _x, int _y, bool _d=false);
	void SetMyPic(const int _img[CHARA_PIC_NUM], const char* _pickey);
	void SetMyDir(int _dir){Dir = direction_tag(_dir);}
	void SetMyVisible(bool _visible){Visible=_visible;}
	void SetMyAlpha(unsigned char _alpha){Alpha = _alpha;}
	void SetMyEffect(int _effectname, int _effectnum[]);
	
	void SetGameMode(gamemode_tag _mode){Mode = _mode;}
	gamemode_tag GetGameMode(){return Mode;}

	void BattleStart(const char* _pic_bg, std::vector<std::string> _enemyList);
	void BattleStart();
	void SetBattleResult(const char* _winmessage, const char* _losemessage); 

	void ChangeTextMode(bool _box, const char* _eventtext = NULL);
	////////////////////////////////////////////
	CFlagSet FlagSet;



////////////////////////////////////////////////////////////////
private:
	//�����o�֐�
		bool CheckEvent(bool _foot=false, bool _push=false);
		bool StartSet (const int _dnum);
		int SaveData(int _dnum, const char _dataname[32] = "");
		void CreateFieldMenu();

	//�����o�ϐ�
		int NowMap;
		int GodX, GodY;	//�_�V�X�e���p�i���������ۑ��j
	
		//���̂����킩��ɂ����Ȃ��Ă����疼�O���Player�Ƃ��Ŋ��낤����
		unsigned int X, Y;
		unsigned int OldX, OldY;
		int ImgPlayer[CHARA_PIC_NUM];	char PlayerPicKey[32];
		enum direction_tag Dir;
		int Step;	//0~3
		int Dx, Dy;
		bool Visible;
		unsigned char Alpha;

		enum charaeffect_tag Effect;	//���ʁi�_�ŁA�������s�Ȃǁj//Player�͂�����x���肳���
			int EffectNumCmd[5];
			int EffectNum[5];

		playdata_tag *PlayData_p;	//Main.cpp����Z�[�u�f�[�^�����ׂă|�C���^�ł�����Ă���

		int TextAutoPlaySpeed;
		gamemode_tag Mode;


	//�����C���X�^���X
		CMap Map;
		CTextBox* TextBox;
			///���ɉ���14/12/11�v���W�F�N�g�̐ݒ肩��X�^�b�N�T�C�Y��1MB����4MB�Ɋg�債�ĉ����Bstackoverflow�������悤����//�������Ɉڂ���Field.Init�Œ�~���錴���s��14/06/10//////////
			CTextBox  TextBox1;
			CTextWrap TextWrap1;
			///////////////////////////////////////////////////////

		CFieldCmdManager FieldCmdManager;
		CFirstSetCmdManager FirstSetCmdManager;
		CCmdList CmdList;
		CEveManager* EveManager;

		CBattle* Battle;
		CFieldMenu FieldMenu;
		CFieldLog FieldLog;
};


////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#endif										////	
////////////////////////////////////////////////

