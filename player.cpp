//====================================
//
// �v���C���[���� [player.cpp]
// Author:Asuma Nishio
//
//====================================

#include "player.h"
#include "input.h"
// #include "bullet.h"
// #include "explosion.h"
#include "fade.h"
#include "bullet.h"
#include "bulletnum.h"
#include "time.h"
#include "sound.h"
//#include "effect.h"
//#include "particle.h"
//#include "sound.h"

//*********************************
// �}�N����`
//*********************************
#define MAX_LIFEBARSIZE_X (400)		// ����
#define MAX_LIFEBARSIZE_Y (20)		// ����
									   
#define MAX_TOP_POS       (20)		// ��ɍs����ő�Y���W
#define MAX_UNDER_POS     (700)		// ���ɍs����ő�Y���W

//�O���[�o���ϐ�
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffplayer = NULL;		// ���_�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureplayer = NULL;				// �e�N�X�`���̃|�C���^										   
Player g_aPlayer;										// �v���C���[�̏��
														   
int g_nApperCnt = 0;									// �v���C���[�̖��G�J�E���g

//=========================
// �v���C���[�̏���������
//=========================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^

	// ���_���̃|�C���^
	VERTEX_2D* pVtx;		

	// �e�N�X�`��2�̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\player.png",
		&g_pTextureplayer);

	// �\���̕ϐ��̏�����
	g_aPlayer.pos =  D3DXVECTOR3(640.0f,610.0f,0.0f);		// �ʒu�̏�����
	g_aPlayer.move = D3DXVECTOR3(0.0f,0.0f,0.0f);			// �ړ��ʂ̏�����
	g_aPlayer.rot =  D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �����̏�����
	g_aPlayer.nLife = 100;									// �̗�
	g_aPlayer.nCntAnimState = 0;							// �q�b�g��Ԃ̃J�E���g�̏�����
	g_aPlayer.bHit = false;									// �����蔻��
	g_aPlayer.nCntState = 0;								// �_���[�W�J�E���g�̏�����
	g_aPlayer.state = PLAYERSTATE_APPEAR;					// �v���C���[�̏������
	g_aPlayer.bDisp = true;									// �v���C���[�̎g�p����
	g_nApperCnt = 0;										// ���G����̃J�E���g��������

	//�Ίp���̒������Z�o
	g_aPlayer.fLength = sqrtf((MAX_SIZE_X * MAX_SIZE_X) + (MAX_SIZE_Y * MAX_SIZE_Y)) / 2.0f;
	
	//�Ίp���̊p�x���Z�o
	g_aPlayer.fAngle = atan2f(MAX_SIZE_X,MAX_SIZE_Y);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY, FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffplayer,
		NULL);

	//���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffplayer->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos.x = g_aPlayer.pos.x + sinf(g_aPlayer.rot.z + (-D3DX_PI + g_aPlayer.fAngle)) * g_aPlayer.fLength;
	pVtx[0].pos.y = g_aPlayer.pos.y + cosf(g_aPlayer.rot.z + (-D3DX_PI + g_aPlayer.fAngle)) * g_aPlayer.fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_aPlayer.pos.x + sinf(g_aPlayer.rot.z + (D3DX_PI - g_aPlayer.fAngle)) * g_aPlayer.fLength;
	pVtx[1].pos.y = g_aPlayer.pos.y + cosf(g_aPlayer.rot.z + (D3DX_PI - g_aPlayer.fAngle)) * g_aPlayer.fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_aPlayer.pos.x + sinf(g_aPlayer.rot.z + (0.0f - g_aPlayer.fAngle)) * g_aPlayer.fLength;
	pVtx[2].pos.y = g_aPlayer.pos.y + cosf(g_aPlayer.rot.z + (0.0f - g_aPlayer.fAngle)) * g_aPlayer.fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_aPlayer.pos.x + sinf(g_aPlayer.rot.z + (0.0f + g_aPlayer.fAngle)) * g_aPlayer.fLength;
	pVtx[3].pos.y = g_aPlayer.pos.y + cosf(g_aPlayer.rot.z + (0.0f + g_aPlayer.fAngle)) * g_aPlayer.fLength;
	pVtx[3].pos.z = 0.0f;

	//rhw�̐ݒ�(1.0f�ŌŒ�)
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(SPLIT_U, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, SPLIT_V);
	pVtx[3].tex = D3DXVECTOR2(SPLIT_U, SPLIT_V);

	//�A�����b�N
	g_pVtxBuffplayer->Unlock();

}
//======================
// �v���C���[�̏I������
//======================
void UninitPlayer(void)
{
	// ���y��~
	StopSound();

	// �e�N�X�`���̔j��
	if (g_pTextureplayer != NULL)
	{
		g_pTextureplayer->Release();
		g_pTextureplayer = NULL;
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffplayer != NULL)
	{
		g_pVtxBuffplayer->Release();
		g_pVtxBuffplayer = NULL;
	}
}
//=====================
// �v���C���[�̍X�V����
//=====================
void UpdatePlayer(void)
{
	// ���_���̃|�C���^
	VERTEX_2D* pVtx;	

	if (GetBulletNum() > 0)
	{
		// �L�[���͏��̎擾
		GetKeyPlayer();

		// �p�b�h�����擾
		GetJoypadPlayer();

		// �X�e�B�b�N���̎擾
		StickState();
	}

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffplayer->Lock(0, 0, (void**)&pVtx, 0);

	switch (g_aPlayer.state)
	{
	case PLAYERSTATE_NORMAL:
		break;

	case PLAYERSTATE_DAMAGE:
		g_aPlayer.nCntState--;

		if (g_aPlayer.nCntState <= 0)
		{
			g_aPlayer.state = PLAYERSTATE_APPEAR;
			g_aPlayer.nCntState = 80;

			// ���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		}
		break;

	case PLAYERSTATE_APPEAR:
		{
			// ���G����
			float farufa = 1.0f;
			g_aPlayer.nCntState--;

			if (g_aPlayer.nCntState % 5 == 0)
			{
				// �����ɂ���
				farufa = 0.0f;
			}
			else
			{
				// �ʏ�F
				farufa = 1.0f;
			}

			if (g_aPlayer.nCntState <= 0)
			{
				// �v���C���[��ʏ��Ԃ�
				farufa = 1.0f;
				g_aPlayer.state = PLAYERSTATE_NORMAL;
			}

			// ���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, farufa);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, farufa);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, farufa);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, farufa);

			break;
		}
	}

	// �A�����b�N
	g_pVtxBuffplayer->Unlock();

}
//======================
// �v���C���[�̕`�揈��
//======================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// �f�o�C�X�ւ̃|�C���^

	if (g_aPlayer.bDisp)
	{
		// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, g_pVtxBuffplayer, 0, sizeof(VERTEX_2D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureplayer);

		// �v���C���[�̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}

}
//====================
// �L�[���͏��̎擾
//====================
void GetKeyPlayer(void)
{
	VERTEX_2D* pVtx;		// ���_���̃|�C���^

	// A�L�[����������
	if (GetKeyboardPress(DIK_A) == true)
	{
		// A�P�̂̏���
		g_aPlayer.move.x -= 2.0f;
	}
	// D����������
	else if (GetKeyboardPress(DIK_D) == true)
	{
		// D�P�̂̏���
		g_aPlayer.move.x += 2.0f;
	}


	if (KeyboardTrigger(DIK_RETURN))
	{// ����L�[
		// �e�̐ݒ�
		SetBullet(g_aPlayer.pos, D3DXVECTOR3(sinf(g_aPlayer.rot.z + D3DX_PI) * 10.0f, cosf(g_aPlayer.rot.z + D3DX_PI) * 10.0f, 0.0f), g_aPlayer.rot, 30.0f, 30.0f, 70, BULLETTYPE_PLAYER);
		DecBulletNum(1);

		// ���y�Đ�
		PlaySound(SOUND_LABEL_PLAYERBULLET_SE);
	}

	// �ʒu���X�V
	g_aPlayer.pos.x += g_aPlayer.move.x;
	g_aPlayer.pos.y += g_aPlayer.move.y;

	// �E��
	if (g_aPlayer.pos.x >= MAX_RIGHT_POS)
	{
		g_aPlayer.pos.x = MAX_RIGHT_POS;
		g_aPlayer.move.x = 0.0f;

	}
	// ����
	else if (g_aPlayer.pos.x <= MAX_LEFT_POS)
	{
		g_aPlayer.pos.x = MAX_LEFT_POS;
		g_aPlayer.move.x = 0.0f;
	}

	// �ړ��ʂ̍X�V(����������)
	g_aPlayer.move.x += (0.0f - g_aPlayer.move.x) * 0.25f;
	g_aPlayer.move.y += (0.0f - g_aPlayer.move.y) * 0.25f;

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffplayer->Lock(0, 0, (void**)&pVtx, 0);

	if (g_aPlayer.nCntAnimState >= 5)
	{
		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		g_aPlayer.nCntAnimState = 0;			// ������Ԃɖ߂�
		g_aPlayer.bHit = false;					// ����𖢎g�p��Ԃɂ���
	}

	// ���_���W�̐ݒ�
	pVtx[0].pos.x = g_aPlayer.pos.x + sinf(g_aPlayer.rot.z + (-D3DX_PI + g_aPlayer.fAngle)) * g_aPlayer.fLength;
	pVtx[0].pos.y = g_aPlayer.pos.y + cosf(g_aPlayer.rot.z + (-D3DX_PI + g_aPlayer.fAngle)) * g_aPlayer.fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_aPlayer.pos.x + sinf(g_aPlayer.rot.z + (D3DX_PI - g_aPlayer.fAngle)) * g_aPlayer.fLength;
	pVtx[1].pos.y = g_aPlayer.pos.y + cosf(g_aPlayer.rot.z + (D3DX_PI - g_aPlayer.fAngle)) * g_aPlayer.fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_aPlayer.pos.x + sinf(g_aPlayer.rot.z + (0.0f - g_aPlayer.fAngle)) * g_aPlayer.fLength;
	pVtx[2].pos.y = g_aPlayer.pos.y + cosf(g_aPlayer.rot.z + (0.0f - g_aPlayer.fAngle)) * g_aPlayer.fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_aPlayer.pos.x + sinf(g_aPlayer.rot.z + (0.0f + g_aPlayer.fAngle)) * g_aPlayer.fLength;
	pVtx[3].pos.y = g_aPlayer.pos.y + cosf(g_aPlayer.rot.z + (0.0f + g_aPlayer.fAngle)) * g_aPlayer.fLength;
	pVtx[3].pos.z = 0.0f;

	// �A�����b�N
	g_pVtxBuffplayer->Unlock();
}
//=================================
// �p�b�h�̏���
//=================================
void GetJoypadPlayer(void)
{
	VERTEX_2D* pVtx;			// ���_���̃|�C�^

	if (JoypadPress(JOYKEY_LEFT) == true)
	{
		// ���L�[�������ꂽ
		g_aPlayer.move.x -= 2.0f;
	}
	else if (JoypadPress(JOYKEY_RIGHT) == true)
	{
		// �E�L�[�������ꂽ
		g_aPlayer.move.x += 2.0f;
	}


	if (JoypadTrigger(JOYKEY_Y) == true)
	{// B�{�^����������
		// �e�̐ݒ�
		SetBullet(g_aPlayer.pos, D3DXVECTOR3(sinf(g_aPlayer.rot.z + D3DX_PI) * 10.0f, cosf(g_aPlayer.rot.z + D3DX_PI) * 10.0f, 0.0f), g_aPlayer.rot, 30.0f, 30.0f, 70, BULLETTYPE_PLAYER);
		DecBulletNum(1);
	}
	//else if (JoypadTrigger(JOYKEY_X) == true)
	//{// X�{�^����������
	//	 �e�̐ݒ�
	//	SetBullet(g_aPlayer.pos, D3DXVECTOR3(sinf(g_aPlayer.rot.z - D3DX_PI * 0.5f) * 10.0f, cosf(g_aPlayer.rot.z - D3DX_PI * 0.5f) * 10.0f, 0.0f), g_aPlayer.rot, 30.0f, 30.0f, 70, BULLETTYPE_PLAYER);
	//}
	//else if (JoypadTrigger(JOYKEY_Y) == true)
	//{// Y�{�^����������
	//	 �e�̐ݒ�
	//	SetBullet(g_aPlayer.pos, D3DXVECTOR3(sinf(g_aPlayer.rot.z + D3DX_PI) * 10.0f, cosf(g_aPlayer.rot.z + D3DX_PI) * 10.0f, 0.0f), g_aPlayer.rot, 30.0f, 30.0f, 70, BULLETTYPE_PLAYER);
	//}
	//else if (JoypadTrigger(JOYKEY_A) == true)
	//{// A�{�^��
	//	 �e�̐ݒ�
	//	SetBullet(g_aPlayer.pos, D3DXVECTOR3(-sinf(g_aPlayer.rot.z + D3DX_PI) * 10.0f, -cosf(g_aPlayer.rot.z + D3DX_PI) * 10.0f, 0.0f), g_aPlayer.rot, 30.0f, 30.0f, 70, BULLETTYPE_PLAYER);
	//}


	// �ʒu���X�V
	g_aPlayer.pos.x += g_aPlayer.move.x;
	g_aPlayer.pos.y += g_aPlayer.move.y;

	// �ړ��ʂ̍X�V(����������)
	g_aPlayer.move.x += (0.0f - g_aPlayer.move.x) * 0.25f;
	g_aPlayer.move.y += (0.0f - g_aPlayer.move.y) * 0.25f;

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffplayer->Lock(0, 0, (void**)&pVtx, 0);

	if (g_aPlayer.nCntAnimState >= 5)
	{
		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		g_aPlayer.nCntAnimState = 0;		// �J�E���^�[��0�ɂ���
		g_aPlayer.bHit = false;				// ���g�p����
	}

	// ���_���W�̐ݒ�
	pVtx[0].pos.x = g_aPlayer.pos.x + sinf(g_aPlayer.rot.z + (-D3DX_PI + g_aPlayer.fAngle)) * g_aPlayer.fLength;
	pVtx[0].pos.y = g_aPlayer.pos.y + cosf(g_aPlayer.rot.z + (-D3DX_PI + g_aPlayer.fAngle)) * g_aPlayer.fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_aPlayer.pos.x + sinf(g_aPlayer.rot.z + (D3DX_PI - g_aPlayer.fAngle)) * g_aPlayer.fLength;
	pVtx[1].pos.y = g_aPlayer.pos.y + cosf(g_aPlayer.rot.z + (D3DX_PI - g_aPlayer.fAngle)) * g_aPlayer.fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_aPlayer.pos.x + sinf(g_aPlayer.rot.z + (0.0f - g_aPlayer.fAngle)) * g_aPlayer.fLength;
	pVtx[2].pos.y = g_aPlayer.pos.y + cosf(g_aPlayer.rot.z + (0.0f - g_aPlayer.fAngle)) * g_aPlayer.fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_aPlayer.pos.x + sinf(g_aPlayer.rot.z + (0.0f + g_aPlayer.fAngle)) * g_aPlayer.fLength;
	pVtx[3].pos.y = g_aPlayer.pos.y + cosf(g_aPlayer.rot.z + (0.0f + g_aPlayer.fAngle)) * g_aPlayer.fLength;
	pVtx[3].pos.z = 0.0f;

	// �A�����b�N
	g_pVtxBuffplayer->Unlock();

}
//================================
// �X�e�B�b�N���
//================================
void StickState(void)
{
	// ���_���̃|�C�^
	VERTEX_2D* pVtx;

	// �X�e�B�b�N���̎擾
	XINPUT_STATE* pStick;
	pStick = GetJoyStickAngle();

	if (GetJoyStick() == true)
	{
		if (pStick->Gamepad.sThumbLX > 10922)
		{
			// �E�ړ�
			g_aPlayer.move.x += 2.0f;
		}
		if (pStick->Gamepad.sThumbLX < -10922)
		{
			// ���ړ�
			g_aPlayer.move.x -= 2.0f;
		}
	}

	// �ʒu���X�V
	g_aPlayer.pos.x += g_aPlayer.move.x;
	g_aPlayer.pos.y += g_aPlayer.move.y;

	// �ړ��ʂ̍X�V(����������)
	g_aPlayer.move.x += (0.0f - g_aPlayer.move.x) * 0.25f;
	g_aPlayer.move.y += (0.0f - g_aPlayer.move.y) * 0.25f;

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffplayer->Lock(0, 0, (void**)&pVtx, 0);

	if (g_aPlayer.nCntAnimState >= 5)
	{
		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		g_aPlayer.nCntAnimState = 0;		//�J�E���^�[��0�ɂ���
		g_aPlayer.bHit = false;				//���g�p����
	}

	// ���_���W�̐ݒ�
	pVtx[0].pos.x = g_aPlayer.pos.x + sinf(g_aPlayer.rot.z + (-D3DX_PI + g_aPlayer.fAngle)) * g_aPlayer.fLength;
	pVtx[0].pos.y = g_aPlayer.pos.y + cosf(g_aPlayer.rot.z + (-D3DX_PI + g_aPlayer.fAngle)) * g_aPlayer.fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_aPlayer.pos.x + sinf(g_aPlayer.rot.z + (D3DX_PI - g_aPlayer.fAngle)) * g_aPlayer.fLength;
	pVtx[1].pos.y = g_aPlayer.pos.y + cosf(g_aPlayer.rot.z + (D3DX_PI - g_aPlayer.fAngle)) * g_aPlayer.fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_aPlayer.pos.x + sinf(g_aPlayer.rot.z + (0.0f - g_aPlayer.fAngle)) * g_aPlayer.fLength;
	pVtx[2].pos.y = g_aPlayer.pos.y + cosf(g_aPlayer.rot.z + (0.0f - g_aPlayer.fAngle)) * g_aPlayer.fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_aPlayer.pos.x + sinf(g_aPlayer.rot.z + (0.0f + g_aPlayer.fAngle)) * g_aPlayer.fLength;
	pVtx[3].pos.y = g_aPlayer.pos.y + cosf(g_aPlayer.rot.z + (0.0f + g_aPlayer.fAngle)) * g_aPlayer.fLength;
	pVtx[3].pos.z = 0.0f;

	// �A�����b�N
	g_pVtxBuffplayer->Unlock();

}
//===================
//�v���C���[�̎擾
//===================
Player* GetPlayer(void)
{
	return &g_aPlayer;
}

