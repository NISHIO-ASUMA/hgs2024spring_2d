//=============================================================================
//
// 残弾数表示処理 [bulletnum.cpp]
// Author : TANEKAWA RIKU
//
//=============================================================================
#include "bulletnum.h"
#include "player.h"
#include "bullet.h"

typedef struct
{
	bool bUse;

}BulletNum;

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureBulletNum = NULL;//テクスチャへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureBulletNum2 = NULL;//テクスチャへのポインタ

LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBulletNum = NULL;//頂点バッファへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBulletNum2 = NULL;//頂点バッファへのポインタ

D3DXVECTOR3 g_posBulletNum;	// 残弾数の位置
BulletNum g_aBulletNum[MAX_DIGIT];

int g_nBulletNum;//残弾数の値

//=============================
// 残弾数の初期化処理
//=============================
void InitBulletNum(void)
{

	int nCnt;
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\time001.png",
		&g_pTextureBulletNum);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\bulletnum.png",
		&g_pTextureBulletNum2);

	g_posBulletNum = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//位置を初期化する(始まりの位置)


	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_DIGIT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBulletNum,
		NULL);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBulletNum2,
		NULL);

	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBulletNum->Lock(0, 0, (void**)&pVtx, 0);

	for (nCnt = 0; nCnt < MAX_DIGIT; nCnt++)
	{
		g_aBulletNum[nCnt].bUse = true;

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(1110.0f + nCnt * 35.0f, 610.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(1110.0f + nCnt * 35.0f + 35.0f, 610.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(1110.0f + nCnt * 35.0f, 680.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(1110.0f + nCnt * 35.0f + 35.0f, 680.0f, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffBulletNum->Unlock();

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBulletNum2->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(900.0f, 630.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(1100.0f, 630.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(900.0f, 680.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(1100.0f, 680.0f, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffBulletNum2->Unlock();

	SetBulletNum(20);
}
//=============================
// 残弾数の終了処理
//=============================
void UninitBulletNum(void)
{
	//テクスチャの破棄
	if (g_pTextureBulletNum != NULL)
	{
		g_pTextureBulletNum->Release();
		g_pTextureBulletNum = NULL;
	}

	//テクスチャの破棄
	if (g_pTextureBulletNum2 != NULL)
	{
		g_pTextureBulletNum2->Release();
		g_pTextureBulletNum2 = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffBulletNum != NULL)
	{
		g_pVtxBuffBulletNum->Release();
		g_pVtxBuffBulletNum = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffBulletNum2 != NULL)
	{
		g_pVtxBuffBulletNum2->Release();
		g_pVtxBuffBulletNum2 = NULL;
	}

}
//=============================
// 残弾数の更新処理
//=============================
void UpdateBulletNum(void)
{






}
//=============================
// 残弾数の描画処理
//=============================
void DrawBulletNum(void)
{

	//必要桁数分の描画

	int nCnt;

	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBulletNum, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCnt = 0; nCnt < MAX_DIGIT; nCnt++)
	{
		if (g_aBulletNum[nCnt].bUse == true)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureBulletNum);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
		}
	}

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBulletNum2, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureBulletNum2);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

}
//=============================
// 残弾数の設定処理
//=============================
void SetBulletNum(int nBulletNum)
{
	VERTEX_2D* pVtx;

	int aPosTexU[MAX_DIGIT] = {};//各桁の数字を格納(桁数分の配列)

	int aData = TEN;//10
	int aData2 = ONE;//1

	int digitNum = 0;
	int nCnt;

	g_nBulletNum = nBulletNum;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBulletNum->Lock(0, 0, (void**)&pVtx, 0);

	//桁ごとに分割する
	for (nCnt = 0; nCnt < MAX_DIGIT; nCnt++)
	{
		if (nCnt == 0)
		{
			aPosTexU[0] = g_nBulletNum / aData;
		}
		else
		{
			aPosTexU[nCnt] = g_nBulletNum % aData / aData2;
			aData = aData / 10;
			aData2 = aData2 / 10;
		}

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f + (0.1f * aPosTexU[nCnt]), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + (0.1f * aPosTexU[nCnt]), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + (0.1f * aPosTexU[nCnt]), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f * aPosTexU[nCnt]), 1.0f);

		pVtx += 4;

	}

	//頂点バッファをアンロックする
	g_pVtxBuffBulletNum->Unlock();

}
//=============================
// 残弾数の加算処理
//=============================
void AddBulletNum(int nBulletNum)
{

	VERTEX_2D* pVtx;

	int aPosTexU[MAX_DIGIT] = {};

	int aData = TEN;//10
	int aData2 = ONE;//1

	int digitNum = 0;
	int nCnt;

	g_nBulletNum += nBulletNum;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBulletNum->Lock(0, 0, (void**)&pVtx, 0);

	if (g_nBulletNum <= 0)
	{
		g_nBulletNum = 0;
	}

	//桁ごとに分割する
	for (nCnt = 0; nCnt < MAX_DIGIT; nCnt++)
	{

		if (nCnt == 0)
		{
			aPosTexU[0] = g_nBulletNum / aData;
		}
		else
		{
			aPosTexU[nCnt] = g_nBulletNum % aData / aData2;
			aData = aData / 10;
			aData2 = aData2 / 10;
		}

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f + (0.1f * aPosTexU[nCnt]), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + (0.1f * aPosTexU[nCnt]), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + (0.1f * aPosTexU[nCnt]), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f * aPosTexU[nCnt]), 1.0f);

		pVtx += 4;

	}

	//頂点バッファをアンロックする
	g_pVtxBuffBulletNum->Unlock();
}
//=============================
// 残弾数の減算処理
//=============================
void DecBulletNum(int nBulletNum)
{

	VERTEX_2D* pVtx;

	int aPosTexU[MAX_DIGIT] = {};

	int aData = TEN;//10
	int aData2 = ONE;//1

	int digitNum = 0;
	int nCnt;

	g_nBulletNum -= nBulletNum;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBulletNum->Lock(0, 0, (void**)&pVtx, 0);

	if (g_nBulletNum <= 0)
	{
		g_nBulletNum = 0;
	}

	//桁ごとに分割する
	for (nCnt = 0; nCnt < MAX_DIGIT; nCnt++)
	{

		if (nCnt == 0)
		{

			aPosTexU[0] = g_nBulletNum / aData;

		}
		else
		{

			aPosTexU[nCnt] = g_nBulletNum % aData / aData2;
			aData = aData / 10;
			aData2 = aData2 / 10;

		}

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f + (0.1f * aPosTexU[nCnt]), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + (0.1f * aPosTexU[nCnt]), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + (0.1f * aPosTexU[nCnt]), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f * aPosTexU[nCnt]), 1.0f);

		pVtx += 4;

	}

	//頂点バッファをアンロックする
	g_pVtxBuffBulletNum->Unlock();
}
//=============================
//桁数カウント処理
//=============================
int DigitNum(int nBulletNum)
{
	int ndigit = 0, nData;//桁数カウント

	nData = nBulletNum;

	for (int nCnt2 = 0; nCnt2 < MAX_DIGIT; nCnt2++)
	{
		if (nData == 0)
		{
			for (int nCnt3 = 0; nCnt3 < ndigit; nCnt3++)
			{
				g_aBulletNum[nCnt3].bUse = true;
			}
			break;
		}
		else
		{
			nData /= 10;
			ndigit++;//桁数をインクリメント
		}
	}

	return ndigit;
}
//=============================
// 残弾数の取得
//=============================
int GetBulletNum(void)
{
	return g_nBulletNum;
}
