//========================================
//
//�`���[�g���A������
//Author:ASUMA NISHIO
//
//========================================

#include "tutorial2.h"
#include "tutorial.h"


LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTutorial2 = NULL;	//���_�o�b�t�@�̃|�C���^(�w�i)
LPDIRECT3DTEXTURE9 g_pTextureTutorial2 = NULL;		//�e�N�X�`���̃|�C���^(�w�i)

D3DXVECTOR3 g_pos = D3DXVECTOR3(0.0f,0.0f,0.0f);	//���W

//========================
//������
//========================
void InitTutorial2(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	VERTEX_2D* pVtx;		//���_���̃|�C���^

	//������
	g_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\tuto2.png",
		&g_pTextureTutorial2);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY, FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTutorial2,
		NULL);

	//���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffTutorial2->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W1�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//1�ڂ̒��_���
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

	//�A�����b�N
	g_pVtxBuffTutorial2->Unlock();

}
//========================
//�I��
//========================
void UninitTutorial2(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureTutorial2 != NULL)
	{
		g_pTextureTutorial2->Release();
		g_pTextureTutorial2 = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffTutorial2 != NULL)
	{
		g_pVtxBuffTutorial2->Release();
		g_pVtxBuffTutorial2 = NULL;
	}

}
//========================
//�X�V
//========================
void UpdateTutorial2(void)
{
	if (GetIsNext() == true)
	{
		if (g_pos.x > -1280.0f)
		{
			g_pos.x -= 40.0f;

			VERTEX_2D* pVtx = NULL;		//���_���̃|�C���^

			//���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
			g_pVtxBuffTutorial2->Lock(0, 0, (void**)&pVtx, 0);

			//���_���W1�̐ݒ�(��ʊO)
			pVtx[0].pos = D3DXVECTOR3(g_pos.x + SCREEN_WIDTH,		0.0f,		   0.0f);		//1�ڂ̒��_���
			pVtx[1].pos = D3DXVECTOR3(g_pos.x + (SCREEN_WIDTH * 2), 0.0f,		   0.0f);		//2�ڂ̒��_���
			pVtx[2].pos = D3DXVECTOR3(g_pos.x + SCREEN_WIDTH,		SCREEN_HEIGHT, 0.0f);		//3�ڂ̒��_���
			pVtx[3].pos = D3DXVECTOR3(g_pos.x + (SCREEN_WIDTH * 2), SCREEN_HEIGHT, 0.0f);		//4�ڂ̒��_���

			//�A�����b�N
			g_pVtxBuffTutorial2->Unlock();
		}

	}
}
//========================
//�`��
//========================
void DrawTutorial2(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTutorial2, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureTutorial2);

	//�`���[�g���A���|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

}
//========================
//�ݒ菈��
//========================
void SetTutorial2(void)
{
	VERTEX_2D* pVtx = NULL;		//���_���̃|�C���^

	//���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffTutorial2->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W1�̐ݒ�(��ʊO)
	pVtx[0].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);			//1�ڂ̒��_���
	pVtx[1].pos = D3DXVECTOR3((SCREEN_WIDTH * 2), 0.0f, 0.0f);		//2�ڂ̒��_���
	pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH, 720.0f, 0.0f);			//3�ڂ̒��_���
	pVtx[3].pos = D3DXVECTOR3((SCREEN_WIDTH * 2), 720.0f, 0.0f);	//4�ڂ̒��_���

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

	//�A�����b�N
	g_pVtxBuffTutorial2->Unlock();

}
