//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : ASUMA NISHIO
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// �T�E���h�ꗗ
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_BGM = 0,
	SOUND_LABEL_SE,
	SOUND_LABEL_SE_ENTER,
	SOUND_LABEL_GAME,
	SOUND_LABEL_PLAYERBULLET,
	SOUND_LABEL_TITLE,
	SOUND_LABEL_TITLEBGM,
	SOUND_LABEL_TUTORIAL,
	SOUND_LABEL_ITEM,
	SOUND_LABEL_EXPLOSION,
	SOUND_LABEL_RANKING,
	SOUND_LABEL_RESULT,
	SOUND_LABEL_HIT,
	SOUND_LABEL_PAUSE,
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// �T�E���h���̍\���̒�`
//*****************************************************************************
typedef struct
{
	const char* pFilename;	// �t�@�C����
	int nCntLoop;			// ���[�v�J�E���g
} SOUNDINFO;


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);
SOUNDINFO* GetSound(void);//�T�E���h�̎擾

#endif
