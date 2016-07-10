////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#ifndef ACTOR_H								////
#define ACTOR_H								////
////////////////////////////////////////////////


#include "Species.h"

class CTextBox;		//�O���錾
class CCmdList;
class CBImgBank;
class CLogWindow;

class CActor : public virtual CSpecies{
public:
	CActor(const CSpecies &obj) : CSpecies(obj){
		CONSTRUCTED;
	}

	CActor(){
		CONSTRUCTED;
	}
	~CActor(){
		DESTRUCTED;
	}

	//�����ݒ�֘A
		void FirstSet(int _playernum, int _enemynum, int _index, CTextBox** _textbox, CCmdList* _cmdlist, CLogWindow* _logWindow);
		void SetRect(int _cx, int _cy);
		//void SetImg(int _img); //�g���ĂȂ�
		bool SetSystemImg(CBImgBank* _bimgbank);
		//void AddTrick(trick_tag const* _trick);	//�g���ĂȂ�
		//void ClearTrick(){TrickList.clear();} //�g���ĂȂ�

	bool Main();
	bool Do();
	virtual void Draw(int _dx=0, int _dy=0)=0;	//�������z�֐��c�e�N���X���ɒ�`�������K�v�������Ȃ�

	//��b���
		int GetActorIndex()const { return ActorIndex; }
		int GetIndex()const{return Index;}
		bool IsPlayer()const{
			return (ActorIndex<PLAYER_NUM);
		}
		
	//�퓬�֘A
		bool GetAlive()const{ return Alive; }
		int GetVisibleStatus()const{ return VisibleStatus;}
		bool GetVisible()const{ return Visible; }
		void SetVisible(bool _visible){ Visible = _visible;}
		
		int GetAtk()const{ return Atk; }
		int GetDef() { return Def; }
		int GetHp()const { return Hp; }
		int GetMaxHp()const{ return MaxHp; }
		trick_tag const* GetNowTrick()const{ return NowTrick; }

		int GetStatus(int _key);
		void SetStatus(int _key, int _value);

		int Damaged(CActor* _attacker, trick_tag const* _trick);
		bool CheckBarMove();	//���S�m�F���ς񂾂�True
		void SetTarget(int _target){ Target=_target; }
	
	//�X�e�[�^�X�ω��Z
		void AddStatusChanger(int _kind, int _powerPercent, int _time);	//���Ԑ����t��
		void ChangeValue(int _kind, int _powerPercent);	//�i���i�X�e�[�^�X�ɒ��ډe���j
		void Heal(int _percent);

	//���̑�
		CRect GetRect()const{return Rect;}

protected:
	//�����o�֐�
		virtual void FirstSet2(){};
		virtual void SetExtraImg(CBImgBank* _bImgBank){}	//SetSystemImg����Ăяo���BPlayer�ɂ̂ݕK�v�ł��邽��Enemy�̎��ɂ͂�������ʂ�
		
		void Draw_Sub(int _dx=0, int _dy=0);


		virtual bool Plan()=0;
		virtual bool Action()=0;

	//��b���
		int PLAYER_NUM;
		int ENEMY_NUM;
		int ActorIndex;	//Actor�S�̂̒ʂ��ԍ�
		int Index;	//Player,Enemy���Ƃ̒ʂ��ԍ�

		int Atk;
		int Def;
		double Spd;
		int MaxHp;

	//�퓬�֘A
		bool Alive;		//�����B���񂾂瑦���ɕύX
		bool Visible;	//�P���ɕ`�悳��邩�ۂ��B�����͌p���B�_�ŕ\���Ɏg�p�B
		enum{
			VISIBLE,
			CHANGING,
			INVISIBLE
		}VisibleStatus;	//�������ł̕`����e�ύX���Ǘ�
		int OldHp;	//�`��p

		std::vector <statusChanger_tag> StatusChangerList;

		enum mode_tag{
			PLAN,
			PREPARE,
			ACTION,
			BEFORE_PLAN,
			MODE_NUM
		}Mode;		//�ҋ@��r��,�s���I���Ȃ�	//private�ɂ�������Player�̖h�䏈���ŃA�N�Z�X���Ă�(15/04/26)
		int MaxTimeGauge; //0~XXX(%)

	//�Z�֘A
		trick_tag const* NowTrick;		//�uconst �� * �����O���ɂ���΁A�w��������̒l�������������Ȃ��B����ɂ���΁A�|�C���^�ϐ����̂������������Ȃ��v
		int Target;

	//�C���X�^���X�̃|�C���^�ێ��p
		CTextBox** B_TextBox_pp;
		CCmdList* CmdList;
		CLogWindow* LogWindow;

	//�O���t�B�b�N
		CRect Rect;
		int Dx; int Dy;

private:
		virtual double CalcDamage(double _damage, CActor* _attacker, trick_tag const* _trick){
			return _damage;
		};

		bool TimeGaugeForward();	//�����I�ɂ͌���
		double TimeGauge;	//MaxTimeGauge��0(%)

		static std::map <std::string, int> BarImg;
		void SetBarImg(CBImgBank* _bImgBank, std::string _key);
		
		static const std::pair <int, int> ATK_UP;
		static const std::pair <int, int> ATK_DOWN;
		static const std::pair <int, int> DEF_UP;
		static const std::pair <int, int> DEF_DOWN;
		static const std::pair <int, int> SPD_UP;
		static const std::pair <int, int> SPD_DOWN;
		static const std::pair <int, int> MAX_STATUSCHANGER_IMGSIZE;
		static int StatusChangerImg[];

		static int HpFontHandle;

		std::map <int, int> Status;	
};


////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#endif										////
////////////////////////////////////////////////