#include "TextBox.h"
#include "CmdList.h"
#include "../Field/ItemManager.h"

CTextBox::CTextBox(){
	CONSTRUCTED;

	for (int i=0; i<STOCK_LINE_NUM; i++){ 
		chStock[i][0] = '\0';
	}
	for (int i=0; i<LINE_MAX; i++){ 
		chText[i][0] = '\0';
		chOldText[i][0] = '\0';
		chDrawText[i][0] = '\0';
	}

	Alive = false;
	Visible = false;
	ReturnVisible = true;

	AutoPlay = false;
	AutoPlayMode = PAGE;
	AutoPlaySpeed = DefaultAutoPlaySpeed = -1;

	PosX = PosY = Width = Height = -1;
	LineNum = WordNum = -1;
	FontSize = RubyFontSize = -1;
	Color1 = Color2 = -1;
	WordWidth = -1;
	
	StockLine = -1;

	NowStock = 0;
	NowTarget = 0;
	PageChange = false;

	NewText = -1;
	Showing = false;
	ShowingTime = -1;

	OriginalDir = DOWN;
}

void CTextBox::Init(int _posx, int _posy, int _width, int _height, int _line , int _words, int _fontsize, int _color1, int _color2, int _autoplayspeed, CFieldLog* _logWindow){
	TalkName.Init();
	FieldLog = _logWindow;

	PosX = _posx;
	PosY = _posy;
	Width = _width;
	Height = _height;

	LineNum = between(1, (int)LINE_MAX, _line);
	WordNum = between(1, (int)WORD_MAX, _words);
	FontSize = _fontsize;
	RubyFontSize = 10;

	Color1 = _color1;
	Color2 = _color2;
	AutoPlaySpeed = DefaultAutoPlaySpeed = max(1, _autoplayspeed);
	
	NowStock = 0;
	NowTarget = 0;

	NewText = -1;
	Showing = false;


	Ruby.clear();

	//AddStock�ł̔��p�����I�[�o�[���������邽�߂ɉ���������t����
		char tmp[WORD_MAX]="��";		int tmpnum=0;
		while(++tmpnum<WordNum/2){
			sprintf_s(tmp, "%s��", tmp);
		}
		WordWidth = GetDrawStringWidth(tmp, strlen(tmp));
}

void CTextBox::Term(CCmdList* _cmdlist){
	
	NowStock = 0;
	NowTarget = 0;

	StockLine = -1;
	StockClear();
		
	Showing = false;
	TextClear();
	Alive = false;
	Visible = false;
	AutoPlay = false;
	AutoPlaySpeed = DefaultAutoPlaySpeed;

	Ruby.clear();
	
	//���O�ɉ�b�I���̈��}��
		FieldLog->Add(" ");
		FieldLog->Add("-----------------");
		FieldLog->Add(" ");
		FieldLog->ResetCurrentPos();

	//�g�[�N���x���i���O�\���j�̃��Z�b�g����\��
		TalkName.Clear(true);
		TalkName.Clear(false);
		
	//�w�i�ꖇ�G���[�h�̉���
		_cmdlist->Add("@BackGround_Change(NULL, 0)");

	if (ObjCount!=-1){	//ObjCount��-1����Ȃ����C�x���g�𓥂��TextBox�ɂ���Ă����i��������Ȃ��ꍇ�F�Ⴆ�΃Z�[�u�����Ȃǂ̃V�X�e�����b�Z�[�W�j
		//�ꎞ�I��NONE�ɕς����i��������Ȃ��jEveObj��Effect�����ɖ߂�
			_cmdlist->Add("@Effect_Set(%s, Tmp)", JOKER_NAME);
	}
}


