//===============================
//
// 背景処理 [ background.h ]
// Author:Asuma Nishio
//
//===============================
#ifndef _BACKGROUND_H_  // このマクロ定義がされてなかったら
#define _BACKGROUND_H_  // 2重インクルード防止のマクロ定義

//******************************************
// インクルードファイル宣言
//******************************************
#include "main.h"

//******************************************
// プロトタイプ宣言
//******************************************
void InitBackground(void);		// 背景の初期化
void UninitBackground(void);	// 背景の終了
void UpdateBackground(void);	// 背景の更新処理
void DrawBackground(void);		// 背景の描画処理

#endif
