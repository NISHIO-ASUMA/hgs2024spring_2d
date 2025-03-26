//---------------------------
//
//�e�̏���
//Author:NISHIO ASUMA
//
//---------------------------

#ifndef _BULLET_H_
#define _BULLET_H_

#include "main.h"


//�e�̎�ނ̗񋓌^
typedef enum
{
	BULLETTYPE_PLAYER = 0,
	BULLETTYPE_BLOCK,
	BULLETTYPE_MAX
}BULLETTYPE;

static const char* BULLET_TEXTURE[BULLETTYPE_MAX] =
{
	"data\\TEXTURE\\bullet.png",	// �v���C���[
	"data\\TEXTURE\\bullet001.png",	// �u���b�N
};

//�v���g�^�C�v�錾
void InitBullet(void);				//�e�̏�����
void UninitBullet(void);			//�e�̏I��
void UpdateBullet(void);			//�e�̍X�V
void DrawBullet(void);				//�e�̕`��

void SetBullet(D3DXVECTOR3 pos,D3DXVECTOR3 move, D3DXVECTOR3 rot, float fWidth,float fHeight,int nLife,BULLETTYPE type);//�e�̐ݒ菈��

#endif

