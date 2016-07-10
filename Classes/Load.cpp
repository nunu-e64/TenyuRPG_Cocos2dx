#include "Load.h"

#include "../Main/CmdList.h"
#include "Map.h"
#include "EveManager.h"


bool CLoad::LoadAddText(const char *_path){
	int FileHandle;
	int OldFileLineNum = FileLineNum;
	

	//�t�@�C�����J��
	FileHandle = FileRead_open(_path) ;
    if (FileHandle==0) {
		ERRORDX("FileOpenError:%s", _path);
		return false;
	}
	
	int i = OldFileLineNum;
	while( ProcessMessage()==0 && FileRead_eof( FileHandle ) == 0){
		if (!(i<TEXT_SIZE)) {ErrorDx("Error->LoadText Size Over!:%s", __FILE__, __LINE__, _path);	i=TEXT_SIZE;	break;}
        if(FileRead_gets(LoadText[i], 256, FileHandle)==-1) ErrorDx("Error->FileReadError", __FILE__, __LINE__,_path) ;		//UNDONE:�t�@�C����Shift-JIS����Ȃ��ꍇ�����Ńt���[�Y	
		
		int j=0;
		while(LoadText[i][j]=='\t'){j++;}
		if( j>0 )strcpy_s(LoadText[i], &LoadText[i][j]);

		if (!(LoadText[i][0] == '\\' || strlen(LoadText[i])==0)){	//�R�����g��󔒍s�łȂ���ΓǍ���
			i++;
			mystrrep(LoadText[i], "\\", "\0");
		}else{
			strcpy_s(LoadText[i],"");
		}
    }

	FileLineNum = i;

    // �t�@�C�������
    FileRead_close( FileHandle ) ;
	return true;
}

void CLoad::LoadMap(const char *_path, unsigned int _mapnum, CMap* _map, CEveManager* _evemanager, bool _event){
	int FileHandle;
	unsigned char *buf;		//�f�[�^�Ǎ��ݗp�|�C���^
	char path[256];

	MAP_MAX_CHECK(_mapnum,);
	strcpy_s(path, _path);
	strcat_s(path, ((!_event)?MAP_FILE_TYPE:EMAP_FILE_TYPE));

	//�t�@�C�����J��
	FileHandle = FileRead_open(path) ;
    if (FileHandle==0){
		ErrorDx("Error->MapOpen->%s", path);
		return;
	}
	
	//�t�@�C���T�C�Y�̎擾
	int FileSize = (int)FileRead_size(path ) ;
    // �t�@�C�����i�[���郁�����̈�̊m��
    buf = new unsigned char[FileSize];
	
	//�t�@�C���ǂݍ���
	FileRead_read(buf, FileSize, FileHandle) ;
	
    // �t�@�C�������
    FileRead_close( FileHandle ) ;
	
    // Map�f�[�^�ɐ���
	if(!_event){
		_map->SetMap(_mapnum, FileSize, buf);
	}else{
		_evemanager->SetMap(_mapnum, FileSize, buf);
	}

    // �������̉��
    delete [] buf;
}

