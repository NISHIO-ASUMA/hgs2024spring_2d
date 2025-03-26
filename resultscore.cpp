//==========================================================================================================================
//
// ���U���g�X�R�A���� [resultscore.cpp]
// Author:Asuma Nishio
//
//==========================================================================================================================

//*****************************************************************************************************************
// �C���N���[�h�t�@�C���錾
//*****************************************************************************************************************
#include "resultscore.h"
#include "result.h"
#include "score.h"
#include <stdio.h>

//********************************************************************************************************************
// �}�N����`
//********************************************************************************************************************
#define MAX_WIDTH  (100.0f)  // ����
#define MAX_HEIGHT (60.0f)   // ����
#define SPEED (12.0f)         // �������Ȃ�X�s�[�h
#define SCALVALUE (23.0f)    // �傫���̔{��

//********************************************************************************************************************
// ���U���g�X�R�A�̍\���̐錾
//********************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos; // �ʒu
	float fWidth, fHeight; // �����A����
	bool bUse; // ���g�p����
}Result;

//********************************************************************************************************************
// �O���[�o���ϐ��錾
//********************************************************************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResultScore = NULL; // ���_�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureResultScore = NULL;	  // �e�N�X�`���̃|�C���^

Result g_aResult[MAX_NUM_SCORE];	// ���U���g�X�R�A�̍\����
int g_nResultScore;					// ���U���g�X�R�A�p
int g_nResultScoreDest;             // �ړI�̃X�R�A�̒l

