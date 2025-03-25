//========================================
//
// �`���[�g���A������ [ tutorial.cpp ]
// Author:Asuma Nishio
//
//========================================

//******************************************
// �C���N���[�h�t�@�C���錾
//******************************************
#include "tutorial.h"
//#include "player.h"
#include "game.h"
#include "fade.h"
//#include "effect.h"
//#include "sound.h"
//#include "particle.h"
#include "input.h"

//******************************************
// �}�N����`
//******************************************
#define SIZE_WIDTH  (50.0f)	// ����
#define SIZE_HEIGHT (50.0f) // ����

//******************************************
// �`���[�g���A���\���̐錾
//******************************************
typedef struct
{
	D3DXVECTOR3 pos;		// ���W
	D3DXVECTOR3 move;		// �ړ���
	int nType;				// ���
	bool bUse;				// �g�p���
	float fHeight;			// ����
	float fWidth;			// ��
	TUTORIALSTATE state;	// ���
}Tutorial;

//******************************************
// �O���[�o���ϐ��錾
//******************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTutorial = NULL;			// ���_�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureTutorial[TUTORIAL_MAX] = {};	// �e�N�X�`���̃|�C���^

LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTutorialBG = NULL;		// ���_�o�b�t�@�̃|�C���^(�w�i)
LPDIRECT3DTEXTURE9 g_pTextureTutorialBG = NULL;				// �e�N�X�`���̃|�C���^(�w�i)

Tutorial g_aTutorial[TUTORIAL_MAX];							// �`���[�g���A���\���̂̏��

int g_nCntTutoAnim;			// �`���[�g���A�����
int g_nPatternTutoAnim;		// ��ԃp�^�[��
bool g_bNext;				// �t���O
int g_nCntMove;				// �ړ��ʂ̃J�E���g

//==============================
// �`���[�g���A���̏���������
//==============================
void InitTutorial(void)
{
	// InitTutorial2();		//2�ڂ̃`���[�g���A���̏�����

	// �f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		

	// ���_���̃|�C���^
	VERTEX_2D* pVtx;		

	for (int nCnt = 0; nCnt < TUTORIAL_MAX; nCnt++)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			TUTORIAL_TEXTURE[nCnt],
			&g_pTextureTutorial[nCnt]);
	}

	// �e�N�X�`���̓ǂݍ���(�w�i)
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\tu1.png",
		&g_pTextureTutorialBG);

	for (int nCntTutorial = 0; nCntTutorial < TUTORIAL_MAX; nCntTutorial++)
	{
		g_aTutorial[nCntTutorial].nType = 0;							//	���
		g_aTutorial[nCntTutorial].fHeight = 0.0f;						//	����
		g_aTutorial[nCntTutorial].fWidth = 0.0f;						//	��
		g_aTutorial[nCntTutorial].bUse = false;							//	���g�p����
		g_aTutorial[nCntTutorial].state = TUTORIALSTATE_NONE;			//	�������Ă��Ȃ����
		g_aTutorial[nCntTutorial].move = D3DXVECTOR3(-20.0f,0.0f,0.0f); //	�ړ���
	}

	g_nCntTutoAnim = 0;			// �`���[�g���A�����
	g_nPatternTutoAnim = 0;		// ��ԃp�^�[��
	g_bNext = false;
	g_nCntMove = 0;

//===========================
// �`���[�g���A���̔w�i�ݒ�
//===========================

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY, FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTutorialBG,
		NULL);

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffTutorialBG->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W1�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 1�ڂ̒��_���
	pVtx[1].pos = D3DXVECTOR3(1280.0f, 0.0f, 0.0f);		// 2�ڂ̒��_���
	pVtx[2].pos = D3DXVECTOR3(0.0f, 720.0f, 0.0f);		// 3�ڂ̒��_���
	pVtx[3].pos = D3DXVECTOR3(1280.0f, 720.0f, 0.0f);	// 4�ڂ̒��_���

	// rhw�̐ݒ�(1.0f�ŌŒ�)
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// �A�����b�N
	g_pVtxBuffTutorialBG->Unlock();

