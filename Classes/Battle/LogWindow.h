////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
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
	void Clear();	//�\���e�L�X�g�S����
	void Term();	//Terminate ���������

	bool Add(const char *_format, ...);
	bool Add(char **_newTextArray);

	bool Main();	
	void SetVisible(bool _visible){Visible = _visible;}
	
protected:
	//�萔
		enum{
			WORD_MAX = 255,		//��s�̕������ő�l Init�Œ��������
		};

	//�����o�ϐ�
		bool Initialized;	//�����ς݂��ۂ�

		char** Text;	//���O���b�Z�[�W�{��

		int PosX, PosY, Width, Height;			//����̍��W�A�����c��
		int StockLineNum, LineNum, WordNum;	//WordNum�̒P�ʂ̓o�C�g LineNum�͕\���s���AStockLineNum�͗����܂߂��ő�s��

		int LineSpace;	//�s��
		int	BoxSpace;
			
		int FontSize;
		int FontHandle;
		int FontColorMain, FontColorSub;	//Color2�͉e
		int BoxColor;

		int WordWidth;		//WordNum��FontSize�Ɋ�Â��ĉ������Z�o

		int NextLine;	//���ɏ�����������ׂ�Text�̔z��C���f�b�N�X
		int BackLine;	//�擪�s�̔z��C���f�b�N�X	
		bool Visible;	//�`�悳��邩

		/*
		int StockLine;		//���s�ڂ܂Ńf�[�^���X�g�b�N����Ă��邩				1~STOCK_LINE_NUM	��F0	//AddText�ŏ���������̂�NoProblem
		int TextLineNum();		//���A���s�ڂ܂ŕ\���p�e�L�X�g�����邩		1~LineNum	��F0
		
		int NowStock;	//���A�X�g�b�N�̉��s�ڂɂ��邩	0~	��F-1	(TextBox�O�ɂ����Ă͎����ׂ�s�͂ǂ���)
		int NowTarget;	//���A�e�L�X�g�{�b�N�X�̉��s�ڂɂ��ď������Ă���̂� 0~	��F-1
		bool PageChange;	//���Ƀy�[�W���Z�b�g���邩�ۂ�

		////�e�L�X�g�̃A�j���[�V�����\���֌W//////////////
			int NewText;		//chText�ɐV���ɒǉ����ꂽ�s -1����ω��Ȃ� 0~����ォ�牽�s�ڈȍ~���ǉ����ꂽ���i�󔒂��J�E���g�j
			bool Showing;		//�e�L�X�g�A�j���[�V���������ݐi�s���FTrue �����FFalse
			int ShowingTime; 

		////AddText�̂Ƃ��ɕύX
			int ObjCount;
			direction_tag OriginalDir;

		//���r�p
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
	int PosXFull, WidthFull;			//�t���T�C�Y���[�h�̂Ƃ�
	bool FullMode;		//���O�E�B���h�E�̑傫����؂�ւ���X�C�b�`

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
	int PosMemo;	//��b�̋�؂�ʒu���L��
	int PosMemoOld; //��b�̋�؂�ʒu���L�����ă��O�Ƀg�[�N���x����}��
};


////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#endif										////
////////////////////////////////////////////////