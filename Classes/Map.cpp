#include "Map.h"


void CMap::Draw(int _mapnum, int _x, int _y, int _dx, int _dy){
	
	int drawx = _x-(WINDOW_WIDTH/MAP_CHIP_SIZE)/2;		//画面左上のマス
	int drawy = _y-(WINDOW_HEIGHT/MAP_CHIP_SIZE)/2;
	
	DrawRectGraph(0, 0, _dx+drawx*MAP_CHIP_SIZE+MAP_CHIP_SIZE/2, _dy+drawy*MAP_CHIP_SIZE, WINDOW_WIDTH, WINDOW_HEIGHT, MapGraph[_mapnum], false, false);

	//int drawx = _x-(WINDOW_WIDTH/MAP_CHIP_SIZE)/2;		//画面左上のマス
	//int drawy = _y-(WINDOW_HEIGHT/MAP_CHIP_SIZE)/2;
	//int maxi = WINDOW_WIDTH/MAP_CHIP_SIZE + 2;
	//int maxj = WINDOW_HEIGHT/MAP_CHIP_SIZE + 1;

	//for (int i = -1; i < maxi; i++){
	//	for (int j = -1; j < maxj; j++){
	//		DrawGraph(-_dx+i*MAP_CHIP_SIZE-MAP_CHIP_SIZE/2, -_dy+j*MAP_CHIP_SIZE, ImgMapChip[_mapnum][BasMap[_mapnum][mod(drawx+i,MAP_SIZE)][mod(drawy+j,MAP_SIZE)]],false);
	//		DrawGraph(-_dx+i*MAP_CHIP_SIZE-MAP_CHIP_SIZE/2, -_dy+j*MAP_CHIP_SIZE, ImgObjChip[_mapnum][ObjMap[_mapnum][mod(drawx+i,MAP_SIZE)][mod(drawy+j,MAP_SIZE)]],true);
	//	}
	//}

}

void CMap::CreateMapGraph(int _mapnum){	//HACK:一気にCreateするのが重過ぎるなら、マップに入ったときに順次Createもあり（-1：まだCreateしてない）
	//MAP_MAX_CHECK(_mapnum,)
	SetTitle("MapLoading...");

	for (int m=0; m<MAP_MAX; m++){
		if (!MapCreated[m]){
			MapCreated[m] = true;
			MapGraph[m] = MakeScreen(MAP_SIZE*MAP_CHIP_SIZE, MAP_SIZE*MAP_CHIP_SIZE);
			SetDrawScreen(MapGraph[m]);

			for (int i=0; i<MAP_SIZE; i++){
				for (int j=0; j<MAP_SIZE; j++){
					DrawGraph(i*MAP_CHIP_SIZE, j*MAP_CHIP_SIZE, ImgMapChip[m][BasMap[m][i][j]],false);
					DrawGraph(i*MAP_CHIP_SIZE, j*MAP_CHIP_SIZE, ImgObjChip[m][ObjMap[m][i][j]],true);
				}
			}
		}
	}

	SetDrawScreen(DX_SCREEN_BACK);
	SetTitle(GAME_TITLE);	
}

void CMap::SetMap(unsigned int _mapnum, int _filesize, unsigned char* buf){
	
	for (int i = 0; i< MAP_SIZE*MAP_SIZE; i++){
		BasMap[_mapnum][i%MAP_SIZE][(int)(i/MAP_SIZE)] = buf[i*2];
		ObjMap[_mapnum][i%MAP_SIZE][(int)(i/MAP_SIZE)] = buf[i*2+1];
	}
	
}