//===============================
// �`���[�g���A���̊e��̏�����
//===============================

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * TUTORIAL_MAX,
		D3DUSAGE_WRITEONLY, FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTutorial,
		NULL);

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffTutorial->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < TUTORIAL_MAX; nCnt++)
	{
		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 1�ڂ̒��_���
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 2�ڂ̒��_���
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 3�ڂ̒��_���
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 4�ڂ̒��_���

		// rhw�̐ݒ�(1.0f�ŌŒ�)
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// ���_���̃|�C���^��i�߂�
		pVtx += 4;
	}

	// �A�����b�N
	g_pVtxBuffTutorial->Unlock();

	// SetTutorial(TUTORIAL_ENTER,			D3DXVECTOR3(1080.0f, 50.0f, 0.0f));			//�`���[�g���A��2
	//SetTutorial(TUTORIAL_KEY_W,			D3DXVECTOR3(300.0f,	170.0f, 0.0f));			//�`���[�g���A��3(w)
	//SetTutorial(TUTORIAL_KEY_A,			D3DXVECTOR3(250.0f, 220.0f, 0.0f));			//�`���[�g���A��4(a)
	//SetTutorial(TUTORIAL_KEY_S,			D3DXVECTOR3(300.0f, 220.0f, 0.0f));			//�`���[�g���A��5(s)
	//SetTutorial(TUTORIAL_KEY_D,			D3DXVECTOR3(350.0f, 220.0f, 0.0f));			//�`���[�g���A��6(d)
	//SetTutorial(TUTORIAL_KEY_SPACE,		D3DXVECTOR3(300.0f, 330.0f, 0.0f));			//�`���[�g���A��7(�X�y�[�X)

	//SetTutorial(TUTORIAL_BUTTON_B,		D3DXVECTOR3(1115.0f, 340.0f, 0.0f));		//�`���[�g���A��8
	//SetTutorial(TUTORIAL_BUTTON_UP,     D3DXVECTOR3(1030.0f, 170.0f, 0.0f));		//�`���[�g���A��9
	//SetTutorial(TUTORIAL_BUTTON_DOWN,   D3DXVECTOR3(1030.0f, 220.0f, 0.0f));		//�`���[�g���A��10
	//SetTutorial(TUTORIAL_BUTTON_RIGHT,  D3DXVECTOR3(1081.0f, 200.0f, 0.0f));		//�`���[�g���A��11
	//SetTutorial(TUTORIAL_BUTTON_LEFT,   D3DXVECTOR3(980.0f, 200.0f, 0.0f));			//�`���[�g���A��12
	//SetTutorial(TUTORIAL_KEY_P,			D3DXVECTOR3(300.0f,	 470.0f, 0.0f));		//�`���[�g���A��12
	//SetTutorial(TUTORIAL_BUTTON_ST,		D3DXVECTOR3(1120.0f, 460.0f, 0.0f));		//�`���[�g���A��12
	//SetTutorial(TUTORIAL_KEY_E,			D3DXVECTOR3(355.0f,	 630.0f, 0.0f));		//�`���[�g���A��13
	//SetTutorial(TUTORIAL_KEY_Q,			D3DXVECTOR3(215.0f,	 630.0f, 0.0f));		//�`���[�g���A��14
	//SetTutorial(TUTORIAL_BUTTON_RB,		D3DXVECTOR3(1200.0f, 625.0f, 0.0f));		//�`���[�g���A��15
	//SetTutorial(TUTORIAL_BUTTON_LB,		D3DXVECTOR3(1040.0f, 625.0f, 0.0f));		//�`���[�g���A��16
	//SetTutorial(TUTORIAL_LSTICK,		D3DXVECTOR3(1190.0f, 200.0f, 0.0f));		//�`���[�g���A��16

	// SetTutorial2();			//2�ڂ̃`���[�g���A��

	// PlaySound(SOUND_LABEL_TUTORIAL);
}
//===========================
// �`���[�g���A���̏I������
//===========================
void UninitTutorial(void)
{
	// StopSound();

//============================
// �`���[�g���A���̊e��̏I��
//============================

	for (int nCnt = 0; nCnt < TUTORIAL_MAX; nCnt++)
	{
		// �e�N�X�`���̔j��
		if (g_pTextureTutorial[nCnt] != NULL)
		{					   
			g_pTextureTutorial[nCnt]->Release();
			g_pTextureTutorial[nCnt] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffTutorial != NULL)
	{
		g_pVtxBuffTutorial->Release();
		g_pVtxBuffTutorial = NULL;
	}

//============================
// �`���[�g���A���̔w�i�̏I��
//============================

	// �e�N�X�`���̔j��
	if (g_pTextureTutorialBG != NULL)
	{					  
		g_pTextureTutorialBG->Release();
		g_pTextureTutorialBG = NULL;
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffTutorialBG != NULL)
	{					  
		g_pVtxBuffTutorialBG->Release();
		g_pVtxBuffTutorialBG = NULL;
	}

	// UninitTutorial2();	// 2�ڂ̃`���[�g���A���̏I��
}
//==========================
// �`���[�g���A���̍X�V����
//==========================
void UpdateTutorial(void)
{
#if 0
	FADE g_fade = GetFade();

	if (KeyboardTrigger(DIK_RETURN) == true || JoypadTrigger(JOYKEY_A) == true)
	{
		PlaySound(SOUND_LABEL_SE_ENTER);

		//Enter�L�[�������ꂽ or START�{�^���������ꂽ
		for (int nCnt = 0; nCnt < TUTORIAL_MAX; nCnt++)
		{
			if (g_aTutorial[nCnt].bUse == true && g_aTutorial[nCnt].nType == TUTORIAL_ENTER)
			{
				g_aTutorial[nCnt].state = TUTORIALSTATE_FLASH;
			}

		}



		g_bNext = true;

		if (g_aTutorial[0].pos.x <= -640.0f)
		{
			g_bNext = false;

		}

		if (g_aTutorial[0].pos.x <= -(SCREEN_WIDTH * 0.5f) && g_bNext == false)
		{
			//���[�h�ݒ�(�Q�[����ʂɈړ�)
			SetFade(MODE_GAME);
		}


	}

	g_nCntMove++;		//���Z

	XINPUT_STATE* pStick;

	pStick = GetJoyStickAngle();

	//���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	VERTEX_2D* pVtx;			//���_���̃|�C���^
	g_pVtxBuffTutorial->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < TUTORIAL_MAX; nCnt++)
	{
		if (g_aTutorial[nCnt].bUse == false)
		{
			//�L�[��������ĂȂ��Ƃ�
			continue;
		}

		//���������̃t���O
		bool bPush = false;

		if (g_aTutorial[nCnt].nType == TUTORIAL_KEY_W)
		{
			if (GetKeyboardPress(DIK_W) == true)
			{
				//��������Ԃɂ���
				bPush = true;
			}
		}

		if (g_aTutorial[nCnt].nType == TUTORIAL_KEY_A)
		{
			if (GetKeyboardPress(DIK_A) == true)
			{
				//��������Ԃɂ���

				bPush = true;
			}
		}

		if (g_aTutorial[nCnt].nType == TUTORIAL_KEY_S)
		{
			if (GetKeyboardPress(DIK_S) == true)
			{
				//��������Ԃɂ���

				bPush = true;
			}
		}

		if (g_aTutorial[nCnt].nType == TUTORIAL_KEY_D)
		{
			if (GetKeyboardPress(DIK_D) == true)
			{
				//��������Ԃɂ���

				bPush = true;
			}
		}

		if (g_aTutorial[nCnt].nType == TUTORIAL_KEY_SPACE)
		{
			if (GetKeyboardPress(DIK_SPACE) == true)
			{
				//��������Ԃɂ���

				bPush = true;
			}
		}
		
		if (g_aTutorial[nCnt].nType == TUTORIAL_BUTTON_B)
		{
#if 0
			if (GetJoyStickRrepeat() == true)
			{
				//��������Ԃɂ���
				bPush = true;
			}
#endif
			if (JoypadTrigger(JOYKEY_B) == true)
			{
				//��������Ԃɂ���
				bPush = true;
			}
		}

		if (g_aTutorial[nCnt].nType == TUTORIAL_BUTTON_UP)
		{
			if (JoypadPress(JOYKEY_UP) == true)
			{
				//��������Ԃɂ���
				bPush = true;
			}
		}

		if (g_aTutorial[nCnt].nType == TUTORIAL_BUTTON_DOWN)
		{
			if (JoypadPress(JOYKEY_DOWN) == true)
			{
				//��������Ԃɂ���
				bPush = true;
			}
		}

		if (g_aTutorial[nCnt].nType == TUTORIAL_BUTTON_RIGHT)
		{
			if (JoypadPress(JOYKEY_RIGHT) == true)
			{
				//��������Ԃɂ���
				bPush = true;
			}
		}

		if (g_aTutorial[nCnt].nType == TUTORIAL_BUTTON_LEFT)
		{
			if (JoypadPress(JOYKEY_LEFT) == true)
			{
				//��������Ԃɂ���
				bPush = true;
			}
		}

		if (g_aTutorial[nCnt].nType == TUTORIAL_KEY_E)
		{
			if (GetKeyboardPress(DIK_E) == true)
			{
				//��������Ԃɂ���

				bPush = true;
			}
		}

		if (g_aTutorial[nCnt].nType == TUTORIAL_KEY_Q)
		{
			if (GetKeyboardPress(DIK_Q) == true)
			{
				//��������Ԃɂ���

				bPush = true;
			}
		}

		if (g_aTutorial[nCnt].nType == TUTORIAL_BUTTON_RB)
		{
			if (JoypadPress(JOYKEY_RIGHT_B) == true)
			{
				//��������Ԃɂ���
				bPush = true;
			}
		}

		if (g_aTutorial[nCnt].nType == TUTORIAL_BUTTON_LB)
		{
			if (JoypadPress(JOYKEY_LEFT_B) == true)
			{
				//��������Ԃɂ���
				bPush = true;
			}
		}

		if (g_aTutorial[nCnt].nType == TUTORIAL_LSTICK)
		{
			if (GetJoyStick() == true)
			{
				//��������Ԃɂ���
				bPush = true;
			}
		}

		if (g_bNext == true)
		{

			g_aTutorial[nCnt].pos += g_aTutorial[nCnt].move;

			//�ꍇ����
			switch (g_aTutorial[nCnt].nType)
			{
			case TUTORIAL_ONE:		//1��

				g_aTutorial[nCnt].fHeight = SIZE_HEIGHT;	//����
				g_aTutorial[nCnt].fWidth = 400.0f;			//����

				break;

			case TUTORIAL_ENTER:	//2��

				g_aTutorial[nCnt].fHeight = SIZE_HEIGHT;	//����
				g_aTutorial[nCnt].fWidth = 400.0f;			//����
				break;

			case TUTORIAL_KEY_W:	//3��

				g_aTutorial[nCnt].fHeight = SIZE_HEIGHT;	//����
				g_aTutorial[nCnt].fWidth = SIZE_WIDTH;		//����


				break;

			case TUTORIAL_KEY_A:	//4��

				g_aTutorial[nCnt].fHeight = SIZE_HEIGHT;	//����
				g_aTutorial[nCnt].fWidth = SIZE_WIDTH;		//����
				break;

			case TUTORIAL_KEY_S:	//5��

				g_aTutorial[nCnt].fHeight = SIZE_HEIGHT;	//����
				g_aTutorial[nCnt].fWidth = SIZE_WIDTH;		//����
				break;

			case TUTORIAL_KEY_D:	//6��

				g_aTutorial[nCnt].fHeight = SIZE_HEIGHT;	 //����
				g_aTutorial[nCnt].fWidth = SIZE_WIDTH;		 //����
				break;

			case TUTORIAL_KEY_SPACE://7��

				g_aTutorial[nCnt].fHeight = SIZE_HEIGHT;	//����
				g_aTutorial[nCnt].fWidth = 150.0f;			//����
				break;

			case TUTORIAL_BUTTON_B:	//8��

				g_aTutorial[nCnt].fHeight = 80.0f;			//����
				g_aTutorial[nCnt].fWidth = 80.0f;			//����
				break;

			case TUTORIAL_BUTTON_UP: //9��

				g_aTutorial[nCnt].fHeight = SIZE_HEIGHT;	//����
				g_aTutorial[nCnt].fWidth = SIZE_WIDTH;		//����
				break;

			case TUTORIAL_BUTTON_DOWN: //10��

				g_aTutorial[nCnt].fHeight = SIZE_HEIGHT;	//����
				g_aTutorial[nCnt].fWidth = SIZE_WIDTH;		//����
				break;

			case TUTORIAL_BUTTON_RIGHT: //11��

				g_aTutorial[nCnt].fHeight = SIZE_HEIGHT;	//����
				g_aTutorial[nCnt].fWidth = SIZE_WIDTH;		//����
				break;

			case TUTORIAL_BUTTON_LEFT: //12��

				g_aTutorial[nCnt].fHeight = SIZE_HEIGHT;	//����
				g_aTutorial[nCnt].fWidth = SIZE_WIDTH;		//����
				break;

			case TUTORIAL_KEY_P:	  //13��

				g_aTutorial[nCnt].fHeight = SIZE_HEIGHT;	//����
				g_aTutorial[nCnt].fWidth = SIZE_WIDTH;		//����
				break;

			case TUTORIAL_BUTTON_ST:	//14��

				g_aTutorial[nCnt].fHeight = 80.0f;			//����
				g_aTutorial[nCnt].fWidth = 80.0f;			//����
				break;

			case TUTORIAL_KEY_E:	//15��

				g_aTutorial[nCnt].fHeight = SIZE_HEIGHT;	//����
				g_aTutorial[nCnt].fWidth = SIZE_WIDTH;		//����
				break;

			case TUTORIAL_KEY_Q:	//16��

				g_aTutorial[nCnt].fHeight = SIZE_HEIGHT;	//����
				g_aTutorial[nCnt].fWidth = SIZE_WIDTH;		//����
				break;

			case TUTORIAL_BUTTON_RB:	//17��

				g_aTutorial[nCnt].fHeight = SIZE_HEIGHT;	//����
				g_aTutorial[nCnt].fWidth = 80.0f;			//����
				break;

			case TUTORIAL_BUTTON_LB:	//18��

				g_aTutorial[nCnt].fHeight = SIZE_HEIGHT;	//����
				g_aTutorial[nCnt].fWidth = 80.0f;			//����
				break;

			case TUTORIAL_LSTICK:	//19��

				g_aTutorial[nCnt].fHeight = 70.0f;			//����
				g_aTutorial[nCnt].fWidth = 70.0f;			//����
				break;

				break;
			}

			//���_���W1�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aTutorial[nCnt].pos.x - g_aTutorial[nCnt].fWidth * 0.5f, g_aTutorial[nCnt].pos.y - g_aTutorial[nCnt].fHeight * 0.5f, 0.0f);		//1�ڂ̒��_���
			pVtx[1].pos = D3DXVECTOR3(g_aTutorial[nCnt].pos.x + g_aTutorial[nCnt].fWidth * 0.5f, g_aTutorial[nCnt].pos.y - g_aTutorial[nCnt].fHeight * 0.5f, 0.0f);		//2�ڂ̒��_���
			pVtx[2].pos = D3DXVECTOR3(g_aTutorial[nCnt].pos.x - g_aTutorial[nCnt].fWidth * 0.5f, g_aTutorial[nCnt].pos.y + g_aTutorial[nCnt].fHeight * 0.5f, 0.0f);		//3�ڂ̒��_���
			pVtx[3].pos = D3DXVECTOR3(g_aTutorial[nCnt].pos.x + g_aTutorial[nCnt].fWidth * 0.5f, g_aTutorial[nCnt].pos.y + g_aTutorial[nCnt].fHeight * 0.5f, 0.0f);		//4�ڂ̒��_���

		}

		if (bPush == true)
		{
			//���_�J���[�̐ݒ�(��)
			pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		}
		else
		{
			//���_�J���[�̐ݒ�(��)
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		
		pVtx += 4;
	}


	//�A�����b�N
	g_pVtxBuffTutorial->Unlock();


	TutorialFlash(TUTORIAL_ENTER);		//�_��

	UpdateTutorial2();		//2�ڂ̃`���[�g���A���̍X�V
#endif
}
//===========================
// �`���[�g���A���̕`�揈��
//===========================
void DrawTutorial(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// �f�o�C�X�ւ̃|�C���^

//==============================
// �`���[�g���A���̔w�i�`��
//==============================

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTutorialBG, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureTutorialBG);

	// �`���[�g���A���|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

//==============================
// �`���[�g���A���̊e��`��
//==============================

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTutorial, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCnt = 0; nCnt < TUTORIAL_MAX; nCnt++)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureTutorial[g_aTutorial[nCnt].nType]);

		// �`���[�g���A���|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
	}

	// DrawTutorial2();	//2�ڂ̃`���[�g���A���̕`��
}
//==================================
// �`���[�g���A���̐ݒ�(�e�N�X�`��)
//==================================
void SetTutorial(int nType, D3DXVECTOR3 pos)
{
	VERTEX_2D* pVtx;//���_���̃|�C���^

	//���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffTutorial->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < TUTORIAL_MAX; nCnt++)
	{
		if (g_aTutorial[nCnt].bUse == false)
		{
			g_aTutorial[nCnt].nType = nType;
			g_aTutorial[nCnt].pos = pos;
			g_aTutorial[nCnt].bUse = true;

			//�ꍇ����
			switch (g_aTutorial[nCnt].nType)
			{
			case TUTORIAL_ONE:		//1��

				g_aTutorial[nCnt].fHeight = 50.0f;	//����
				g_aTutorial[nCnt].fWidth = 400.0f;	//����

				break;

			case TUTORIAL_ENTER:	//2��

				g_aTutorial[nCnt].fHeight = 50.0f;	//����
				g_aTutorial[nCnt].fWidth = 400.0f;  //����
				break;

			case TUTORIAL_KEY_W:	//3��

				g_aTutorial[nCnt].fHeight = 50.0f;	//����
				g_aTutorial[nCnt].fWidth = 50.0f;   //����


				break;

			case TUTORIAL_KEY_A:	//4��

				g_aTutorial[nCnt].fHeight = 50.0f; //����
				g_aTutorial[nCnt].fWidth = 50.0f;  //����
				break;

			case TUTORIAL_KEY_S:	//5��

				g_aTutorial[nCnt].fHeight = 50.0f; //����
				g_aTutorial[nCnt].fWidth = 50.0f;  //����
				break;

			case TUTORIAL_KEY_D:	//6��

				g_aTutorial[nCnt].fHeight = 50.0f; //����
				g_aTutorial[nCnt].fWidth = 50.0f;  //����
				break;

			case TUTORIAL_KEY_SPACE://7��

				g_aTutorial[nCnt].fHeight = 50.0f; //����
				g_aTutorial[nCnt].fWidth = 150.0f; //����
				break;

			case TUTORIAL_BUTTON_B:	//8��

				g_aTutorial[nCnt].fHeight = 80.0f; //����
				g_aTutorial[nCnt].fWidth = 80.0f;  //����
				break;

			case TUTORIAL_BUTTON_UP: //9��

				g_aTutorial[nCnt].fHeight = 50.0f; //����
				g_aTutorial[nCnt].fWidth = 50.0f;  //����
				break;

			case TUTORIAL_BUTTON_DOWN: //10��

				g_aTutorial[nCnt].fHeight = 50.0f; //����
				g_aTutorial[nCnt].fWidth = 50.0f;  //����
				break;

			case TUTORIAL_BUTTON_RIGHT: //11��

				g_aTutorial[nCnt].fHeight = 50.0f; //����
				g_aTutorial[nCnt].fWidth = 50.0f;  //����
				break;

			case TUTORIAL_BUTTON_LEFT: //12��

				g_aTutorial[nCnt].fHeight = 50.0f; //����
				g_aTutorial[nCnt].fWidth = 50.0f;  //����
				break;

			case TUTORIAL_KEY_P:	  //13��

				g_aTutorial[nCnt].fHeight = 50.0f; //����
				g_aTutorial[nCnt].fWidth = 50.0f;  //����
				break;

			case TUTORIAL_BUTTON_ST://14��

				g_aTutorial[nCnt].fHeight = 80.0f; //����
				g_aTutorial[nCnt].fWidth = 80.0f;  //����
				break;

			case TUTORIAL_KEY_E:	//15��

				g_aTutorial[nCnt].fHeight = SIZE_HEIGHT;	//����
				g_aTutorial[nCnt].fWidth = SIZE_WIDTH;		//����
				break;

			case TUTORIAL_KEY_Q:	//16��

				g_aTutorial[nCnt].fHeight = SIZE_HEIGHT;	//����
				g_aTutorial[nCnt].fWidth = SIZE_WIDTH;		//����
				break;

			case TUTORIAL_BUTTON_RB:	//17��

				g_aTutorial[nCnt].fHeight = SIZE_HEIGHT;	//����
				g_aTutorial[nCnt].fWidth = 80.0f;			//����
				break;
				
			case TUTORIAL_BUTTON_LB:	//18��

				g_aTutorial[nCnt].fHeight = SIZE_HEIGHT;	//����
				g_aTutorial[nCnt].fWidth = 80.0f;			//����
				break;

			case TUTORIAL_LSTICK:	//19��

				g_aTutorial[nCnt].fHeight = 70.0f;			//����
				g_aTutorial[nCnt].fWidth = 70.0f;			//����
				break;

				break;
			}

			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aTutorial[nCnt].pos.x - g_aTutorial[nCnt].fWidth * 0.5f, g_aTutorial[nCnt].pos.y - g_aTutorial[nCnt].fHeight * 0.5f, 0.0f);//1�ڂ̒��_���
			pVtx[1].pos = D3DXVECTOR3(g_aTutorial[nCnt].pos.x + g_aTutorial[nCnt].fWidth * 0.5f, g_aTutorial[nCnt].pos.y - g_aTutorial[nCnt].fHeight * 0.5f, 0.0f);//2�ڂ̒��_���
			pVtx[2].pos = D3DXVECTOR3(g_aTutorial[nCnt].pos.x - g_aTutorial[nCnt].fWidth * 0.5f, g_aTutorial[nCnt].pos.y + g_aTutorial[nCnt].fHeight * 0.5f, 0.0f);//3�ڂ̒��_���
			pVtx[3].pos = D3DXVECTOR3(g_aTutorial[nCnt].pos.x + g_aTutorial[nCnt].fWidth * 0.5f, g_aTutorial[nCnt].pos.y + g_aTutorial[nCnt].fHeight * 0.5f, 0.0f);//4�ڂ̒��_���

			break;
		}

		pVtx += 4;
	}

	// �A�����b�N
	g_pVtxBuffTutorial->Unlock();
}
//==========================
// �_�ł̏���
//==========================
void TutorialFlash(int nType)
{
	VERTEX_2D* pVtx = 0;		// ���_���̃|�C���^

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffTutorial->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < TUTORIAL_MAX; nCnt++)
	{
		if (g_aTutorial[nCnt].bUse == true && g_aTutorial[nCnt].state == TUTORIALSTATE_FLASH)
		{
			g_nCntTutoAnim++;			// �J�E���^�[�����Z

			if (g_nCntTutoAnim == 5)	// 5�̎�
			{
				// ���_�J���[�̐ݒ�
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.2f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.2f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.2f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.2f);

			}

			else if (g_nCntTutoAnim == 10)	// 10�̎�
			{
				// ���_�J���[�̐ݒ�
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				g_nCntTutoAnim = 0;		// �������ɖ߂�
			}
		}
		pVtx += 4;
	}
	// �A�����b�N
	g_pVtxBuffTutorial->Unlock();
}
//==========================
// ���̉�ʂ̎擾
//==========================
bool GetIsNext(void)
{
	return g_bNext;
}
//==========================
// ���̉�ʂւ̈ړ��ʂ̎擾
//==========================
int GetTutorialCountMove(void)
{
	return g_nCntMove;
}