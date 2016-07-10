////多重インクルード防止（インクルードガード）//
#ifndef DEFINE_H							////
#define DEFINE_H							////
////////////////////////////////////////////////

//#define PRODUCT_MODE  //製品版
	#ifdef PRODUCT_MODE
		#define MYLOG_DISABLE
		#define WARNINGDX_DISABLE	//マクロの有効化/無効化
		#define DEBUGDX_DISABLE	//マクロの有効化/無効化
		#define CHECK_TIME_DISABLE	//マクロの有効化/無効化
		#define MEMORY_CHECK_DISABLE //マクロの有効化/無効化
		#define FPS_DISABLE
	#endif

#define DEBUG_MODE	//プログラマデバッグ版 （他のメンバーに渡す時にはコメントアウト）
	#ifndef DEBUG_MODE
		#define DEBUGDX_DISABLE	//マクロの有効化/無効化
		#define CHECK_TIME_DISABLE	//マクロの有効化/無効化
		#define MEMORY_CHECK_DISABLE //マクロの有効化/無効化
	#endif

#include "Dxlib.h"
#include <vector>
#include <map>
#include <string>

#define MEMORY_CHECK	//メモリの解放し忘れをチェック。Debugビルドで使用可能。
	#if defined(MEMORY_CHECK) && !defined(MEMORY_CHECK_DISABLE)
		#define _CRTDBG_MAP_ALLOC
		#include <stdlib.h>
		#include <crtdbg.h>
		#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)
	#endif

#include "nunuLib.h"
//using namespace nunuLib;

//"Mrt.h"でDefine.hをインクルードしているのでMrt.hは使うとことで任意にインクルード


////////////////////////////////////////////////
#define MAP_MAX_CHECK(_mapnum, _return) {	\
	if (!(_mapnum<MAP_MAX)){				\
		ERRORDX("TooLargeMapNumber:%d(MAPMAX=%d)",_mapnum,MAP_MAX);return _return;	\
	}else if(_mapnum<0){					\
		ERRORDX("mapnum<0: %d",_mapnum);return _return;	\
	}}

