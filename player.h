//================================
//
// �v���C���[���� [ player.h ]
// Author:Asuma Nishio
//
//================================
#ifndef _PLAYER_H_ //���̃}�N����`������ĂȂ�������
#define _PLAYER_H_ //2�d�C���N���[�h�h�~�̃}�N����`

//************************************
// �C���N���[�h�t�@�C��
//************************************
#include "main.h"

//**************************************
// �}�N����`
//**************************************
#define SPLIT_U (1.0f)				//���̕�����
#define SPLIT_V (1.0f)				//�c�̕�����
#define MAX_ANIMATION_TIME (10)		//�ő�A�j���[�V��������
#define MAX_ANIMATION_PATTERN (8)	//�ő�p�^�[����
#define MAX_SIZE_X (40.0f)			//���̃T�C�Y(���@)
#define MAX_SIZE_Y (40.0f)			//�c�̃T�C�Y(���@)

// �v���C���[�̏��
typedef enum
{
	PLAYERSTATE_APPEAR = 0,			//���G���
	PLAYERSTATE_NORMAL,				//�ʏ���
	PLAYERSTATE_DAMAGE,				//�_���[�W���
	PLAYERSTATE_DEATH,				//���S���
	PLAYERSTATE_MAX
}PLAYERSTATE;

// �v���C���[�\���̂��`
typedef struct
{
	D3DXVECTOR3 pos;				// �ʒu
	D3DXVECTOR3 move;				// �ړ���
	D3DXVECTOR3 rot;				// z��
	float fLength;					// �Ίp���̒���
	float fAngle;					// �Ίp���̊p�x
	bool bHit;						// �e����������
	int nLife;						// ���@�̗̑�
	int nCntAnimState;				// ��Ԃ��J�E���g
	int nCntState;					// �J�E���^�[
	PLAYERSTATE state;				// �v���C���[�̏��
	bool bDisp;						// �\�����邩���Ȃ���
}Player;

// �v���g�^�C�v�錾
void InitPlayer(void);				// �v���C���[�̏�����
void UninitPlayer(void);			// �v���C���[�̏I��
void UpdatePlayer(void);			// �v���C���[�̍X�V����
void DrawPlayer(void);				// �v���C���[�̕`�揈��
void GetKeyPlayer(void);			// �L�[���͏����擾
Player* GetPlayer(void);			// �v���C���[���擾
void GetJoypadPlayer(void);			// �W���C�p�b�h�����擾
void StickState(void);				// �X�e�B�b�N�����擾



#endif