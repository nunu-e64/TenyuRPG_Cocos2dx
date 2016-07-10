#include "EnemyTargetter.h"
#include "Enemy.h"



const int CEnemyTargetter::ATTENTION_RATIO[MAX_PLAYER_NUM] = {4, 2, 1};


void CEnemyTargetter::CalcAttentionRank(){


	//�A�e���V�����̑召��Rank�̌��܂��(14/04/27)///////////////
	// a>b>c -> 0,1,2
	// a=b>c -> 0,0,1
	// a=b=c -> 0,0,0
	// a>b=c -> 0,2,2
	////////////////////////////////////////////

	for (int i=0; i<PLAYER_NUM; i++) {
		AttentionRank[i] = 0;

		for (int j=0; j<PLAYER_NUM; j++) {
			if ((i != j) && (Attention[i] <= Attention[j])) {
				++AttentionRank[i];
			}
		} 
	}

	//��ɋl�߂�
	int i = 0;
	while(AttentionRank[i] != 0){
		if (i == PLAYER_NUM-1) {
			i = 0;
			for (int j=0; j<PLAYER_NUM; j++) {
				--AttentionRank[j];
			}
		} else {
			++i;
		}
	}

	myLogf("Attention", "Player[0]:%d,rank:%d, Player[1]:%d,rank:%d, Player[2]:%d,rank:%d", Attention[0], AttentionRank[0],  Attention[1], AttentionRank[1], Attention[2], AttentionRank[2]);
}

int CEnemyTargetter_DEFAULT::GetTarget(const CEnemy* _enemy){

	//�S�����S���Ă���G���[��f���ďI��
	for (int i=0; i<PLAYER_NUM; i++){
		if (Actor[i]->GetAlive()) break;

		if (i==PLAYER_NUM-1) {
			ERRORDX("'%s':All_Players_Died", _enemy->GetName().c_str());
			return -1;
		}
	}

	//�A�e���V�����̏��ʂɉ����ă^�[�Q�b�g����
	int probability = 0;
	int target = -1;

	for (int i=0; i<PLAYER_NUM; i++){
		if (!Actor[i]->GetAlive()) continue;		//���Ɏ��S���Ă���Ƃ��̓^�[�Q�b�g�ɂ��Ȃ�
		probability += ATTENTION_RATIO[GetAttentionRank(i)];
		if ((rand()%100)/(double)100 * probability < ATTENTION_RATIO[GetAttentionRank(i)]){
			target = i;
		}
	}
	

	//�G���[�`�F�b�N
	if (target < 0) {
		ERRORDX("%s:Target < 0 :%d", _enemy->GetName().c_str(), target);
		return -1;
	} else if (target>=PLAYER_NUM) {
		ERRORDX("%s:Target >= PLAYER_NUM :%d", _enemy->GetName().c_str(), target);
		return -1;
	}

	return target;
}

int CEnemyTargetter_REVERSE::GetTarget(const CEnemy* _enemy){
	return 0;
}

