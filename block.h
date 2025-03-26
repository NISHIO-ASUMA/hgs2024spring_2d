//=============================
//
// �u���b�N�̏��� [block.h]
// Author:NISHIO ASUMA
//
//=============================
#ifndef _BLOCK_H_
#define _BLOCK_H_

//******************************
// �C���N���[�h�t�@�C���錾
//******************************
#include "main.h"

//******************************
// �}�N����`
//******************************
#define MAX_BLOCK (256)			 // �u���b�N�̍ő吔
#define MAX_BLOCKSIZE_X (100.0f) // �u���b�N�̃T�C�Y
#define MAX_BLOCKSIZE_Y (100.0f) // �u���b�N�̃T�C�Y
#define HALF (0.5f)				 // ����

//******************************
// �u���b�N�̗񋓌^���
//******************************
typedef enum
{
	BLOCKTYPE_GROUND = 0,			// ��1
	BLOCKTYPE_WALL,					// ��2
	BLOCKTYPE_NORMAL,				// �m�[�}���u���b�N
	BLOCKTYPE_VERTICAL,				// �c�����u���b�N
	BLOCKTYPE_HORIZONTAL,			// �������u���b�N
	BLOCKTYPE_MAX
}BLOCKTYPE;

//**************************
// �u���b�N�\���̂̒�`
//**************************
typedef struct
{
	D3DXVECTOR3 pos;			// �ʒu
	D3DXVECTOR3 move;			// �ړ���
	bool bUse;					// �g�p���Ă��邩�ǂ���
	float fWidth;				// ����
	float fHeight;				// ����
	int nType;					// ���
	int nLife;					// �̗�
	bool bHitBlock;				// �����������ǂ���
	int nCntBlockstateCount;    // �u���b�N�̏�ԊǗ��J�E���^�[
	D3DXVECTOR3 bOldpos;		// �O��̈ʒu
}Block;

//**************************
// �u���b�N�̃e�N�X�`���ݒ�
//**************************
static const char* BLOCK_TEXTURE[BLOCKTYPE_MAX] =
{
	"data\\TEXTURE\\Block000.png",			// �n�ʃu���b�N
	"data\\TEXTURE\\Block001.png",			// �ǃu���b�N
	"data\\TEXTURE\\Block_normal.png",		// �m�[�}���u���b�N
	"data\\TEXTURE\\Block_vertical.png",	// �c�����u���b�N
	"data\\TEXTURE\\Block_horizontal.png",	// �������u���b�N

};

//*************************
// �v���g�^�C�v�錾
//*************************
void InitBlock(void);			// �u���b�N�̏�����
void UninitBlock(void);			// �u���b�N�̏I��
void UpdateBlock(void);			// �u���b�N�̍X�V
void DrawBlock(void);			// �u���b�N�̕`��
void SetBlock(D3DXVECTOR3 pos, int nType, float fWidth, float fHeight);		// �u���b�N�̐ݒ菈��			
bool CollisionBlockBotton(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, float fWidth, float fHeight); // ���݂̈ʒu,�O��̈ʒu,�ړ���,��,����
bool CollisionBlockX(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, float fWidth, float fHeight);  // ���݂̈ʒu,�O��̈ʒu,�ړ���,��,����
bool CollisionBlockOn(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, float fWidth, float fHeight); // ���݂̈ʒu,�O��̈ʒu,�ړ���,��,����
Block* GetBlockInfo(void);		// �u���b�N���
int GetBlock(void);				// �u���b�N���̎擾
void LoadBlockText(void);		// �z�u���̓ǂݍ��ݏ���
void HitBlock(int nCntBlock, int nDamage); // �q�b�g����

#endif

