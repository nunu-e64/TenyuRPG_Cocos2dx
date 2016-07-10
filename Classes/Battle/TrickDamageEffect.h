////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#ifndef TRICKDAMAGEEFFECT_H					////
#define TRICKDAMAGEEFFECT_H					////
////////////////////////////////////////////////

#include <map>
#include <string>
//#include "Battle.h"

class CBattle;
class CBImgBank;
class CTrickDamageEffect{ //�퓬�_���[�W�G�t�F�N�g�p�̃C���^�[�t�F�[�X
public: 
	CTrickDamageEffect(std::string _name){
		CONSTRUCTED;;
		Name = _name;
	}
	~CTrickDamageEffect(){
		DESTRUCTED;
	}
	virtual void DrawDamageEffect(CBattle* _battle, CBImgBank* _bimgbank, CRect _attackerR, CRect _targetR) const =0;
	std::string GetName() const {return Name;}
protected:
	std::string Name;	//�G�t�F�N�g�̖��O ��FRED_BOMB, BLUE_BOMB, BIG_SPREAD, SMALL_SPREAD
	//std::string TypeName;	//Type�̖��O ��FBOMB, SPREAD
};


class CTrickDamageEffect_BOMB: public CTrickDamageEffect{	//���쐬
public:
	CTrickDamageEffect_BOMB(std::string _name, std::vector<std::string>_argList);
	~CTrickDamageEffect_BOMB(){
		DESTRUCTED;
	}
	void DrawDamageEffect(CBattle* _battle, CBImgBank* _bimgbank, CRect _attackerR, CRect _targetR) const;	//����
private:
	enum{ ARG_NUM=5 };
	int Color;
	int Size;
	int Time;
};

class CTrickDamageEffect_PROTO: public CTrickDamageEffect{
	/*�������8�̌������W�܂��āA��̌����ƂȂ��đ���߂����Ĕ��ł���	*/
public:
	CTrickDamageEffect_PROTO(std::string _name, std::vector<std::string>_argList);
	~CTrickDamageEffect_PROTO(){
		DESTRUCTED;
	}
	void DrawDamageEffect(CBattle* _battle, CBImgBank* _bimgbank, CRect _attackerR, CRect _targetR) const;	//����
private:
	enum{ ARG_NUM=3 };
	int Size1;	//���������̑傫��
	int Size2;	//�ł�������ăq�b�g��������̑傫��
	int Radius;  //���������̏����z�u���a�i�L�����̒��S���W����̋����j
};




////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#endif										////
////////////////////////////////////////////////