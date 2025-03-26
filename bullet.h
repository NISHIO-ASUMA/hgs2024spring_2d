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
	BULLETTYPE_ENEMY,
	BULLETTYPE_MAX
}BULLETTYPE;

//�v���g�^�C�v�錾
void InitBullet(void);				//�e�̏�����
void UninitBullet(void);			//�e�̏I��
void UpdateBullet(void);			//�e�̍X�V
void DrawBullet(void);				//�e�̕`��

void SetBullet(D3DXVECTOR3 pos,D3DXVECTOR3 move, D3DXVECTOR3 rot, float fWidth,float fHeight,int nLife,BULLETTYPE type/*,int nBulletType*/);//�e�̐ݒ菈��

#endif

