////多重インクルード防止（インクルードガード）//
#ifndef CMDMANAGER_H						////
#define CMDMANAGER_H						////
////////////////////////////////////////////////

class CWorldManager;
class CField;
class CMap;
class CEveManager;
class CTextBox;

class CBattle;
class CBImgBank;
class CPlayerSpeciesManager;
class CEnemySpeciesManager;
class CTrickManager;


class CCmdManager{
public:
	CCmdManager(){CONSTRUCTED;}
	~CCmdManager(){DESTRUCTED;	};
	
protected:

	bool NextCommand(CCmdList* _cmdlist, char* commandline, char* command, char* &argument);
	bool ArgCut(const char* _command, char* _argument, char** _arg, int _argnum, bool _warning=true, int _minimum = 0);

	bool FieldCmdSolve (const char* _command, char* _argument, CField* _field, CMap* _map, CTextBox* _textbox, CEveManager* _evemanager);
	bool SystemCmdSolve(const char* _command, char* _argument, CField* _field, CMap* _map,                     CEveManager* _evemanager);
	bool WindowCmdSolve(const char* _command, char* _argument, CWorldManager* _worldmanager, CMap* _map, CTextBox* _textbox);
	bool TextCmdSolve  (const char* _command, char* _argument, CWorldManager* _worldmanager, CTextBox* _textbox);

	bool MusicSystemCmdSolve(const char* _command, char* _argument);
	bool MusicCmdSolve(const char* _command, char* _argument);

	bool BattleSystemCmdSolve(const char* _command, char* _argument, CBImgBank* _bimgbank, CPlayerSpeciesManager* _playerSpeciesManager, CEnemySpeciesManager* _enemySpeciesManager, CTrickManager* _trickManager);
	bool BattleCmdSolve(const char* _command, char* _argument, CBattle* _battle);


};

class CFirstSetCmdManager : public CCmdManager{
public:
	CFirstSetCmdManager(){CONSTRUCTED;}
	~CFirstSetCmdManager(){DESTRUCTED;}
	void Main(CCmdList* _cmdlist, CField* _field, CMap* _map, CEveManager* _evemanager);
};

class CFieldCmdManager : public CCmdManager{
public:
	CFieldCmdManager(){CONSTRUCTED;}
	~CFieldCmdManager(){DESTRUCTED;}
	void Main(CCmdList* _cmdlist, CField* _field, CMap* _map, CTextBox* _textbox, CEveManager* _evemanager);
};

class CBattleCmdManager : public CCmdManager{
public:
	CBattleCmdManager(){
		CONSTRUCTED;
		Map_p=NULL;
	}
	~CBattleCmdManager(){DESTRUCTED;}
	void Main(CCmdList* _cmdlist, CBattle* _battle, CTextBox* _textbox);
	void Init(CMap* _map){Map_p=_map;}

private:
	CMap* Map_p;				//Fieldから受け継ぐ
};

class CBattleFirstSetCmdManager : public CCmdManager{
public:
	CBattleFirstSetCmdManager(){CONSTRUCTED;}
	~CBattleFirstSetCmdManager(){DESTRUCTED;}
	void Main(CCmdList* _cmdlist, CBImgBank* _bimgbank, CPlayerSpeciesManager* _playerSpeciesManager, CEnemySpeciesManager* _enemySpeciesManager, CTrickManager* _trickManager);
};


////多重インクルード防止（インクルードガード）//
#endif										////
////////////////////////////////////////////////