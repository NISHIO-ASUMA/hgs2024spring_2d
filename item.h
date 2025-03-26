//===============================
//
// �A�C�e������ [item.h]
// Author:Asuma Nishio
//
//================================
#ifndef _ITEM_H_
#define _ITEM_H_

#include "main.h"

// �A�C�e���^�C�v�̗񋓌^
typedef enum
{
	ITEMTYPE_ONE = 0,			// 1��
	ITEMTYPE_TWO,				// 2��
	ITEMTYPE_THREE,				// 3��
	ITEMTYPE_FOUR,				// 4��
	ITEMTYPE_FIVE,				// 5��
	ITEMTYPE_SIX,				// 6��
	ITEMTYPE_MAX
}ITEMTYPE;

// ���
typedef enum
{
	ITEMSTATE_POP = 0,
	ITEMSTATE_MOVE,
	ITEMSTATE_STOP,
	ITEMSTATE_MOVEUNDER,
	ITEMSTATE_MAX,
}ITEMSTATE;

// �e�N�X�`��
static const char* ITEM_TEXTURE[ITEMTYPE_MAX] =   // �A�C�e���̃e�N�X�`���̐ݒ�
{
	"data\\TEXTURE\\item000.png",
	"data\\TEXTURE\\item001.png",
	"data\\TEXTURE\\item002.png",
	"data\\TEXTURE\\item003.png",
	"data\\TEXTURE\\item004.png",
	"data\\TEXTURE\\item005.png",
};

// �A�C�e���̍\����
typedef struct
{
	D3DXVECTOR3 pos;						// ���W
	D3DXVECTOR3 move;						// �ړ���
	D3DXCOLOR col;							// �F
	int nType;								// �G�̎��
	bool bUse;								// �g�p���Ă��邩
	int nLife;								// �A�C�e���̗̑�
	int nItemCounter;						// �A�C�e�������J�E���g
	bool bRight;                            // �t���O
	ITEMSTATE state;						// �A�C�e�����
}ITEM;

// �v���g�^�C�v�錾							   
void InitItem(void);						// ������
void UninitItem(void);						// �I��
void UpdateItem(void);						// �X�V
void DrawItem(void);						// �`��
void SetItem(D3DXVECTOR3 pos,int nType);	// �ݒ�
void HitItem(int nCntItem, int nDamage);					// �A�C�e���̓����蔻��
ITEM* GetItem(void);						// ������擾

#endif