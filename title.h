//===============================
//
// �^�C�g������ [ title.cpp ]
// Author:NISHIO ASUMA
//
//===============================
#ifndef  _TITLE_H_
#define  _TITLE_H_

//******************************************
// �C���N���[�h�t�@�C���錾
//******************************************
#include "main.h"

//******************************************
// �v���g�^�C�v�錾	
//******************************************
void InitTitle(void);					 // �^�C�g���̏�����
void UninitTitle(void);					 // �^�C�g���̏I��
void UpdateTitle(void);					 // �^�C�g���̍X�V
void DrawTitle(void);					 // �^�C�g���̕`��
void SetTitle(int nType,D3DXVECTOR3 pos);// �^�C�g���̐ݒ�
void TitleFlash(int nType);				 // �^�C�g���̓_��

#endif
