////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#ifndef EVEOBJ_H							////
#define EVEOBJ_H							////
////////////////////////////////////////////////


class CEveObj{
public:
	CEveObj(unsigned char _datanum=0){
		//CONSTRUCTED;
		Mapnum = 0;
		Datanum = _datanum;
		Visible = false;
		strcpy_s(Name, "");
		Count = 0;
		Dir = DOWN;
		Kind = objkind_tag::NUM;

		for (int i=0; i<ARRAY_SIZE(Img); i++){
			Img[i]=-1;
		}
		strcpy_s(PicKey, "");

		Step=0;
		Dx=0;
		Dy=0;
		Alpha=255;

		Effect=NONE;
		for(int i=0; i<ARRAY_SIZE(EffectNum); i++){
			EffectNum[i] = -1;
			EffectNumCmd[i] = -1;
		}
	}
	
	~CEveObj(){
		//DESTRUCTED;
	}

	//�ϐ�
		//Mapnum��Datanum�̑g�ݍ��킹�œ���ł���
		unsigned int Mapnum;	
		unsigned char Datanum;	//�C�x���g�}�b�v�f�[�^���̃o�C�i���ԍ�

		char Name[32];
		std::vector<char256> Text;
		int Count;						//���ɂȂ鎖�͂Ȃ�
	
		int Img[CHARA_PIC_NUM];	char PicKey[32];		//NPC�ȊO��Img[1~]��-1�Ȃ̂Ŏg�p�֎~
		bool Visible;

		enum direction_tag Dir;		//DOWN�ŏ�����
		enum objkind_tag::type Kind;
		int Step;		//0~3
		int Dx, Dy;		//�`�掞�ɂ��炷�Ԃ�i���s��W�����v�Ȃǁj
		unsigned char Alpha;

		enum charaeffect_tag Effect;	//���ʁi�_�ŁA�������s�Ȃǁj
			int EffectNumCmd[5];
			int EffectNum[5];
			//BLINK
				//[0]...����, [1]...���, [2]...��������, ([3]...�ω���) �����Ə���́�, �ω��ʂ�255�x�[�X
			//RND_DIR
				//[0]...������ς��鎞��,�i[1]..�J�E���^�[�j

	//�֐�
		void Draw(int _x, int _y);
};


////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#endif										////
////////////////////////////////////////////////