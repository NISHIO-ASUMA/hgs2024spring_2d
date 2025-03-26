//=============================================================================
//
// �c�e���\������ [bulletnum.cpp]
// Author : TANEKAWA RIKU
//
//=============================================================================
#include "bulletnum.h"
#include "player.h"
#include "bullet.h"

typedef struct
{
	bool bUse;

}BulletNum;

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureBulletNum = NULL;//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureBulletNum2 = NULL;//�e�N�X�`���ւ̃|�C���^

LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBulletNum = NULL;//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBulletNum2 = NULL;//���_�o�b�t�@�ւ̃|�C���^

D3DXVECTOR3 g_posBulletNum;	// �c�e���̈ʒu
BulletNum g_aBulletNum[MAX_DIGIT];

int g_nBulletNum;//�c�e���̒l

//=============================
// �c�e���̏���������
//=============================
void InitBulletNum(void)
{

	int nCnt;
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\time001.png",
		&g_pTextureBulletNum);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\bulletnum.png",
		&g_pTextureBulletNum2);

	g_posBulletNum = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�ʒu������������(�n�܂�̈ʒu)


	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_DIGIT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBulletNum,
		NULL);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBulletNum2,
		NULL);

	VERTEX_2D* pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBulletNum->Lock(0, 0, (void**)&pVtx, 0);

	for (nCnt = 0; nCnt < MAX_DIGIT; nCnt++)
	{
		g_aBulletNum[nCnt].bUse = true;

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(1110.0f + nCnt * 35.0f, 610.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(1110.0f + nCnt * 35.0f + 35.0f, 610.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(1110.0f + nCnt * 35.0f, 680.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(1110.0f + nCnt * 35.0f + 35.0f, 680.0f, 0.0f);

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBulletNum->Unlock();

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBulletNum2->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(900.0f, 630.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(1100.0f, 630.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(900.0f, 680.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(1100.0f, 680.0f, 0.0f);

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBulletNum2->Unlock();

	SetBulletNum(20);
}
//=============================
// �c�e���̏I������
//=============================
void UninitBulletNum(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureBulletNum != NULL)
	{
		g_pTextureBulletNum->Release();
		g_pTextureBulletNum = NULL;
	}

	//�e�N�X�`���̔j��
	if (g_pTextureBulletNum2 != NULL)
	{
		g_pTextureBulletNum2->Release();
		g_pTextureBulletNum2 = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffBulletNum != NULL)
	{
		g_pVtxBuffBulletNum->Release();
		g_pVtxBuffBulletNum = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffBulletNum2 != NULL)
	{
		g_pVtxBuffBulletNum2->Release();
		g_pVtxBuffBulletNum2 = NULL;
	}

}
//=============================
// �c�e���̍X�V����
//=============================
void UpdateBulletNum(void)
{






}
//=============================
// �c�e���̕`�揈��
//=============================
void DrawBulletNum(void)
{

	//�K�v�������̕`��

	int nCnt;

	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBulletNum, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCnt = 0; nCnt < MAX_DIGIT; nCnt++)
	{
		if (g_aBulletNum[nCnt].bUse == true)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureBulletNum);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
		}
	}

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBulletNum2, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureBulletNum2);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

}
//=============================
// �c�e���̐ݒ菈��
//=============================
void SetBulletNum(int nBulletNum)
{
	VERTEX_2D* pVtx;

	int aPosTexU[MAX_DIGIT] = {};//�e���̐������i�[(�������̔z��)

	int aData = TEN;//10
	int aData2 = ONE;//1

	int digitNum = 0;
	int nCnt;

	g_nBulletNum = nBulletNum;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBulletNum->Lock(0, 0, (void**)&pVtx, 0);

	//�����Ƃɕ�������
	for (nCnt = 0; nCnt < MAX_DIGIT; nCnt++)
	{
		if (nCnt == 0)
		{
			aPosTexU[0] = g_nBulletNum / aData;
		}
		else
		{
			aPosTexU[nCnt] = g_nBulletNum % aData / aData2;
			aData = aData / 10;
			aData2 = aData2 / 10;
		}

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f + (0.1f * aPosTexU[nCnt]), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + (0.1f * aPosTexU[nCnt]), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + (0.1f * aPosTexU[nCnt]), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f * aPosTexU[nCnt]), 1.0f);

		pVtx += 4;

	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBulletNum->Unlock();

}
//=============================
// �c�e���̉��Z����
//=============================
void AddBulletNum(int nBulletNum)
{

	VERTEX_2D* pVtx;

	int aPosTexU[MAX_DIGIT] = {};

	int aData = TEN;//10
	int aData2 = ONE;//1

	int digitNum = 0;
	int nCnt;

	g_nBulletNum += nBulletNum;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBulletNum->Lock(0, 0, (void**)&pVtx, 0);

	if (g_nBulletNum <= 0)
	{
		g_nBulletNum = 0;
	}

	//�����Ƃɕ�������
	for (nCnt = 0; nCnt < MAX_DIGIT; nCnt++)
	{

		if (nCnt == 0)
		{
			aPosTexU[0] = g_nBulletNum / aData;
		}
		else
		{
			aPosTexU[nCnt] = g_nBulletNum % aData / aData2;
			aData = aData / 10;
			aData2 = aData2 / 10;
		}

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f + (0.1f * aPosTexU[nCnt]), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + (0.1f * aPosTexU[nCnt]), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + (0.1f * aPosTexU[nCnt]), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f * aPosTexU[nCnt]), 1.0f);

		pVtx += 4;

	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBulletNum->Unlock();
}
//=============================
// �c�e���̌��Z����
//=============================
void DecBulletNum(int nBulletNum)
{

	VERTEX_2D* pVtx;

	int aPosTexU[MAX_DIGIT] = {};

	int aData = TEN;//10
	int aData2 = ONE;//1

	int digitNum = 0;
	int nCnt;

	g_nBulletNum -= nBulletNum;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBulletNum->Lock(0, 0, (void**)&pVtx, 0);

	if (g_nBulletNum <= 0)
	{
		g_nBulletNum = 0;
	}

	//�����Ƃɕ�������
	for (nCnt = 0; nCnt < MAX_DIGIT; nCnt++)
	{

		if (nCnt == 0)
		{

			aPosTexU[0] = g_nBulletNum / aData;

		}
		else
		{

			aPosTexU[nCnt] = g_nBulletNum % aData / aData2;
			aData = aData / 10;
			aData2 = aData2 / 10;

		}

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f + (0.1f * aPosTexU[nCnt]), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + (0.1f * aPosTexU[nCnt]), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + (0.1f * aPosTexU[nCnt]), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f * aPosTexU[nCnt]), 1.0f);

		pVtx += 4;

	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBulletNum->Unlock();
}
//=============================
//�����J�E���g����
//=============================
int DigitNum(int nBulletNum)
{
	int ndigit = 0, nData;//�����J�E���g

	nData = nBulletNum;

	for (int nCnt2 = 0; nCnt2 < MAX_DIGIT; nCnt2++)
	{
		if (nData == 0)
		{
			for (int nCnt3 = 0; nCnt3 < ndigit; nCnt3++)
			{
				g_aBulletNum[nCnt3].bUse = true;
			}
			break;
		}
		else
		{
			nData /= 10;
			ndigit++;//�������C���N�������g
		}
	}

	return ndigit;
}
//=============================
// �c�e���̎擾
//=============================
int GetBulletNum(void)
{
	return g_nBulletNum;
}
