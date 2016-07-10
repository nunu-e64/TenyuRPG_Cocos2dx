#include "Mrt.h"


using namespace mrtlib;		//using�f�B���N�e�B�u���g�����ǂ����͂��C�����܂�

namespace mrt{		//using�Ƃ̈ʒu�֌W������ł����Ă�̂��͕s��

void Opening(){	//�Ԃ�l�F�Ȃ� �����F�Ȃ�
		
	/*/////////////////////////////////////////////////////////
	�u�͂��߂���v�v���C����Ƃ����������I�[�v�j���O���o�B
	*//////////////////////////////////////////////////////////

	int timecount = 0;
	int scene = 0;
	int fontsize = 14;
	char* optext[] = {"����̕���͓����\�����V�A",
		"���̍��ɂ͂��đ��z�̐_�Ɛ��̐_�����_�������݂�",
		"�M�҂̒��ɂ͐M��������_���疂�͂���������̂�����",
		"���̑��z����͂�������g���z�̖��@�g���h��",
		"��̐�����͂�������g���̖��@�g���h�ł���",
		"�������A�}���Ȕ��B���i�ޓs�s���ł͐M�͔���",
		"�\�����V�A���ɂ���ĐV���ȏ@�����������",
		"�e�����ꂽ�����k�����́A�i���ɂ������l�X�Ǝ��g�ݔ����̘T�����グ��",
		"���ꂩ��30�N�A���̍��ɂ͍��Ȃ������̉Ύ킪�����Ԃ葱���Ă���",
		"�����č��A���W�X�^���X�ɐV���ȃ��[�_�[���a������",
		"����́A�����ɂ킽�镴���ɐV���ȗ��j�����ށA�Ⴋ���[�_�[�̉p�Y杂ł���",
		" ",
		"�i�����܂ł��ׂĉR�j"};
	
	SetFontSize(fontsize);
	while( BasicLoop() && !CheckHitKeyDown(KEY_INPUT_ESCAPE) && !CheckHitKeyDown(KEY_INPUT_OK) && scene != -1){
		
		switch(scene){
		case 0:
			if(timecount>=3300){
				scene = -1;
			}
			for (int i=0; i<11; i++){
				DrawCenterString(WINDOW_WIDTH/2, (int)(WINDOW_HEIGHT-timecount/3) + i*(fontsize*4), WHITE, optext[i]);
			}
			break;
		}
		
		//��Ctrl��4�{��
		if (CheckHitKey(KEY_INPUT_LCONTROL)){ timecount+=4; }else{ timecount++;}
	}
}

int Title(){	//�Ԃ�l�Ftitle_tag  �����F�Ȃ�	
	/*///////////////////////////////////////
	���L���Q�l��Z�L�[���������Ƃ��ɓK�؂Ȓl��Ԃ��Ă�������
	enum title_tag{
		TITLE_LOADSTART,
		TITLE_FIRSTSTART,
		TITLE_SETTING,
		TITLE_GAMEEND,
		TITLE_NUM,
	};
	////////////////////////////////////////*/

	int target = 0;
	int timecount = 0;
	
	while( BasicLoop() && !CheckHitKeyDown(KEY_INPUT_ESCAPE)){
		
		if (CheckHitKeyDown(KEY_INPUT_DOWN)){
			target = mod(target+1, TITLE_NUM);
		}else if (CheckHitKeyDown(KEY_INPUT_UP)){
			target = mod(target-1, TITLE_NUM);
		}else if (CheckHitKeyDown(KEY_INPUT_OK)){
			return target;
		}

		timecount++;

		DrawCenterString(WINDOW_WIDTH/2, (int)(WINDOW_HEIGHT*1/10), "- �V �C -" , WHITE );
		//DEBUG:�Q�[�����j���[����f�o�b�O�p�Ɉꎞ�t�]
		DrawCenterString(WINDOW_WIDTH/2, (int)(WINDOW_HEIGHT*5/10), "�͂��߂���", WHITE);
		DrawCenterString(WINDOW_WIDTH/2, (int)(WINDOW_HEIGHT*6/10), "�Â�����", WHITE);
		DrawCenterString(WINDOW_WIDTH/2, (int)(WINDOW_HEIGHT*7/10), "�@�ݒ�@�@", WHITE);
		DrawCenterString(WINDOW_WIDTH/2, (int)(WINDOW_HEIGHT*8/10), "�Q�[���I��", WHITE);
		
		if (int(timecount/15)%2) DrawCenterString(WINDOW_WIDTH/2, (int)(WINDOW_HEIGHT*(target+5)/10), "|>�@�@�@�@�@�@�@", WHITE);
	}

	return TITLE_GAMEEND;
}


int ChooseData(playdata_tag* _playdata){	//�Ԃ�l�F���[�h����f�[�^�ԍ��i�G���[�F-1[backtotitle]�j  �����F�Z�[�u�f�[�^�z��
	
	/*///////////////////////////////////////
	Exist��true�̃Z�[�u�f�[�^�̂ݕ\�����Ă��������B�iplaydata_tag�̎d�l�ɂ��Ă�Define.h�Q�Ɓj
	�Z�[�u�f�[�^��PLAYDATA_NUM����܂��B�i_playdata[0]~_playdata[PLAYDATA_NUM-1]�j
	////////////////////////////////////////*/

	int dnum = 0;
	char tmpstring[256];

	while( BasicLoop()){
		sprintf_s(tmpstring, "�f�[�^�ԍ��F%d �f�[�^���F%s", dnum, (_playdata[dnum].Exist?_playdata[dnum].DataName:"No Data")); 
		DrawCenterString(WINDOW_WIDTH/2, (int)(WINDOW_HEIGHT*1/4), "���[�h����f�[�^��I��", WHITE);
		DrawCenterString(WINDOW_WIDTH/2, (int)(WINDOW_HEIGHT*2/4), tmpstring, (_playdata[dnum].Exist? WHITE:GRAY));
		DrawCenterString(WINDOW_WIDTH/2, (int)(WINDOW_HEIGHT*3/4), "����:�f�[�^�I�� Z:���� Esc:�^�C�g���ɖ߂�", WHITE);

		if (CheckHitKeyDown (KEY_INPUT_DOWN)){
			dnum = mod(dnum+1, PLAYDATA_NUM);
		}else if (CheckHitKeyDown (KEY_INPUT_UP)){
			dnum = mod(dnum-1, PLAYDATA_NUM);
		}

		if (CheckHitKeyDown(KEY_INPUT_OK)){
			if (_playdata[dnum].Exist){
				return dnum;
			}

		}else if (CheckHitKeyDown(KEY_INPUT_ESCAPE)){
			return -1;
		}
	}

	WarningDx("Warning->Unexpected Error Happened", __FILE__, __LINE__);
	return -1;
}

bool Setting(){	//�Ԃ�l�c�ݒ�ύX(true) �ݒ�I��(false)  �����c���܂̂Ƃ���Ȃ�
	
	/*///////////////////////////////////////
		�ڍז���B�d�l�͂��ꂩ��l�߂�B
		���ʏグ�������炢�Ȃ炱�̊֐����ŏ������Ă�����Ă����������c�H
	////////////////////////////////////////*/

	int fontsize = 10;

	SetFontSize(fontsize);
	while( BasicLoop() && !CheckHitKeyDown(KEY_INPUT_ESCAPE)){
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);	
		DrawCenterString(WINDOW_WIDTH/2, (int)(WINDOW_HEIGHT*2/5), "�e��ݒ�i���ʒ����A�e�L�X�g�X�s�[�h�����A�f�U�C���ύX(�ł����)�A���Ɖ��H�j���ł���悤�ɂ���BEsc�ŏI���B", WHITE);
	}

	return false;

}


}	//namespace����