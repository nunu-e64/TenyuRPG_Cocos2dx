////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
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
	//�萔
		enum{ TEXT_SIZE = 1000};
	
	//�����o�ϐ�
		char LoadText[TEXT_SIZE][256];		//�߂������ɕʂ̃N���X�Ɏ��������������������i�R�}���h����n�Ȃǁj�������͂��̂܂܂ł���
		int FileLineNum;				//�s���iLoadText[FileLineNum]�͋󔒍s);
	
	//�����o�֐�
		void Punctuate(CEveManager* _evemanager, const char* _command, int _kind);
		void CmdArg(const char* string, char* name);
};


////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#endif										////	
////////////////////////////////////////////////