void CLoad::LoadPlayData(playdata_tag _playdata[]){
	int FileHandle;
	FILE *fp;
	char filename[256];

	//��Ƀ��Z�b�g���Ă���
		for (int i=0; i<PLAYDATA_NUM; i++){
			_playdata[i].Exist = false;
			_playdata[i].DataName[0] = '\0';
		}

	//save�t�H���_�̑��݊m�F������ �� dataname.rpg�̑��݊m�F������
		_mkdir(DIR_SAVE);
		FileHandle = FileRead_open(FILE_DATANAME) ;
		if (FileHandle==0) {
			fopen_s(&fp, filename, "w");
			fclose(fp);
			return;
		}

	for( int h = 0; FileRead_eof( FileHandle ) == 0 && h < PLAYDATA_NUM; h++){
        // �Z�[�u�f�[�^������s���ǂݍ���
			if (FileRead_gets( _playdata[h].DataName, ARRAY_SIZE(_playdata[h].DataName), FileHandle ) == ARRAY_SIZE(_playdata[h].DataName)-1){
				//�t�@�C����Shift-JIS����Ȃ��ꍇ�����Ńt���[�Y
				//31�������傤�ǂ̂Ƃ��Ɏ���FileRead_gets�ł�NULL���������������Ȃ��d�l�����邽�߃t�@�C���|�C���^�̈ʒu�����炵�Ă��K�v������B�Ȃ�2�������������Ȃ��Ƃ����Ȃ��̂��͕s��
				if (FileRead_seek(FileHandle, +2, SEEK_CUR)==-1) ErrorDx("Error->LoadPlayData") ;
			}

			if (strlen(_playdata[h].DataName)==0) continue;

		_playdata[h].Exist = true;
		
		for (int i=0; i<3; i++){
			switch(i){
			case 0:
				sprintf_s(filename, "%s/%s/%s", DIR_SAVE, _playdata[h].DataName,"pos.dat");
				break;
			case 1:
				sprintf_s(filename, "%s/%s/%s", DIR_SAVE, _playdata[h].DataName,"flg.dat");
				break;
			case 2:
				sprintf_s(filename, "%s/%s/%s", DIR_SAVE, _playdata[h].DataName,"eve.dat");
				break;
			}

			//�t�@�C�����J��
			fopen_s(&fp, filename, "rb" );
			if( fp== NULL ){
				ErrorDx("Error->Can't Open SaveData->%s", filename);
				_playdata[h].Exist = false;
				break;	//���̍s�ցi���̃Z�[�u�f�[�^�ԍ��ցj
			}
	
			/////////////////////////////////////////////////////
			//�e��f�[�^�Ɋi�[/////////////////////////////////////
			switch(i){
			case 0:
				fread(&_playdata[h].NowMap, sizeof(_playdata[h].NowMap), 1, fp);
				fread(&_playdata[h].X, sizeof(char), 1, fp);
				fread(&_playdata[h].Y, sizeof(char), 1, fp);
				fread(&_playdata[h].Dir, sizeof(char), 1, fp);
				fread(&_playdata[h].PlayerPicKey, sizeof(char), sizeof(_playdata[h].PlayerPicKey), fp);
				break;
			case 1:
				char tmpname[32];
				int tmpvalue;
				while( feof(fp)==0){
					fread(&tmpname, sizeof(char), sizeof(tmpname), fp);
					fread(&tmpvalue, sizeof(tmpvalue), 1, fp);
					_playdata[h].FlagSet.SetFlag(tmpname, tmpvalue,false,true);
				}
				break;
			case 2:
				CEveObj tmpeveobj;
				while( feof(fp)==0){
					fread(&tmpeveobj.Name, sizeof(char), sizeof(tmpeveobj.Name), fp);
					fread(&tmpeveobj.Count, sizeof(int), 1, fp);
					fread(&tmpeveobj.Dx, sizeof(int), 1, fp);
					fread(&tmpeveobj.Dy, sizeof(int), 1, fp);
					fread(&tmpeveobj.Kind, sizeof(char), 1, fp);
					fread(&tmpeveobj.Dir, sizeof(char), 1, fp);
					fread(&tmpeveobj.PicKey, sizeof(char), sizeof(tmpeveobj.PicKey), fp);
					fread(&tmpeveobj.Alpha, sizeof(unsigned char), 1, fp);
					fread(&tmpeveobj.Visible, sizeof(char), 1, fp);
				
					fread(&tmpeveobj.Effect, sizeof(char), 1, fp);			
					for (int j=0; j<ARRAY_SIZE(tmpeveobj.EffectNum); j++){
						fread(&tmpeveobj.EffectNumCmd[j], sizeof(int), 1, fp);
					}

					_playdata[h].EveObj.push_back(tmpeveobj);
				}
				break;
			}
			/////////////////////////////////////////////////////
		
			// �t�@�C�������
			fclose(fp);
		}
	}	
	FileRead_close( FileHandle ) ;
}

void CLoad::CommandCopy(CCmdList* _cmdlist){	
	for (int i = 0; i<FileLineNum; i++){
		_cmdlist->Add(LoadText[i]);
	}
}

void CLoad::EventTextCopy(CEveManager* _evemanager){
	Punctuate( _evemanager, "@EVENT", -1);

	Punctuate( _evemanager, "@NPC",		  objkind_tag::NPC);
	Punctuate( _evemanager, "@BLOCK",	  objkind_tag::BLOCK);
	Punctuate( _evemanager, "@PANEL",	  objkind_tag::PANEL);
	Punctuate( _evemanager, "@COVER",	  objkind_tag::COVER);
	Punctuate( _evemanager, "@PUSHBLOCK", objkind_tag::PUSHBLOCK);
}

void CLoad::Punctuate(CEveManager* _evemanager, const char* _command, int _kind){
	char combegin[256], comend[256];
		strcpy_s(combegin, _command);	strcat_s(combegin, "_BEGIN");
		strcpy_s(comend, _command);		strcat_s(comend, "_END");
	int j = 0;
	
	for(int i = 0; i < FileLineNum; i++){
		if (mystrcmp(LoadText[i], combegin, 'l')){
			char eventtext[1000][256];
			char name[32];		CmdArg(LoadText[i], name);

			for (j = 0; !mystrcmp(LoadText[i+j+1], comend, 'l'); j++){
				strcpy_s(eventtext[j], LoadText[i+j+1]);
			}
			i = i+j+1;
			_evemanager->SetText(eventtext, j, name, _kind);
		}
	}
}

void CLoad::CmdArg(const char* string, char* name){
	int j;

	for (int i=0; string[i]!='\0'; i++){
		if (string[i]=='('){
			for (j=0; (string[i+1+j]!=')')&&(string[i+1+j]!='\0'); j++){
				name[j] = string[i+j+1];
			}
			name[j]='\0';
			break;
		}
	}
}