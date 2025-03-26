//======================================
//
// �`���[�g���A������ [ tutorial.h ]
// Author:Asuma Nishio
//
//======================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

//********************************************
// �C���N���[�h�t�@�C���錾
//********************************************
#include "main.h"

//********************************************
// �`���[�g���A���̎�ނ̗񋓌^
//********************************************
typedef enum
{
	TUTORIAL_ONE = 0,		// 1�ڂ̃`���[�g���A���摜
	TUTORIAL_ENTER,			// 2�ڂ̃`���[�g���A���摜
	TUTORIAL_KEY_W,			// 3�ڂ̃`���[�g���A���摜
	TUTORIAL_KEY_A,			// 4�ڂ̃`���[�g���A���摜
	TUTORIAL_KEY_S,			// 5�ڂ̃`���[�g���A���摜
	TUTORIAL_KEY_D,			// 6�ڂ̃`���[�g���A���摜
	TUTORIAL_KEY_SPACE,		// 7�ڂ̃`���[�g���A���摜
	TUTORIAL_BUTTON_B,		// 8�ڂ̃`���[�g���A���摜
	TUTORIAL_BUTTON_UP,		// 9�ڂ̃`���[�g���A���摜
	TUTORIAL_BUTTON_DOWN,	// 10�ڂ̃`���[�g���A���摜
	TUTORIAL_BUTTON_RIGHT,	// 11�ڂ̃`���[�g���A���摜
	TUTORIAL_BUTTON_LEFT,	// 12�ڂ̃`���[�g���A���摜
	TUTORIAL_KEY_P,         // P�L�[
	TUTORIAL_BUTTON_ST,		// start�{�^��
	TUTORIAL_KEY_E,			// E�L�[
	TUTORIAL_KEY_Q,			// Q�L�[
	TUTORIAL_BUTTON_RB,		// RB�{�^��
	TUTORIAL_BUTTON_LB,		// LB�{�^��
	TUTORIAL_LSTICK,		// L�X�e�B�b�N
	TUTORIAL_MAX
}TUTORIAL;

//********************************************
// �`���[�g���A���̏�ԗ񋓌^
//********************************************
typedef enum
{
	TUTORIALSTATE_NONE = 0, // �^�C�g�����(�ʏ���)
	TUTORIALSTATE_FLASH,	// �_�ŏ��
	TUTORIALSTATE_MAX
}TUTORIALSTATE;

//********************************************
// �`���[�g���A���̃e�N�X�`���̐ݒ�
//********************************************
static const char* TUTORIAL_TEXTURE[TUTORIAL_MAX] =   
{
	"data\\TEXTURE\\Enter000.png",		//1��
	"data\\TEXTURE\\Enter000.png",			//2��
	"data\\TEXTURE\\tutorial020.png",		//3��(w)
	"data\\TEXTURE\\tutorial030.png",		//4��(a)
	"data\\TEXTURE\\tutorial040.png",		//5��(s)
	"data\\TEXTURE\\tutorial050.png",		//6��(d)
	"data\\TEXTURE\\tutorial060.png",		//7��(�X�y�[�X�L�[)
	"data\\TEXTURE\\tutorialbuttonB.png",	//8��(b�{�^��)
	"data\\TEXTURE\\tutorialup.png",		//9��(UP�{�^��)
	"data\\TEXTURE\\tutorialdown.png",		//10��(down�{�^��)
	"data\\TEXTURE\\tutorialright.png",		//11��(�E�{�^��)
	"data\\TEXTURE\\tutorialleft.png",		//12��(���{�^��)
	"data\\TEXTURE\\tutorialbuttonA.png",	//P�L�[
	"data\\TEXTURE\\tutorialbuttonS.png",	//statr�{�^��
	"data\\TEXTURE\\tutorial080.png",		//15��(e)
	"data\\TEXTURE\\tutorial090.png",		//16��(q)
	"data\\TEXTURE\\tutorial100.png",		//17��(RB)
	"data\\TEXTURE\\tutorial110.png",		//18��(LB)
	"data\\TEXTURE\\tutorialbuttonL.png"	//19��(L�X�e�B�b�N)
};

//********************************************
// �v���g�^�C�v�錾
//********************************************
void InitTutorial(void);						// �`���[�g���A���̏�����
void UninitTutorial(void);						// �`���[�g���A���̏I��
void UpdateTutorial(void);						// �`���[�g���A���̍X�V
void DrawTutorial(void);						// �`���[�g���A���̕`��
void SetTutorial(int nType, D3DXVECTOR3 pos);   // �`���[�g���A���̐ݒ�
void TutorialFlash(int nType);					// �`���[�g���A���̓_��
bool GetIsNext(void);							// �`���[�g���A���̎��̃y�[�W
int GetTutorialCountMove(void);					// �J�E���^�[���擾

#endif