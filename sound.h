//=============================================================================
//
// サウンド処理 [sound.h]
// Author : ASUMA NISHIO
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// サウンド一覧
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
// サウンド情報の構造体定義
//*****************************************************************************
typedef struct
{
	const char* pFilename;	// ファイル名
	int nCntLoop;			// ループカウント
} SOUNDINFO;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);
SOUNDINFO* GetSound(void);//サウンドの取得

#endif