//===========================================================================================================
// ���U���g�X�R�A�̏�����
//===========================================================================================================
void InitResultScore(void)
{
	// �f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_���̃|�C���^
	VERTEX_2D* pVtx;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\score001.png",
		&g_pTextureResultScore);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_NUM_SCORE,
		D3DUSAGE_WRITEONLY, FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffResultScore,
		NULL);

	// �O���[�o���ϐ��̏�����
	g_nResultScore = 0;
	g_nResultScoreDest = 0;

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffResultScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_NUM_SCORE; nCnt++)
	{
		// �g�p���Ă�����
		g_aResult[nCnt].bUse = true;
		g_aResult[nCnt].fHeight = MAX_HEIGHT * SCALVALUE; // ���������߂�
		g_aResult[nCnt].fWidth = MAX_WIDTH * SCALVALUE;   // ���������߂�

		g_aResult[nCnt].pos = D3DXVECTOR3(120.0f, 340.0f, 0.0f);

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// rhw�̐ݒ�(1.0f�ŌŒ�)
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		// ���_����i�߂�
		pVtx += 4;

	}

	// �A�����b�N
	g_pVtxBuffResultScore->Unlock();

	LoadScore();			// �X�R�A��ǂݍ���
}
//========================================================================================================
// ���U���g�X�R�A�̏I��
//========================================================================================================
void UninitResultScore(void)
{
	// �e�N�X�`���̔j��
	if (g_pTextureResultScore != NULL)
	{
		g_pTextureResultScore->Release();
		g_pTextureResultScore = NULL;
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffResultScore != NULL)
	{
		g_pVtxBuffResultScore->Release();
		g_pVtxBuffResultScore = NULL;
	}

}
//========================================================================================================
// ���U���g�X�R�A�̍X�V
//========================================================================================================
void UpdateResultScore(void)
{
	// ���_���̃|�C���^
	VERTEX_2D* pVtx;

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffResultScore->Lock(0, 0, (void**)&pVtx, 0);

	// ��������
	for (int nCnt = 0; nCnt < MAX_NUM_SCORE; nCnt++)
	{

		if (g_aResult[nCnt].fWidth <= MAX_WIDTH)
		{
			g_aResult[nCnt].fWidth = MAX_WIDTH;
		}
		else
		{
			g_aResult[nCnt].fWidth -= (MAX_WIDTH / SCALVALUE) * SPEED;
		}

		if (g_aResult[nCnt].fHeight <= MAX_HEIGHT)
		{
			g_aResult[nCnt].fHeight = MAX_HEIGHT;
		}
		else
		{
			g_aResult[nCnt].fHeight -= (MAX_HEIGHT / SCALVALUE) * SPEED;
		}
		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aResult[nCnt].pos.x + g_aResult[nCnt].fWidth + nCnt * 110.0f, g_aResult[nCnt].pos.y - g_aResult[nCnt].fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aResult[nCnt].pos.x + g_aResult[nCnt].fWidth + nCnt * 110.0f + 110.0f, g_aResult[nCnt].pos.y - g_aResult[nCnt].fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aResult[nCnt].pos.x + g_aResult[nCnt].fWidth + nCnt * 110.0f, g_aResult[nCnt].pos.y + g_aResult[nCnt].fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aResult[nCnt].pos.x + g_aResult[nCnt].fWidth + nCnt * 110.0f + 110.0f, g_aResult[nCnt].pos.y + g_aResult[nCnt].fHeight, 0.0f);
		pVtx += 4;
	}
	// �A�����b�N
	g_pVtxBuffResultScore->Unlock();

	// 3�b�����ĖړI�̃X�R�A�ɂ���
	g_nResultScore += g_nResultScoreDest / 120.0f;

	// �ړI�̃X�R�A�𒴂�����
	if (g_nResultScore >= g_nResultScoreDest)
	{
		// �ړI�̃X�R�A�ɂ���
		g_nResultScore = g_nResultScoreDest;
	}

	SetResultScore(g_nResultScore);		// ���U���g�X�R�A�̐ݒ�
}
//========================================================================================================
// ���U���g�X�R�A�̕`��
//========================================================================================================
void DrawResultScore(void)
{
	// �f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffResultScore, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntscore = 0; nCntscore < MAX_NUM_SCORE; nCntscore++)
	{
		if (g_aResult[nCntscore].bUse)
		{// �g�p����̎�
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureResultScore);

			// �`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntscore * 4, 2);
		}
	}
}
//===============================================================================================================
// ���U���g�X�R�A�̐ݒ�
//===============================================================================================================
void SetResultScore(int nScore)
{
	int aPosTexU[MAX_NUM_SCORE] = {};	// �������̐������i�[
	int aData = 10000000;				// 8��
	int aData2 = 1000000;				// 7��

	VERTEX_2D* pVtx;		// ���_���̃|�C���^

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffResultScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntscore = 0; nCntscore < MAX_NUM_SCORE; nCntscore++)
	{
		if (nCntscore == 0)		// 0�Ԗڂ̎�
		{
			aPosTexU[0] = nScore / aData;
		}
		else
		{
			// 0�ԖڈȊO�̎�
			aPosTexU[nCntscore] = nScore % aData / aData2;
			aData = aData / 10;
			aData2 = aData2 / 10;
		}

		// �e�N�X�`���ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f + (0.1f * aPosTexU[nCntscore]), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + (0.1f * aPosTexU[nCntscore]), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + (0.1f * aPosTexU[nCntscore]), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f * aPosTexU[nCntscore]), 1.0f);

		// ���_����i�߂�
		pVtx += 4;

	}

	// �A�����b�N
	g_pVtxBuffResultScore->Unlock();
}
//==================================================================================================================
// �X�R�A�̓ǂݍ���
//==================================================================================================================
void LoadScore(void)
{
	// �t�@�C��
	FILE* pFile;

	// �t�@�C���J��
	pFile = fopen("data\\LastScore.txt", "r");

	if (pFile != NULL)
	{
		// �t�@�C���J������
		fscanf(pFile, "%d", &g_nResultScoreDest);

	}
	else
	{
		// �J���Ȃ�������
		// ���b�Z�[�WBOX�̕\��
		MessageBox(NULL, "�J���܂���", "�G���[(ResultScore.cpp)", MB_OK);

		return;
	}

	// �t�@�C�������
	fclose(pFile);
}

