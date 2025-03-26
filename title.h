//=============================================================================
//
// タイトル処理 [title.h]
// Author : TANEKAWA RIKU
//
//=============================================================================
#ifndef _TITLE_H_//このマクロ定義がされていなかったら
#define _TITLE_H_//2重インクルード防止のマクロ定義

#include "main.h"

//タイトルの種類の構造体
typedef enum
{
	TITLE_FIRST = 0,//一つ目のタイトル
	TITLE_SECOND,//二つ目のタイトル
	TITLE_MAX
}TITLE;

//タイトルの状態
typedef enum
{
	TITLE_NONE = 0,//通常状態
	TITLE_FLASH,//点滅状態
	STATE_MAX
}TITLESTATE;

//プロトタイプ宣言
void InitTitle(void);
void UninitTitle(void);
void UpdateTitle(void);
void DrawTitle(void);

void SetTitle(int nType, D3DXVECTOR3 pos);//タイトルの設定
void TitleFlash(int nType);//タイトルの点滅

#endif