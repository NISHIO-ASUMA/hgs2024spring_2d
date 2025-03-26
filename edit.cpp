//==================================
//
// �G�f�B�^�[��ʏ��� [ edit.h ]
// Author:Asuma Nishio
//
//==================================

//***********************************************
// �C���N���[�h�t�@�C���錾
//***********************************************
#include "edit.h"
#include "input.h"
#include <stdio.h>
#include "block.h"
#include "fade.h"

//***********************************************
// �}�N����`
//***********************************************
#define EDIT_MOVE (10.0f)		// �z�u���̈ړ���

//***********************************************
// �����o���p�X�̗񋓌^
//***********************************************
typedef enum
{
	FILLPASS_0 = 0,
	FILLPASS_1,
	FILLPASS_2,
	FILLPASS_3,
	FILLPASS_4,
	FILLPASS_5,
	FILLPASS_6,
	FILLPASS_MAX
}FILLPASS;

const char* TEXT_FILEPASS[FILLPASS_MAX] = 
{
	"data\\TEXT\\SetBlock000.txt",
	"data\\TEXT\\SetBlock001.txt",
	"data\\TEXT\\SetBlock002.txt",
	"data\\TEXT\\SetBlock003.txt",
	"data\\TEXT\\SetBlock004.txt",
	"data\\TEXT\\SetBlock005.txt",
	"data\\TEXT\\SetBlock006.txt",

};

//***********************************************
// �G�f�B�^�[�\���̂̐錾
//***********************************************
typedef struct
{
	D3DXVECTOR3 pos;	// �ʒu
	int nType;			// ���
	float fWidth;		// ����
	float fHeight;		// ����
	bool bUse;			// �g�p���
}EditInfo;

//***********************************************
// �O���[�o���ϐ��錾
//***********************************************
int g_nCntEdit;								// �z�u���������J�E���g
EditInfo g_aEditInfo[MAX_BLOCK];			// �\���̏��
LPDIRECT3DTEXTURE9 g_apTextureEdit[BLOCKTYPE_MAX] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEdit = NULL;			// ���_�o�b�t�@�̃|�C���^
int nFillPass = 0;

