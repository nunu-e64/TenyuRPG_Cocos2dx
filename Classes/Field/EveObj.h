////多重インクルード防止（インクルードガード）//
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

	//変数
		//MapnumとDatanumの組み合わせで特定できる
		unsigned int Mapnum;	
		unsigned char Datanum;	//イベントマップデータ内のバイナリ番号

		char Name[32];
		std::vector<char256> Text;
		int Count;						//負になる事はない
	
		int Img[CHARA_PIC_NUM];	char PicKey[32];		//NPC以外のImg[1~]は-1なので使用禁止
		bool Visible;

		enum direction_tag Dir;		//DOWNで初期化
		enum objkind_tag::type Kind;
		int Step;		//0~3
		int Dx, Dy;		//描画時にずらすぶん（歩行やジャンプなど）
		unsigned char Alpha;

		enum charaeffect_tag Effect;	//効果（点滅、自動歩行など）
			int EffectNumCmd[5];
			int EffectNum[5];
			//BLINK
				//[0]...下限, [1]...上限, [2]...往復時間, ([3]...変化量) 下限と上限は％, 変化量は255ベース
			//RND_DIR
				//[0]...向きを変える時間,（[1]..カウンター）

	//関数
		void Draw(int _x, int _y);
};


////多重インクルード防止（インクルードガード）//
#endif										////
////////////////////////////////////////////////