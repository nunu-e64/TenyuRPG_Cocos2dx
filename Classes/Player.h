////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#ifndef PLAYER_H							////
#define PLAYER_H							////
////////////////////////////////////////////////

#include "Actor.h"
#include "../Menu.h"
#include "../Field/Item.h"


class CPlayer : public CActor, public CPlayerSpecies{
public:
	CPlayer(){
		CONSTRUCTED;
	}
	CPlayer(const CPlayerSpecies& obj) : CPlayerSpecies(obj){
		CONSTRUCTED;
	};
	~CPlayer(){DESTRUCTED;}
	
	void CreateBattleMenu(std::vector <std::string> _battleItemList);
	void Draw(int _dx=0, int _dy=0);

	void MpHeal(int _count);

private:
	//�����o�֐�
		void FirstSet2();
		void SetExtraImg(CBImgBank* _bImgBank);

		bool Plan();
		bool Action();
		
		double CalcDamage(double _damage, CActor* _attacker, trick_tag const* _trick);
	

	//�����o�ϐ�
		int MagicCounterImg[2];

		CBattleMenu BattleMenu;

		CConsumptionItem const* NowItem;
};


////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#endif										////
////////////////////////////////////////////////