//===============================
//
// �^�C�g������ [ title.cpp ]
// Author:NISHIO ASUMA
//
//===============================

//******************************************
// �C���N���[�h�t�@�C���錾
//******************************************
#include "title.h"
#include "input.h"
#include "fade.h"

//#include "sound.h"

//******************************************
// �e�N�X�`���̎�ނ̗񋓌^
//******************************************
typedef enum
{
	TITLE_FIRST = 0,	// �`���[�g���A���J�n
	TITLE_SECOND,		// �Q�[���J�n
	TITLE_MAX
}TITLE;

//******************************************
// �^�C�g���̏�Ԃ̗񋓌^
//******************************************
typedef enum
{
	TITLE_NONE = 0,		// �ʏ���
	TITLE_FLASH,		// �_�ŏ��
	STATE_MAX
}TITLESTATE;

//******************************************
// �^�C�g���e�N�X�`���̗񋓌^�錾
//******************************************
const char* TITLE_TEX[TITLE_MAX] =
{
	"data\\TEXTURE\\title_tuto.png",
	"data\\TEXTURE\\title_game.png",
};

//******************************************
// �^�C�g���\���̂�錾
//******************************************
typedef struct
{
	D3DXVECTOR3 pos;	// ���W
	D3DXVECTOR3 col;	// �J���[
	int nType;			// ���
	bool bUse;			// �g�p���
	float fHeight;		// ����
	float fWidth;		// ��
	TITLESTATE state;	// ���
}Title;

//******************************************
// �O���[�o���ϐ��錾
//******************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL;			// ���_�o�b�t�@�̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitleback = NULL;		// �^�C�g���w�i
														   
LPDIRECT3DTEXTURE9 g_pTextureTitle[TITLE_MAX] = {};		// �e�N�X�`���̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureTitleback = NULL;			// �^�C�g���w�i�e�N�X�`���̃|�C���^

Title g_aTitle[TITLE_MAX];			// �^�C�g���\���̂̏��

int g_nCntTitleAnim;				// �^�C�g�����
int g_nPatternAnim;					// ��ԃp�^�[��
int g_aTimeCount;					// �^�C���J�E���g
int g_aSelect;						// �I��p�̕ϐ�

//===========================
// �^�C�g���̏���������
//===========================
void InitTitle(void)
{
	// �f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		

	// ���_���̃|�C���^
	VERTEX_2D* pVtx;				

	// �e�N�X�`���̓ǂݍ��� (�^�C�g���w�i)
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Actiontitle.png",
		&g_pTextureTitleback);

	// �I�����e�N�X�`���̓ǂݍ���
	for (int nCnt = 0; nCnt < TITLE_MAX; nCnt++)
	{
		D3DXCreateTextureFromFile(pDevice,
			TITLE_TEX[nCnt],
			&g_pTextureTitle[nCnt]);
	}

	// �O���[�o���ϐ��̏�����
	g_nCntTitleAnim = 0;		// �^�C�g�����
	g_nPatternAnim = 0;			// ��ԃp�^�[��
	g_aTimeCount = 0;			// ������
	g_aSelect = TITLE_FIRST;   // �����̑I�����

//==================
// �^�C�g���w�i
//==================
	// ���_�o�b�t�@�̐��� �w�i
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY, FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitleback,
		NULL);

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffTitleback->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 1�ڂ̒��_���
	pVtx[1].pos = D3DXVECTOR3(1280.0f,0.0f, 0.0f);		// 2�ڂ̒��_���
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
	g_pVtxBuffTitleback->Unlock();

