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

//***********************************************
// マクロ定義
//***********************************************

//***********************************************
// グローバル変数宣言
//***********************************************
GAMESTATE g_gameState = GAMESTATE_NONE;		// ゲーム状態
int g_nCounterGameState = 0;				// 状態管理カウンター
bool g_bPause = false;						// ポーズ中かどうか

//=====================
// ゲームの初期化処理
//=====================
void InitGame(void)
{
	InitBlock();			// ブロックの初期化

	InitPause();			// ポーズの初期化

	InitTime();				// タイムの初期化

	InitBullet();			// 弾の初期化

	InitPlayer();			// プレイヤーの初期化

	InitExplosion();		//爆発の初期化

	LoadBlockText();		// 配置したブロック情報の読み込み

#if 0
	InitBackground();		//背景

	InitBullet();			//弾


	InitEnemy();			//敵


	InitScore();			//スコア

	InitItem();				//アイテム

	InitEffect();			//エフェクト

	InitParticle();			//パーティクル

	InitTimer();			//タイマー

	InitPlayerLifeBar();	//ライフバー

	InitBlock();					//ブロックの初期化

	PlaySound(SOUND_LABEL_GAME);
#endif

	g_gameState = GAMESTATE_NORMAL;//通常状態に設定
	g_nCounterGameState = 0;
	g_bPause = false;//ポーズ解除

}
//===================
// ゲームの終了
//===================
void UninitGame(void)
{
	SaveScore();			// スコアを保存

	UninitBlock();			// ブロックの終了

	UninitPause();			// ポーズの終了

	UninitTime();			// タイムの終了処理

	UninitPlayer();			// プレイヤーの終了処理	

	UninitBullet();			// 弾の終了処理

	UninitExplosion();		// 爆発終了処理

#if 0
	StopSound();


	UninitBackground();		//背景

	UninitBullet();			//弾


	UninitEnemy();			//敵の終了


	UninitScore();			//スコア終了

	UninitItem();			//アイテム

	UninitEffect();			//エフェクト

	UninitParticle();		//パーティクル

	UninitPause();			//ポーズ

	UninitTimer();			//タイマー

	UninitPlayerLifeBar();	//ライフバー

	UninitBlock();			//ブロック
#endif
}
//===================
// ゲームの更新
//===================
void UpdateGame(void)
{

#ifdef _DEBUG
	if (KeyboardTrigger(DIK_F2))
	{
		// エディター起動
		SetFade(MODE_EDIT);
	}

	if (KeyboardTrigger(DIK_RETURN))
	{// ENTERキー
		// リザルトに遷移
		SetFade(MODE_RESULT);
	}

#endif // _DEBUG

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

		UpdateTime();		// タイムの更新

		UpdateBullet();		// 弾の更新処理
		
		UpdatePlayer();		// プレイヤーの更新処理

		UpdateExplosion();	// 爆発の更新処理

	}

#if 1
	//// プレイヤーの取得
	//Player* pPlayer = GetPlayer();

	//// 敵の取得
	//int nNum = GetNumEnemy();

	if (GetTimeEnd() == true && g_gameState != GAMESTATE_NONE)
	{

		g_gameState = GAMESTATE_END;  //終了状態

		//PlaySound(SOUND_LABLE_EXIT);
	}

	switch(g_gameState)
	{
	case GAMESTATE_NORMAL://通常状態
		break;

	case GAMESTATE_END:
  		g_nCounterGameState++;
		if (g_nCounterGameState >= 30)
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

	//else 
	//{
	//	//ポーズ中で無ければ
	//	UpdateBackground();			//背景

	//	UpdatePlayer();				//プレイヤー

	//	UpdateBullet();				//弾

	//	UpdateEnemy();				//敵

	//	UpdateScore();				//スコア更新

	//	UpdateItem();				//アイテム

	//	UpdateEffect();				//エフェクト

	//	UpdateParticle();			//パーティクル

	//	UpdateTimer();				//タイマー

	//	UpdatePlayerLifeBar();	//ライフバー
	//}
#endif
}
//===================
// ゲームの描画
//===================
void DrawGame(void)
{
	if (g_bPause == true)
	{// ポーズ中
		DrawPause();		// ポーズの描画
	}

	DrawBlock();			// ブロックの描画

	DrawTime();				// タイムの描画

	DrawBullet();			// 弾の描画処理

	DrawPlayer();			// プレイヤーの描画

	DrawExplosion();		// 爆発の描画

#if 0
	//背景
	DrawBackground();

	//アイテム
	DrawItem();

	//弾
	DrawBullet();

	//プレイヤー
	DrawPlayer();

	//敵
	DrawEnemy();

	//スコア
	//DrawScore();

	//エフェクト
	DrawEffect();

	//パーティクル
	DrawParticle();

	//タイマー
	DrawTimer();

	//ライフバー
	DrawPlayerLifeBar();

#endif
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
