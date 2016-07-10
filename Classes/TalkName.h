////多重インクルード防止（インクルードガード）//
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

	//定数
		enum{SIDE_NUM=2, NAME_NUM=10};

	//関数
		void Clear(bool _left);
		bool Add(bool _left, int _num, ...);
		bool Dec(bool _left, int _num, ...);
	
		bool SetNowName(bool _left, char* _name, bool _add=true);
		void SetNowSide(bool _left){NowLeft=_left;};
		bool GetVisible();
		std::string GetNowName();

		void Draw(int _left, int _right, int bottom);

private:
	//変数
		char Name[SIDE_NUM][NAME_NUM][32];	//0...left, 1...right, 左右それぞれ0~9の10人分名前保存
		int ImgLabel[3];
		bool NowLeft;
};

////多重インクルード防止（インクルードガード）//
#endif										////
////////////////////////////////////////////////