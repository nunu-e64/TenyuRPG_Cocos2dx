////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#ifndef IMGBANK_H						////
#define IMGBANK_H						////
////////////////////////////////////////////////

#include <map>
#include <string>
#include <vector>

class CImgBank{
public:
	CImgBank(){
		CONSTRUCTED;
		DammyImg = 0;
	}
	~CImgBank(){
		DESTRUCTED;
		Clear();
	}

	void Clear();

	void LoadPic(const char *_path, const char _key[32], const char _kind[32]);	//CMap�Ƃ̌݊����m��

	bool AddImg(const char* _key, const int _img, int _sizeX = 1, int _sizeY = 1);
	
	int GetImg(const char* _key);
	bool GetImg(const char* _key, int _img[], int _sizeX, int _sizeY=1);
	int* GetImg(const char* _key, int _sizeX, int _sizeY=1);

private:
    std::map <std::string, int*> ImgBank;

	int DammyImg;

};


////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#endif										////
////////////////////////////////////////////////