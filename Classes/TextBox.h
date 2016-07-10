////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#ifndef TEXTBOX_H							////
#define TEXTBOX_H							////
////////////////////////////////////////////////

#include "TalkName.h"
#include "../Battle/LogWindow.h"

	//static const int TextShowMode = 0;	//0�c�s���� 1�c�y�[�W����


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

	//TextName�i�b����̖��O��TextBox�̏�ɕ\���j
		CTalkName TalkName;
		void LogTalkName();

protected:
	//�萔
		enum{
			STOCK_LINE_NUM = 1000,
			LINE_MAX = 20,		//�s���ő�l		 Init�Œ��������
			WORD_MAX = 256,		//��s�̕������ő�l Init�Œ��������
			LINE_SPACE = 10,	//HACK:INIT�ɒǉ����ׂ����e
			SHOWING_SPEED = 80	//1�b�����艽����[�S�p]
		};


	//�����o�֐�
		void StockClear();	//�X�g�b�N��S����
		void TextClear();	//�\���e�L�X�g�S����

		void Draw_Animation(bool _showingstop);
		void Draw_Ruby();

		bool NextLine(CCmdList *_cmdlist, CFlagSet *_flagset);
		bool Solve(const char* string, CFlagSet *_flagset);
		void ArgCut(const char* _string, char** &command, char** &arg, int _argnum);

	//�����o�ϐ�
		char chStock[STOCK_LINE_NUM][WORD_MAX];
		char chText[LINE_MAX][WORD_MAX];
		bool Alive;
		bool Visible;	//�R�}���h����ύX�\
		bool ReturnVisible;	//Walk�n�̂Ƃ��ɑ���

		///AutoPlay�֌W////////////////////////////////////////
		bool AutoPlay;	//�R�}���h����ύX�\�i�f�t�H���gfalse�j
			int AutoPlaySpeed;	//�R�}���h����ύX�\�i�f�t�H���g�ł�DefaultAutoPlaySpeed�j
			int DefaultAutoPlaySpeed;	//Init����Field.cpp����l���󂯎�菉���������
			enum{
				PAGE
			}AutoPlayMode;
		////////////////////////////////////////////////////////

		char chOldText[LINE_MAX][WORD_MAX];
		char chDrawText[LINE_MAX][WORD_MAX];

		int PosX, PosY, Width, Height;			//����̍��W�A�����c��
		int LineNum, WordNum;	//WordNum�̒P�ʂ̓o�C�g LineNum�͍s��
		int FontSize;
		int RubyFontSize;
		int Color1, Color2;	//Color2�͉e
		int WordWidth;		//WordNum��FontSize�Ɋ�Â��ĉ������Z�o

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

		//���O�N���X
			CFieldLog* FieldLog;
};


////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#endif								t////////////