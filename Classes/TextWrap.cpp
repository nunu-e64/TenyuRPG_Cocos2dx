#include "TextWrap.h"
#include "CmdList.h"


void CTextWrap::Term(CCmdList* _cmdlist){	//Terminate
	
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
			
	//Box���[�h�ɖ߂�
		_cmdlist->Add("@TextMode_Set(box)");
}

void CTextWrap::Draw(bool _showingstop){

	if(!Alive || !Visible) return;

	int oldfontsize = GetFontSize();

	//�w�i�𔖈Â�
		SetDrawBlendMode( DX_BLENDMODE_ALPHA , 50*255/100) ;		//50%�A���t�@�u�����h
		DrawBox(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, BLACK, true);	//HACK:DrawBox�g��Ȃ������y���炵��
		SetDrawBlendMode( DX_BLENDMODE_NOBLEND , 0 ) ;

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
		if (!Showing && !AutoPlay) {
			int i;
			for (i=LineNum-1; i>=0; i--){
				if (strlen(chDrawText[i])) break;
			}
			DrawString(5+PosX+Width/2-WordNum*(FontSize+1)/4+strlen(chDrawText[i])*(FontSize+1)/2, PosY+Height/2 - LineNum*(FontSize+LINE_SPACE)/2 + (FontSize+LINE_SPACE)*i + (GetNowCount()/100)%5, "��", WHITE);
		}
	

	SetFontSize(oldfontsize);
}


