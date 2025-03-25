//===============================
//
// タイトル処理 [ title.cpp ]
// Author:NISHIO ASUMA
//
//===============================
#ifndef  _TITLE_H_
#define  _TITLE_H_

//******************************************
// インクルードファイル宣言
//******************************************
#include "main.h"

//******************************************
// プロトタイプ宣言	
//******************************************
void InitTitle(void);					 // タイトルの初期化
void UninitTitle(void);					 // タイトルの終了
void UpdateTitle(void);					 // タイトルの更新
void DrawTitle(void);					 // タイトルの描画
void SetTitle(int nType,D3DXVECTOR3 pos);// タイトルの設定
void TitleFlash(int nType);				 // タイトルの点滅

#endif
