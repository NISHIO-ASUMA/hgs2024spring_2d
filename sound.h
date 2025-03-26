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
	SOUND_LABEL_SE_ENTER = 0,
	SOUND_LABEL_TITLEBGM,
	SOUND_LABEL_TUTOBGM,
	SOUND_LABEL_GAMEBGM,
	SOUND_LABEL_RESULTBGM,
	SOUND_LABEL_RANKINGBGM,
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
