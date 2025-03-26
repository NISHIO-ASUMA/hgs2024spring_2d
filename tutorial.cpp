//========================================
//
// チュートリアル処理 [ tutorial.cpp ]
// Author:Asuma Nishio
//
//========================================

//******************************************
// インクルードファイル宣言
//******************************************
#include "tutorial.h"
#include "game.h"
#include "fade.h"
#include "sound.h"
#include "input.h"

//******************************************
// マクロ定義
//******************************************
#define SIZE_WIDTH  (50.0f)	// 横幅
#define SIZE_HEIGHT (50.0f) // 高さ

//******************************************
// チュートリアル構造体宣言
//******************************************
typedef struct
{
	D3DXVECTOR3 pos;		// 座標
	D3DXVECTOR3 move;		// 移動量
	int nType;				// 種類
	bool bUse;				// 使用状態
	float fHeight;			// 高さ
	float fWidth;			// 幅
	TUTORIALSTATE state;	// 状態
}Tutorial;

//******************************************
// グローバル変数宣言
//******************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTutorial = NULL;			// 頂点バッファのポインタ
LPDIRECT3DTEXTURE9 g_pTextureTutorial[TUTORIAL_MAX] = {};	// テクスチャのポインタ

LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTutorialBG = NULL;		// 頂点バッファのポインタ(背景)
LPDIRECT3DTEXTURE9 g_pTextureTutorialBG = NULL;				// テクスチャのポインタ(背景)

Tutorial g_aTutorial[TUTORIAL_MAX];							// チュートリアル構造体の情報

int g_nCntTutoAnim;			// チュートリアル状態
int g_nPatternTutoAnim;		// 状態パターン
bool g_bNext;				// フラグ
int g_nCntMove;				// 移動量のカウント

//==============================
// チュートリアルの初期化処理
//==============================
void InitTutorial(void)
{
	// デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		

	// 頂点情報のポインタ
	VERTEX_2D* pVtx;		

	for (int nCnt = 0; nCnt < TUTORIAL_MAX; nCnt++)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			TUTORIAL_TEXTURE[nCnt],
			&g_pTextureTutorial[nCnt]);
	}

	// テクスチャの読み込み(背景)
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\tutorial2.png",
		&g_pTextureTutorialBG);

	for (int nCntTutorial = 0; nCntTutorial < TUTORIAL_MAX; nCntTutorial++)
	{
		g_aTutorial[nCntTutorial].nType = 0;							//	種類
		g_aTutorial[nCntTutorial].fHeight = 0.0f;						//	高さ
		g_aTutorial[nCntTutorial].fWidth = 0.0f;						//	幅
		g_aTutorial[nCntTutorial].bUse = false;							//	未使用判定
		g_aTutorial[nCntTutorial].state = TUTORIALSTATE_NONE;			//	何もしていない状態
		g_aTutorial[nCntTutorial].move = D3DXVECTOR3(-20.0f,0.0f,0.0f); //	移動量
	}

	g_nCntTutoAnim = 0;			// チュートリアル状態
	g_nPatternTutoAnim = 0;		// 状態パターン
	g_bNext = false;
	g_nCntMove = 0;

//===========================
// チュートリアルの背景設定
//===========================

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY, FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTutorialBG,
		NULL);

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffTutorialBG->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標1の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 1つ目の頂点情報
	pVtx[1].pos = D3DXVECTOR3(1280.0f, 0.0f, 0.0f);		// 2つ目の頂点情報
	pVtx[2].pos = D3DXVECTOR3(0.0f, 720.0f, 0.0f);		// 3つ目の頂点情報
	pVtx[3].pos = D3DXVECTOR3(1280.0f, 720.0f, 0.0f);	// 4つ目の頂点情報

	// rhwの設定(1.0fで固定)
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// アンロック
	g_pVtxBuffTutorialBG->Unlock();