bool CTextBox::Main(CCmdList* _cmdlist, CFlagSet *_flagset){	
	static int sumstrlen=0;
	static int starttime=0;

	if(Alive){
		if(!Showing) {
			if (!AutoPlay) {
				if (CheckHitKeyDown(KEY_INPUT_OK)) NextPage(_cmdlist, _flagset);
				if (TextLineNum()==0) NextPage(_cmdlist, _flagset);
			}else{
				//AutoPlay���[�h
				if (sumstrlen==0){
					starttime = GetNowCount();			sumstrlen=1;//�󔒃y�[�W�ł̃o�O�h�~
					/*1����������̃X�s�[�h�w�莮�̂Ƃ�
					for (int i=0; i<LineNum; i++){
						sumstrlen += strlen(chText[i]);
					}*/
				}

				if ((GetNowCount()-starttime) > AutoPlaySpeed){	//(500+sumstrlen*AutoPlaySpeed)){	//1����������̃X�s�[�h�w��	
					NextPage(_cmdlist, _flagset);
				}
			}
		}else{
			sumstrlen = 0;
		}
	}

	return Alive;
}


bool CTextBox::AddStock(char *String, int dir, const int count){		//�R�����g�s��󔒍s��Load�̒i�K�Ŕr������Ă���
	
	if (!Alive) {
		StockLine = 0;
		ObjCount = count;
		OriginalDir = (direction_tag)dir;
	}

	if (strlen(String) < 256){
		if (StockLine < STOCK_LINE_NUM){

			if (String[0]=='@' && !isdigit(String[1])) {					//�R�}���h�Ȃ�\�����Ȃ��̂�256���ȉ��ł�������Ȃ��s�̎������C�ɂ��Ȃ�				
				strcpy_s(chStock[StockLine], String);
				StockLine++;
				Visible=true;


			////���p�����Ŗ��ߐs�������Ƃ��Ƀe�L�X�g�{�b�N�X���I�[�o�[����o�O���C��=�����`�掞�̃T�C�Y�Ŕ��f
			}else if (GetDrawStringWidth(String, strlen(String)) > WordWidth){			//�����I�[�o�[������ WordWidth��Init()�Œ�`
				char chOverstring[WORD_MAX];
				char chTruestring[WORD_MAX];
				char tmp[WORD_MAX]="";
				
				int tmpnum=0;				//��s���̉����ŋ�؂邽�߂̈ʒu�T��
				while(GetDrawStringWidth(tmp, strlen(tmp)) <= WordWidth){
					tmp[tmpnum]=String[tmpnum];
					tmp[++tmpnum]='\0';
				}

				if (_ismbblead(String[tmpnum-1])) {
					tmpnum--;		//�s�����S�p������1�o�C�g�ڂ������ꍇ�A������������̂�1�o�C�g���炷
					//UNKOWN:�����炪���Ă΂����炪����������[��WRAP�������������������H
				}
				strcpy_s(chOverstring, String+tmpnum);
				strncpy_s(chTruestring, String, tmpnum);

				AddStock(chTruestring);
				if (strlen(chOverstring)>0) AddStock(chOverstring);				//�I�[�o�[��������؂����čēx�ǂݍ���
		

			}else if (mystrchr(String, '[')!=NULL){	//���r����	//'['��'�['�𕶎��R�[�h�̊֌W���F���₪��̂Ŕ��p�������S�p�������̃`�F�b�N���K�v�ɂȂ遨�撣������
				//��̂͂ݏo�����s��������ɂ��Ȃ��Ă͂Ȃ�Ȃ��̂ł́H
				//TODO:�����s�����܂郋�r�͕ʃ��r�ƈ����̂ł͂ݏo�����������OK�B�������͂ݏo���������ɉ��s�ӏ��ł̃��r���������͕K�v�B

				ruby_tag tmpruby;
				char tmp[WORD_MAX];		mystrcpy(tmp, String);
				char chrubynum[3];		int rubynum[2];
				char *p, *q, *r;

				if (Ruby.size() > 99){
					ErrorDx("Error->AddStock_Ruby->MAX[100] Over :%s", __FILE__, __LINE__, String);
					goto normal;
				}	
				rubynum[1] = Ruby.size()%10;
				rubynum[0] = (Ruby.size()-rubynum[1])/10;
				sprintf_s(chrubynum, "%d%d", rubynum[0], rubynum[1]);

				p = mystrchr(tmp, '[');	
				//'['�̂������ꏊ��'@'�ƃ��r�ԍ���ڈ�Ƃ��đ}��
					char tmp2[WORD_MAX];		mystrcpy(tmp2, ++p);
					*(--p) = '@';	p+=3;
					*p = '\0';		p-=2;
					*p = chrubynum[0];	p++;
					*p = chrubynum[1];
					mystrcat(tmp, tmp2);p++;
				if ((q=mystrchr(p, '@'))==NULL){
					ErrorDx("Error->AddStock_Ruby->Not Find '@' :%s", __FILE__, __LINE__, p);
					goto normal;
				}	
				*q = '\0';	q++;
				if ((r=mystrchr(q, ']'))==NULL){
					ErrorDx("Error->AddStock_Ruby->Not Find ']' :%s", __FILE__, __LINE__, q);
					goto normal;
				}
				*r = '\0';	r++;
					
				/*DebugDx("p:%s", p);	//���r�t����P��
				DebugDx("q:%s", q);	//���r
				DebugDx("r:%s", r);	//']'���r�ȍ~�̕�����	*/

				//���r�𕪗�����Ruby�ɓo�^
					mystrcpy(tmpruby.Word, p);
					mystrcpy(tmpruby.Ruby, q);
					mystrcpy(tmpruby.chNum, chrubynum);				//���s�����ʃ��r�ɑΉ�����ɂ́A�����s���ŉ��ԖڂɌ��������̂��܂ŋL�^����΂���
					Ruby.push_back(tmpruby);

				//���r�������ڈ�@����ꂽ�c��̃e�L�X�g�𕁒ʂ�AddStock
					mystrcat(tmp, r);
					AddStock(tmp);
	
			/*
			}else if (mystrlen(String) > WordNum){			//�����I�[�o�[�������i���ۂ͕`��T�C�Y�ŏ�������悤�ɏ�L�ɏ������������߂�������g�����Ƃ͂Ȃ��Ǝv����j
				char chOverstring[WORD_MAX];
				char chTruestring[WORD_MAX];
				
				int d = (_ismbblead(String[WordNum-1])? -1:0);	//�s�����S�p������1�o�C�g�ڂ������ꍇ�A������������̂�1�o�C�g���炷
				strcpy_s(chOverstring, String+WordNum+d);
				strncpy_s(chTruestring, String, WordNum+d);
				
				//ebugDx("Debug->�����I�[�o�[:%s", chOverstring);

				AddStock(chTruestring);
				AddStock(chOverstring);				//�I�[�o�[��������؂����čēx�ǂݍ���
			*/
				
			}else{
normal:
				strcpy_s(chStock[StockLine], String);
				StockLine++;
				Visible=true;
			}

			if (!Alive) FieldLog->MemorizeCurrentPos();
			Alive = true;
			

			return true;
		}
	}
	ErrorDx("Error->AddStock:%s", String);
	return false;
}

