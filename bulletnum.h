//=============================================================================
//
// �c�e���\������ [bulletnum.h]
// Author : TANEKAWA RIKU
//
//=============================================================================
#ifndef _BULLETNUM_H_
#define _BULLETNUM_H_

#include "main.h"

//�}�N����`
#define TEN (10)
#define ONE (1)
#define MAX_DIGIT (2)//���̍ő吔

//�v���g�^�C�v�錾
void InitBulletNum(void);
void UninitBulletNum(void);
void UpdateBulletNum(void);
void DrawBulletNum(void);

void SetBulletNum(int nBulletNum);
void AddBulletNum(int nBulletNum);
void DecBulletNum(int nBulletNum);
int DigitNum(int nBulletNum);
int GetBulletNum(void);

#endif
