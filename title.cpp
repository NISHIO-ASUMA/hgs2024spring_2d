//===============================
//
// タイトル処理 [ title.cpp ]
// Author:NISHIO ASUMA
//
//===============================

//******************************************
// インクルードファイル宣言
//******************************************
#include "title.h"
#include "input.h"
#include "fade.h"

//#include "sound.h"

//******************************************
// テクスチャの種類の列挙型
//******************************************
typedef enum
{
	TITLE_FIRST = 0,	// チュートリアル開始
	TITLE_SECOND,		// ゲーム開始
	TITLE_MAX
}TITLE;

//******************************************
// タイトルの状態の列挙型
//******************************************
typedef enum
{
	TITLE_NONE = 0,		// 通常状態
	TITLE_FLASH,		// 点滅状態
	STATE_MAX
}TITLESTATE;

//******************************************
// タイトルテクスチャの列挙型宣言
//******************************************
const char* TITLE_TEX[TITLE_MAX] =
{
	"data\\TEXTURE\\title_tuto.png",
	"data\\TEXTURE\\title_game.png",
};

//******************************************
// タイトル構造体を宣言
//******************************************
typedef struct
{
	D3DXVECTOR3 pos;	// 座標
	D3DXVECTOR3 col;	// カラー
	int nType;			// 種類
	bool bUse;			// 使用状態
	float fHeight;		// 高さ
	float fWidth;		// 幅
	TITLESTATE state;	// 状態
}Title;

//******************************************
// グローバル変数宣言
//******************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL;			// 頂点バッファのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitleback = NULL;		// タイトル背景
														   
LPDIRECT3DTEXTURE9 g_pTextureTitle[TITLE_MAX] = {};		// テクスチャのポインタ
LPDIRECT3DTEXTURE9 g_pTextureTitleback = NULL;			// タイトル背景テクスチャのポインタ

Title g_aTitle[TITLE_MAX];			// タイトル構造体の情報

int g_nCntTitleAnim;				// タイトル状態
int g_nPatternAnim;					// 状態パターン
int g_aTimeCount;					// タイムカウント
int g_aSelect;						// 選択用の変数

//===========================
// タイトルの初期化処理
//===========================
void InitTitle(void)
{
	// デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		

	// 頂点情報のポインタ
	VERTEX_2D* pVtx;				

	// テクスチャの読み込み (タイトル背景)
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Actiontitle.png",
		&g_pTextureTitleback);

	// 選択肢テクスチャの読み込み
	for (int nCnt = 0; nCnt < TITLE_MAX; nCnt++)
	{
		D3DXCreateTextureFromFile(pDevice,
			TITLE_TEX[nCnt],
			&g_pTextureTitle[nCnt]);
	}

	// グローバル変数の初期化
	g_nCntTitleAnim = 0;		// タイトル状態
	g_nPatternAnim = 0;			// 状態パターン
	g_aTimeCount = 0;			// 初期化
	g_aSelect = TITLE_FIRST;   // 初期の選択状態

//==================
// タイトル背景
//==================
	// 頂点バッファの生成 背景
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY, FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitleback,
		NULL);

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffTitleback->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 1つ目の頂点情報
	pVtx[1].pos = D3DXVECTOR3(1280.0f,0.0f, 0.0f);		// 2つ目の頂点情報
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
	g_pVtxBuffTitleback->Unlock();

