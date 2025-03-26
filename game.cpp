//========================================
//
// �Q�[������ [ game.cpp ]
// Author:Asuma Nishio
//
//========================================

//***********************************************
// �C���N���[�h�t�@�C���錾
//***********************************************
#include "game.h"
#include "Player.h"
#include "background.h"
#include "input.h"
#include "fade.h"
#include "score.h"
#include "title.h"
#include "ranking.h"
#include "rankscore.h"
#include "block.h"
#include "pause.h"
#include "time.h"
#include "bullet.h"
#include "explosion.h"
#include "item.h"
#include "bulletnum.h"
#include "sound.h"

//***********************************************
// �}�N����`
//***********************************************

//***********************************************
// �O���[�o���ϐ��錾
//***********************************************
GAMESTATE g_gameState = GAMESTATE_NONE;		// �Q�[�����
int g_nCounterGameState = 0;				// ��ԊǗ��J�E���^�[
bool g_bPause = false;						// �|�[�Y�����ǂ���
int g_nCntWaveTimeCount = 0;				// �ǂݍ��݃E�F�[�u�J�E���g

//=====================
// �Q�[���̏���������
//=====================
void InitGame(void)
{
	InitBackground();		// �w�i�̏�����

	InitBlock();			// �u���b�N�̏�����

	InitPause();			// �|�[�Y�̏�����

	//InitTime();				// �^�C���̏�����

	InitBullet();			// �e�̏�����

	InitPlayer();			// �v���C���[�̏�����

	InitExplosion();		// �����̏�����

	InitScore();			// �X�R�A�̏�����

	InitItem();				// �A�C�e���̏�����

	InitBulletNum();		// �c�e���̏�����

	LoadBlockText();		// �z�u�����u���b�N���̓ǂݍ���

#if 0
	InitEffect();			//�G�t�F�N�g

	InitParticle();			//�p�[�e�B�N��

#endif

	g_gameState = GAMESTATE_NORMAL;	// �ʏ��Ԃɐݒ�
	g_nCounterGameState = 0;		// �Q�[�����
	g_bPause = false;				// �|�[�Y����
	g_nCntWaveTimeCount = 0;		// �N�[���^�C��

	PlaySound(SOUND_LABEL_GAMEBGM);

}
//===================
// �Q�[���̏I��
//===================
void UninitGame(void)
{
	SaveScore();			// �X�R�A��ۑ�

	StopSound();			// ���y��~

	UninitBlock();			// �u���b�N�̏I��

	UninitPause();			// �|�[�Y�̏I��

	UninitBackground();

	//UninitTime();			// �^�C���̏I������

	UninitPlayer();			// �v���C���[�̏I������	

	UninitBullet();			// �e�̏I������

	UninitExplosion();		// �����I������

	UninitScore();			// �X�R�A�̏I��

	UninitItem();			// �A�C�e���̏I��

	UninitBulletNum();		// �c�e���̏I������

#if 0

	UninitEffect();			//�G�t�F�N�g

	UninitParticle();		//�p�[�e�B�N��

#endif
}
//===================
// �Q�[���̍X�V
//===================
void UpdateGame(void)
{
	// �|�[�Y���
	if (KeyboardTrigger(DIK_P) == true || JoypadTrigger(JOYKEY_START))
	{
		// p�������ꂽ or start�{�^��
		g_bPause = g_bPause ? false : true;
	}


	if (g_bPause == true)
	{// �|�[�Y��
		UpdatePause();		// �|�[�Y�̍X�V����
	}
	else
	{
		UpdateBlock();		// �u���b�N�̍X�V

		UpdateBullet();		// �e�̍X�V����
		
		UpdatePlayer();		// �v���C���[�̍X�V����

		UpdateExplosion();	// �����̍X�V����

		UpdateItem();		// �A�C�e���̍X�V

		UpdateBulletNum();	// �c�e���̍X�V
	}

	if ((GetFinish() == true  || GetBulletNum() <= 0)
		&& g_gameState != GAMESTATE_NONE)
	{
		g_gameState = GAMESTATE_END;  //�I�����

		//PlaySound(SOUND_LABLE_EXIT);
	}

	if (GetBlock() <= 0)
	{// �X�e�[�W�̃u���b�N��S����������

		// �J�E���g�����Z
		g_nCntWaveTimeCount++;

		if (g_nCntWaveTimeCount >= 90)
		{// �J�E���g��90����Ȃ�

			// �V�K�E�F�[�u��ǂݍ���
			LoadBlockText();

			// �J�E���g��������
			g_nCntWaveTimeCount = 0;
		}
	}

	switch(g_gameState)
	{
	case GAMESTATE_NORMAL://�ʏ���
		break;

	case GAMESTATE_END:
  		g_nCounterGameState++;
		if (g_nCounterGameState >= 60)
		{
			//�J�E���^�[��������
			g_nCounterGameState = 0;

			//1�b�o��
			g_gameState = GAMESTATE_NONE;		//�������Ă��Ȃ����

			//��ʐݒ�
			SetFade(MODE_RESULT);

			//�ǂݍ���
			ResetRanking();

			//�����L���O�̐ݒ�
			SetRanking(GetScore());
		}
		break;
	}


#ifdef _DEBUG
	if (KeyboardTrigger(DIK_F4) == true)
	{
		// ���[�h�ύX
		SetFade(MODE_EDIT);
	}
#endif

}
//===================
// �Q�[���̕`��
//===================
void DrawGame(void)
{
	DrawBackground();

	DrawBlock();			// �u���b�N�̕`��

	//DrawTime();				// �^�C���̕`��

	DrawBullet();			// �e�̕`�揈��

	DrawPlayer();			// �v���C���[�̕`��

	DrawExplosion();		// �����̕`��

	DrawItem();				// �A�C�e���̕`��

	DrawBulletNum();		// �c�e���̕`��

	if (g_bPause == true)
	{// �|�[�Y��
		DrawPause();		// �|�[�Y�̕`��
	}

}
//=========================
// �|�[�Y�̗L�������ݒ�
//=========================
void SetEnablePause(bool bPause)
{
	g_bPause = bPause;
}
//=========================
// �Q�[����Ԑݒ�
//=========================
void SetGamestate(GAMESTATE state)
{
	g_gameState = state;
	g_nCounterGameState = 0;
}
//==========================
// ���݂̃Q�[����Ԃ�Ԃ�
//==========================
GAMESTATE GetGameState(void)
{
	return g_gameState;
}