#define NUM_MAX_CHECK(_num, _max, _return) {	\
	if (!(_num<_max)){							\
		ERRORDX("'%s=%d' must < %d", #_num, _num, _max); return _return;	\
	}else if(_num<0){					\
		ERRORDX("'%s=%d' must >= 0", #_num, _num); return _return;	\
	}}

#define CONSTRUCTED //myLog("MyClassLog", "%s.Constructed", typeid(*this).name())
#define DESTRUCTED //myLog("MyClassLog", "%s.Destructed", typeid(*this).name())


//戦闘関連///////////////////////////////////////////////
const int MAX_PLAYER_NUM = 3;
const int MAX_ENEMY_NUM = 3;
const int MAX_MAGIC_COUNT = 15;
const int MAX_ATTENTION = 10;

const int DEFFENCE_MC = 1;	//増えてきたら必要に応じてenumにまとめる
const double MAGIC_DEFFENCE_RATE = 0.1;
const double MAX_MAGIC_COUNTER_DAMAGE_RATE = 0.3;
const int PRAY_RECOVERY_MC = 5;

const double BASIC_SPD = 1.0;
const int DEFFENCE_TIME = 100;
const int PRAY_TIME = 100;
const int WAIT_TIME = 50;
/////////////////////////////////////////////////////////

//rpgファイル名等///////////////////////////////////////////
const char FILE_B_SYSTEM[] = "tenyu_data/b_system.rpg";	//戦闘全般（エンカウント・技・敵味方ステータスなど）関するテキストファイル
const char FILE_SYSTEM[] = "tenyu_data/system.rpg";		//システム全般に関する読み込みテキストファイル
const char FILE_SCENARIO[] = "tenyu_data/scenario.rpg";	//シナリオ及びイベントに関するテキストファイル
const char FILE_EVENT[] = "tenyu_data/event.rpg";		//オリジナルイベントに関するテキストファイル
const char DIR_SAVE[] = "tenyu_data/save";				//セーブデータを保存するディレクトリ
const char FILE_DATANAME[] = "tenyu_data/save/dataname.rpg";	//セーブデータ名を管理するためのテキストファイル

//システムグラフィック///////////////////////////////////
const char IMAGE_TALKNAMELABEL[3][64] ={ "tenyu_data/pic/sys/talkname00.bmp",
									     "tenyu_data/pic/sys/talkname01.bmp",
									     "tenyu_data/pic/sys/talkname02.bmp"};	//pngではゲーム読み込み時のTalkName.Initで不正メモリアクセスエラーが出るため対症療法的にbmpを使用(150906)


//システムグラフィックのPicKey///////////////////////
const char TARGET_CURSOR[] = "TARGET_CURSOR";
const char HP_BAR[]			= "HP_BAR";
const char TIME_BAR1[]		= "TIME_BAR1";
const char TIME_BAR2[]		= "TIME_BAR2";
const char TIME_WAIT[]		= "TIME_WAIT";
const char TIME_TRICK[]		= "TIME_TRICK";
const char TIME_DEFFENCE[]	= "TIME_DEFFENCE";
const char TIME_PRAY[]		= "TIME_PRAY";
const char ATTENTION_MARKER[] = "ATTENTION_MARKER";
const char ATTENTION_BOARD[]  = "ATTENTION_BOARD";
const char ATTENTION_EFFECT[] = "ATTENTION_EFFECT";
const char LOG_WINDOW_BUTTON[] = "LOG_WINDOW_BUTTON";
const char MAGIC_COUNTER[] = "MAGIC_COUNTER";
const char STATUS_CHANGER[] = "STATUS_CHANGER";

//ゲーム名///////////////////////////////////////////
const char GAME_TITLE[] = "TENYU";
const char VERSION[] = "ver0.00";

//ゲーム内//////////////////////////////////////////////////////
const int PLAYDATA_NUM = 0;	//セーブデータの数(0~3)
const int MAP_CHIP_SIZE = 32;
const int MAP_SIZE = 128;		//正方形を想定
const int MAP_DATA_SIZE = 256;	//バイナリデータに格納されているデータの最大値（256=0~255=8bit=1byte）

const char MAP_FILE_TYPE[] = ".Map2";
const char EMAP_FILE_TYPE[] = ".eMap";

const unsigned int MAP_MAX = 2;	//読み込めるマップの最大数	//MAPデータ配列の一次の要素数
const int CHARA_PIC_NUM = 16;	//キャラの絵が一枚何コマなのか

//プログラム内//////////////////////////////////////////////////////
const char CMD_SEPARATOR[] = " ,	";
const char EOP[] = "EOP";	//EndOfParagraph

const char IFBEGIN[] = "IF_BEGIN";
const char IFEND[] = "IF_END";
const char IFCASE[] = "CASE";
const char _IFBEGIN[] = "_IF_BEGIN";
const char _IFEND[] = "_IF_END";
const char _IFCASE[] = "_CASE";
const char JOKER_NAME[] = "joker";

const char NO_EQUIP[] = "装備なし";
const char REMOVE_EQUIP[] = "装備しない";
////////////////////////////////////////////////////

enum gamemode_tag{
	MODE_PLAYING,
	MODE_GAMEOVER,
	MODE_GAMECLEAR,
	MODE_BACKTOTITLE,
	MODE_GAMEEND,
};
enum title_tag{
	TITLE_FIRSTSTART,
	TITLE_LOADSTART,
	TITLE_SETTING,
	TITLE_GAMEEND,
	TITLE_NUM,
};
enum direction_tag{		//順番変更禁止（System.cppで使用）
	RIGHT,
	LEFT,
	DOWN,
	UP,
	DIRECTION_NUM
};
extern ENUM(objkind_tag, PANEL, WALKABLE_NUM, BLOCK, NPC, PUSHBLOCK, UNDERDRAW_NUM, COVER);
	//PANEL,				//踏むことでイベント開始
	//WALKABLE_NUM,	//↑これより上のKindはプレイヤーが上を歩くことができる（当たり判定がなく踏むことでイベント）
	//BLOCK,				//BLOCK,NPCは調べることでイベント開始
	//NPC,
	//PUSHBLOCK,			//押すことでイベント開始（調べない）
	//UNDERDRAW_NUM,	//↑これより上（数字としては小さい）のKindはプレイヤーキャラの下に描写
	//COVER,				//イベント開始方法がない、下を歩行可能
	//KIND_NUM

enum charaeffect_tag{
	NONE,
	BLINK,
	KEEP_NUM,		//↓これより下（数字としては大きい）ときはイベント発生時に、一時的にEffectがNONEになる
	RND_DIR,
	WALK,
	EFFECT_NUM
};
enum btlresult_tag{
	WIN,
	LOSE,
	LOSE_NOSCREENCHANGE
};
enum attention_tag{
	ATTENTION_DAMAGE = 2,
	ATTENIOTN_DEFFENCE = -1,
	ATTENIOTN_WAIT = -1,
	ATTENTION_PRAY = -1
};
enum actor_status_tag{
	DEFFENCE,
	PRAY,
	WAIT
};


struct char256{
	char text[256];
	bool operator<(const char256& obj)const{
		return mystrgrt(text, obj.text, false);
	}
	bool operator>(const char256& obj)const{
		return mystrgrt(text, obj.text, true);
	}
};

//共用関数やクラス/////////////////////////////////
namespace sys{
	direction_tag TurnDir(int _dir, signed int _rightspin);
	direction_tag StrtoDir(const char* _str, int _originaldir=DOWN);
	bool PlayerName(const char* _str);
	bool TrueOrFalse(const char* _str, bool _torf);
	int rank3(const char* _str, int _exception=2);
	bool CheckStrNULL(const char* _str);
	bool CheckStrNULL(const std::string _str);
}

//			・status_effect_tag{img, time, maxtime, statustype, power}

struct statusChanger_tag {
	int Img;
	double Time;
	int StatusKind;
	int Power;
};

extern ENUM(target_tag, ME, SINGLE_ENEMY, SINGLE_FRIEND, ALL_FRIEND, ALL_ENEMY);

struct sideEffect_tag{
	ENUM(type_tag, ATK, DEF, SPD, HEAL, MPHEAL, ATTENTION, SET_TIMEGAUGE, HEAL_AFTER_ATTACK, HEAL_AFTER_SELECTCOMMAND);
	type_tag::type EffectType;	//タグ名を付けない場合はint型で宣言しないと代入処理ができない。
	target_tag::type EffectTarget;	//効果対象範囲

	int Power;		//効果力
	int Incidence;	//発生確率
	int Time;	//有効時間（バーと同じく％計算）
};
struct trick_tag{
	char Name[32];
	int Power;
	int Cost;
	int Time;
	std::vector <sideEffect_tag> SideEffect;
	int DamageEffectIndex;	//描画エフェクトの種別番号

	target_tag::type Target;
	/*enum specialType_tag{ //Actor個別処理
		NORMAL,
		NODEF,
		即死,
	  }
	*/
};

struct flag_tag{
	char Key[32];
	int Num;
};

class CFlagSet{
public:
	CFlagSet(){
		CONSTRUCTED;
	}
	~CFlagSet(){
		DESTRUCTED;
	}

	bool CreateNewFlag(const char* _key){
		for(unsigned int i=0; i<Flag.size(); i++){
			if (mystrcmp(Flag[i].Key, _key)) return false;
		}
		CreateFlag(_key, 0);
		return true;
	}

	void SetFlag(const char* _key, int _num=0, bool _add=false, bool _create=false){
		for(unsigned int i=0; i<Flag.size(); i++){
			if (mystrcmp(Flag[i].Key, _key)) {
				int num = (_add? Flag[i].Num+_num: _num);

				if (num<0) ErrorDx("Error->You can't set [num<0] for FLAG (changed to 0):%s", _key);
				Flag[i].Num=max(0,num);
				return;
			}
		}

		if (_create){
			CreateFlag(_key, _num);
		}else{
			ErrorDx("Error->Not Found Flag[%s]", _key);
		}
	};

	int GetFlagNum(const char* _key){
		for(unsigned int i=0; i<Flag.size(); i++){
			if (mystrcmp(Flag[i].Key, _key)) return Flag[i].Num;
		}
		ErrorDx("Error->Not Found Flag[%s] (return -2)", _key);
		return -2;	//未定義
	};
	
	std::vector <flag_tag> Flag;

private:
	
	bool CreateFlag(const char* _key, int _num=0){
		flag_tag newflag;
		mystrcpy(newflag.Key, _key, 32);

		if (_num<0){
			ErrorDx("Error->You can't use [num<0] for FLAG (changed to 0) :%s", _num);
			newflag.Num = 0;
		}else{
			newflag.Num = _num;
		}
		
		Flag.push_back(newflag);
		return true;
	};
};

////////////////////////////////////////////////


//セーブデータ用の構造体/////////////////////////////////
class CEveObj;
struct playdata_tag{
	bool Exist;		//セーブデータが存在するか否か

	int NowMap;
	unsigned int X, Y;
	unsigned int OldX, OldY;
	char PlayerPicKey[32];
	enum direction_tag Dir;
	int Step;	//0~3
	int Dx, Dy;
	bool Visible;

	CFlagSet FlagSet;
	std::vector<CEveObj> EveObj;

	char DataName[32];

};
///////////////////////////////////////////////////


////多重インクルード防止（インクルードガード）//
#endif										////	
////////////////////////////////////////////////