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
//#include "Player.h"
#include "background.h"
#include "input.h"
#include "fade.h"
#include "score.h"
#include "title.h"
#include "ranking.h"
#include "rankscore.h"
#include "block.h"
#include "pause.h"

//***********************************************
// �}�N����`
//***********************************************

//***********************************************
// �O���[�o���ϐ��錾
//***********************************************
GAMESTATE g_gameState = GAMESTATE_NONE;		// �Q�[�����
int g_nCounterGameState = 0;				// ��ԊǗ��J�E���^�[
bool g_bPause = false;						// �|�[�Y�����ǂ���

//=====================
// �Q�[���̏���������
//=====================
void InitGame(void)
{
	InitBlock();			 // �u���b�N�̏�����

	InitPause();			 // �|�[�Y�̏�����

	LoadBlockText();		 // �z�u�����u���b�N���̓ǂݍ���

#if 0
	InitBackground();		//�w�i

	InitBullet();			//�e

	InitPlayer();			//�v���C���[

	InitEnemy();			//�G

	InitExplosion();		//����

	InitScore();			//�X�R�A

	InitItem();				//�A�C�e��

	InitEffect();			//�G�t�F�N�g

	InitParticle();			//�p�[�e�B�N��

	InitTimer();			//�^�C�}�[

	InitPlayerLifeBar();	//���C�t�o�[

	InitBlock();					//�u���b�N�̏�����

	PlaySound(SOUND_LABEL_GAME);
#endif

	g_gameState = GAMESTATE_NORMAL;//�ʏ��Ԃɐݒ�
	g_nCounterGameState = 0;
	g_bPause = false;//�|�[�Y����

}
//===================
// �Q�[���̏I��
//===================
void UninitGame(void)
{
	SaveScore();			// �X�R�A��ۑ�

	UninitBlock();			// �u���b�N�̏B�I��

	UninitPause();			// �|�[�Y�̏I��
#if 0
	StopSound();


	UninitBackground();		//�w�i

	UninitBullet();			//�e

	UninitPlayer();			//�v���C���[

	UninitEnemy();			//�G�̏I��

	UninitExplosion();		//����

	UninitScore();			//�X�R�A�I��

	UninitItem();			//�A�C�e��

	UninitEffect();			//�G�t�F�N�g

	UninitParticle();		//�p�[�e�B�N��

	UninitPause();			//�|�[�Y

	UninitTimer();			//�^�C�}�[

	UninitPlayerLifeBar();	//���C�t�o�[

	UninitBlock();			//�u���b�N
#endif
}
//===================
// �Q�[���̍X�V
//===================
void UpdateGame(void)
{

#ifdef _DEBUG
	if (KeyboardTrigger(DIK_F2))
	{
		// �G�f�B�^�[�N��
		SetFade(MODE_EDIT);
	}

	if (KeyboardTrigger(DIK_RETURN))
	{// ENTER�L�[
		// ���U���g�ɑJ��
		SetFade(MODE_RESULT);
	}

#endif // _DEBUG

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
	}

#if 0
	// �v���C���[�̎擾
	Player* pPlayer = GetPlayer();

	// �G�̎擾
	int nNum = GetNumEnemy();

	if ((pPlayer->bDisp == false || nTime <= 0 || bExit == true) && g_gameState != GAMESTATE_NONE)
	{

		g_gameState = GAMESTATE_END;  //�I�����

		PlaySound(SOUND_LABLE_EXIT);
	}

	switch(g_gameState)
	{
	case GAMESTATE_NORMAL://�ʏ���
		break;

	case GAMESTATE_END:
  		g_nCounterGameState++;
		if (g_nCounterGameState >= 30)
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

	else 
	{
		//�|�[�Y���Ŗ������
		UpdateBackground();			//�w�i

		UpdatePlayer();				//�v���C���[

		UpdateBullet();				//�e

		UpdateEnemy();				//�G

		UpdateExplosion();			//����

		UpdateScore();				//�X�R�A�X�V

		UpdateItem();				//�A�C�e��

		UpdateEffect();				//�G�t�F�N�g

		UpdateParticle();			//�p�[�e�B�N��

		UpdateTimer();				//�^�C�}�[

		UpdatePlayerLifeBar();	//���C�t�o�[
	}
#endif
}
//===================
// �Q�[���̕`��
//===================
void DrawGame(void)
{
	DrawBlock();			// �u���b�N�̕`��

	if (g_bPause == true)
	{// �|�[�Y��
		DrawPause();		// �|�[�Y�̕`��
	}

#if 0
	//�w�i
	DrawBackground();

	//�A�C�e��
	DrawItem();

	//�e
	DrawBullet();

	//�v���C���[
	DrawPlayer();

	//�G
	DrawEnemy();

	//���j
	DrawExplosion();

	//�X�R�A
	//DrawScore();

	//�G�t�F�N�g
	DrawEffect();

	//�p�[�e�B�N��
	DrawParticle();

	//�^�C�}�[
	DrawTimer();

	//���C�t�o�[
	DrawPlayerLifeBar();

#endif
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