//=========================
// �Z���N�g�\�ȃ^�C�g��
//=========================

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * TITLE_MAX,
		D3DUSAGE_WRITEONLY, FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitle,
		NULL);

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTitle = 0; nCntTitle < TITLE_MAX; nCntTitle++)
	{
		// �\���̕ϐ��̏�����
		g_aTitle[nCntTitle].nType = 0;					// ���
		g_aTitle[nCntTitle].fHeight = 0.0f;				// ����
		g_aTitle[nCntTitle].fWidth = 0.0f;				// ��
		g_aTitle[nCntTitle].bUse = false;				// ���g�p����
		g_aTitle[nCntTitle].state = TITLE_NONE;			// �������Ă��Ȃ����

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 1�ڂ̒��_���
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 2�ڂ̒��_���
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 3�ڂ̒��_���
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 4�ڂ̒��_���

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

		// �|�C���^���X�V
		pVtx += 4;
	}

	//SetTitle(TITLE_FIRST, D3DXVECTOR3(650.0f, 400.0f, 0.0f));	// �^�C�g��1
	//SetTitle(TITLE_SECOND, D3DXVECTOR3(650.0f, 550.0f, 0.0f));	// �^�C�g��2

	//�A�����b�N
	g_pVtxBuffTitle->Unlock();

	// PlaySound(SOUND_LABEL_TITLEBGM);

}
//=======================
// �^�C�g���̏I������
//=======================
void UninitTitle(void)
{
	// StopSound();
//==================
// �^�C�g���̔w�i
//==================
 
	// �e�N�X�`���̔j��
	if (g_pTextureTitleback != NULL)
	{				   
		g_pTextureTitleback->Release();
		g_pTextureTitleback = NULL;
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffTitleback != NULL)
	{
		g_pVtxBuffTitleback->Release();
		g_pVtxBuffTitleback = NULL;
	}

//==================
// �^�C�g���̏I��
//==================
	// StopSound();
	 
	// �e�N�X�`���̔j��
	for (int nCntTitle = 0; nCntTitle < TITLE_MAX; nCntTitle++)
	{

		if (g_pTextureTitle[nCntTitle] != NULL)
		{
			g_pTextureTitle[nCntTitle]->Release();
			g_pTextureTitle[nCntTitle] = NULL;
		}
	}
	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffTitle != NULL)
	{
		g_pVtxBuffTitle->Release();
		g_pVtxBuffTitle = NULL;
	}
}
//==================
// �^�C�g���̍X�V����
//==================
void UpdateTitle(void)
{
	// ���_���̃|�C���^
	VERTEX_2D* pVtx;				

	if (KeyboardTrigger(DIK_RETURN))
	{
		// �Q�[����ʂ�
		SetFade(MODE_GAME);
	}

	g_aTimeCount++; // �J�E���g���Z

	if (g_aTimeCount >= 600)
	{// ���Ԍo��
		// �����L���O��ʂ�
		SetFade(MODE_RANKING);
	}


#ifdef _DEBUG

	if (KeyboardTrigger(DIK_F1) == true)
	{
		//F1�L�[�������ꂽ
		//���[�h�ݒ�(�G�f�B�b�g��ʂɈړ�)
		SetFade(MODE_EDIT);
	}
#endif

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCount = 0; nCount < TITLE_MAX; nCount++)
	{
		// ���_�J���[��ݒ�(��)
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		pVtx += 4;
	}

	pVtx -= 4 * TITLE_MAX;		// pVtx��߂�

	// ���j���[�̑I��
	if (KeyboardTrigger(DIK_UP) == true || JoypadTrigger(JOYKEY_UP) == true || KeyboardTrigger(DIK_W) == true)
	{
		// �㉟����
		g_aSelect--;// �f�N�������g

		if (g_aSelect < TITLE_FIRST)
		{
			//��ԉ�
			g_aSelect = TITLE_SECOND;
		}
	}

	else if (KeyboardTrigger(DIK_DOWN) == true || JoypadTrigger(JOYKEY_DOWN) == true || KeyboardTrigger(DIK_S) == true)
	{
		//���������Ƃ�
		g_aSelect++;//�C���N�������g

		if (g_aSelect >= TITLE_MAX)
		{
			//��ԏ�
			g_aSelect = TITLE_FIRST;
		}
	}

	pVtx += 4 * g_aSelect;							//�I����


	pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);

	//�A�����b�N
	g_pVtxBuffTitle->Unlock();


	//if (KeyboardTrigger(DIK_RETURN) == true || JoypadTrigger(JOYKEY_A) == true)
	//{
	//	//�G���^�[�L�[�������ꂽ
	//	//�e��ʂɈړ�
	//	switch (g_aSelect)
	//	{
	//	case TITLE_FIRST:
	//		//�`���[�g���A���Ɉڍs
	//		SetFade(MODE_TUTORIAL);

	//		//Enter�L�[�������ꂽ
	//		for (int nCntTitle = 0; nCntTitle < NUM_TITLE; nCntTitle++)
	//		{
	//			if (g_aTitle[nCntTitle].bUse == true && g_aTitle[nCntTitle].nType == TITLE_FIRST)
	//			{
	//				g_aTitle[nCntTitle].state = TITLE_FLASH;
	//			}
	//		}
	//		PlaySound(SOUND_LABEL_TITLE);
	//		break;

	//	case TITLE_SECOND:

	//		//�Q�[���Ɉڍs
	//		SetFade(MODE_GAME);
	//		for (int nCntTitle = 0; nCntTitle < NUM_TITLE; nCntTitle++)
	//		{
	//			if (g_aTitle[nCntTitle].bUse == true && g_aTitle[nCntTitle].nType == TITLE_SECOND)
	//			{
	//				g_aTitle[nCntTitle].state = TITLE_FLASH;
	//			}
	//		}
	//		PlaySound(SOUND_LABEL_TITLE);

	//		break;

	//	}
	//}

	//TitleFlash(TITLE_FIRST);		//�^�C�g���̓_��
	//TitleFlash(TITLE_SECOND);		//�^�C�g���̓_��
}
//======================
// �^�C�g���̕`�揈��
//======================
void DrawTitle(void)
{
	// �f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

//=====================
// �^�C�g���w�i�̕`��
//=====================

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTitleback, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureTitleback);

	// �^�C�g���|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	
