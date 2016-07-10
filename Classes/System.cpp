
#include "Main.h"

struct objkind_tag objkind_tag;
struct target_tag target_tag;

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	#ifdef _DEBUG
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);  //new��delete���Ă��Ȃ��������o
	#endif

	
	SetMainWindowText(GAME_TITLE);
	ChangeWindowMode( TRUE );			// �E�C���h�E���[�h�ɕύX
	
	if( DxLib_Init() == -1 ) return -1 ; // �c�w���C�u����������
	
	SetDrawScreen( DX_SCREEN_BACK ); //�`������ʗ���
	SetTransColor(255, 0, 255);	//���ߐF�w��
	SetMouseDispFlag( false );	//�}�E�X�J�[�\����\�����Ȃ�
		
	myLogf("MyLog_Printed", "--------START------------------------------------------------------\n");
	myLog("--------START------------------------------------------------------\n");

		CMain* Main = CMain::GetInstance();		//����ȉ������ׂ�CMain::Main�Ƃ����ÓI�����o�֐��ɂ���̂�����
		if (Main->Init()){
			while( BasicLoop() ){
		
				if(!Main->GameLoop()) break;
			
			}
		}

	DxLib_End(); // DX���C�u�����I������

	myLogf("MyLog_Printed", "----FINISH-----------------------------------------------------");	
	myLog("----FINISH-----------------------------------------------------");

    return 0;
}  

direction_tag sys::TurnDir(int _dir, signed int _rightspin){		

	int _turneddir;

	switch (_dir){
	case UP:
		_turneddir = 0;		break;
	case RIGHT:
		_turneddir = 1;		break;
	case DOWN:
		_turneddir = 2;		break;
	case LEFT:
		_turneddir = 3;		break;
	default:
		_turneddir = 0;		break;
	}
	
	_turneddir += _rightspin;
	_turneddir%=4;	//-3~3
	_turneddir+=4;	//1~7
	_turneddir%=4;	//0~3

	return (direction_tag) choose(_turneddir+1, UP, RIGHT, DOWN, LEFT);
}

direction_tag sys::StrtoDir(const char* _str, int _originaldir){	//CCmdManager��private�֐��ɂ��Ă���������
	int dir;
	int spin = 0;
	char* p;
	char* str = new char[strlen(_str)+1];
	mystrcpy(str, _str);

	if ((p=strchr(str, '+'))!=NULL){
		mystrtol(p, &spin);
		*p = '\0';
	}else if ((p=strchr(str, '-'))!=NULL){
		mystrtol(p, &spin);
		*p = '\0';
	}

	if( mystrcmp2(str, "right") || mystrcmp(str, 'p', 3,"��","0","�E") ){
		dir = RIGHT;
	}else if( mystrcmp2(str, "left") || mystrcmp(str, 'p', 3,"��","1","��") ){
		dir = LEFT;
	}else if( mystrcmp2(str, "down") || mystrcmp(str, 'p', 3,"��","2","��") ){
		dir = DOWN;
	}else if( mystrcmp2(str, "up") || mystrcmp(str, 'p', 3,"��","3","��") ){
		dir = UP;
	}else if( mystrcmp2(str, "original") || mystrcmp(str, "-1") ){
		dir = (direction_tag)_originaldir;
	}else{
		WarningDx("Warning->StrtoDir failed:%s", __FILE__, __LINE__, str);
		dir = DOWN;
	}

	delete [] str;
	return TurnDir(dir, spin);
}

bool sys::PlayerName(const char* _str){
	return (mystrcmp2(_str, "me") || mystrcmp2(_str, "player"));
}

bool sys::TrueOrFalse(const char* _str, bool _torf){
	if (_torf){
		return (mystrcmp(_str, "1") || mystrcmp2(_str, "true"));
	}else{
		return (mystrcmp(_str, "0") || mystrcmp2(_str, "false"));
	}
}

int sys::rank3(const char* _str, int _exception) {
	if (mystrcmp2(_str, "low")) {
		return 1;
	} else if (_str==NULL || mystrcmp2(_str, "middle")) {
		return 2;
	} else if (mystrcmp2(_str, "high")) {
		return 3;
	} else {
		WarningDx("Warning->Check _str[low,middle,high]:%s", _str);
		return _exception;
	}
}

bool sys::CheckStrNULL(const char* _str){
	if (mystrcmp2(_str, "NULL")) {
		return true;
	} else {
		return false;
	}
}