bool CTextBox::AddStock(char **String, int dir, const int count){
	if (String==NULL) return false;
	
	for (int i=0; !mystrcmp(String[i], EOP)&&strlen(String[i]); i++){
		if (!AddStock(String[i], dir, count)) return false;
	}
	return true;
}

void CTextBox::Draw(bool _showingstop){

	if(!Alive || !Visible) return;

	int oldfontsize = GetFontSize();
	
	//�{�b�N�X
		DrawBox(PosX, PosY, PosX+Width, PosY+Height, GetColor(30, 20, 80), true);
		DrawBox(PosX+5, PosY+5, PosX+Width-5, PosY+Height-5, GRAY, false);

	//�e�L�X�g
		//�\���A�j���[�V��������
		Draw_Animation(_showingstop);	
	
		//@���r��Ruby�Əƍ�,�\��
		Draw_Ruby();

		//�e�L�X�g�`��
		SetFontSize(FontSize);
		for (int i=0; i<LineNum; i++){
			if (Color2!=-1) DrawString(PosX+Width/2 - WordNum*(FontSize+1)/4 + 1, PosY+Height/2 + LINE_SPACE/2 - LineNum*(FontSize+LINE_SPACE)/2 + (FontSize+LINE_SPACE)*i+1, chDrawText[i], Color2);
							DrawString(PosX+Width/2 - WordNum*(FontSize+1)/4	, PosY+Height/2 + LINE_SPACE/2 - LineNum*(FontSize+LINE_SPACE)/2 + (FontSize+LINE_SPACE)*i	,  chDrawText[i], Color1);
		}

		//����������Ƃ��̃}�[�N�\��
		if (!Showing && !AutoPlay && ReturnVisible) {
			DrawString(PosX+Width/2-10, PosY+Height-10 + (GetNowCount()/100)%5, "��", WHITE);
		}
	
	//TextName�̕`��
		if (TalkName.GetVisible()) TalkName.Draw(PosX, PosX+Width, PosY);	

	SetFontSize(oldfontsize);
}

