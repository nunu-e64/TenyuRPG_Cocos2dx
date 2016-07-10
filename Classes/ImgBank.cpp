#include "ImgBank.h"


void CImgBank::Clear(){

	std::map <std::string, int*>::iterator it = ImgBank.begin();
	while(!ImgBank.empty() && it!=ImgBank.end()){
		myLog("deleting ImgBank:%s...", (*it).first.c_str());
		delete [] (*it).second;	
		++it;
	}

	ImgBank.clear();
}

void CImgBank::LoadPic(const char *_path, const char _key[32], const char _kind[32]){
	AddImg(_key, LoadGraph(_path, true));
}

bool CImgBank::AddImg(const char* _key, const int _img, int _sizeX, int _sizeY){

	if (mystrcmp2(_key, "null")){
		ERRORDX("You can't use \"null\" for PicKey:%s->(cancel)", _key);
		return false;
	} else if (ImgBank.find(_key)!=ImgBank.end()) {
		ERRORDX("AlreadyResiteredImageKey (cancel):%s", _key);
		return false;
	}

	int* newImg = new int[_sizeX*_sizeY];

	if (_sizeX * _sizeY > 1) {

		CVector imgSize = GetGraphSize(_img);
		for (int i = 0; i < _sizeY; i++) {
			for (int j = 0; j < _sizeX; j++) {
				newImg[i*_sizeX+j] = DerivationGraph((int)(j*imgSize.x / (double)_sizeX), (int)(i*imgSize.y / (double)_sizeY), (int)imgSize.x/_sizeX, (int)imgSize.y/_sizeY, _img);
				//DEBUGDX("%s:%d=%d,%d", _key, i*_sizeX + j, j, i);
			}
		}
	
	} else {
		newImg[0] = _img;
	}
	
	ImgBank.insert( std::map<std::string, int*>::value_type( _key, newImg) );
	myLog("new ImgBank:%s...", _key);

	myLogf("ImgBank", "Add  %s:%d", _key, _img);
	myLogf("ImgBank", "Add  %s:%d", _key, ImgBank[_key][0]);

	return true;
}

int CImgBank::GetImg(const char* _key){

	if (mystrcmp2(_key, "null")) return 0;

	if (ImgBank.find(_key)!=ImgBank.end()) {
		myLogf("ImgBank", "Get  %s:%d", _key, ImgBank[_key][0]);
		return ImgBank[_key][0];
	} else {
		ERRORDX("Not Found:%s", _key);		
		return 0;
	}
}

bool CImgBank::GetImg(const char* _key, int _img[], int _sizeX, int _sizeY){

	if (mystrcmp2(_key, "null")) return false;

	if (ImgBank.find(_key)!=ImgBank.end()) {
		
		for (int i = 0; i < _sizeY; i++) {
			for (int j = 0; j < _sizeX; j++) {
				_img[i*_sizeX + j] = ImgBank[_key][i*_sizeX + j];
				//DEBUGDX("%s:%d=%d,%d", _key, i*_sizeX + j, j, i);
			}
		}
		return true;

	} else {
		ERRORDX("Not Found:%s", _key);		
		return false;
	}

}

int* CImgBank::GetImg(const char* _key, int _sizeX, int _sizeY){

	if (mystrcmp2(_key, "null")) return NULL;

	if (ImgBank.find(_key)!=ImgBank.end()) {
		return ImgBank[_key];
	} else {
		ERRORDX("Not Found:%s", _key);		
		return NULL;
	}
}
