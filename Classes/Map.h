////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#ifndef MAP_H								////
#define MAP_H								////
////////////////////////////////////////////////


class CMap{
public:
	CMap(){
		CONSTRUCTED;
		ImgData.clear();
		for (int i=0; i<CHARA_PIC_NUM; i++){
			dammyimg.Img[i]=NULL;
		}
		ImgData.push_back(dammyimg);

		for (int m=0; m<MAP_MAX; m++){
			MapGraph[m] = -1;
			MapCreated[m] = false;
		}
	};
	~CMap(){
		DESTRUCTED;
	}
	
	void Init(){ImgData.clear();}

	void Draw(int _mapnum, int _x, int _y, int _dx=0, int _dy=0);

	void SetMap(unsigned int _mapnum, int _filesize, unsigned char* buf);
	void LoadChip(const char *_path, int _mapnum, bool _mapchip=true);
	void LoadPic(const char *_path, const char _key[32], const char _kind[32]=NULL);
	
	void CreateMapGraph(int _mapnum = -1);
	
	int GetMapData(int _mapnum, int _x, int _y, int layer=0);
	int* GetImgData(const char _key[32]);

	bool SetMapMusic(int _mapNum, std::string _musicKey, bool _isBattle = false);
	std::string GetMapMusic(int _mapNum, bool _isBattle = false);

private:
	unsigned char BasMap[MAP_MAX][MAP_SIZE][MAP_SIZE];	//[x][y]
	unsigned char ObjMap[MAP_MAX][MAP_SIZE][MAP_SIZE];	//[x][y]

	int ImgMapChip[MAP_MAX][256];
	int ImgObjChip[MAP_MAX][256];
	
	int MapGraph[MAP_MAX];		//�}�b�v�S�̂̃O���t�B�b�N�����O������Draw�ł�DrawRectGraph�����ŕ`��
	bool MapCreated[MAP_MAX];	//�}�b�v�O���t�B�b�N�̎��O�������ł��Ă��邩	

	//@Load_Pic�p	
		//HACK:vector�ł͂Ȃ�map�ɂ���
		struct ImgData_tag{	
			int	Img[CHARA_PIC_NUM];
			char Key[32];
			char Kind[32];		//�Ԃ����ႯNPC���ǂ����������ł͂Ȃ�
		};
		std::vector<ImgData_tag> ImgData;
		ImgData_tag dammyimg;
	
	//���y
		std::string MapMusic[MAP_MAX];
		std::string BattleMusic[MAP_MAX];
};



////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#endif										////	
////////////////////////////////////////////////