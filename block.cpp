//===============================
//
// �u���b�N�̏��� [ block.cpp ]
// Author:Asuma Nishio
//
//===============================

//******************************
// �C���N���[�h�t�@�C���錾
//******************************
#include "block.h"
#include <stdio.h>
#include "bullet.h"
#include "explosion.h"
#include "player.h"
#include "score.h"
#include "bulletnum.h"

//**************************
// �O���[�o���ϐ�
//**************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBlock = NULL;         // ���_�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureBlock[BLOCKTYPE_MAX] = {};	// �e�N�X�`���̃|�C���^
Block g_aBlock[MAX_BLOCK];								// �u���b�N�̏��
int g_nCntblock;										// �u���b�N�̎擾
int g_nWave;											// �E�F�[�u�J�E���g
bool g_bFinish = false;
//**************************
// �v���g�^�C�v�錾
//**************************
void InitStruct();    // �\���̏��������

//=========================
// �u���b�N�̏���������
//=========================
void InitBlock(void)
{
	// �f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	// �e�N�X�`���ǂݍ���
	for (int nTex = 0; nTex < BLOCKTYPE_MAX; nTex++)
	{
		D3DXCreateTextureFromFile(pDevice,
			BLOCK_TEXTURE[nTex],
			&g_pTextureBlock[nTex]);
	}

	// �\���̏��������
	InitStruct();

	g_nCntblock = 0;
	g_nWave = 0;
	g_bFinish = false;

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BLOCK,
		D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBlock,
		NULL);

	// ���_���̃|�C���^
	VERTEX_2D* pVtx;	

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);	// 1�ڂ̒��_���
		pVtx[1].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);	// 2�ڂ̒��_���
		pVtx[2].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);	// 3�ڂ̒��_���
		pVtx[3].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);	// 4�ڂ̒��_���

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

		// ���_�f�[�^�̃|�C���^��4���i�߂�
		pVtx += 4;	
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffBlock->Unlock();
}
//========================
// �u���b�N�̏I������
//========================
void UninitBlock(void)
{
	// �e�N�X�`���̔j��
	for (int nCnt = 0; nCnt < BLOCKTYPE_MAX;nCnt++)
	{
		if (g_pTextureBlock[nCnt] != NULL)
		{
			g_pTextureBlock[nCnt]->Release();
			g_pTextureBlock[nCnt] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffBlock != NULL)
	{
		g_pVtxBuffBlock->Release();
		g_pVtxBuffBlock = NULL;
	}
}
//=============================
// �u���b�N�̍X�V����
//=============================
void UpdateBlock(void)
{
	// ���_���̃|�C���^
	VERTEX_2D* pVtx;		

	//�@���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_BLOCK; nCnt++)
	{
		if (g_aBlock[nCnt].nType == BLOCKTYPE_MOVEBLOCK)
		{// �^�C�v�������u���b�N�Ȃ�
			// �c�̈ړ��ʂ�0�ɂ���
			g_aBlock[nCnt].move.y = 0.0f;

			// ���E�ɓ���
			if (g_aBlock[nCnt].bRight == true)
			{
				// �E�ړ���true
				g_aBlock[nCnt].move.x = 5.0f;

				if (g_aBlock[nCnt].pos.x >= SCREEN_WIDTH - g_aBlock[nCnt].fWidth)
				{
					g_aBlock[nCnt].bRight = false;
				}
			}
			else
			{
				g_aBlock[nCnt].move.x = -5.0f;
				
				if (g_aBlock[nCnt].pos.x <= g_aBlock[nCnt].fWidth)
				{// �������[�ɂ���
					g_aBlock[nCnt].bRight = true;
				}

			}
		}

		// �ړ��ʂ̌���
		g_aBlock[nCnt].move.x += (0.0f - g_aBlock[nCnt].move.x) * 0.25f;
		g_aBlock[nCnt].move.y += (0.0f - g_aBlock[nCnt].move.y) * 0.25f;

		// �ړ��ʂ̍X�V
		g_aBlock[nCnt].pos.x += g_aBlock[nCnt].move.x;
		g_aBlock[nCnt].pos.y += g_aBlock[nCnt].move.y;

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aBlock[nCnt].pos.x - g_aBlock[nCnt].fWidth, g_aBlock[nCnt].pos.y - g_aBlock[nCnt].fHeight,0.0f);	// 1�ڂ̒��_���
		pVtx[1].pos = D3DXVECTOR3(g_aBlock[nCnt].pos.x + g_aBlock[nCnt].fWidth, g_aBlock[nCnt].pos.y - g_aBlock[nCnt].fHeight,0.0f);	// 2�ڂ̒��_���
		pVtx[2].pos = D3DXVECTOR3(g_aBlock[nCnt].pos.x - g_aBlock[nCnt].fWidth, g_aBlock[nCnt].pos.y + g_aBlock[nCnt].fHeight,0.0f);	// 3�ڂ̒��_���
		pVtx[3].pos = D3DXVECTOR3(g_aBlock[nCnt].pos.x + g_aBlock[nCnt].fWidth, g_aBlock[nCnt].pos.y + g_aBlock[nCnt].fHeight,0.0f);	// 4�ڂ̒��_���

		// ���_��񕪂��炷
		pVtx += 4;
	}

	// �A�����b�N
	g_pVtxBuffBlock->Unlock();
}
//===========================
// �u���b�N�̕`�揈��
//===========================
void DrawBlock(void)
{
	// �f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBlock, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == true)
		{// �g�p���
			// ��ނ�ۑ�
			int nType = g_aBlock[nCntBlock].nType;

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureBlock[nType]);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBlock * 4, 2);
		}
	}
}
//============================
// �u���b�N�̐ݒ菈��
//============================
void SetBlock(D3DXVECTOR3 pos,int nType,float fWidth,float fHeight) 
{
	// ���_���̃|�C���^
	VERTEX_2D* pVtx;

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
 	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (!g_aBlock[nCntBlock].bUse)
		{
			g_aBlock[nCntBlock].pos = pos;			// ���W
			g_aBlock[nCntBlock].nType = nType;		// ���
			g_aBlock[nCntBlock].fWidth = fWidth;	// ����
			g_aBlock[nCntBlock].fHeight = fHeight;	// ����
			g_aBlock[nCntBlock].bUse = true;		// �g�p��Ԃɂ���

			if (g_aBlock[nCntBlock].nType == BLOCKTYPE_MOVEBLOCK)
			{
				g_aBlock[nCntBlock].nLife = 3;
			}
			else
			{
				g_aBlock[nCntBlock].nLife = 1;
			}

			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(pos.x - fWidth, pos.y - fHeight,0.0f);	// 1�ڂ̒��_���
			pVtx[1].pos = D3DXVECTOR3(pos.x + fWidth, pos.y - fHeight,0.0f);	// 2�ڂ̒��_���
			pVtx[2].pos = D3DXVECTOR3(pos.x - fWidth, pos.y + fHeight,0.0f);	// 3�ڂ̒��_���
			pVtx[3].pos = D3DXVECTOR3(pos.x + fWidth, pos.y + fHeight,0.0f);	// 4�ڂ̒��_���

			break;
		}
		// ���_��񕪂��炷
		pVtx += 4;	
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffBlock->Unlock();

}
//===========================
// �u���b�N�̏�̓����蔻��
//===========================
bool CollisionBlockOn(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, float fWidth, float fHeight)
{
	// ���n���Ă��邩�ǂ���
	bool Check = false;

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (!g_aBlock[nCntBlock].bUse)
		{
			//�u���b�N���g�p����ĂȂ��Ƃ�
			continue;
		}

		//�l��ۑ�����
		const float fPlayerHalfWidth = fWidth * HALF;												// �v���C���[�̉��̔����̃T�C�Y
		const float fBlockHalfWidth = g_aBlock[nCntBlock].fWidth * HALF;							// �u���b�N�̉��̔����̃T�C�Y
		const float fBlockHalfHeight = g_aBlock[nCntBlock].fHeight * HALF;							// �u���b�N�̏c�̔����̃T�C�Y

		const float fBlockUp = g_aBlock[nCntBlock].pos.y - fBlockHalfHeight;						// �u���b�N�̏�
		const float fBlockUnder = g_aBlock[nCntBlock].pos.y + fBlockHalfHeight + fHeight;			// �u���b�N�̉�
		const float fBlockLeft = g_aBlock[nCntBlock].pos.x - fBlockHalfWidth - fPlayerHalfWidth;	// �u���b�N�̍�
		const float fBlockRight = g_aBlock[nCntBlock].pos.x + fBlockHalfWidth + fPlayerHalfWidth;	// �u���b�N�̉E

		// �㉺�̓����蔻��
		if (pPos->x > fBlockLeft &&
			pPos->x < fBlockRight)
		{
			// �v���C���[���u���b�N�̏ォ�牺�ւ߂荞��ł��鎞
			if ((pPosOld->y <= fBlockUp &&
				pPos->y >= fBlockUp)
				|| (g_aBlock[nCntBlock].bOldpos.y >= pPos->y &&
				fBlockUp <= pPos->y))
			{
				Check = true;
				// pPlayer->bLanding = true;	// ���n����(�ォ�牺�ɗ��鎞�������n���Ă���)
				pPos->y = fBlockUp;				// �u���b�N�̏�̈ʒu�Ƀv���C���[�����킹��
				pMove->y = 0.0f;	            // �c�̈ړ��ʂ��Ȃ���
			}
		}
	}

	// �����Ԃ�
	return Check;			
}
//===========================
// �u���b�N�̉��̓����蔻��
//===========================
bool CollisionBlockBotton(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, float fWidth, float fHeight) 
{
	// ���n���Ă��邩�ǂ���
	bool Check = false;     

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (!g_aBlock[nCntBlock].bUse)
		{
			//�u���b�N���g�p����ĂȂ��Ƃ�
			continue;
		}

		//�l��ۑ�����
		const float fPlayerHalfWidth = fWidth * HALF;												// �v���C���[�̉��̔����̃T�C�Y
		const float fBlockHalfWidth = g_aBlock[nCntBlock].fWidth * HALF;							// �u���b�N�̉��̔����̃T�C�Y
		const float fBlockHalfHeight = g_aBlock[nCntBlock].fHeight * HALF;							// �u���b�N�̏c�̔����̃T�C�Y

		const float fBlockUp = g_aBlock[nCntBlock].pos.y - fBlockHalfHeight;						// �u���b�N�̏�
		const float fBlockUnder = g_aBlock[nCntBlock].pos.y + fBlockHalfHeight + fHeight;			// �u���b�N�̉�
		const float fBlockLeft = g_aBlock[nCntBlock].pos.x - fBlockHalfWidth - fPlayerHalfWidth;	// �u���b�N�̍�
		const float fBlockRight = g_aBlock[nCntBlock].pos.x + fBlockHalfWidth + fPlayerHalfWidth;	// �u���b�N�̉E

		// �㉺�̓����蔻��
		if (pPos->x > fBlockLeft &&
			pPos->x < fBlockRight)
		{
			// �v���C���[���u���b�N�̉������ւ߂荞��ł���
			if (pPosOld->y >= fBlockUnder &&
				pPos->y <= fBlockUnder)
			{
				pPos->y = fBlockUnder;				// �u���b�N�̉��̈ʒu�Ƀv���C���[�����킹��
				pMove->y = 0.0f;					// �c�̉��̈ړ��ʂ��Ȃ���
			}
		}

	}

	// �����Ԃ�
	return Check;			
}
//===========================
// �u���b�N�̍��E�̓����蔻��
//===========================
bool CollisionBlockX(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, float fWidth, float fHeight)
{
	// ����
	bool bhit = false;

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (!g_aBlock[nCntBlock].bUse)
		{
			//�u���b�N���g�p����ĂȂ��Ƃ�
			continue;
		}

		//�l��ۑ�����
		const float fPlayerHalfWidth = fWidth * HALF;												// �v���C���[�̉��̔����̃T�C�Y
		const float fBlockHalfWidth = g_aBlock[nCntBlock].fWidth * HALF;							// �u���b�N�̉��̔����̃T�C�Y
		const float fBlockHalfHeight = g_aBlock[nCntBlock].fHeight * HALF;							// �u���b�N�̏c�̔����̃T�C�Y

		const float fBlockUp = g_aBlock[nCntBlock].pos.y - fBlockHalfHeight;						// �u���b�N�̏�
		const float fBlockUnder = g_aBlock[nCntBlock].pos.y + fBlockHalfHeight + fHeight;			// �u���b�N�̉�
		const float fBlockLeft = g_aBlock[nCntBlock].pos.x - fBlockHalfWidth - fPlayerHalfWidth;	// �u���b�N�̍�
		const float fBlockRight = g_aBlock[nCntBlock].pos.x + fBlockHalfWidth + fPlayerHalfWidth;	// �u���b�N�̉E

		// ���E�̓����蔻��
		if (pPos->y > fBlockUp &&
			pPos->y < fBlockUnder)
		{
			// �v���C���[���u���b�N�̏c�͈͓̔��ɂ���
			if (pPosOld->x <= fBlockLeft &&
				pPos->x >= fBlockLeft)
			{
				// �v���C���[���u���b�N�̍�����E�ւ߂荞��ł���
				pPos->x = fBlockLeft;									// �u���b�N�̍��̈ʒu�Ƀv���C���[�����킹��
				pMove->x = 0.0f;										// �ړ��ʂ��Ȃ���
			}
			else if (pPosOld->x >= fBlockRight &&
				pPos->x <= fBlockRight)
			{
				// �v���C���[���u���b�N�̉E���獶�ւ߂荞��ł���
				pPos->x = fBlockRight;									// �u���b�N�̉E�̈ʒu�Ƀv���C���[�����킹��
				pMove->x = 0.0f;										// ���̈ړ��ʂ��Ȃ���
			}

		}
	}

	// �����Ԃ�
	return bhit;
}
//====================
// �u���b�N���擾
//====================
Block* GetBlockInfo(void)
{
	return &g_aBlock[0];
}
//====================
// �u���b�N�̐����擾
//====================
int GetBlock()
{
	return g_nCntblock;
}
//===================================
// �z�u���̓ǂݍ��ݏ���
//===================================
void LoadBlockText()
{
	// �t�@�C���|�C���^�̐錾
	FILE* pFile;

	//�E�F�[�u�֌W
	switch (g_nWave)
	{

	case 0:
		//�����E�F�u
		//�t�@�C���J��
		pFile = fopen("data\\TEXT\\SetBlock000.txt", "r");
		break;

	case 1:

		//�t�@�C���J��
		pFile = fopen("data\\TEXT\\SetBlock001.txt", "r");
		break;

	case 2:
		//�t�@�C�����J��
		pFile = fopen("data\\TEXT\\SetBlock002.txt", "r");

		break;

	case 3:
		//�t�@�C�����J��
		pFile = fopen("data\\TEXT\\SetBlock003.txt", "r");

		break;

	case 4:
		//�t�@�C�����J��
		pFile = fopen("data\\TEXT\\SetBlock004.txt", "r");

		break;

	default:
		pFile = NULL;
		g_bFinish = true;	//�E�F�[�u�̏I��
		break;
	}

	if (pFile != NULL)
	{// �t�@�C�����J������
		// ������
		char astr[256] = {};
		char aGomi[5] = {}; // =�i�[�p
		int nData = 0;
		int nBlock = 0;
		int nType = 0;
		D3DXVECTOR3 pos(0.0f, 0.0f, 0.0f);

		float fWidth = 0.0f;
		float fHeight = 0.0f;

		while (1)
		{
			// nData�ɑ������
			nData = fscanf(pFile, "%s", &astr[0]);

			if (strcmp(astr, "NUM_BLOCK") == 0)
			{// NUMT_BLOCK��ǂݎ������
				// �ǂݔ�΂�
				fscanf(pFile, "%s", &aGomi[0]);

				// �u���b�N�̍ő吔��ǂݍ���
				fscanf(pFile, "%d", &nBlock);
			}

			if (strcmp(astr, "SETBLOCK") == 0)
			{// SETBLOCK��ǂݎ������
				while (g_nCntblock < nBlock)
				{
					// ������
					fscanf(pFile, "%s", astr);

					if (strcmp(astr, "POS") == 0)
					{// POS��ǂݎ������
						// �ǂݔ�΂�
						fscanf(pFile, "%s", aGomi);

						// ���W����
						fscanf(pFile, "%f", &pos.x);
						fscanf(pFile, "%f", &pos.y);
						fscanf(pFile, "%f", &pos.z);

					}

					if (strcmp(astr, "WIDTH") == 0)
					{// WIDTH��ǂݎ������
						// �ǂݔ�΂�
						fscanf(pFile, "%s", aGomi);

						// ��������
						fscanf(pFile, "%f", &fWidth);
					}

					if (strcmp(astr, "HEIGHT") == 0)
					{// HEIGHT��ǂݎ������
						// �ǂݔ�΂�
						fscanf(pFile, "%s", aGomi);

						// ��������
						fscanf(pFile, "%f", &fHeight);
					}

					if (strcmp(astr, "TYPE") == 0)
					{// TYPE��ǂݎ������
						// �ǂݔ�΂�
						fscanf(pFile, "%s", aGomi);

						// ��ޔԍ�����
						fscanf(pFile, "%d", &nType);
					}

					if (strcmp(astr, "END_SETBLOCK") == 0)
					{// END_SETPOLYGON��ǂݎ������
						// �u���b�N�ɏ���������
						SetBlock(pos, nType, fWidth, fHeight);

						// �C���N�������g
						g_nCntblock++;
					}
				}
			}
			if (nData == EOF)
			{// EOF��ǂݎ������
				break;
			}
		}

		// �t�@�C�������
		fclose(pFile);

		//�E�F�[�u�̃J�E���g�A�b�v
		g_nWave++;

	}
	else
	{
		// ���b�Z�[�WBOX�̕\��
		MessageBox(NULL, "�J���܂���(SetBlock.txt)", "�G���[", MB_OK);
		pFile = NULL;
		return;
	}


}
//======================
// �E�F�[�u�̒l��Ԃ�
//======================
int GetWave(void)
{
	return g_nWave;
}
//=========================
// �E�F�[�u�̏I�������Ԃ�
//=========================
bool GetFinish(void)
{
	return g_bFinish;
}
//====================
// �\���̏������֐�
//====================
void InitStruct()
{
	// �u���b�N���̏�����
	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		g_aBlock[nCntBlock].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	 // ���W
		g_aBlock[nCntBlock].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	 // �ړ���
		g_aBlock[nCntBlock].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	 // ����
		g_aBlock[nCntBlock].bUse = false;							 // �g�p���ĂȂ���Ԃɂ���
		g_aBlock[nCntBlock].fWidth = 0.0f;							 // ����
		g_aBlock[nCntBlock].fHeight = 0.0f;							 // ����
		g_aBlock[nCntBlock].nType = 0;								 // ���
		g_aBlock[nCntBlock].nLife = 0;								 // �̗�
		g_aBlock[nCntBlock].bHitBlock = false;						 // �����������ǂ���
		g_aBlock[nCntBlock].bLeft = false;							 // ���[�ɒ��������ǂ���
		g_aBlock[nCntBlock].bRight = false;							 // �E�[�ɒ��������ǂ���
		g_aBlock[nCntBlock].nCntBlockstateCount = 0;				 // �J�E���^�[�̏�����
		g_aBlock[nCntBlock].bOldpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // ���W
	}

}
//=======================================
// �u���b�N�̃q�b�g����
//=======================================
void HitBlock(int nCntBlock, int nDamage)
{
	g_aBlock[nCntBlock].nLife -= nDamage;

	if (g_aBlock[nCntBlock].nLife <= 0)
	{
		// ����
		SetExplosion(g_aBlock[nCntBlock].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		if (g_aBlock[nCntBlock].nType == BLOCKTYPE_NORMAL || g_aBlock[nCntBlock].nType == BLOCKTYPE_MOVEBLOCK)
		{
			g_aBlock[nCntBlock].bUse = false;
		}

		// �e�u���b�N��j�󂵂�
		if (g_aBlock[nCntBlock].nType == BLOCKTYPE_BULLETBLOCK)
		{

			g_aBlock[nCntBlock].bUse = false;
		}

		if (g_aBlock[nCntBlock].nType == BLOCKTYPE_VERTICAL)
		{
			// �����
			SetBullet(g_aBlock[nCntBlock].pos,
				D3DXVECTOR3(0.0f, 10.0f, 0.0f),
				g_aBlock[nCntBlock].rot,
				30.0f,
				30.0f,
				80.0f, BULLETTYPE_BLOCK);

			// ������
			SetBullet(g_aBlock[nCntBlock].pos,
				D3DXVECTOR3(0.0f, -10.0f, 0.0f),
				g_aBlock[nCntBlock].rot,
				30.0f,
				30.0f,
				80.0f, BULLETTYPE_BLOCK);

			g_aBlock[nCntBlock].bUse = false;
		}
		if (g_aBlock[nCntBlock].nType == BLOCKTYPE_HORIZONTAL)
		{
			// �E����
			SetBullet(g_aBlock[nCntBlock].pos,
				D3DXVECTOR3(10.0f, 0.0f, 0.0f),
				g_aBlock[nCntBlock].rot,
				30.0f,
				30.0f,
				80.0f, BULLETTYPE_BLOCK);

			// ������
			SetBullet(g_aBlock[nCntBlock].pos,
				D3DXVECTOR3(-10.0f, 0.0f, 0.0f),
				g_aBlock[nCntBlock].rot,
				30.0f,
				30.0f,
				80.0f, BULLETTYPE_BLOCK);

			g_aBlock[nCntBlock].bUse = false;

			switch (g_aBlock[nCntBlock].nType)
			{
			case  BLOCKTYPE_NORMAL:		// �ʏ�
				// �X�R�A�����Z
				AddScore(1000);
				break;

			case BLOCKTYPE_VERTICAL:	// �c�ɒe���o��u���b�N
				// �X�R�A�����Z
				AddScore(3000);
				break;


			case BLOCKTYPE_HORIZONTAL:	// ���ɒe���o��u���b�N
				// �X�R�A�����Z
				AddScore(3000);
				break;

			case BLOCKTYPE_BULLETBLOCK:	// �c�e�������̃u���b�N

				// TODO : �����Ƀv���C���[�̒e�̎c�e���𑝂₷�֐��Ă�
				AddBulletNum(1);
				break;

			default:
				break;
			}

		}

		g_nCntblock--; // �f�N�������g
	}
}