//=========================
// セレクト可能なタイトル
//=========================

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * TITLE_MAX,
		D3DUSAGE_WRITEONLY, FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitle,
		NULL);

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTitle = 0; nCntTitle < TITLE_MAX; nCntTitle++)
	{
		// 構造体変数の初期化
		g_aTitle[nCntTitle].nType = 0;					// 種類
		g_aTitle[nCntTitle].fHeight = 0.0f;				// 高さ
		g_aTitle[nCntTitle].fWidth = 0.0f;				// 幅
		g_aTitle[nCntTitle].bUse = false;				// 未使用判定
		g_aTitle[nCntTitle].state = TITLE_NONE;			// 何もしていない状態

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 1つ目の頂点情報
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 2つ目の頂点情報
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 3つ目の頂点情報
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 4つ目の頂点情報

		// rhwの設定(1.0fで固定)
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// ポインタを更新
		pVtx += 4;
	}

	//SetTitle(TITLE_FIRST, D3DXVECTOR3(650.0f, 400.0f, 0.0f));	// タイトル1
	//SetTitle(TITLE_SECOND, D3DXVECTOR3(650.0f, 550.0f, 0.0f));	// タイトル2

	//アンロック
	g_pVtxBuffTitle->Unlock();

	// PlaySound(SOUND_LABEL_TITLEBGM);

}
//=======================
// タイトルの終了処理
//=======================
void UninitTitle(void)
{
	// StopSound();
//==================
// タイトルの背景
//==================
 
	// テクスチャの破棄
	if (g_pTextureTitleback != NULL)
	{				   
		g_pTextureTitleback->Release();
		g_pTextureTitleback = NULL;
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffTitleback != NULL)
	{
		g_pVtxBuffTitleback->Release();
		g_pVtxBuffTitleback = NULL;
	}

//==================
// タイトルの終了
//==================
	// StopSound();
	 
	// テクスチャの破棄
	for (int nCntTitle = 0; nCntTitle < TITLE_MAX; nCntTitle++)
	{

		if (g_pTextureTitle[nCntTitle] != NULL)
		{
			g_pTextureTitle[nCntTitle]->Release();
			g_pTextureTitle[nCntTitle] = NULL;
		}
	}
	// 頂点バッファの破棄
	if (g_pVtxBuffTitle != NULL)
	{
		g_pVtxBuffTitle->Release();
		g_pVtxBuffTitle = NULL;
	}
}
//==================
// タイトルの更新処理
//==================
void UpdateTitle(void)
{
	// 頂点情報のポインタ
	VERTEX_2D* pVtx;				

	if (KeyboardTrigger(DIK_RETURN))
	{
		// ゲーム画面へ
		SetFade(MODE_GAME);
	}

	g_aTimeCount++; // カウント加算

	if (g_aTimeCount >= 600)
	{// 時間経過
		// ランキング画面へ
		SetFade(MODE_RANKING);
	}


#ifdef _DEBUG

	if (KeyboardTrigger(DIK_F1) == true)
	{
		//F1キーが押された
		//モード設定(エディット画面に移動)
		SetFade(MODE_EDIT);
	}
#endif

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCount = 0; nCount < TITLE_MAX; nCount++)
	{
		// 頂点カラーを設定(白)
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		pVtx += 4;
	}

	pVtx -= 4 * TITLE_MAX;		// pVtxを戻す

	// メニューの選択
	if (KeyboardTrigger(DIK_UP) == true || JoypadTrigger(JOYKEY_UP) == true || KeyboardTrigger(DIK_W) == true)
	{
		// 上押した
		g_aSelect--;// デクリメント

		if (g_aSelect < TITLE_FIRST)
		{
			//一番下
			g_aSelect = TITLE_SECOND;
		}
	}

	else if (KeyboardTrigger(DIK_DOWN) == true || JoypadTrigger(JOYKEY_DOWN) == true || KeyboardTrigger(DIK_S) == true)
	{
		//下押したとき
		g_aSelect++;//インクリメント

		if (g_aSelect >= TITLE_MAX)
		{
			//一番上
			g_aSelect = TITLE_FIRST;
		}
	}

	pVtx += 4 * g_aSelect;							//選択肢


	pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);

	//アンロック
	g_pVtxBuffTitle->Unlock();


	//if (KeyboardTrigger(DIK_RETURN) == true || JoypadTrigger(JOYKEY_A) == true)
	//{
	//	//エンターキーが押された
	//	//各画面に移動
	//	switch (g_aSelect)
	//	{
	//	case TITLE_FIRST:
	//		//チュートリアルに移行
	//		SetFade(MODE_TUTORIAL);

	//		//Enterキーが押された
	//		for (int nCntTitle = 0; nCntTitle < NUM_TITLE; nCntTitle++)
	//		{
	//			if (g_aTitle[nCntTitle].bUse == true && g_aTitle[nCntTitle].nType == TITLE_FIRST)
	//			{
	//				g_aTitle[nCntTitle].state = TITLE_FLASH;
	//			}
	//		}
	//		PlaySound(SOUND_LABEL_TITLE);
	//		break;

	//	case TITLE_SECOND:

	//		//ゲームに移行
	//		SetFade(MODE_GAME);
	//		for (int nCntTitle = 0; nCntTitle < NUM_TITLE; nCntTitle++)
	//		{
	//			if (g_aTitle[nCntTitle].bUse == true && g_aTitle[nCntTitle].nType == TITLE_SECOND)
	//			{
	//				g_aTitle[nCntTitle].state = TITLE_FLASH;
	//			}
	//		}
	//		PlaySound(SOUND_LABEL_TITLE);

	//		break;

	//	}
	//}

	//TitleFlash(TITLE_FIRST);		//タイトルの点滅
	//TitleFlash(TITLE_SECOND);		//タイトルの点滅
}
//======================
// タイトルの描画処理
//======================
void DrawTitle(void)
{
	// デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

//=====================
// タイトル背景の描画
//=====================

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTitleback, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureTitleback);

	// タイトルポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	
