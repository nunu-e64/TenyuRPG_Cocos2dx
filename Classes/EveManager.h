////多重インクルード防止（インクルードガード）//
#ifndef EVEOBJMANAGER_H						////
#define EVEOBJMANAGER_H						////
////////////////////////////////////////////////

#include "EveObj.h"
//#include "Field.h"		//無限ループになってないかこえええ

class CField;

class CEveManager{
public:
	static CEveManager* GetInstance(){
		static CEveManager instance;
		return &instance;
	}

	~CEveManager(){DESTRUCTED;}

	void Init(){
		for (int i=0; i<MAP_MAX; i++){
			EveObj[i].clear();
		}
		OriginalEvent.clear();
	};

	void Save(FILE *fp);
	
	void Draw(int _mapnum, int _x, int _y, bool _overdraw=false, int _dx=0, int _dy=0 );

	void SetMap(unsigned int _mapnum, int _filesize, unsigned char* buf);
	void SetEveObj(int _mapnum, int _datanum, int _kind, int _img[CHARA_PIC_NUM], const char* _pickey, char* _name, bool _visible);
		void SetPosition(const char* _name, int _dx, int _dy, bool _d=false);
		void SetPosition(const char* _name, const char* _targetname);
		void SetPosition(const char* _name, int _datanum);
	void SetPic(const char* _name, const int _img[CHARA_PIC_NUM], const char* _pickey);
	
	bool GetPos(const char* _name, int* _mapnum, int* _x, int*  _y);
	bool GetPos(const int _mapnum, const int _datanum, int* _x, int*  _y);
	
	bool GetText(char** &_text, int &_count, int _mapnum, int _x, int _y, int _mydir, int _kind=-1);
	void SetText(const char _eventtext[1000][256], const int _line, const char* _name, const int _kind);
	bool CopyOriginalEvent(std::vector<char256> *vectext_p, const char* _eventtext, int _count=0);

	bool CheckWalkable(int _mapnum, int _x, int _y);
	
	void SetDir(const char* _name, int _dir);
	void SetVisible(const char* _name, bool _visible);
	void SetAlpha(const char* _name, unsigned char _alpha);
	void SetEffect(const char* _name, int _effectname, int _effectnum[]);
	void SetCount(const char* _name, int _count, bool _add=false);
	void SetNowName(const char* _name){	mystrcpy(NowName, _name, 32);}

	void Jump(CField* _field, char* _name);
	void Walk(CField* _field, char* _name, int _dir, int _walkspeed, bool _walk=true, int _fade=0);

private:
	struct eventset_tag{
		char Name[32];
		std::vector<char256> Text;
	};


	//メンバ関数	
		bool GetEveObj(CEveObj** _eveobj_p, int _mapnum, int _x, int _y, int _kind=-1, bool _forcheck=true);
		bool GetEveObj(CEveObj** _eveobj_p, const char* _name, const int _kind=-1, int _mapnum=-1);
		bool GetOriginalEvent(eventset_tag** _originalevent_p, const char* _name);

	//メンバ変数
		std::vector<CEveObj> EveObj[MAP_MAX];
		CEveObj EveObj_dammy;

		std::vector<eventset_tag> OriginalEvent;
		eventset_tag OriginalEvent_dammy;

		char NowName[32];	//JOKER_NAME関連で使用。今、調べているオブジェクトの名前
		enum charaeffect_tag TmpEffect;

		unsigned char EveMap[MAP_MAX][MAP_SIZE][MAP_SIZE];
		
	//単一性を保証(Singleton)//////////////////////////////////////////
		CEveManager(){
			CONSTRUCTED;
		}
		CEveManager(const CEveManager& hoge);
		CEveManager& operator=(const CEveManager& hoge);
	//////////////////////////////////////////////////////////////////
};


////多重インクルード防止（インクルードガード）//
#endif										////
////////////////////////////////////////////////