void SaveEdit(void);			//�t�@�C�������o��
void ReloadEdit();				// �ēǂݍ���
//========================
// �G�f�B�^�[����������
//========================
void InitEdit(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^

	// �\���̕ϐ��̏�����
	for (int nCnt = 0; nCnt < MAX_BLOCK; nCnt++)
	{
		g_aEditInfo[nCnt].nType = 0;							// ���
		g_aEditInfo[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu
		g_aEditInfo[nCnt].fHeight = 20.0f;						// ����
		g_aEditInfo[nCnt].fWidth = 20.0f;						// ����
		g_aEditInfo[nCnt].bUse = false;							// �g�p����
	}

	// �ŏ��̔ԍ������g�p����
	g_aEditInfo[0].bUse = true;
	g_nCntEdit = 0;
	nFillPass = FILLPASS_0; // �����p�X

	// ���_���̃|�C���^
	VERTEX_2D* pVtx;			

	for (int nCnt = 0; nCnt < BLOCKTYPE_MAX; nCnt++)
	{
		//�G�̃e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			BLOCK_TEXTURE[nCnt],
			&g_apTextureEdit[nCnt]);
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BLOCK,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEdit,
		NULL);

	//���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffEdit->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEdit = 0; nCntEdit < MAX_BLOCK; nCntEdit++)
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
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	// �A�����b�N
	g_pVtxBuffEdit->Unlock();
}
//========================
// �G�f�B�^�[�I������
//========================
void UninitEdit(void)
{
	// �e�N�X�`���̔j��
	for (int nCntEdit = 0; nCntEdit < BLOCKTYPE_MAX; nCntEdit++)
	{
		if (g_apTextureEdit[nCntEdit] != NULL)
		{
			g_apTextureEdit[nCntEdit]->Release();
			g_apTextureEdit[nCntEdit] = NULL;
		}
	}
	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffEdit != NULL)
	{
		g_pVtxBuffEdit->Release();
		g_pVtxBuffEdit = NULL;
	}
}
//========================
//�@�G�f�B�^�[�X�V����
//========================
void UpdateEdit(void)
{
	// �ړ�����
	if (GetKeyboardPress(DIK_A) == true)
	{
		// A�P�̂̏���
		g_aEditInfo[g_nCntEdit].pos.x -= EDIT_MOVE;
	}
	else if (GetKeyboardPress(DIK_D) == true)
	{
		// D�P�̂̏���
		g_aEditInfo[g_nCntEdit].pos.x += EDIT_MOVE;
	}
	else if (GetKeyboardPress(DIK_W) == true)
	{
		// W�P�̂̏���
		g_aEditInfo[g_nCntEdit].pos.y -= EDIT_MOVE;
	}
	else if (GetKeyboardPress(DIK_S) == true)
	{
		// S�P�̂̏���
		g_aEditInfo[g_nCntEdit].pos.y += EDIT_MOVE;
	}

	if (KeyboardTrigger(DIK_UP) == true)
	{// ��ސ؂�ւ�
		if (g_aEditInfo[g_nCntEdit].nType < BLOCKTYPE_MAX - 1)
		{
			// �ő吔��菬�����Ƃ�
			g_aEditInfo[g_nCntEdit].nType++;
		}

	}
	else if (KeyboardTrigger(DIK_DOWN) == true)
	{// ��ސ؂�ւ�
		if (g_aEditInfo[g_nCntEdit].nType > BLOCKTYPE_BULLETBLOCK)
		{
			// 0���傫���Ƃ�
			g_aEditInfo[g_nCntEdit].nType--;
		}
	}

	// �z�u����
	if (KeyboardTrigger(DIK_RETURN) == true)
	{
		// ���̓G�̈ʒu��z�u�����G�Ɠ����ɂ���
		g_aEditInfo[g_nCntEdit + 1].pos = g_aEditInfo[g_nCntEdit].pos;

		// ��ނ𓯂��ɂ���
		g_aEditInfo[g_nCntEdit + 1].nType = g_aEditInfo[g_nCntEdit].nType;

		// ����,�����𓯂��ɂ���
		g_aEditInfo[g_nCntEdit + 1].fHeight = g_aEditInfo[g_nCntEdit].fHeight;

		g_aEditInfo[g_nCntEdit + 1].fWidth = g_aEditInfo[g_nCntEdit].fWidth;

		// �g�p����ɂ���
		g_aEditInfo[g_nCntEdit + 1].bUse = true;	

		// �C���N�������g
		g_nCntEdit++;
	}

	if (g_nCntEdit > 0)
	{// 0�ȏ�Ȃ�
		if (KeyboardTrigger(DIK_BACKSPACE))
		{// BACKSPACE�L�[�������ꂽ

			// ���g�p����ɂ���
			g_aEditInfo[g_nCntEdit].bUse = false;

			// �f�N�������g
			g_nCntEdit--;
		}
	}

	if (KeyboardTrigger(DIK_Q))
	{
		// ��������
		g_aEditInfo[g_nCntEdit].fWidth += 5.0f;
	}
	else if (KeyboardTrigger(DIK_E))
	{
		// ��������
		g_aEditInfo[g_nCntEdit].fWidth -= 5.0f;

		if (g_aEditInfo[g_nCntEdit].fWidth <= 5.0f)
		{// 5�ȉ��Ȃ�
			g_aEditInfo[g_nCntEdit].fWidth = 5.0f;
		}
	}

	if (KeyboardTrigger(DIK_Z))
	{
		// ��������
		g_aEditInfo[g_nCntEdit].fHeight += 5.0f;
	}
	else if (KeyboardTrigger(DIK_C))
	{
		// ��������
		g_aEditInfo[g_nCntEdit].fHeight -= 5.0f;

		if (g_aEditInfo[g_nCntEdit].fHeight <= 5.0f)
		{// 5�ȉ��Ȃ�
			g_aEditInfo[g_nCntEdit].fHeight = 5.0f;
		}
	}

	if (KeyboardTrigger(DIK_F4))
	{
		// �t�@�C���p�X��؂�ւ�
		nFillPass = (nFillPass + 1) % FILLPASS_MAX;

	}

	// �����o������
	if (KeyboardTrigger(DIK_F7) == true)
	{
		//�ۑ�����
		SaveEdit();
	}

	if (KeyboardTrigger(DIK_F9))
	{
		ReloadEdit();
	}

	if (KeyboardTrigger(DIK_F3))
	{
		// ��ʑJ��
		SetFade(MODE_GAME);
	}

	// ���_���̃|�C���^
	VERTEX_2D* pVtx;	

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffEdit->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEdit = 0; nCntEdit < MAX_BLOCK; nCntEdit++)
	{
		if (g_aEditInfo[nCntEdit].bUse)
		{
			// ���_���̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aEditInfo[nCntEdit].pos.x - g_aEditInfo[nCntEdit].fWidth, g_aEditInfo[nCntEdit].pos.y - g_aEditInfo[nCntEdit].fHeight, 0.0f); // 1�ڂ̒��_���
			pVtx[1].pos = D3DXVECTOR3(g_aEditInfo[nCntEdit].pos.x + g_aEditInfo[nCntEdit].fWidth, g_aEditInfo[nCntEdit].pos.y - g_aEditInfo[nCntEdit].fHeight, 0.0f); // 2�ڂ̒��_���
			pVtx[2].pos = D3DXVECTOR3(g_aEditInfo[nCntEdit].pos.x - g_aEditInfo[nCntEdit].fWidth, g_aEditInfo[nCntEdit].pos.y + g_aEditInfo[nCntEdit].fHeight, 0.0f); // 3�ڂ̒��_���
			pVtx[3].pos = D3DXVECTOR3(g_aEditInfo[nCntEdit].pos.x + g_aEditInfo[nCntEdit].fWidth, g_aEditInfo[nCntEdit].pos.y + g_aEditInfo[nCntEdit].fHeight, 0.0f); // 4�ڂ̒��_���
		}

		pVtx += 4;
	}
	// �A�����b�N
	g_pVtxBuffEdit->Unlock();

}
//========================
// �G�f�B�^�[�`�揈��
//========================
void DrawEdit(void)
{
	// �f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffEdit, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntEdit = 0; nCntEdit < MAX_BLOCK; nCntEdit++)
	{
		if (g_aEditInfo[nCntEdit].bUse)
		{// �g�p����Ă�
			// ��ނ�ۑ�����
			int nType = g_aEditInfo[nCntEdit].nType;

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureEdit[nType]);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEdit * 4, 2);

		}
	}
}
//========================
// �����o������
//========================
void SaveEdit(void)
{
	FILE* pFile;	// �t�@�C���|�C���^��錾

	//�t�@�C�����J��
	pFile = fopen(TEXT_FILEPASS[nFillPass],"w");

	if (pFile != NULL)
	{// �����o���J�n
		int nCnt = 0; // �J�E���g�p�ϐ�

		// �^�C�g���֌W
		fprintf(pFile, "//**************************************************\n");
		fprintf(pFile, "//                  �u���b�N�z�u���              \n");
		fprintf(pFile, "//**************************************************\n\n");

		// �z�u��,��ސ�
		fprintf(pFile, "NUM_BLOCK =   %d   # ���z�u��\n\n", g_nCntEdit);

		while (nCnt < g_nCntEdit)
		{// nCnt�����̕����������o��

			// �u���b�N���Ƃ̔z�u���
			fprintf(pFile, "//-------------------------------\n");
			fprintf(pFile, "//   [ %d ] �Ԗڂ̃u���b�N\n", nCnt);
			fprintf(pFile, "//-------------------------------\n");
			fprintf(pFile, "SETBLOCK\n");
			fprintf(pFile, "POS  = %.2f %.2f %.2f # ���W\n", g_aEditInfo[nCnt].pos.x, g_aEditInfo[nCnt].pos.y, g_aEditInfo[nCnt].pos.z);
			fprintf(pFile, "WIDTH = %.2f  # ����\n", g_aEditInfo[nCnt].fWidth);
			fprintf(pFile, "HEIGHT = %.2f # ����\n", g_aEditInfo[nCnt].fHeight);
			fprintf(pFile, "TYPE = %d     # ��ޔԍ�\n", g_aEditInfo[nCnt].nType);
			fprintf(pFile, "END_SETBLOCK\n\n");

			// ���Z
			nCnt++;
		}
	}
	else
	{
		// �t�@�C�����J���Ȃ�������
		// ���b�Z�[�WBOX�̕\��
		MessageBox(NULL, "�J���܂���", "�G���[", MB_OK);

		return;
	}

	//�t�@�C�������
	fclose(pFile);
}
//========================
// �ēǂݍ��ݏ���
//========================
void ReloadEdit()
{
	// �t�@�C���|�C���^��錾
	FILE* pFile;

	// �C�ӂ̃t�@�C�����J��
	pFile = fopen(TEXT_FILEPASS[nFillPass], "r");

	if (pFile != NULL)
	{// �t�@�C�����J������

		// �\���̂�������
		for (int nCnt = 0; nCnt < MAX_BLOCK; nCnt++)
		{
			g_aEditInfo[nCnt].nType = 0;							// ���
			g_aEditInfo[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu
			g_aEditInfo[nCnt].fHeight = 20.0f;						// ����
			g_aEditInfo[nCnt].fWidth = 20.0f;						// ����
			g_aEditInfo[nCnt].bUse = false;							// �g�p����
		}

		// ���[�J���ϐ�---------------------------------
		int nData = 0; // �t�@�C���ǂݍ��݂̕Ԃ�l
		int nCnt = 0; // �J�E���g�p�ϐ�
		char aString[256] = {}; // �������ǂݍ���
		char aGomi[5] = {}; // =���i�[���镶����
		//-----------------------------------------------

		while (1)
		{
			// nData�ɑ������
			nData = fscanf(pFile, "%s", &aString[0]);

			if (strcmp(aString, "NUM_BLOCK") == 0)
			{// NUM_BLOCK��ǂݎ������
				// �ǂݔ�΂�
				fscanf(pFile, "%s", &aGomi[0]);

				// �u���b�N�̔z�u���ꂽ����ǂݍ���
				fscanf(pFile, "%d", &g_nCntEdit);
			}

			if (strcmp(aString, "SETBLOCK") == 0)
			{// START_BLOCKSET��ǂݎ������

				while (1)
				{
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(aString, "POS") == 0)
					{// POS��ǂݎ������
						// �ǂݔ�΂�
						fscanf(pFile, "%s", &aGomi[0]);

						// ���W����
						fscanf(pFile, "%f", &g_aEditInfo[nCnt].pos.x);
						fscanf(pFile, "%f", &g_aEditInfo[nCnt].pos.y);
						fscanf(pFile, "%f", &g_aEditInfo[nCnt].pos.z);

					}

					if (strcmp(aString, "WIDTH") == 0)
					{// WIDTH��ǂݎ������
						// �ǂݔ�΂�
						fscanf(pFile, "%s", &aGomi[0]);

						// ��������
						fscanf(pFile, "%f", &g_aEditInfo[nCnt].fWidth);

					}

					if (strcmp(aString, "HEIGHT") == 0)
					{// HEIGHT��ǂݎ������
						// �ǂݔ�΂�
						fscanf(pFile, "%s", &aGomi[0]);

						// ��������
						fscanf(pFile, "%f", &g_aEditInfo[nCnt].fHeight);

					}

					if (strcmp(aString, "TYPE") == 0)
					{// TYPE��ǂݎ������
						// �ǂݔ�΂�
						fscanf(pFile, "%s", &aGomi[0]);

						// ���݂̃u���b�N�̎�ޔԍ�����
						fscanf(pFile, "%d", &g_aEditInfo[nCnt].nType);
					}

					if (strcmp(aString, "END_SETBLOCK") == 0)
					{// END_BLOCKSET��ǂݎ������
						// �g�p����ɂ���
						g_aEditInfo[nCnt].bUse = true;

						// �C���N�������g���Ď��̃u���b�N����
						nCnt++;

						break;
					}
				}
			}

			if (nData == EOF)
			{// EOF��ǂݎ������
				// �����������Ԃɖ߂�
				g_aEditInfo[g_nCntEdit].bUse = true;

				break;
			}
		}
	}
	else
	{
		// ���b�Z�[�WBOX�̕\��
		MessageBox(NULL, "�ēǂݍ��ݎ��s(ReloadTextFile)", "�G���[", MB_OK);
		pFile = NULL;
		return ;
	}

	// �t�@�C�������
	fclose(pFile);


}

int GeteditBlock()
{
	return g_nCntEdit;
}
int ReturnPass()
{
	return nFillPass;
}