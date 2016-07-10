////多重インクルード防止（インクルードガード）//
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
	ストップさせたい→リストに追加する（アドレスと現在の音量と減少量or減少時間）→

	*/



	//単一性を保証(Singleton)////////////////////////////////
	CMusicManager() {
		CONSTRUCTED;
	}
	CMusicManager(const CMusicManager& hoge);
	CMusicManager& operator=(const CMusicManager& hoge);
	/////////////////////////////////////////////////////////
};

////多重インクルード防止（インクルードガード）//
#endif										////	
////////////////////////////////////////////////

