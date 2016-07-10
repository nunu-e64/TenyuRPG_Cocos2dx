////多重インクルード防止（インクルードガード）//
#ifndef TRICKMANAGER_H						////
#define TRICKMANAGER_H						////
////////////////////////////////////////////////

#include <vector>
#include <map>
#include <string>
#include "TrickDamageEffect.h"

class CTrickManager{
public:

	static CTrickManager* GetInstance(){
		static CTrickManager instance;
		return &instance;
	}

	~CTrickManager(){
		DESTRUCTED;	
		Clear();
	}

	void Add(trick_tag _trick, const char* _key);
	void Add(char _name[32], int _level, int _cost, int _time, target_tag::type _targetType, std::string _damageEffectName, std::vector<sideEffect_tag> sideEffectList);
	void Clear();

	trick_tag const* GetTrick(const char _name[32], bool _errorMessage=false);


	//ダメージエフェクト関係
		void CreateDamageEffect(std::string _typeName, std::string _effectName, std::vector<std::string>_argList);
		int GetTrickDamageEffectIndex(std::string _name);
		void DrawEffect(int _effectIndex, CBattle* _battle, CBImgBank* _bimgbank, CRect _attackerR, CRect _targetR) {
			if (_effectIndex>=0) TrickDamageEffectBank[_effectIndex]->DrawDamageEffect(_battle, _bimgbank, _attackerR, _targetR);
		}

	//DEBUG: ///////////////////////
	void DebugShowSideEffect(std::vector <sideEffect_tag> _sideEffectList);
	////////////////////////////////

private:
	std::map <char256, trick_tag> TrickBank;
	bool TrickBankLock;

	trick_tag Trick_dammy;

	std::vector <const CTrickDamageEffect*> TrickDamageEffectBank;		

	//単一性を保証(Singleton)////////////////////////////////
		CTrickManager(){
			CONSTRUCTED;
			TrickBankLock = false;
		}
		CTrickManager(const CTrickManager& hoge);
		CTrickManager& operator=(const CTrickManager& hoge);
	/////////////////////////////////////////////////////////

};


////多重インクルード防止（インクルードガード）//
#endif										////
////////////////////////////////////////////////