void CTextBox::Draw_Animation(bool _showingstop){
	
	if (Showing){	//�e�L�X�g�A�j���[�V������
		if (!_showingstop) ShowingTime++;
	}else if (NewText != -1 && !Showing){	//�e�L�X�g�A�j���[�V�����O
		ShowingTime = 0;
		Showing = true;
	}else if (NewText == -1 && !Showing){	//�e�L�X�g�A�j���[�V�����ς�
		for (int i = 0; i<LineNum; i++){
			strcpy_s(chDrawText[i], chText[i]);
		}
	}			
			
	if (Showing && !_showingstop){
		int NowShow = (int)(SHOWING_SPEED*ShowingTime/60)*2;
		int Sumstrlen = 0;

		for (int i = 0; i<NewText; i++){
			strcpy_s(chDrawText[i], chText[i]);
		}

		for (int i = NewText; i<LineNum; i++){
			if (NowShow-Sumstrlen <= 0) {
				strcpy_s(chDrawText[i], "");
			}else{
				strncpy_s(chDrawText[i], chText[i], min(NowShow-Sumstrlen, mystrlen(chText[i])));	//TODO:���r�p@�͕\�����Ȃ��̂ł����ŃA�j���[�V����������Ȃ��悤�ɂ���R�[�h�i�܂������ăi�C�j��//min(NowShow-Sumstrlen+(chText[i][min(NowShow-Sumstrlen, mystrlen(chText[i]))]=='@'?1:0), mystrlen(chText[i]))
			}
			Sumstrlen += strlen(chText[i]);
		} 
		
		if (NowShow > Sumstrlen) {
			Showing = false;
			NewText = -1;
		}
	}
}

