////多重インクルード防止（インクルードガード）//
#ifndef TEXTBOX_H							////
#define TEXTBOX_H							////
////////////////////////////////////////////////

#include "TalkName.h"
#include "../Battle/LogWindow.h"

	//static const int TextShowMode = 0;	//0…行送り 1…ページ送り


class CCmdList;
class CLogWindow;

class CTextBox{
public:
	CTextBox();
	~CTextBox(){
		DESTRUCTED;
	};

	void Init(int _posx, int _posy, int _width, int _height, int _line , int _words, int _fontsize, int _color1, int _color2, int _autoplayspeed, CFieldLog* _logWindow);
	virtual void Term(CCmdList* _cmdlist);	//Terminate

	bool Main(CCmdList* _cmdlist, CFlagSet *_flagset);	
	virtual void Draw(bool _showingstop=false);
	
	bool AddStock(char *String, int dir=DOWN, int count=-1);
	bool AddStock(char **String, int dir=DOWN, int count=-1);
	void NextPage(CCmdList* _cmdlist, CFlagSet *_flagset);

	direction_tag GetOriginalDir()const{ return OriginalDir; }

	void SetVisible(bool _visible){Visible = _visible;}
	void SetReturnVisible(bool _visible){ReturnVisible = _visible;}
	void SetAutoPlay(bool _autoplay, int _autoplayspeed = NULL);

	//TextName（話し手の名前をTextBoxの上に表示）
		CTalkName TalkName;
		void LogTalkName();

protected:
	//定数
		enum{
			STOCK_LINE_NUM = 1000,
			LINE_MAX = 20,		//行数最大値		 Initで調整される
			WORD_MAX = 256,		//一行の文字数最大値 Initで調整される
			LINE_SPACE = 10,	//HACK:INITに追加すべき内容
			SHOWING_SPEED = 80	//1秒当たり何文字[全角]
		};


	//メンバ関数
		void StockClear();	//ストックを全消去
		void TextClear();	//表示テキスト全消去

		void Draw_Animation(bool _showingstop);
		void Draw_Ruby();

		bool NextLine(CCmdList *_cmdlist, CFlagSet *_flagset);
		bool Solve(const char* string, CFlagSet *_flagset);
		void ArgCut(const char* _string, char** &command, char** &arg, int _argnum);

	//メンバ変数
		char chStock[STOCK_LINE_NUM][WORD_MAX];
		char chText[LINE_MAX][WORD_MAX];
		bool Alive;
		bool Visible;	//コマンドから変更可能
		bool ReturnVisible;	//Walk系のときに操作

		///AutoPlay関係////////////////////////////////////////
		bool AutoPlay;	//コマンドから変更可能（デフォルトfalse）
			int AutoPlaySpeed;	//コマンドから変更可能（デフォルトではDefaultAutoPlaySpeed）
			int DefaultAutoPlaySpeed;	//Init時にField.cppから値を受け取り初期化される
			enum{
				PAGE
			}AutoPlayMode;
		////////////////////////////////////////////////////////

		char chOldText[LINE_MAX][WORD_MAX];
		char chDrawText[LINE_MAX][WORD_MAX];

		int PosX, PosY, Width, Height;			//左上の座標、横幅縦幅
		int LineNum, WordNum;	//WordNumの単位はバイト LineNumは行数
		int FontSize;
		int RubyFontSize;
		int Color1, Color2;	//Color2は影
		int WordWidth;		//WordNumとFontSizeに基づいて横幅を算出

		int StockLine;		//何行目までデータがストックされているか				1~STOCK_LINE_NUM	空：0	//AddTextで初期化するのでNoProblem
		int TextLineNum();		//今、何行目まで表示用テキストがあるか		1~LineNum	空：0
		
		int NowStock;	//今、ストックの何行目にいるか	0~	空：-1	(TextBox外においては次調べる行はどこか)
		int NowTarget;	//今、テキストボックスの何行目について処理しているのか 0~	空：-1
		bool PageChange;	//次にページリセットするか否か

		////テキストのアニメーション表示関係//////////////
			int NewText;		//chTextに新たに追加された行 -1･･･変化なし 0~･･･上から何行目以降が追加されたか（空白もカウント）
			bool Showing;		//テキストアニメーションが現在進行中：True 完了：False
			int ShowingTime; 

		////AddTextのときに変更
			int ObjCount;
			direction_tag OriginalDir;

		//ルビ用
			struct ruby_tag{
				char Word[32];
				char Ruby[32];
				char chNum[3];	//00~99
			};
			std::vector <ruby_tag> Ruby;

		//ログクラス
			CFieldLog* FieldLog;
};


////多重インクルード防止（インクルードガード）//
#endif								t////////////