void CMap::LoadChip(const char *_path, int _mapnum, bool _mapchip){
	
	MAP_MAX_CHECK(_mapnum,);		//マップの配列宣言最大数を超えてる場合は読み込めない

	//下準備（チップが正方形ならMAP_CHIP_SIZEに基づいてチップリストがどんな形でも読み込める）
		int sizeX , sizeY , grHandle, width, height ;

		grHandle = LoadGraph(_path, true) ;
		GetGraphSize( grHandle , &sizeX , &sizeY ) ;
		width = sizeX/MAP_CHIP_SIZE;
		height = sizeY/MAP_CHIP_SIZE;
		DeleteGraph(grHandle);

	//読み込み
	if (_mapchip){
		if (LoadDivGraph(_path, width*height, width, height, MAP_CHIP_SIZE, MAP_CHIP_SIZE, ImgMapChip[_mapnum]) == -1) ErrorDx("Error->LoadChip->%s", __FILE__, __LINE__, _path);
	}else{
		if (LoadDivGraph(_path, width*height, width, height, MAP_CHIP_SIZE, MAP_CHIP_SIZE, ImgObjChip[_mapnum]) == -1) ErrorDx("Error->LoadChip->%s", __FILE__, __LINE__, _path);
	}
}

//イベント用画像の事前準備
void CMap::LoadPic(const char *_path, const char _key[32], const char _kind[32]){
	
	ImgData_tag newimg;
	for (int i=0; i<CHARA_PIC_NUM; i++){
		newimg.Img[i]=0;	//0で初期化
	}

	if (mystrcmp(_key, "NULL")){
		ErrorDx("Error->CMap::LoadPic->You can't use \"NULL\" for PicKey(cancel):%s", __FILE__, __LINE__, _key);
		return;
	}
	
	for (unsigned int i=0; i < ImgData.size(); i++){
		if (mystrcmp(ImgData[i].Key, _key)){
			ErrorDx("Error->AlreadyResiteredKey (cancel):%s", __FILE__, __LINE__, _key);
			return;
		}
	}

	if(mystrcmp(_kind,"NPC")){
		if (LoadDivGraph(_path, CHARA_PIC_NUM, 1, CHARA_PIC_NUM, MAP_CHIP_SIZE, MAP_CHIP_SIZE, newimg.Img) == -1){
			ErrorDx("FileLoadError->@Load_Pic->%s", __FILE__, __LINE__, _path);
			return;
		}

	}else{
		newimg.Img[0] = LoadGraph(_path, true);
		if (newimg.Img[0]==-1){
			ErrorDx("FileLoadError->@Load_Pic->%s", __FILE__, __LINE__, _path);
			return;
		}
	}	

	//エラー発生しなかった時
	strcpy_s(newimg.Key, _key);
	strcpy_s(newimg.Kind, _kind);
	ImgData.push_back(newimg);
}


//マップのバイナリデータを返す
int CMap::GetMapData(int _mapnum, int _x, int _y, int layer){
	MAP_MAX_CHECK(_mapnum, -1)

	switch(layer){
	case 0:
		return BasMap[_mapnum][_x][_y];
		break;
	case 1:
		return ObjMap[_mapnum][_x][_y];
		break;
	default:
		return -1;
		break;
	}
}


//読み込んだ画像へのパスを返す
int* CMap::GetImgData(const char _key[32]){

	if (ImgData.empty()) {
		ErrorDx("Error->No Image Loaded->%s", __FILE__, __LINE__, _key);
		return dammyimg.Img;	//読み込み済み画像がなければ0を返す
	}
	for (unsigned int i=0; i<ImgData.size(); i++){
		if (mystrcmp(ImgData[i].Key, _key)){
			return ImgData[i].Img;	//CMapのPrivate変数へのアドレスをMap→CmdManager、CmdManager→Fieldでfor文で値渡し  ∴多分安全
		}
	}

	if (!mystrcmp(_key, "NULL")) ErrorDx("Error->Don't match Pickey(all 0):%s", __FILE__, __LINE__, _key);		//"NULL"ですらないのはエラー
	return dammyimg.Img;
}

bool CMap::SetMapMusic(int _mapNum, std::string _musicKey, bool _isBattle)
{
	MAP_MAX_CHECK(_mapNum, false);
	
	if (_isBattle) {
		BattleMusic[_mapNum] = _musicKey;
	} else {
		MapMusic[_mapNum] = _musicKey;
	}

	return true;
}

std::string CMap::GetMapMusic(int _mapNum, bool _isBattle)
{
	MAP_MAX_CHECK(_mapNum, "");
	return (_isBattle? BattleMusic[_mapNum]: MapMusic[_mapNum]);
}