void CTextBox::Draw_Ruby(){
	if (!Ruby.empty()){						
		for (int i=0; i<LineNum; i++){
			char *p;	char chrubynum[3];		
			SetFontSize(FontSize);

			if ((p=mystrchr(chDrawText[i],'@')) != NULL){
				if (*(++p)!='\0'){
					chrubynum[0]=*p;
				}else{
					*(--p)='\0';
					continue;
				}
				if (*(++p)!='\0'){
					chrubynum[1]=*p;
					chrubynum[2]='\0';
				}else{
					*(p-2)='\0';
					continue;
				}

				//"@OO"�������Ȃ�
					p++;
					*(p-3)='\0';
					char tmp[WORD_MAX];
					sprintf_s(tmp, chDrawText[i]);
					mystrcat(chDrawText[i], p);
					p-=3;

				for (unsigned int j=0; j<Ruby.size(); j++){
					if (mystrcmp(chrubynum, Ruby[j].chNum) && mystrcmp(p, Ruby[j].Word, 'l')){
						int left = GetDrawStringWidth(tmp, strlen(tmp));
							left += GetDrawStringWidth(Ruby[j].Word, strlen(Ruby[j].Word))/2;
							left += PosX+Width/2 - WordNum*(FontSize+1)/4;
						int top = PosY+Height/2  + LINE_SPACE/2 - LineNum*(FontSize+LINE_SPACE)/2 + (FontSize+LINE_SPACE)*i;
							top -= RubyFontSize;
						SetFontSize(RubyFontSize);
			
						if (Color2!=-1) DrawCenterString(left+1, top+1, Ruby[j].Ruby, Color2);
										DrawCenterString(left  , top  , Ruby[j].Ruby, Color1);
						break;
					}

					if (!Showing && j==Ruby.size()-1)	ErrorDx("Error->Unexpected '@' was found. Check ruby.:", chDrawText[i]);
					//��Word�\���r���͈�v���Ȃ����߃G���[���͂�������
				}
					
				//��s�ɕ�������ꍇ�̂��߂ɂ��̍s���ă`�F�b�N
				i--;
			}
		}
	}
}


void CTextBox::NextPage(CCmdList* _cmdlist, CFlagSet *_flagset){
	
	if (!Alive) return;

	NowTarget = 0;
	NewText = -1;

	if (NowStock==StockLine){	//�X�g�b�N�̍Ō�܂ŕ\�������Ȃ�\���I��

		Term(_cmdlist);	//Terminate
	
	}else{
			
		if (TextLineNum()==0){
			//��s�ڂ��炷�ׂĖ��߂�
			PageChange = false;
			NewText = 0;
			for (NowTarget = 0; NowTarget<LineNum; NowTarget++){
				if (! NextLine(_cmdlist, _flagset) ) {
					PageChange = true;
					break;
				}
			}

		}else if (PageChange){
			//�y�[�W���Z�b�g������A��s�ڂ��炷�ׂĖ��߂�
			
			PageChange = false;
			NewText = 0;
			for (int i = 0; i < LineNum; i++){
				strcpy_s(chText[i], "");
			}
			for (NowTarget = 0; NowTarget<LineNum; NowTarget++){
				if (! NextLine(_cmdlist, _flagset) ) {
					PageChange = true;
					break;
				}
			}

		}else if (TextLineNum()<LineNum){
			//�r���s����ŏI�s�܂œǂݍ���
			
			PageChange = false;
			NewText = TextLineNum();
			for (NowTarget=TextLineNum(); NowTarget<LineNum; NowTarget++){
				if (! NextLine(_cmdlist, _flagset) ) {
					PageChange = true;
					break;
				}
			};

		}else{
			//�J��グ�čŏI�s�̂ݒǉ��ǂݍ���
			
			for (int i = 0; i < LineNum-1; i++){
				strcpy_s(chText[i], chText[i+1]);
			}
			NowTarget = LineNum-1;
			PageChange = false;

			if (! NextLine(_cmdlist, _flagset) ) {
				//���̂Ƃ��́A�y�[�W���Z�b�g���āA��s�ڂ��炷�ׂĖ��߂�
				if (!Alive) return;
				NewText = 0;
				for (int i = 0; i < LineNum; i++){
					strcpy_s(chText[i], "");
				}
				for (NowTarget = 0; NowTarget<LineNum; NowTarget++){
					if (! NextLine(_cmdlist, _flagset) ) {
						PageChange = true;
						break;
					}
				}
			}else{
				NewText = NowTarget;
			}
		}

		if (Alive) FieldLog->MemorizeCurrentPos();
	}


}

void CTextBox::SetAutoPlay(bool _autoplay, int _autoplayspeed){
	AutoPlay = _autoplay;
	if (AutoPlay){
		if (_autoplayspeed>0){
			AutoPlaySpeed = _autoplayspeed;
		}else{
			AutoPlaySpeed = DefaultAutoPlaySpeed;
		}
	}
}

