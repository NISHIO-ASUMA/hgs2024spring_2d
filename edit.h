//==================================
//
// エディター画面処理 [ edit.h ]
// Author:Asuma Nishio
//
//==================================
#ifndef _EDIT_H_
#define _EDIT_H_

//***********************************************
// インクルードファイル宣言
//***********************************************
#include "main.h"

//***********************************************
// プロトタイプ宣言宣言
//***********************************************
void InitEdit(void);			//エディット画面の初期化
void UninitEdit(void);			//エディット画面の終了
void UpdateEdit(void);			//エディット画面の更新
void DrawEdit(void);			//エディット画面の描画
int GeteditBlock();
int ReturnPass();

#endif