//==================================
//
// �G�f�B�^�[��ʏ��� [ edit.h ]
// Author:Asuma Nishio
//
//==================================
#ifndef _EDIT_H_
#define _EDIT_H_

//***********************************************
// �C���N���[�h�t�@�C���錾
//***********************************************
#include "main.h"

//***********************************************
// �v���g�^�C�v�錾�錾
//***********************************************
void InitEdit(void);			//�G�f�B�b�g��ʂ̏�����
void UninitEdit(void);			//�G�f�B�b�g��ʂ̏I��
void UpdateEdit(void);			//�G�f�B�b�g��ʂ̍X�V
void DrawEdit(void);			//�G�f�B�b�g��ʂ̕`��
int GeteditBlock();
int ReturnPass();

#endif