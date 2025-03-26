//===============================
//
// �A�C�e������ [ item.cpp ]
// Author:Asuma Nishio
//
//================================

//*******************************
// �C���N���[�h
//*******************************
#include "item.h"
#include "player.h"
#include "explosion.h"
#include "score.h"
// #include "sound.h"

#define MAX_ITEM (40) // �ő吔

//*******************************
// �O���[�o���ϐ��錾
//*******************************
LPDIRECT3DTEXTURE9 g_apTextureItem[ITEMTYPE_MAX] = {};		// �e�N�X�`�����W
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffItem = NULL;				// ���_�o�b�t�@�̃|�C���^
ITEM  g_aItem[MAX_ITEM];								// �A�C�e���̏��
ITEMTYPE g_Item;											// �A�C�e���̗񋓌^
int nCntItem;												// �A�C�e�������J�E���g

//==============================
//�A�C�e���̏���������
//==============================
void InitItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// �f�o�C�X�ւ̃|�C���^

	VERTEX_2D* pVtx;				// ���_���̃|�C���^

	for (int nCnt = 0; nCnt < ITEMTYPE_MAX; nCnt++)
	{
		//�A�C�e���̃e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			ITEM_TEXTURE[nCnt],
			&g_apTextureItem[nCnt]);
	}

	for (int nItem = 0; nItem < MAX_ITEM; nItem++)
	{
		g_aItem[nItem].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//���W
		g_aItem[nItem].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ړ���
		g_aItem[nItem].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	//�F
		g_aItem[nItem].nLife = 1;								//�̗�
		g_aItem[nItem].nType = 0;								//���
		g_aItem[nItem].bUse = false;							//���g�p���
		g_aItem[nItem].nItemCounter = 0;						//�A�C�e���J�E���^�[�̏�����
		g_aItem[nItem].bRight = false;							//�t���O
		g_aItem[nItem].state = ITEMSTATE_POP;					//�������
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ITEM,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffItem,
		NULL);

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	for (int nItem1 = 0; nItem1 < MAX_ITEM; nItem1++)
	{

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
		pVtx[0].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}
	// �A�����b�N
	g_pVtxBuffItem->Unlock();

}
//========================
// �A�C�e���̏I��
//========================
void UninitItem(void)
{
	// StopSound();

	// �e�N�X�`�����W�̔j��
	for (int nItem = 0; nItem < ITEMTYPE_MAX; nItem++)
	{
		if (g_apTextureItem[nItem] != NULL)
		{			   
			g_apTextureItem[nItem]->Release();
			g_apTextureItem[nItem] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffItem != NULL)
	{			  
		g_pVtxBuffItem->Release();
		g_pVtxBuffItem = NULL;
	}
}
//============================
// �A�C�e���̍X�V����
//============================
void UpdateItem(void)
{
	VERTEX_2D* pVtx;		// ���_���̃|�C���^

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_ITEM; nCnt++)
	{
		// �v���C���[�̎擾
		Player* pPlayer = GetPlayer();

		switch (g_aItem[nCnt].state)
		{
		case ITEMSTATE_POP:
			if (g_aItem[nCnt].pos.x <= (SCREEN_WIDTH - 50.0f) * 0.5f || g_aItem[nCnt].pos.x >= (SCREEN_WIDTH + 50.0f) * 0.5f)
			{
				// ��ԕω�
				g_aItem[nCnt].state = ITEMSTATE_MOVE;
			}
			break;

		case ITEMSTATE_MOVE:
			if (g_aItem[nCnt].pos.x >= (SCREEN_WIDTH - 50.0f) * 0.5f && g_aItem[nCnt].pos.x <= (SCREEN_WIDTH + 50.0f) * 0.5f)
			{
				// ��ԕω�
				g_aItem[nCnt].state = ITEMSTATE_STOP;
			}
			break;

		case ITEMSTATE_STOP:
			//�������~�߂�
			g_aItem[nCnt].move.x = 0.0f;
			// ��ԕω�
			g_aItem[nCnt].state = ITEMSTATE_MOVEUNDER;
			break;

		case ITEMSTATE_MOVEUNDER:
			// ���Ɉړ�
			g_aItem[nCnt].move.y += 1.5f;
			break;

		default:
			break;
		}

		if (g_aItem[nCnt].bUse == true)
		{
			// �g�p��ԂȂ�
			if (   g_aItem[nCnt].pos.x >= pPlayer->pos.x - MAX_SIZE_X 
				&& g_aItem[nCnt].pos.x <= pPlayer->pos.x + MAX_SIZE_X 
				&& g_aItem[nCnt].pos.y >= pPlayer->pos.y - MAX_SIZE_Y 
				&& g_aItem[nCnt].pos.y <= pPlayer->pos.y + MAX_SIZE_Y )
			{
				// �����蔻��
				HitItem(nCnt,1);
				// PlaySound(SOUND_LABEL_ITEM);
			}

			if (g_aItem[nCnt].state == ITEMSTATE_MOVE)
			{
				// state��move�̎�
				g_aItem[nCnt].move.y = 0.0f;

				if (g_aItem[nCnt].nType == ITEMTYPE_ONE)
				{
					// 1��ޖ�
					if (g_aItem[nCnt].pos.x <= SCREEN_WIDTH * 0.5f)
					{
						g_aItem[nCnt].move.x = 4.0f;
					}
					else if (g_aItem[nCnt].pos.x >= SCREEN_WIDTH * 0.5f)
					{
						g_aItem[nCnt].move.x = -4.0f;

					}
				}
				if (g_aItem[nCnt].nType == ITEMTYPE_TWO)
				{
					// 2��ޖ�
					if (g_aItem[nCnt].pos.x <= SCREEN_WIDTH * 0.5f)
					{
						g_aItem[nCnt].move.x = 4.0f;

					}
					else if (g_aItem[nCnt].pos.x >= SCREEN_WIDTH * 0.5f)
					{
						g_aItem[nCnt].move.x = -4.0f;
					}
				}
				if (g_aItem[nCnt].nType == ITEMTYPE_THREE)
				{
					// 3��ޖ�
					if (g_aItem[nCnt].pos.x <= SCREEN_WIDTH * 0.5f)
					{
						g_aItem[nCnt].move.x = 4.0f;

					}
					else if (g_aItem[nCnt].pos.x >= SCREEN_WIDTH * 0.5f)
					{
						g_aItem[nCnt].move.x = -4.0f;
					}
				}
				if (g_aItem[nCnt].nType == ITEMTYPE_FOUR)
				{
					// 4��ޖ�
					if (g_aItem[nCnt].pos.x <= SCREEN_WIDTH * 0.5f)
					{
						g_aItem[nCnt].move.x = 4.0f;

					}
					else if (g_aItem[nCnt].pos.x >= SCREEN_WIDTH * 0.5f)
					{
						g_aItem[nCnt].move.x = -4.0f;
					}
				}
				if (g_aItem[nCnt].nType == ITEMTYPE_FIVE)
				{
					// 5��ޖ�
					if (g_aItem[nCnt].pos.x <= SCREEN_WIDTH * 0.5f)
					{
						g_aItem[nCnt].move.x = 3.0f;

					}
					else if (g_aItem[nCnt].pos.x >= SCREEN_WIDTH * 0.5f)
					{
						g_aItem[nCnt].move.x = -3.0f;
					}
				}
				if (g_aItem[nCnt].nType == ITEMTYPE_SIX)
				{
					// 6��ޖ�
					if (g_aItem[nCnt].pos.x <= SCREEN_WIDTH * 0.5f)
					{
						g_aItem[nCnt].move.x = 4.0f;

					}
					else if (g_aItem[nCnt].pos.x >= SCREEN_WIDTH * 0.5f)
					{
						g_aItem[nCnt].move.x = -4.0f;
					}
				}

			}

			if (g_aItem[nCnt].bUse == true)
			{
				if (g_aItem[nCnt].pos.y >= SCREEN_HEIGHT)
				{
					g_aItem[nCnt].bUse = false;  // ��ʊO�ɂ�������false
				}
			}

			// �ړ��ʂ̌���
			g_aItem[nCnt].move.x += (0.0f - g_aItem[nCnt].move.x) * 0.25f;
			g_aItem[nCnt].move.y += (0.0f - g_aItem[nCnt].move.y) * 0.25f;

			// �ʒu�̍X�V
			g_aItem[nCnt].pos += g_aItem[nCnt].move;

		}

		// ���_���W�̍X�V
		pVtx[0].pos = D3DXVECTOR3(g_aItem[nCnt].pos.x - 25.0f, g_aItem[nCnt].pos.y - 25.0f, 0.0f);		// 1�ڂ̒��_���
		pVtx[1].pos = D3DXVECTOR3(g_aItem[nCnt].pos.x + 25.0f, g_aItem[nCnt].pos.y - 25.0f, 0.0f);		// 2�ڂ̒��_���
		pVtx[2].pos = D3DXVECTOR3(g_aItem[nCnt].pos.x - 25.0f, g_aItem[nCnt].pos.y + 25.0f, 0.0f);		// 3�ڂ̒��_���
		pVtx[3].pos = D3DXVECTOR3(g_aItem[nCnt].pos.x + 25.0f, g_aItem[nCnt].pos.y + 25.0f, 0.0f);		// 4�ڂ̒��_���

		// �|�C���^��i�߂�
		pVtx += 4;

	}

	//�A�����b�N
	g_pVtxBuffItem->Unlock();

}
//=============================
// �A�C�e���̕`��
//=============================
void DrawItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// �f�o�C�X�ւ̃|�C���^

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffItem, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nItem = 0; nItem < ITEMTYPE_MAX; nItem++)
	{
		if (g_aItem[nItem].bUse == true)
		{
			// �g�p����Ă�
			int nType = g_aItem[nItem].nType;

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureItem[nType]);

			//�| ���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nItem * 4, 2); // �n�_
		}
	}
}
//===========================
//�A�C�e���̐ݒ�
//===========================
void SetItem(D3DXVECTOR3 pos, int nType)
{
	VERTEX_2D* pVtx;	// ���_���̃|�C���^

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	for (int nItem = 0; nItem < MAX_ITEM; nItem++)
	{
		if (!g_aItem[nItem].bUse)
		{
			// �g�p����ĂȂ�
			g_aItem[nItem].pos = pos;								// ���W
			g_aItem[nItem].nType = nType;							// ���
			g_aItem[nItem].bUse = true;								// �g�p���
			g_aItem[nItem].nLife = 1;								// �̗�
			g_aItem[nItem].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);	// �F
			g_aItem[nItem].state = ITEMSTATE_POP;					// ���
																	   
			// ���_���̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aItem[nItem].pos.x - 25.0f, g_aItem[nItem].pos.y - 25.0f, 0.0f);		// 1�ڂ̒��_���
			pVtx[1].pos = D3DXVECTOR3(g_aItem[nItem].pos.x + 25.0f, g_aItem[nItem].pos.y - 25.0f, 0.0f);		// 2�ڂ̒��_���
			pVtx[2].pos = D3DXVECTOR3(g_aItem[nItem].pos.x - 25.0f, g_aItem[nItem].pos.y + 25.0f, 0.0f);		// 3�ڂ̒��_���
			pVtx[3].pos = D3DXVECTOR3(g_aItem[nItem].pos.x + 25.0f, g_aItem[nItem].pos.y + 25.0f, 0.0f);		// 4�ڂ̒��_���

			// ���_�J���[
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			break;

		}

		pVtx += 4;
	}
	// �A�����b�N
	g_pVtxBuffItem->Unlock();
}
//=======================
// �A�C�e�����̓����蔻��
//=======================
void HitItem(int nCntItem,int nDamage)
{
	VERTEX_2D* pVtx;		// ���_���̃|�C���^

	g_aItem[nCntItem].nLife -= nDamage;		// ���C�t�����炷

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	if (g_aItem[nCntItem].nLife <= 0)
	{
		g_aItem[nCntItem].bUse = false;		// ���g�p���

		if (g_aItem[nCntItem].nType == ITEMTYPE_ONE)
		{
			// �X�R�A�̉��Z
			AddScore(1000);
		}
		else if (g_aItem[nCntItem].nType == ITEMTYPE_TWO)
		{
			// �X�R�A�̉��Z
			AddScore(2000);
		}
		else if (g_aItem[nCntItem].nType == ITEMTYPE_THREE)
		{
			// �X�R�A�̉��Z
			AddScore(4000);
		}
		else if (g_aItem[nCntItem].nType == ITEMTYPE_FOUR)
		{
			// �X�R�A�̉��Z
			AddScore(5000);
		}
		else if (g_aItem[nCntItem].nType == ITEMTYPE_FIVE)
		{
			// �X�R�A�̉��Z
			AddScore(10000);
		}
		else if (g_aItem[nCntItem].nType == ITEMTYPE_SIX)
		{
			// �X�R�A�̉��Z
			AddScore(20000);
		}
	}
	else
	{
		pVtx += 4 * nCntItem;			// �A�C�e�������炷

		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);

	}

	// �A�����b�N
	g_pVtxBuffItem->Unlock();

}
//=======================
// �A�C�e���̔�����擾
//=======================
ITEM* GetItem(void)
{
	return &g_aItem[0];
}