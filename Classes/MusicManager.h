////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#ifndef MUSICMANAGER_H						////
#define MUSICMANAGER_H						////
////////////////////////////////////////////////

class CMusicManager{
public:
	static CMusicManager* GetInstance() {
		static CMusicManager instance;
		return &instance;
	}
	~CMusicManager(){DESTRUCTED;}

	void Init();
	void Update();
	
	void LoadMusic(std::string _key, const char* _address, bool _isPress);
	void PlayMusic(std::string _key);
	void PlaySound(std::string _key);
	void StopMusic(std::string _key);
	void StopAllMusic();
	void PauseMusic(std::string _key);
	
	void ChangeNextMusicVolume(std::string _key, int _volumePercent);


private:

	int GetMusicHandle(std::string _key, bool _errorAlert = true);

	std::map <std::string, int> MusicBank;
	std::map <int, bool> PauseFlag;

	struct fadeVolume_tag {
		int Handle;
		int Diff;
		int MaxVolume;
	};
	std::vector <fadeVolume_tag> FadeOutVolume;
	std::vector <fadeVolume_tag> FadeInVolume;

	/*
	�X�g�b�v�������������X�g�ɒǉ�����i�A�h���X�ƌ��݂̉��ʂƌ�����or�������ԁj��

	*/



	//�P�ꐫ��ۏ�(Singleton)////////////////////////////////
	CMusicManager() {
		CONSTRUCTED;
	}
	CMusicManager(const CMusicManager& hoge);
	CMusicManager& operator=(const CMusicManager& hoge);
	/////////////////////////////////////////////////////////
};

////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#endif										////	
////////////////////////////////////////////////