//==================
// タイトルの描画
//==================

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTitle, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntTitle = 0; nCntTitle < TITLE_MAX; nCntTitle++)
	{
		// 種類を代入
		int nType = g_aTitle[nCntTitle].nType;

		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureTitle[nType]);

		// タイトルポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTitle * 4, 2);
	}
}
//======================
// タイトルの設定処理
//======================
void SetTitle(int nType, D3DXVECTOR3 pos,float fWidth,float fHeight)
{
	VERTEX_2D* pVtx;		// 頂点情報のポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTitle = 0; nCntTitle < TITLE_MAX; nCntTitle++)
	{
		if (!g_aTitle[nCntTitle].bUse)
		{// 未使用だったら
			g_aTitle[nCntTitle].nType = nType;		 // 種類
			g_aTitle[nCntTitle].pos = pos;			 // 座標
			g_aTitle[nCntTitle].fWidth = fWidth;	 // 横幅
			g_aTitle[nCntTitle].fHeight = fHeight;	 // 高さ
			g_aTitle[nCntTitle].bUse = true;		 // 使用判定

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aTitle[nCntTitle].pos.x - fWidth, g_aTitle[nCntTitle].pos.y - fHeight, 0.0f);		//1つ目の頂点情報
			pVtx[1].pos = D3DXVECTOR3(g_aTitle[nCntTitle].pos.x + fWidth, g_aTitle[nCntTitle].pos.y - fHeight, 0.0f);		//2つ目の頂点情報
			pVtx[2].pos = D3DXVECTOR3(g_aTitle[nCntTitle].pos.x - fWidth, g_aTitle[nCntTitle].pos.y + fHeight, 0.0f);		//3つ目の頂点情報
			pVtx[3].pos = D3DXVECTOR3(g_aTitle[nCntTitle].pos.x + fWidth, g_aTitle[nCntTitle].pos.y + fHeight, 0.0f);		//4つ目の頂点情報

			break;
		}

		pVtx += 4;
	}

	// アンロック
	g_pVtxBuffTitle->Unlock();
}
//==============================
// タイトルの点滅処理
//==============================
void TitleFlash(int nType)
{
	// 頂点情報のポインタ
	VERTEX_2D* pVtx = 0;		

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTitle = 0; nCntTitle < TITLE_MAX; nCntTitle++)
	{
		if (g_aTitle[nCntTitle].bUse && g_aTitle[nCntTitle].state == TITLE_FLASH)
		{// 使用状態 かつ 状態が点滅状態
			//　カウンターを加算
			g_nCntTitleAnim++;				

			if (g_nCntTitleAnim == 5)		
			{//　5の時
				// 頂点カラーの設定
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.2f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.2f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.2f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.2f);

			}

			else if(g_nCntTitleAnim == 10)	
			{// 10の時
				// 頂点カラーの設定
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				g_nCntTitleAnim = 0;		// 初期値に戻す
			}
		}
		// 頂点情報を進める
		pVtx += 4;
	}
	// アンロック
	g_pVtxBuffTitle->Unlock();
}