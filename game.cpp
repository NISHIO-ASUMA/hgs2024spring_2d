//========================================
//
// ゲーム処理 [ game.cpp ]
// Author:Asuma Nishio
//
//========================================

//***********************************************
// インクルードファイル宣言
//***********************************************
#include "game.h"
#include "Player.h"
#include "background.h"
#include "input.h"
#include "fade.h"
#include "score.h"
#include "title.h"
#include "ranking.h"
#include "rankscore.h"
#include "block.h"
#include "pause.h"
#include "time.h"
#include "bullet.h"
#include "explosion.h"
#include "item.h"
#include "bulletnum.h"
#include "sound.h"

//***********************************************
// マクロ定義
//***********************************************

//***********************************************
// グローバル変数宣言
//***********************************************
GAMESTATE g_gameState = GAMESTATE_NONE;		// ゲーム状態
int g_nCounterGameState = 0;				// 状態管理カウンター
bool g_bPause = false;						// ポーズ中かどうか
int g_nCntWaveTimeCount = 0;				// 読み込みウェーブカウント

//=====================
// ゲームの初期化処理
//=====================
void InitGame(void)
{
	InitBackground();		// 背景の初期化

	InitBlock();			// ブロックの初期化

	InitPause();			// ポーズの初期化

	//InitTime();				// タイムの初期化

	InitBullet();			// 弾の初期化

	InitPlayer();			// プレイヤーの初期化

	InitExplosion();		// 爆発の初期化

	InitScore();			// スコアの初期化

	InitItem();				// アイテムの初期化

	InitBulletNum();		// 残弾数の初期化

	LoadBlockText();		// 配置したブロック情報の読み込み

#if 0
	InitEffect();			//エフェクト

	InitParticle();			//パーティクル

#endif

	g_gameState = GAMESTATE_NORMAL;	// 通常状態に設定
	g_nCounterGameState = 0;		// ゲーム状態
	g_bPause = false;				// ポーズ解除
	g_nCntWaveTimeCount = 0;		// クールタイム

	PlaySound(SOUND_LABEL_GAMEBGM);

}
//===================
// ゲームの終了
//===================
void UninitGame(void)
{
	SaveScore();			// スコアを保存

	StopSound();			// 音楽停止

	UninitBlock();			// ブロックの終了

	UninitPause();			// ポーズの終了

	UninitBackground();

	//UninitTime();			// タイムの終了処理

	UninitPlayer();			// プレイヤーの終了処理	

	UninitBullet();			// 弾の終了処理

	UninitExplosion();		// 爆発終了処理

	UninitScore();			// スコアの終了

	UninitItem();			// アイテムの終了

	UninitBulletNum();		// 残弾数の終了処理

#if 0

	UninitEffect();			//エフェクト

	UninitParticle();		//パーティクル

#endif
}
//===================
// ゲームの更新
//===================
void UpdateGame(void)
{
	// ポーズ画面
	if (KeyboardTrigger(DIK_P) == true || JoypadTrigger(JOYKEY_START))
	{
		// pが押された or startボタン
		g_bPause = g_bPause ? false : true;
	}


	if (g_bPause == true)
	{// ポーズ中
		UpdatePause();		// ポーズの更新処理
	}
	else
	{
		UpdateBlock();		// ブロックの更新

		UpdateBullet();		// 弾の更新処理
		
		UpdatePlayer();		// プレイヤーの更新処理

		UpdateExplosion();	// 爆発の更新処理

		UpdateItem();		// アイテムの更新

		UpdateBulletNum();	// 残弾数の更新
	}

	if ((GetFinish() == true  || GetBulletNum() <= 0)
		&& g_gameState != GAMESTATE_NONE)
	{
		g_gameState = GAMESTATE_END;  //終了状態

		//PlaySound(SOUND_LABLE_EXIT);
	}

	if (GetBlock() <= 0)
	{// ステージのブロックを全部消したら

		// カウントを加算
		g_nCntWaveTimeCount++;

		if (g_nCntWaveTimeCount >= 90)
		{// カウントが90より上なら

			// 新規ウェーブを読み込む
			LoadBlockText();

			// カウントを初期化
			g_nCntWaveTimeCount = 0;
		}
	}

	switch(g_gameState)
	{
	case GAMESTATE_NORMAL://通常状態
		break;

	case GAMESTATE_END:
  		g_nCounterGameState++;
		if (g_nCounterGameState >= 60)
		{
			//カウンターを初期化
			g_nCounterGameState = 0;

			//1秒経過
			g_gameState = GAMESTATE_NONE;		//何もしていない状態

			//画面設定
			SetFade(MODE_RESULT);

			//読み込み
			ResetRanking();

			//ランキングの設定
			SetRanking(GetScore());
		}
		break;
	}


#ifdef _DEBUG
	if (KeyboardTrigger(DIK_F4) == true)
	{
		// モード変更
		SetFade(MODE_EDIT);
	}
#endif

}
//===================
// ゲームの描画
//===================
void DrawGame(void)
{
	DrawBackground();

	DrawBlock();			// ブロックの描画

	//DrawTime();				// タイムの描画

	DrawBullet();			// 弾の描画処理

	DrawPlayer();			// プレイヤーの描画

	DrawExplosion();		// 爆発の描画

	DrawItem();				// アイテムの描画

	DrawBulletNum();		// 残弾数の描画

	if (g_bPause == true)
	{// ポーズ中
		DrawPause();		// ポーズの描画
	}

}
//=========================
// ポーズの有効無効設定
//=========================
void SetEnablePause(bool bPause)
{
	g_bPause = bPause;
}
//=========================
// ゲーム状態設定
//=========================
void SetGamestate(GAMESTATE state)
{
	g_gameState = state;
	g_nCounterGameState = 0;
}
//==========================
// 現在のゲーム状態を返す
//==========================
GAMESTATE GetGameState(void)
{
	return g_gameState;
}
