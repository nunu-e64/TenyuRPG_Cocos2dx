////多重インクルード防止（インクルードガード）//
#ifndef ACTOR_H								////
#define ACTOR_H								////
////////////////////////////////////////////////


#include "Species.h"

class CTextBox;		//前方宣言
class CCmdList;
class CBImgBank;
class CLogWindow;

class CActor : public virtual CSpecies{
public:
	CActor(const CSpecies &obj) : CSpecies(obj){
		CONSTRUCTED;
	}

	CActor(){
		CONSTRUCTED;
	}
	~CActor(){
		DESTRUCTED;
	}

	//初期設定関連
		void FirstSet(int _playernum, int _enemynum, int _index, CTextBox** _textbox, CCmdList* _cmdlist, CLogWindow* _logWindow);
		void SetRect(int _cx, int _cy);
		//void SetImg(int _img); //使ってない
		bool SetSystemImg(CBImgBank* _bimgbank);
		//void AddTrick(trick_tag const* _trick);	//使ってない
		//void ClearTrick(){TrickList.clear();} //使ってない

	bool Main();
	bool Do();
	virtual void Draw(int _dx=0, int _dy=0)=0;	//純粋仮想関数…親クラス側に定義を書く必要が無くなる

	//基礎情報
		int GetActorIndex()const { return ActorIndex; }
		int GetIndex()const{return Index;}
		bool IsPlayer()const{
			return (ActorIndex<PLAYER_NUM);
		}
		
	//戦闘関連
		bool GetAlive()const{ return Alive; }
		int GetVisibleStatus()const{ return VisibleStatus;}
		bool GetVisible()const{ return Visible; }
		void SetVisible(bool _visible){ Visible = _visible;}
		
		int GetAtk()const{ return Atk; }
		int GetDef() { return Def; }
		int GetHp()const { return Hp; }
		int GetMaxHp()const{ return MaxHp; }
		trick_tag const* GetNowTrick()const{ return NowTrick; }

		int GetStatus(int _key);
		void SetStatus(int _key, int _value);

		int Damaged(CActor* _attacker, trick_tag const* _trick);
		bool CheckBarMove();	//死亡確認が済んだらTrue
		void SetTarget(int _target){ Target=_target; }
	
	//ステータス変化技
		void AddStatusChanger(int _kind, int _powerPercent, int _time);	//時間制限付き
		void ChangeValue(int _kind, int _powerPercent);	//永続（ステータスに直接影響）
		void Heal(int _percent);

	//その他
		CRect GetRect()const{return Rect;}

protected:
	//メンバ関数
		virtual void FirstSet2(){};
		virtual void SetExtraImg(CBImgBank* _bImgBank){}	//SetSystemImgから呼び出す。Playerにのみ必要であるためEnemyの時にはこっちを通る
		
		void Draw_Sub(int _dx=0, int _dy=0);


		virtual bool Plan()=0;
		virtual bool Action()=0;

	//基礎情報
		int PLAYER_NUM;
		int ENEMY_NUM;
		int ActorIndex;	//Actor全体の通し番号
		int Index;	//Player,Enemyごとの通し番号

		int Atk;
		int Def;
		double Spd;
		int MaxHp;

	//戦闘関連
		bool Alive;		//生死。死んだら即時に変更
		bool Visible;	//単純に描画されるか否か。処理は継続。点滅表示に使用。
		enum{
			VISIBLE,
			CHANGING,
			INVISIBLE
		}VisibleStatus;	//生→死での描画内容変更を管理
		int OldHp;	//描画用

		std::vector <statusChanger_tag> StatusChangerList;

		enum mode_tag{
			PLAN,
			PREPARE,
			ACTION,
			BEFORE_PLAN,
			MODE_NUM
		}Mode;		//待機や詠唱,行動選択など	//privateにしたいがPlayerの防御処理でアクセスしてる(15/04/26)
		int MaxTimeGauge; //0~XXX(%)

	//技関連
		trick_tag const* NowTrick;		//「const が * よりも前方にあれば、指し示す先の値が書き換えられない。後方にあれば、ポインタ変数自体が書き換えられない」
		int Target;

	//インスタンスのポインタ保持用
		CTextBox** B_TextBox_pp;
		CCmdList* CmdList;
		CLogWindow* LogWindow;

	//グラフィック
		CRect Rect;
		int Dx; int Dy;

private:
		virtual double CalcDamage(double _damage, CActor* _attacker, trick_tag const* _trick){
			return _damage;
		};

		bool TimeGaugeForward();	//内部的には減少
		double TimeGauge;	//MaxTimeGauge→0(%)

		static std::map <std::string, int> BarImg;
		void SetBarImg(CBImgBank* _bImgBank, std::string _key);
		
		static const std::pair <int, int> ATK_UP;
		static const std::pair <int, int> ATK_DOWN;
		static const std::pair <int, int> DEF_UP;
		static const std::pair <int, int> DEF_DOWN;
		static const std::pair <int, int> SPD_UP;
		static const std::pair <int, int> SPD_DOWN;
		static const std::pair <int, int> MAX_STATUSCHANGER_IMGSIZE;
		static int StatusChangerImg[];

		static int HpFontHandle;

		std::map <int, int> Status;	
};


////多重インクルード防止（インクルードガード）//
#endif										////
////////////////////////////////////////////////