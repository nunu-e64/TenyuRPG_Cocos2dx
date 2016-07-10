////多重インクルード防止（インクルードガード）//
#ifndef LOGWINDOW_H							////
#define LOGWINDOW_H							////
////////////////////////////////////////////////


class CCmdList;
class CBImgBank;

class CLogWindow{
public:
	CLogWindow();
	~CLogWindow(){
		Term();
		DESTRUCTED;
	};

	virtual void Init(int _posx, int _posy, int _width, int _height, int _boxColor, int _stockLine , int _fontSize, int _fontColorMain, int _fontColorSub);
	void Clear();	//表示テキスト全消去
	void Term();	//Terminate メモリ解放

	bool Add(const char *_format, ...);
	bool Add(char **_newTextArray);

	bool Main();	
	void SetVisible(bool _visible){Visible = _visible;}
	
protected:
	//定数
		enum{
			WORD_MAX = 255,		//一行の文字数最大値 Initで調整される
		};

	//メンバ変数
		bool Initialized;	//初期済みか否か

		char** Text;	//ログメッセージ本体

		int PosX, PosY, Width, Height;			//左上の座標、横幅縦幅
		int StockLineNum, LineNum, WordNum;	//WordNumの単位はバイト LineNumは表示行数、StockLineNumは履歴含めた最大行数

		int LineSpace;	//行間
		int	BoxSpace;
			
		int FontSize;
		int FontHandle;
		int FontColorMain, FontColorSub;	//Color2は影
		int BoxColor;

		int WordWidth;		//WordNumとFontSizeに基づいて横幅を算出

		int NextLine;	//次に書き換えられるべきTextの配列インデックス
		int BackLine;	//先頭行の配列インデックス	
		bool Visible;	//描画されるか

		/*
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
			*/
};

class CBattleLog: public CLogWindow {
public:
	void Init(int _smallposx, int _posy, int _smallwidth, int _height, int _boxColor, int _stockLine, int _fontSize, int _fontColorMain, int _fontColorSub, CBImgBank* _bImgBank);
	void Draw();
	void Clear();
	bool Main();

	void SetWindowMode(bool _fullMode) {
		FullMode = _fullMode;
	}

private:
	int PosXFull, WidthFull;			//フルサイズモードのとき
	bool FullMode;		//ログウィンドウの大きさを切り替えるスイッチ

	int ButtonImg[2];
	CVector ButtonImgSize;
};

class CFieldLog: public CLogWindow {
public:
	CFieldLog();

	void Draw();
	bool Main();
	void MemorizeCurrentPos();
	void ResetCurrentPos();
	void InsertToMemoPos(const char* _string);

private:
	int PosMemo;	//会話の区切り位置を記憶
	int PosMemoOld; //会話の区切り位置を記憶してログにトークラベルを挿入
};


////多重インクルード防止（インクルードガード）//
#endif										////
////////////////////////////////////////////////