//===============================
// チュートリアルの各種の初期化
//===============================

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * TUTORIAL_MAX,
		D3DUSAGE_WRITEONLY, FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTutorial,
		NULL);

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffTutorial->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < TUTORIAL_MAX; nCnt++)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 1つ目の頂点情報
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 2つ目の頂点情報
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 3つ目の頂点情報
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 4つ目の頂点情報

		// rhwの設定(1.0fで固定)
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点情報のポインタを進める
		pVtx += 4;
	}

	// アンロック
	g_pVtxBuffTutorial->Unlock();

	// 音楽再生
	PlaySound(SOUND_LABEL_TUTOBGM);
}
//===========================
// チュートリアルの終了処理
//===========================
void UninitTutorial(void)
{
	StopSound();

//============================
// チュートリアルの各種の終了
//============================

	for (int nCnt = 0; nCnt < TUTORIAL_MAX; nCnt++)
	{
		// テクスチャの破棄
		if (g_pTextureTutorial[nCnt] != NULL)
		{					   
			g_pTextureTutorial[nCnt]->Release();
			g_pTextureTutorial[nCnt] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffTutorial != NULL)
	{
		g_pVtxBuffTutorial->Release();
		g_pVtxBuffTutorial = NULL;
	}

//============================
// チュートリアルの背景の終了
//============================

	// テクスチャの破棄
	if (g_pTextureTutorialBG != NULL)
	{					  
		g_pTextureTutorialBG->Release();
		g_pTextureTutorialBG = NULL;
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffTutorialBG != NULL)
	{					  
		g_pVtxBuffTutorialBG->Release();
		g_pVtxBuffTutorialBG = NULL;
	}
}
//==========================
// チュートリアルの更新処理
//==========================
void UpdateTutorial(void)
{
	FADE g_fade = GetFade();

	if (KeyboardTrigger(DIK_RETURN) == true || GetAnyJoypadTrigger())
	{
		//モード設定(ゲーム画面に移動)
		SetFade(MODE_GAME);
	}
}
//===========================
// チュートリアルの描画処理
//===========================
void DrawTutorial(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// デバイスへのポインタ

//==============================
// チュートリアルの背景描画
//==============================

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTutorialBG, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureTutorialBG);

	// チュートリアルポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

//==============================
// チュートリアルの各種描画
//==============================

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTutorial, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCnt = 0; nCnt < TUTORIAL_MAX; nCnt++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureTutorial[g_aTutorial[nCnt].nType]);

		// チュートリアルポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
	}

	// DrawTutorial2();	//2個目のチュートリアルの描画
}
//==================================
// チュートリアルの設定(テクスチャ)
//==================================
void SetTutorial(int nType, D3DXVECTOR3 pos)
{
	VERTEX_2D* pVtx;//頂点情報のポインタ

	//頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffTutorial->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < TUTORIAL_MAX; nCnt++)
	{
		if (g_aTutorial[nCnt].bUse == false)
		{
			g_aTutorial[nCnt].nType = nType;
			g_aTutorial[nCnt].pos = pos;
			g_aTutorial[nCnt].bUse = true;

			//場合分け
			switch (g_aTutorial[nCnt].nType)
			{
			case TUTORIAL_ONE:		//1つ目

				g_aTutorial[nCnt].fHeight = 50.0f;	//高さ
				g_aTutorial[nCnt].fWidth = 400.0f;	//横幅

				break;

			case TUTORIAL_ENTER:	//2つ目

				g_aTutorial[nCnt].fHeight = 50.0f;	//高さ
				g_aTutorial[nCnt].fWidth = 400.0f;  //横幅
				break;

			case TUTORIAL_KEY_W:	//3つ目

				g_aTutorial[nCnt].fHeight = 50.0f;	//高さ
				g_aTutorial[nCnt].fWidth = 50.0f;   //横幅


				break;

			case TUTORIAL_KEY_A:	//4つ目

				g_aTutorial[nCnt].fHeight = 50.0f; //高さ
				g_aTutorial[nCnt].fWidth = 50.0f;  //横幅
				break;

			case TUTORIAL_KEY_S:	//5つ目

				g_aTutorial[nCnt].fHeight = 50.0f; //高さ
				g_aTutorial[nCnt].fWidth = 50.0f;  //横幅
				break;

			case TUTORIAL_KEY_D:	//6つ目

				g_aTutorial[nCnt].fHeight = 50.0f; //高さ
				g_aTutorial[nCnt].fWidth = 50.0f;  //横幅
				break;

			case TUTORIAL_KEY_SPACE://7つ目

				g_aTutorial[nCnt].fHeight = 50.0f; //高さ
				g_aTutorial[nCnt].fWidth = 150.0f; //横幅
				break;

			case TUTORIAL_BUTTON_B:	//8つ目

				g_aTutorial[nCnt].fHeight = 80.0f; //高さ
				g_aTutorial[nCnt].fWidth = 80.0f;  //横幅
				break;

			case TUTORIAL_BUTTON_UP: //9つ目

				g_aTutorial[nCnt].fHeight = 50.0f; //高さ
				g_aTutorial[nCnt].fWidth = 50.0f;  //横幅
				break;

			case TUTORIAL_BUTTON_DOWN: //10個目

				g_aTutorial[nCnt].fHeight = 50.0f; //高さ
				g_aTutorial[nCnt].fWidth = 50.0f;  //横幅
				break;

			case TUTORIAL_BUTTON_RIGHT: //11個目

				g_aTutorial[nCnt].fHeight = 50.0f; //高さ
				g_aTutorial[nCnt].fWidth = 50.0f;  //横幅
				break;

			case TUTORIAL_BUTTON_LEFT: //12個目

				g_aTutorial[nCnt].fHeight = 50.0f; //高さ
				g_aTutorial[nCnt].fWidth = 50.0f;  //横幅
				break;

			case TUTORIAL_KEY_P:	  //13個目

				g_aTutorial[nCnt].fHeight = 50.0f; //高さ
				g_aTutorial[nCnt].fWidth = 50.0f;  //横幅
				break;

			case TUTORIAL_BUTTON_ST://14個目

				g_aTutorial[nCnt].fHeight = 80.0f; //高さ
				g_aTutorial[nCnt].fWidth = 80.0f;  //横幅
				break;

			case TUTORIAL_KEY_E:	//15個目

				g_aTutorial[nCnt].fHeight = SIZE_HEIGHT;	//高さ
				g_aTutorial[nCnt].fWidth = SIZE_WIDTH;		//横幅
				break;

			case TUTORIAL_KEY_Q:	//16個目

				g_aTutorial[nCnt].fHeight = SIZE_HEIGHT;	//高さ
				g_aTutorial[nCnt].fWidth = SIZE_WIDTH;		//横幅
				break;

			case TUTORIAL_BUTTON_RB:	//17個目

				g_aTutorial[nCnt].fHeight = SIZE_HEIGHT;	//高さ
				g_aTutorial[nCnt].fWidth = 80.0f;			//横幅
				break;
				
			case TUTORIAL_BUTTON_LB:	//18個目

				g_aTutorial[nCnt].fHeight = SIZE_HEIGHT;	//高さ
				g_aTutorial[nCnt].fWidth = 80.0f;			//横幅
				break;

			case TUTORIAL_LSTICK:	//19個目

				g_aTutorial[nCnt].fHeight = 70.0f;			//高さ
				g_aTutorial[nCnt].fWidth = 70.0f;			//横幅
				break;

				break;
			}

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aTutorial[nCnt].pos.x - g_aTutorial[nCnt].fWidth * 0.5f, g_aTutorial[nCnt].pos.y - g_aTutorial[nCnt].fHeight * 0.5f, 0.0f);//1つ目の頂点情報
			pVtx[1].pos = D3DXVECTOR3(g_aTutorial[nCnt].pos.x + g_aTutorial[nCnt].fWidth * 0.5f, g_aTutorial[nCnt].pos.y - g_aTutorial[nCnt].fHeight * 0.5f, 0.0f);//2つ目の頂点情報
			pVtx[2].pos = D3DXVECTOR3(g_aTutorial[nCnt].pos.x - g_aTutorial[nCnt].fWidth * 0.5f, g_aTutorial[nCnt].pos.y + g_aTutorial[nCnt].fHeight * 0.5f, 0.0f);//3つ目の頂点情報
			pVtx[3].pos = D3DXVECTOR3(g_aTutorial[nCnt].pos.x + g_aTutorial[nCnt].fWidth * 0.5f, g_aTutorial[nCnt].pos.y + g_aTutorial[nCnt].fHeight * 0.5f, 0.0f);//4つ目の頂点情報

			break;
		}

		pVtx += 4;
	}

	// アンロック
	g_pVtxBuffTutorial->Unlock();
}
//==========================
// 点滅の処理
//==========================
void TutorialFlash(int nType)
{
	VERTEX_2D* pVtx = 0;		// 頂点情報のポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffTutorial->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < TUTORIAL_MAX; nCnt++)
	{
		if (g_aTutorial[nCnt].bUse == true && g_aTutorial[nCnt].state == TUTORIALSTATE_FLASH)
		{
			g_nCntTutoAnim++;			// カウンターを加算

			if (g_nCntTutoAnim == 5)	// 5の時
			{
				// 頂点カラーの設定
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.2f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.2f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.2f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.2f);

			}

			else if (g_nCntTutoAnim == 10)	// 10の時
			{
				// 頂点カラーの設定
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				g_nCntTutoAnim = 0;		// 初期化に戻す
			}
		}
		pVtx += 4;
	}
	// アンロック
	g_pVtxBuffTutorial->Unlock();
}
//==========================
// 次の画面の取得
//==========================
bool GetIsNext(void)
{
	return g_bNext;
}
//==========================
// 次の画面への移動量の取得
//==========================
int GetTutorialCountMove(void)
{
	return g_nCntMove;
}