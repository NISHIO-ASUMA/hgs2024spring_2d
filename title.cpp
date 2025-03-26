//=============================================================================
//
// タイトル処理 [title.cpp]
// Author : TANEKAWA RIKU
//
//=============================================================================
#include "title.h"
#include "input.h"
#include "fade.h"
//#include "sound.h"

//マクロ定義
#define NUM_TITLE (2)//タイトル数

//タイトルの種類の構造体
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXCOLOR col;
	int nType;
	bool bUse;//使用状態
	float fHeight;//高さ
	float fWidth;//幅
	TITLESTATE state;//状態
}Title;


//グローバル変数
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL;//頂点バッファのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitleback = NULL;//タイトル背景

LPDIRECT3DTEXTURE9 g_pTextureTitle[NUM_TITLE] = {};//テクスチャのポインタ
LPDIRECT3DTEXTURE9 g_pTextureTitleback = NULL;//タイトル背景テクスチャのポインタ

Title g_aTitle[NUM_TITLE];//タイトル構造体の情報
TITLE g_Title;//タイトル列挙型の情報

int g_nCntTitleAnim;//タイトル状態
int g_nPatternAnim;//状態パターン
int g_aTimeCount;//タイムカウント

//==================
//タイトルの初期化
//==================
void InitTitle(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	VERTEX_2D* pVtx;//頂点情報のポインタ

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Block001.png",
		&g_pTextureTitleback);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\gametitle.png",
		&g_pTextureTitle[0]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\enter.png",
		&g_pTextureTitle[1]);

	// 初期化
	for (int nCntTitle = 0; nCntTitle < NUM_TITLE; nCntTitle++)
	{
		g_aTitle[nCntTitle].nType = 0;
		g_aTitle[nCntTitle].fHeight = 0.0f;
		g_aTitle[nCntTitle].fWidth = 0.0f;
		g_aTitle[nCntTitle].bUse = false;
		g_aTitle[nCntTitle].state = TITLE_NONE;//何もしていない状態
	}

	g_nCntTitleAnim = 0;//タイトル状態
	g_nPatternAnim = 0;//状態パターン
	g_aTimeCount = 0;//初期化

	//==================
	//タイトル背景
	//==================

	//頂点バッファの生成 背景
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY, FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitleback,
		NULL);

	//頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffTitleback->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//1つ目の頂点情報
	pVtx[1].pos = D3DXVECTOR3(1280.0f, 0.0f, 0.0f);//2つ目の頂点情報
	pVtx[2].pos = D3DXVECTOR3(0.0f, 720.0f, 0.0f);//3つ目の頂点情報
	pVtx[3].pos = D3DXVECTOR3(1280.0f, 720.0f, 0.0f);//4つ目の頂点情報

	//rhwの設定(1.0fで固定)
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//アンロック
	g_pVtxBuffTitleback->Unlock();

	//==================
	//タイトル描画
	//==================

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_TITLE,
		D3DUSAGE_WRITEONLY, FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitle,
		NULL);

	//頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTitle = 0; nCntTitle < NUM_TITLE; nCntTitle++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//1つ目の頂点情報
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//2つ目の頂点情報
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//3つ目の頂点情報
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//4つ目の頂点情報

		//rhwの設定(1.0fで固定)
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	SetTitle(TITLE_FIRST, D3DXVECTOR3(650.0f, 300.0f, 0.0f));//タイトル1
	SetTitle(TITLE_SECOND, D3DXVECTOR3(650.0f, 600.0f, 0.0f));//タイトル2

	//アンロック
	g_pVtxBuffTitle->Unlock();



	////サウンドの再生
	//PlaySound(SOUND_LABEL_BGM);

	////サウンドの停止
	//StopSound(SOUND_LABEL_RANK);

}
//==================
//タイトルの終了
//==================
void UninitTitle(void)
{
	////サウンドの停止
	//StopSound(SOUND_LABEL_BGM);

	//==================
	//タイトルの背景
	//==================

	//テクスチャの破棄
	if (g_pTextureTitleback != NULL)
	{
		g_pTextureTitleback->Release();
		g_pTextureTitleback = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffTitleback != NULL)
	{
		g_pVtxBuffTitleback->Release();
		g_pVtxBuffTitleback = NULL;
	}

	//==================
	//タイトル
	//==================

	//テクスチャの破棄
	for (int nCntTitle = 0; nCntTitle < NUM_TITLE; nCntTitle++)
	{

		if (g_pTextureTitle[nCntTitle] != NULL)
		{
			g_pTextureTitle[nCntTitle]->Release();
			g_pTextureTitle[nCntTitle] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffTitle != NULL)
	{
		g_pVtxBuffTitle->Release();
		g_pVtxBuffTitle = NULL;
	}

}
//==================
//タイトルの更新
//==================
void UpdateTitle(void)
{

	FADE g_fade = GetFade(); // 現在の状態

	VERTEX_2D* pVtx = 0;     // 頂点情報のポインタ

	static float alpha = 0.0f;			// アルファ値
	static bool bIncreasing = true;		// アルファ値が増加中かどうか

	// アルファ値を増減させるロジック
	if (bIncreasing)
	{
		alpha += 0.02f; // 増加
		if (alpha >= 1.0f)
		{
			alpha = 1.0f;
			bIncreasing = false; // 減少に切り替える
		}
	}
	else
	{
		alpha -= 0.02f; // 減少
		if (alpha <= 0.0f)
		{
			alpha = 0.0f;
			bIncreasing = true; // 増加に切り替える
		}
	}

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTitle = 0; nCntTitle < NUM_TITLE; nCntTitle++)
	{

		if (g_aTitle[nCntTitle].nType == TITLE_SECOND)
		{
			// 頂点カラーのアルファ値を設定
			D3DXCOLOR color(1.0f, 1.0f, 1.0f, alpha);

			pVtx[0].col = color;
			pVtx[1].col = color;
			pVtx[2].col = color;
			pVtx[3].col = color;
		}

		if (g_fade == FADE_NONE && (GetAnyKeyTrigger() || GetAnyJoypadTrigger()))
		{
			alpha = 0.0f;

			//PlaySound(SOUND_LABEL_ENTERSE);

			//Enterキーが押された
			for (int nCntTitle = 0; nCntTitle < NUM_TITLE; nCntTitle++)
			{
				if (g_aTitle[nCntTitle].bUse == true && g_aTitle[nCntTitle].nType == TITLE_SECOND)
				{
					g_aTitle[nCntTitle].state = TITLE_FLASH;
				}
			}

			//モード設定(チュートリアル画面に移動)
			SetFade(MODE_TUTORIAL);

		}

		TitleFlash(TITLE_SECOND);//プレスエンターの点滅

		// 次の頂点に移動
		pVtx += 4;
	}

	// アンロック
	g_pVtxBuffTitle->Unlock();
}
//==================
//タイトルの描画
//==================
void DrawTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//==================
	//タイトル背景の描画
	//==================

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTitleback, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureTitleback);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//==================
	//タイトルの描画
	//==================

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTitle, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntTitle = 0; nCntTitle < NUM_TITLE; nCntTitle++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureTitle[g_aTitle[nCntTitle].nType]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTitle * 4, 2);
	}

}
//======================
//タイトルの設定処理
//======================
void SetTitle(int nType, D3DXVECTOR3 pos)
{
	VERTEX_2D* pVtx;//頂点情報のポインタ

	//頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTitle = 0; nCntTitle < NUM_TITLE; nCntTitle++)
	{
		if (g_aTitle[nCntTitle].bUse == false)
		{
			g_aTitle[nCntTitle].nType = nType;
			g_aTitle[nCntTitle].pos = pos;
			g_aTitle[nCntTitle].bUse = true;


			//場合分け
			switch (nType)
			{
			case TITLE_FIRST:

				g_aTitle[nCntTitle].fHeight = 400.0f;//高さ
				g_aTitle[nCntTitle].fWidth = 800.0f;//横幅
				break;

			case TITLE_SECOND:

				g_aTitle[nCntTitle].fHeight = 70.0f;//高さ
				g_aTitle[nCntTitle].fWidth = 670.0f; //横幅
				break;
			}

			//頂点座標1の設定
			pVtx[0].pos = D3DXVECTOR3(g_aTitle[nCntTitle].pos.x - g_aTitle[nCntTitle].fWidth * 0.5f, g_aTitle[nCntTitle].pos.y - g_aTitle[nCntTitle].fHeight * 0.5f, 0.0f);//1つ目の頂点情報
			pVtx[1].pos = D3DXVECTOR3(g_aTitle[nCntTitle].pos.x + g_aTitle[nCntTitle].fWidth * 0.5f, g_aTitle[nCntTitle].pos.y - g_aTitle[nCntTitle].fHeight * 0.5f, 0.0f);//2つ目の頂点情報
			pVtx[2].pos = D3DXVECTOR3(g_aTitle[nCntTitle].pos.x - g_aTitle[nCntTitle].fWidth * 0.5f, g_aTitle[nCntTitle].pos.y + g_aTitle[nCntTitle].fHeight * 0.5f, 0.0f);//3つ目の頂点情報
			pVtx[3].pos = D3DXVECTOR3(g_aTitle[nCntTitle].pos.x + g_aTitle[nCntTitle].fWidth * 0.5f, g_aTitle[nCntTitle].pos.y + g_aTitle[nCntTitle].fHeight * 0.5f, 0.0f);//4つ目の頂点情報

			break;
		}

		pVtx += 4;
	}

	//アンロック
	g_pVtxBuffTitle->Unlock();
}
//==============================
//タイトルの点滅
//==============================
void TitleFlash(int nType)
{
	VERTEX_2D* pVtx = 0;//頂点情報のポインタ

	//頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTitle = 0; nCntTitle < NUM_TITLE; nCntTitle++)
	{
		if (g_aTitle[nCntTitle].bUse == true && g_aTitle[nCntTitle].state == TITLE_FLASH)
		{
			g_nCntTitleAnim++;//カウンターを加算

			if (g_nCntTitleAnim == 2)//7の時
			{
				//頂点カラーの設定
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.2f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.2f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.2f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.2f);

			}
			else if (g_nCntTitleAnim == 7)//7の時
			{
				//頂点カラーの設定
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				g_nCntTitleAnim = 0;//初期値に戻す
			}
		}
		pVtx += 4;
	}

	//アンロック
	g_pVtxBuffTitle->Unlock();

}