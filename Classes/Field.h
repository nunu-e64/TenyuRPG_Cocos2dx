////多重インクルード防止（インクルードガード）//
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
	//メンバ関数
		bool CheckEvent(bool _foot=false, bool _push=false);
		bool StartSet (const int _dnum);
		int SaveData(int _dnum, const char _dataname[32] = "");
		void CreateFieldMenu();

	//メンバ変数
		int NowMap;
		int GodX, GodY;	//神システム用（未実装未保存）
	
		//そのうちわかりにくくなってきたら名前空間Playerとかで括ろうかな
		unsigned int X, Y;
		unsigned int OldX, OldY;
		int ImgPlayer[CHARA_PIC_NUM];	char PlayerPicKey[32];
		enum direction_tag Dir;
		int Step;	//0~3
		int Dx, Dy;
		bool Visible;
		unsigned char Alpha;

		enum charaeffect_tag Effect;	//効果（点滅、自動歩行など）//Playerはある程度限定される
			int EffectNumCmd[5];
			int EffectNum[5];

		playdata_tag *PlayData_p;	//Main.cppからセーブデータをすべてポインタでもらっておく

		int TextAutoPlaySpeed;
		gamemode_tag Mode;


	//所持インスタンス
		CMap Map;
		CTextBox* TextBox;
			///遂に解決14/12/11プロジェクトの設定からスタックサイズを1MBから4MBに拡大して解決。stackoverflowだったようだ→//こっちに移すとField.Initで停止する原因不明14/06/10//////////
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


////多重インクルード防止（インクルードガード）//
#endif										////	
////////////////////////////////////////////////

