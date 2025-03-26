//===============================
//
// 背景処理 [ background.cpp ]
// Author:Asuma Nishio
//
//===============================

//******************************************
// インクルードファイル宣言
//******************************************
#include "background.h"
// #include "player.h"
// #include "input.h"

//******************************************
// グローバル変数宣言
//******************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBackground = NULL;	//頂点バッファのポインタ
LPDIRECT3DTEXTURE9 g_apTextureBackground = NULL;		//テクスチャポインタ
//=======================
// 背景の初期化処理
//=======================
void InitBackground(void)
{
	// デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\fu.jpg",
		&g_apTextureBackground);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBackground,
		NULL);

	// 頂点情報のポインタ
	VERTEX_2D* pVtx;		

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffBackground->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 1つ目の頂点情報
	pVtx[1].pos = D3DXVECTOR3(1280.0f, 0.0f, 0.0f);			// 2つ目の頂点情報
	pVtx[2].pos = D3DXVECTOR3(0.0f, 720.0f, 0.0f);			// 3つ目の頂点情報
	pVtx[3].pos = D3DXVECTOR3(1280.0f, 720.0f, 0.0f);		// 4つ目の頂点情報

	// rhwの設定(1.0fで固定)
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//アンロック
	g_pVtxBuffBackground->Unlock();
}
//======================
// 背景の終了処理
//======================
void UninitBackground(void)
{
	// テクスチャの破棄
	if (g_apTextureBackground != NULL)
	{
		g_apTextureBackground->Release();
		g_apTextureBackground = NULL;
	}

	//  頂点バッファの破棄
	if (g_pVtxBuffBackground != NULL)
	{
		g_pVtxBuffBackground->Release();
		g_pVtxBuffBackground = NULL;
	}
}
//=====================
// 背景の更新処理
//=====================
void UpdateBackground(void)
{
	// なし
}
//=====================
// 背景の描画処理
//=====================
void DrawBackground(void)
{
	// デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBackground, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_apTextureBackground);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}
