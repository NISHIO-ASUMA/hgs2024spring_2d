//=============================
//
// �e�̏��� [bullet.cpp]
// Author:Asuma Nishio
//
//=============================

//************************************
// �C���N���[�h
//***********************************
#include "bullet.h"
#include "player.h"
#include "main.h"
#include "block.h"
//#include "explosion.h"
//#include "effect.h"
//#include "particle.h"
//#include "sound.h"

//************************************
// �}�N����`
//***********************************
#define MAX_BULLET (128)			// �e�̍ő吔
#define MAX_BULLETSIZE_X (15.0f)	// �e�̃T�C�Y
#define MAX_BULLETSIZE_Y (15.0f)	// �e�̃T�C�Y

//�e�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;			// �ʒu
	D3DXVECTOR3 move;			// �ړ���
	D3DXVECTOR3 rot;			// z��
	int nLife;					// �e�̎���
	bool bUse;					// �g�p���Ă��邩�ǂ���
	BULLETTYPE type;			// �e�̎��
	float fAngle;				// �p�x
	float fLength;				// �Ίp���̒���
	int nBulletType;			// �e�̎��
	int nBulletNum;				// �e�̐�
}Bullet;

//�O���[�o���ϐ�
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;        //���_�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureBullet[BULLETTYPE_MAX] = {};   //�e�N�X�`���̃|�C���^
Bullet g_aBullet[MAX_BULLET];							//�e�̏��

//=========================
// �e�̏���������
//=========================
void InitBullet(void)
{
	// �f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	

	for (int nCnt = 0; nCnt < BULLETTYPE_MAX; nCnt++)
	{
		//�e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			BULLET_TEXTURE[nCnt],
			&g_pTextureBullet[nCnt]);
	}

	// �e���̏�����
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ���W
		g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ړ���
		g_aBullet[nCntBullet].nLife = 100;								// �̗�
		g_aBullet[nCntBullet].bUse = false;								// �g�p���ĂȂ���Ԃɂ���
		BULLETTYPE type = {};											// �^�C�v
		g_aBullet[nCntBullet].rot = D3DXVECTOR3(0.0f,0.0f,0.0f);		// �p�x
		g_aBullet[nCntBullet].nBulletType = 0;							// �e�̎��

		//�Ίp���̒������Z�o
		g_aBullet[nCntBullet].fLength = sqrtf((MAX_BULLETSIZE_X * MAX_BULLETSIZE_X) + (MAX_BULLETSIZE_Y * MAX_BULLETSIZE_Y)) / 2.0f;

		//�Ίp���̊p�x���Z�o
		g_aBullet[nCntBullet].fAngle = atan2f(MAX_BULLETSIZE_X, MAX_BULLETSIZE_Y);

	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BULLET,
		D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL);

	VERTEX_2D* pVtx;//���_���̃|�C���^

	//���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f ,0.0f);	//1�ڂ̒��_���
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//2�ڂ̒��_���
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//3�ڂ̒��_���
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//4�ڂ̒��_���

		//rhw�̐ݒ�(1.0f�ŌŒ�)
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;	//���_�f�[�^�̃|�C���^��4���i�߂�
	}
	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffBullet->Unlock();
}
//========================
// �e�̏I������
//========================
void UninitBullet(void)
{
	// StopSound();	// �T�E���h�̒�~

	// �e�N�X�`���̔j��
	for (int nCnt = 0; nCnt < BULLETTYPE_MAX; nCnt++)
	{
		if (g_pTextureBullet[nCnt] != NULL)
		{
			g_pTextureBullet[nCnt]->Release();
			g_pTextureBullet[nCnt] = NULL;
		}

	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}

}
//=============================
// �e�̍X�V����
//=============================
void UpdateBullet(void)
{
	VERTEX_2D* pVtx ; // ���_���̃|�C���^

	// �v���C���[�̎擾
	Player* pPlayer = GetPlayer();

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse)
		{
			if (g_aBullet[nCntBullet].type == BULLETTYPE_PLAYER)
			{// ��ނ��v���C���[
				// �u���b�N�̎擾
				Block* pBlock = GetBlockInfo();

				for (int nCntEnemy = 0; nCntEnemy < MAX_BLOCK; nCntEnemy++, pBlock++)
				{
					if (pBlock->bUse == true)
					{
						// �v���C���[���g�p����Ă���
						if (
							g_aBullet[nCntBullet].pos.x >= pBlock->pos.x - 30.0f &&
							g_aBullet[nCntBullet].pos.x <= pBlock->pos.x + 30.0f &&	// ��U���l
							g_aBullet[nCntBullet].pos.y >= pBlock->pos.y - 30.0f &&
							g_aBullet[nCntBullet].pos.y <= pBlock->pos.y + 30.0f)
						{
							// TODO : �u���b�N�ɓ����鏈��
							HitBlock(nCntEnemy, 1);

							g_aBullet[nCntBullet].bUse = false;
						}
					}
				}
			}
			else if (g_aBullet[nCntBullet].type == BULLETTYPE_BLOCK)
			{// ��ނ��u���b�N

				// �u���b�N�̎擾
				Block* pBlock = GetBlockInfo();

				for (int nCntEnemy = 0; nCntEnemy < MAX_BLOCK; nCntEnemy++, pBlock++)
				{
					if (pBlock->bUse == true)
					{
						// �v���C���[���g�p����Ă���
						if (
							g_aBullet[nCntBullet].pos.x >= pBlock->pos.x - 30.0f &&
							g_aBullet[nCntBullet].pos.x <= pBlock->pos.x + 30.0f &&	// ��U���l
							g_aBullet[nCntBullet].pos.y >= pBlock->pos.y - 30.0f &&
							g_aBullet[nCntBullet].pos.y <= pBlock->pos.y + 30.0f)
						{
							// TODO : �u���b�N�ɓ����鏈��
							HitBlock(nCntEnemy, 1);
						}
					}
				}
			}

			// �e�̈ʒu�X�V����
			g_aBullet[nCntBullet].pos.x += g_aBullet[nCntBullet].move.x;
			g_aBullet[nCntBullet].pos.y += g_aBullet[nCntBullet].move.y;

			// ���_���W�̍X�V
			pVtx[0].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z + (-D3DX_PI + g_aBullet[nCntBullet].fAngle)) * g_aBullet[nCntBullet].fLength;
			pVtx[0].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z + (-D3DX_PI + g_aBullet[nCntBullet].fAngle)) * g_aBullet[nCntBullet].fLength;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z + (D3DX_PI - g_aBullet[nCntBullet].fAngle)) * g_aBullet[nCntBullet].fLength;
			pVtx[1].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z + (D3DX_PI - g_aBullet[nCntBullet].fAngle)) * g_aBullet[nCntBullet].fLength;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z + (0.0f - g_aBullet[nCntBullet].fAngle)) * g_aBullet[nCntBullet].fLength;
			pVtx[2].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z + (0.0f - g_aBullet[nCntBullet].fAngle)) * g_aBullet[nCntBullet].fLength;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z + (0.0f + g_aBullet[nCntBullet].fAngle)) * g_aBullet[nCntBullet].fLength;
			pVtx[3].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z + (0.0f + g_aBullet[nCntBullet].fAngle)) * g_aBullet[nCntBullet].fLength;
			pVtx[3].pos.z = 0.0f;

			g_aBullet[nCntBullet].nLife--;//�̗͂����炷

			if (g_aBullet[nCntBullet].nLife <= 0)//�����s����
			{
				g_aBullet[nCntBullet].bUse = false;//���g�p���
			}
		}
		
		pVtx += 4; // 4���i�߂�
	}
	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffBullet->Unlock();

}
//===========================
//�e�̕`�揈��
//===========================
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		//�e���g�p����Ă���
		if (g_aBullet[nCntBullet].bUse)
		{
			int nType = g_aBullet[nCntBullet].type;

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureBullet[nType]);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBullet * 4, 2); // �v���C���[�̎n�_(0�Ԗځ`3�Ԗ�)

		}
	}
}
//============================
//�e�̐ݒ菈��
//============================
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, float fWidth, float fHeight, int nLife, BULLETTYPE type)
{
	VERTEX_2D* pVtx; // ���_���̃|�C���^

	//���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
 	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == false)
		{
			//�Ίp���̒������Z�o
			g_aBullet[nCntBullet].fLength = sqrtf((fWidth * fWidth) + (fHeight * fHeight)) / 2.0f;

			//�Ίp���̊p�x���Z�o
			g_aBullet[nCntBullet].fAngle = atan2f(fWidth, fHeight);

			//�e���g���Ă��Ȃ�
			g_aBullet[nCntBullet].pos = pos;		//���W
			g_aBullet[nCntBullet].rot = rot;		//�p�x
			g_aBullet[nCntBullet].move = move;		//�ړ���
			g_aBullet[nCntBullet].nLife = nLife;	//�̗�
			g_aBullet[nCntBullet].type = type;		//���
			g_aBullet[nCntBullet].bUse = true;		//�g�p��Ԃɂ���
			
			//���_���W�̐ݒ�
			pVtx[0].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z + (-D3DX_PI + g_aBullet[nCntBullet].fAngle)) * g_aBullet[nCntBullet].fLength;
			pVtx[0].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z + (-D3DX_PI + g_aBullet[nCntBullet].fAngle)) * g_aBullet[nCntBullet].fLength;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z + (D3DX_PI - g_aBullet[nCntBullet].fAngle)) * g_aBullet[nCntBullet].fLength;
			pVtx[1].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z + (D3DX_PI - g_aBullet[nCntBullet].fAngle)) * g_aBullet[nCntBullet].fLength;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z + (0.0f - g_aBullet[nCntBullet].fAngle)) * g_aBullet[nCntBullet].fLength;
			pVtx[2].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z + (0.0f - g_aBullet[nCntBullet].fAngle)) * g_aBullet[nCntBullet].fLength;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z + (0.0f + g_aBullet[nCntBullet].fAngle)) * g_aBullet[nCntBullet].fLength;
			pVtx[3].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z + (0.0f + g_aBullet[nCntBullet].fAngle)) * g_aBullet[nCntBullet].fLength;
			pVtx[3].pos.z = 0.0f;

			//rhw�̐ݒ�(1.0f�ŌŒ�)
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			break;
		}

		pVtx += 4;	//4���ړ�
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffBullet->Unlock();

}