void CTextBox::LogTalkName() {
	FieldLog->InsertToMemoPos(TalkName.GetNowName().c_str());
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
bool CTextBox::NextLine(CCmdList *_cmdlist, CFlagSet *_flagset){
	
	//�A�N�V�����R�}���h�Ƒ��������Ƃ��́ANowStock���e�L�X�g�ɂ�����܂ňړ��A�e�L�X�g���擾����
	//�g�[�N�R�}���h�Ƒ��������Ƃ���NowStock���e�L�X�g�̏ꏊ�܂ŃX�L�b�v�����āA""��������False��Ԃ��i�����ł͂Ȃ��ANowStock���e�L�X�g�ɂ�����܂œ������ׂ��H�j

	while(1){
		if (!Alive) return false;
		if (NowStock>=StockLine) {NowStock=StockLine; return false;}
	
		if(chStock[NowStock][0]=='@' && !isdigit(chStock[NowStock][1])){	//�R�}���h


			if (mystrcmp(chStock[NowStock], 'p', 3, "@NextPage", "@Stop", "@Quit") || mystrcmp(chStock[NowStock], 'm', 3, "_IF", "_ENDIF", "_CASE")){		//�����Ƀg�[�N�R�}���h��ǉ�
				//�g�[�N�R�}���h�̏���
					if (mystrcmp(chStock[NowStock], "@NextPage")){
						while (NowTarget<LineNum){
							strcpy_s(chText[NowTarget], "");
							NowTarget++;
						}
						NowStock++;
//						FieldLog->MemorizeCurrentPos();
						return false;
					
					}else if (mystrcmp(chStock[NowStock], "@Stop")){

						if(NowTarget==0 || TextLineNum()==LineNum){
							NowStock++;
						}else if(NowTarget==LineNum-1){
							strcpy_s(chText[NowTarget], "");	
							NowStock++;						
//							FieldLog->MemorizeCurrentPos();
							return true;
						}else{
							strcpy_s(chText[NowTarget], "");
//							FieldLog->MemorizeCurrentPos();
							return true;
						}

					}else if (mystrcmp(chStock[NowStock], "@Quit")){
						Term(_cmdlist);
						return false;

					}else{
						//���̑��̃g�[�N�R�}���h�̏���
						Solve(chStock[NowStock], _flagset);		//���̕��͂̂���͂��̂Ƃ����T������NowStock�𑗂�
						
						if (NowStock==StockLine && (NowTarget==0 || TextLineNum()==3)) {	//UNKNOWN:�܂��o�O�c���Ă邩��  ������Ȃ񌾂��Ă��c
							Term(_cmdlist);			//return�̂��ƌĂяo����[NextPage]�̕��ŏ����������̂ł߂�����o�O�N���肻���B�|��(��)
							return false;
						}
					}

			}else{
				//�A�N�V�����R�}���h
				_cmdlist->Add(chStock[NowStock]);
				NowStock++;
				if (NowStock==StockLine && NowTarget==0) {
					Term(_cmdlist);			//return�̂��ƌĂяo����[NextPage]�̕��ŏ����������̂ł߂�����o�O�N���肻���B�|��(��)
					return false;
				}
			}
				
		}else{								//��ʃe�L�X�g
			strcpy_s(chText[NowTarget], chStock[NowStock]);
			
			FieldLog->Add("  %s", chStock[NowStock]);	//���O�ɋL�^

			NowStock++;	
			return true;
		}
	}
}

bool CTextBox::Solve(const char* string, CFlagSet *_flagset){
	
	bool _return = true;
	const int argnum = 3;

	char** command = new char*[2];	command[0] = new char[256];	command[1] = new char[256];
	char** arg = new char*[argnum];
	
	ArgCut(string, command, arg, argnum);

	
		char stringcase[256];
			strcpy_s(stringcase, command[0]);
			strcat_s(stringcase, _IFCASE);
			if ((mystrcmp(command[1],IFBEGIN) && arg[0]!=NULL) || (mystrcmp(command[1],IFCASE) && arg[1]!=NULL)){
				strcat_s(stringcase, "(");
				strcat_s(stringcase, arg[0]);
				strcat_s(stringcase, ",");
			}

		char stringend[256];
			strcpy_s(stringend, command[0]);
			strcat_s(stringend, _IFEND);
			if ((mystrcmp(command[1],IFBEGIN) && arg[0]!=NULL) || (mystrcmp(command[1],IFCASE) && arg[1]!=NULL)){
				strcat_s(stringend, "(");
				strcat_s(stringend, arg[0]);
				strcat_s(stringend, ")");
			}
	

	if (mystrcmp(command[1], IFBEGIN)){

		for (int i=NowStock; i<StockLine; i++){
			if (mystrcmp(chStock[i], stringcase, 'l')){					
				long int num;
				ArgCut(chStock[i], command, arg, 3);
				
				if (mystrcmp(command[0], "@COUNT")){
					if (!mystrtol(arg[0], &num)){
						if (mystrcmp(arg[0], 'p', 3, "ELSE", "Else", "else")) {
							num = -1;
						}else{
							ErrorDx("Error->Could not change argument type->%s", __FILE__, __LINE__, chStock[i]);
							continue;
						}
					}else if(num<-1){
						ErrorDx("Error->You can't use arg[num]<-1 for COUNT:%s", arg[0]);
					}

					if (num == ObjCount || num == -1){ 
						NowStock = i+1;		//_CASE��Num����v�����ꍇ�A���̎��̍s�ɕW�������킹��NextLine�̃��[�v�ɕԂ�
						break;
					}
					
				}else if (mystrcmp(command[0], "@FLAG")){
					if (arg[1]==NULL) {
						ErrorDx("@FRAG_CASE needs 2 arg:[flagname,num]");
					}else{
						if (!mystrtol(arg[1], &num)){
							if (mystrcmp(arg[1], 'p', 3, "ELSE", "Else", "else")) {
								num = -1;
							}else{
								ErrorDx("Error->Could not change argument type->%s", __FILE__, __LINE__, chStock[i]);
								continue;
							}
						}else if(num<-1){
							ErrorDx("Error->You can't use arg[num]<-1 for FLAG:%s", arg[1]);
						}

						if (num == _flagset->GetFlagNum(arg[0]) || num == -1){
							NowStock = i+1;		//_CASE��Num����v�����ꍇ�A���̎��̍s�ɕW�������킹��NextLine�̃��[�v�ɕԂ�
							break;
						}
					}

				} else if (mystrcmp(command[0], "@ITEM")) {
					if (arg[1] == NULL) {
						ErrorDx("@ITEM_CASE needs 2 arg:[ItemName,num]");
					} else {
						if (!mystrtol(arg[1], &num)) {
							if (mystrcmp(arg[1], 'p', 3, "ELSE", "Else", "else")) {
								num = -1;
							} else {
								ERRORDX("Could not change argument type->%s", chStock[i]);
								continue;
							}
						} else if (num<-1) {
							ERRORDX("You can't use arg[num]<-1 for ITEM:%s", arg[1]);
						}

						if (num == CItemManager::GetInstance()->GetPlayerItemNum(arg[0]) || num == -1) {
							NowStock = i + 1;		//_CASE��Num����v�����ꍇ�A���̎��̍s�ɕW�������킹��NextLine�̃��[�v�ɕԂ�
							break;
						}
					}
					
				}else if (mystrcmp(command[0], "@DIR")){
					if (!mystrtol(arg[0], &num)){
						if (mystrcmp(arg[0], 'p', 3, "ELSE", "Else", "else")) {
							num = -1;
						}else{
							num = sys::StrtoDir(arg[0]);
						}
					}
					if (num == OriginalDir || num == -1){
						NowStock = i+1;		//_CASE��Num����v�����ꍇ�A���̎��̍s�ɕW�������킹��NextLine�̃��[�v�ɕԂ�
						break;
					}

				}else{
					ErrorDx("Error->Unregisterd command:%s", __FILE__, __LINE__, string);
					NowStock++;
					_return = false;
				}


			}else if (mystrcmp(chStock[i], stringend, 'l')){
				NowStock = i+1;			//��v����CASE��������O��END�����������ꍇ�A���̎��̍s�ɕW�������킹��NextLine�̃��[�v�ɕԂ�
				break;
			}

			if (i==StockLine-1){	//�Ō�܂ōs���Ă��`_END��������Ȃ�������
				ErrorDx("Error->TextBox.Solve->Not Found CloseCommand:%s", __FILE__, __LINE__, stringend);		
				NowStock++;
				_return = false;
				break;
			}
		}
		

	}else if(mystrcmp(command[1], IFCASE)){
		
		for (int i=NowStock; i<StockLine; i++){
			if (mystrcmp(chStock[i], stringend, 'l')){
				NowStock = i+1;
				break;
			}

			if (i==StockLine-1){	//�Ō�܂ōs���Ă��`_END��������Ȃ�������
				ErrorDx("Error->TextBox.Solve->Not Found CloseCommand:%s", __FILE__, __LINE__, stringend);		
				NowStock++;
				_return = false;
				break;
			}
		}

	}else if(mystrcmp(command[1], IFEND)){
		NowStock++;
		_return = false;

	}else{
		ErrorDx("Error->Unregisterd command:%s", __FILE__, __LINE__, string);
		NowStock++;
	}

	//�z���delete�����܂������Ȃ��B�R�����g���ŃG���[�͂łȂ��Ȃ������A�K�؂�delete�ł��Ă���̂��͋^��B�������Ȃ��ꍇ��new���g��Ȃ����@�ɐ؂�ւ���ׂ���140121
	delete [] command[0];
	delete [] command[1];
	delete [] command;
	//for (int i=0; i<argnum; i++){
	//	DebugD("ok01.5:%d", i);
	////	delete [] arg[i];
	//}
	delete [] arg;

	if (NowStock>=StockLine && Alive) NowStock = StockLine;
	return _return;
}

void CTextBox::ArgCut(const char* _string, char** &command, char** &arg, int _argnum){

	char *cntx;		//strtok_s�p�̎G�p
	char* string_copy = new char[strlen(_string)+1];		//delete���ĂȂ����Ԓl�ɗ��p���Ă���炵�����̃v���V�[�W����delete����ƃG���[���o��  �������Ă�
	mystrcpy(string_copy, _string);
	char* tmp;

	tmp = strtok_s(string_copy, "_ ", &cntx );	//ex) @FLAG, @COUNT
	mystrcpy(command[0], tmp);
	tmp = strtok_s(NULL, "( ", &cntx );			//ex) IF_BEGIN, IF_ENG
	mystrcpy(command[1], tmp);
	
	for (int i=0; i<_argnum; i++){
		arg[i] = strtok_s( NULL, CMD_SEPARATOR , &cntx);

		if( arg[i] == NULL && i == 0){
			break;
		}else if(arg[i] == NULL){
			char *cntx;
			strtok_s(arg[i-1], ") " , &cntx);
			break;
		}else if(i == _argnum-1){
			char *cntx;
			strtok_s(arg[i], ") " , &cntx);
			break;
		}
	}

	delete [] string_copy;
}


int CTextBox::TextLineNum(){
	int line = 0;

	for (int i=0; i<LineNum; i++){ 
		if (strlen(chText[i])) line = i+1;
	}
	return line;
}

void CTextBox::StockClear(){
	for (int i=0; i<STOCK_LINE_NUM; i++){ 
		strcpy_s(chStock[i], "");
	}
};
void CTextBox::TextClear(){
	for (int i=0; i<LineNum; i++){ 
		strcpy_s(chText[i], "");
	}
};


