////多重インクルード防止（インクルードガード）//
#ifndef LOAD_H								////
#define LOAD_H								////
////////////////////////////////////////////////

//#include "CmdList.h"
//#include "Map.h"
//#include "EveManager.h"

class CCmdList;
class CMap;
class CEveManager;

class CLoad{
public:
	CLoad(){
		CONSTRUCTED;
		FileLineNum=0;
	}
	~CLoad(){
		DESTRUCTED;
	}
	bool LoadAddText(const char *_path);
	void LoadMap(const char *_path, unsigned int _mapnum, CMap* _map, CEveManager* _evemanager, bool _event=false);
	void LoadPlayData(playdata_tag _playdata[]);
	void CommandCopy(CCmdList* _cmdlist);
	void EventTextCopy(CEveManager* _evemanager);

private:
	//定数
		enum{ TEXT_SIZE = 1000};
	
	//メンバ変数
		char LoadText[TEXT_SIZE][256];		//近いうちに別のクラスに持たせた方がいいかも（コマンド判定系など）←当分はこのままでいい
		int FileLineNum;				//行数（LoadText[FileLineNum]は空白行);
	
	//メンバ関数
		void Punctuate(CEveManager* _evemanager, const char* _command, int _kind);
		void CmdArg(const char* string, char* name);
};


////多重インクルード防止（インクルードガード）//
#endif										////	
////////////////////////////////////////////////