//==================
// �^�C�g���̕`��
//==================

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTitle, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntTitle = 0; nCntTitle < TITLE_MAX; nCntTitle++)
	{
		// ��ނ���
		int nType = g_aTitle[nCntTitle].nType;

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureTitle[nType]);

		// �^�C�g���|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTitle * 4, 2);
	}
}
//======================
// �^�C�g���̐ݒ菈��
//======================
void SetTitle(int nType, D3DXVECTOR3 pos,float fWidth,float fHeight)
{
	VERTEX_2D* pVtx;		// ���_���̃|�C���^

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTitle = 0; nCntTitle < TITLE_MAX; nCntTitle++)
	{
		if (!g_aTitle[nCntTitle].bUse)
		{// ���g�p��������
			g_aTitle[nCntTitle].nType = nType;		 // ���
			g_aTitle[nCntTitle].pos = pos;			 // ���W
			g_aTitle[nCntTitle].fWidth = fWidth;	 // ����
			g_aTitle[nCntTitle].fHeight = fHeight;	 // ����
			g_aTitle[nCntTitle].bUse = true;		 // �g�p����

			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aTitle[nCntTitle].pos.x - fWidth, g_aTitle[nCntTitle].pos.y - fHeight, 0.0f);		//1�ڂ̒��_���
			pVtx[1].pos = D3DXVECTOR3(g_aTitle[nCntTitle].pos.x + fWidth, g_aTitle[nCntTitle].pos.y - fHeight, 0.0f);		//2�ڂ̒��_���
			pVtx[2].pos = D3DXVECTOR3(g_aTitle[nCntTitle].pos.x - fWidth, g_aTitle[nCntTitle].pos.y + fHeight, 0.0f);		//3�ڂ̒��_���
			pVtx[3].pos = D3DXVECTOR3(g_aTitle[nCntTitle].pos.x + fWidth, g_aTitle[nCntTitle].pos.y + fHeight, 0.0f);		//4�ڂ̒��_���

			break;
		}

		pVtx += 4;
	}

	// �A�����b�N
	g_pVtxBuffTitle->Unlock();
}
//==============================
// �^�C�g���̓_�ŏ���
//==============================
void TitleFlash(int nType)
{
	// ���_���̃|�C���^
	VERTEX_2D* pVtx = 0;		

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTitle = 0; nCntTitle < TITLE_MAX; nCntTitle++)
	{
		if (g_aTitle[nCntTitle].bUse && g_aTitle[nCntTitle].state == TITLE_FLASH)
		{// �g�p��� ���� ��Ԃ��_�ŏ��
			//�@�J�E���^�[�����Z
			g_nCntTitleAnim++;				

			if (g_nCntTitleAnim == 5)		
			{//�@5�̎�
				// ���_�J���[�̐ݒ�
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.2f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.2f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.2f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.2f);

			}

			else if(g_nCntTitleAnim == 10)	
			{// 10�̎�
				// ���_�J���[�̐ݒ�
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				g_nCntTitleAnim = 0;		// �����l�ɖ߂�
			}
		}
		// ���_����i�߂�
		pVtx += 4;
	}
	// �A�����b�N
	g_pVtxBuffTitle->Unlock();
}