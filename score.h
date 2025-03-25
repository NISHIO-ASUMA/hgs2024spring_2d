//---------------------------
//
//スコア処理
//Author:NISHIO ASUMA
//
//---------------------------

#ifndef  _SCORE_H_
#define  _SCORE_H_

#include "main.h"

//***************************************
// マクロ定義
//***************************************
#define MAX_NUM_SCORE (8)		// 表示する桁の最大数
#define NUM_DIGITS_8 (10000000) // 割る最大桁数(8桁)
#define NUM_DIGITS_7 (1000000)  // 割る桁数(7桁)

//プロト
void InitScore(void);				//スコアの初期化
void UninitScore(void);				//スコアの終了
void UpdateScore(void);				//スコアの更新
void DrawScore(void);				//スコアの描画
void SetScore(int nScore);			//スコアの設定
void AddScore(int nValue);			//スコアの加算
int ScoreCounter(int nScore);		//スコアの0をカウント
int GetScore(void);					//スコアを取得
void SaveScore(void);				//スコアの書き出し

#endif // ! _SCORE_H_
