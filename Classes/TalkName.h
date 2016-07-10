////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#ifndef TALKNAME_H							////
#define TALKNAME_H							////
////////////////////////////////////////////////


class CTalkName{
public:
	CTalkName(){
		CONSTRUCTED;
		NowLeft = true;
	}
	~CTalkName(){
		DESTRUCTED;
	}
	void Init();

	//�萔
		enum{SIDE_NUM=2, NAME_NUM=10};

	//�֐�
		void Clear(bool _left);
		bool Add(bool _left, int _num, ...);
		bool Dec(bool _left, int _num, ...);
	
		bool SetNowName(bool _left, char* _name, bool _add=true);
		void SetNowSide(bool _left){NowLeft=_left;};
		bool GetVisible();
		std::string GetNowName();

		void Draw(int _left, int _right, int bottom);

private:
	//�ϐ�
		char Name[SIDE_NUM][NAME_NUM][32];	//0...left, 1...right, ���E���ꂼ��0~9��10�l�����O�ۑ�
		int ImgLabel[3];
		bool NowLeft;
};

////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#endif										////
////////////////////////////////////////////////