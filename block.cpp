//===============================
//
// ブロックの処理 [ block.cpp ]
// Author:Asuma Nishio
//
//===============================

//******************************
// インクルードファイル宣言
//******************************
#include "block.h"
#include <stdio.h>
#include "bullet.h"
#include "explosion.h"
#include "player.h"
#include "score.h"
#include "bulletnum.h"

//**************************
// グローバル変数
//**************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBlock = NULL;         // 頂点バッファのポインタ
LPDIRECT3DTEXTURE9 g_pTextureBlock[BLOCKTYPE_MAX] = {};	// テクスチャのポインタ
Block g_aBlock[MAX_BLOCK];								// ブロックの情報
int g_nCntblock;										// ブロックの取得
int g_nWave;											// ウェーブカウント
bool g_bFinish = false;
//**************************
// プロトタイプ宣言
//**************************
void InitStruct();    // 構造体初期化情報

//=========================
// ブロックの初期化処理
//=========================
void InitBlock(void)
{
	// デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	// テクスチャ読み込み
	for (int nTex = 0; nTex < BLOCKTYPE_MAX; nTex++)
	{
		D3DXCreateTextureFromFile(pDevice,
			BLOCK_TEXTURE[nTex],
			&g_pTextureBlock[nTex]);
	}

	// 構造体初期化情報
	InitStruct();

	g_nCntblock = 0;
	g_nWave = 0;
	g_bFinish = false;

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BLOCK,
		D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBlock,
		NULL);

	// 頂点情報のポインタ
	VERTEX_2D* pVtx;	

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);	// 1つ目の頂点情報
		pVtx[1].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);	// 2つ目の頂点情報
		pVtx[2].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);	// 3つ目の頂点情報
		pVtx[3].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);	// 4つ目の頂点情報

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

		// 頂点データのポインタを4つ分進める
		pVtx += 4;	
	}

	// 頂点バッファをアンロック
	g_pVtxBuffBlock->Unlock();
}
//========================
// ブロックの終了処理
//========================
void UninitBlock(void)
{
	// テクスチャの破棄
	for (int nCnt = 0; nCnt < BLOCKTYPE_MAX;nCnt++)
	{
		if (g_pTextureBlock[nCnt] != NULL)
		{
			g_pTextureBlock[nCnt]->Release();
			g_pTextureBlock[nCnt] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffBlock != NULL)
	{
		g_pVtxBuffBlock->Release();
		g_pVtxBuffBlock = NULL;
	}
}
//=============================
// ブロックの更新処理
//=============================
void UpdateBlock(void)
{
	// 頂点情報のポインタ
	VERTEX_2D* pVtx;		

	//　頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_BLOCK; nCnt++)
	{
		if (g_aBlock[nCnt].nType == BLOCKTYPE_MOVEBLOCK)
		{// タイプが動くブロックなら
			// 縦の移動量を0にする
			g_aBlock[nCnt].move.y = 0.0f;

			// 左右に動く
			if (g_aBlock[nCnt].bRight == true)
			{
				// 右移動がtrue
				g_aBlock[nCnt].move.x = 5.0f;

				if (g_aBlock[nCnt].pos.x >= SCREEN_WIDTH - g_aBlock[nCnt].fWidth)
				{
					g_aBlock[nCnt].bRight = false;
				}
			}
			else
			{
				g_aBlock[nCnt].move.x = -5.0f;
				
				if (g_aBlock[nCnt].pos.x <= g_aBlock[nCnt].fWidth)
				{// 横幅が端についた
					g_aBlock[nCnt].bRight = true;
				}

			}
		}

		// 移動量の減衰
		g_aBlock[nCnt].move.x += (0.0f - g_aBlock[nCnt].move.x) * 0.25f;
		g_aBlock[nCnt].move.y += (0.0f - g_aBlock[nCnt].move.y) * 0.25f;

		// 移動量の更新
		g_aBlock[nCnt].pos.x += g_aBlock[nCnt].move.x;
		g_aBlock[nCnt].pos.y += g_aBlock[nCnt].move.y;

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aBlock[nCnt].pos.x - g_aBlock[nCnt].fWidth, g_aBlock[nCnt].pos.y - g_aBlock[nCnt].fHeight,0.0f);	// 1つ目の頂点情報
		pVtx[1].pos = D3DXVECTOR3(g_aBlock[nCnt].pos.x + g_aBlock[nCnt].fWidth, g_aBlock[nCnt].pos.y - g_aBlock[nCnt].fHeight,0.0f);	// 2つ目の頂点情報
		pVtx[2].pos = D3DXVECTOR3(g_aBlock[nCnt].pos.x - g_aBlock[nCnt].fWidth, g_aBlock[nCnt].pos.y + g_aBlock[nCnt].fHeight,0.0f);	// 3つ目の頂点情報
		pVtx[3].pos = D3DXVECTOR3(g_aBlock[nCnt].pos.x + g_aBlock[nCnt].fWidth, g_aBlock[nCnt].pos.y + g_aBlock[nCnt].fHeight,0.0f);	// 4つ目の頂点情報

		// 頂点情報分ずらす
		pVtx += 4;
	}

	// アンロック
	g_pVtxBuffBlock->Unlock();
}
//===========================
// ブロックの描画処理
//===========================
void DrawBlock(void)
{
	// デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBlock, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == true)
		{// 使用状態
			// 種類を保存
			int nType = g_aBlock[nCntBlock].nType;

			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureBlock[nType]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBlock * 4, 2);
		}
	}
}
//============================
// ブロックの設定処理
//============================
void SetBlock(D3DXVECTOR3 pos,int nType,float fWidth,float fHeight) 
{
	// 頂点情報のポインタ
	VERTEX_2D* pVtx;

	// 頂点バッファをロックし,頂点情報へのポインタを取得
 	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (!g_aBlock[nCntBlock].bUse)
		{
			g_aBlock[nCntBlock].pos = pos;			// 座標
			g_aBlock[nCntBlock].nType = nType;		// 種類
			g_aBlock[nCntBlock].fWidth = fWidth;	// 横幅
			g_aBlock[nCntBlock].fHeight = fHeight;	// 高さ
			g_aBlock[nCntBlock].bUse = true;		// 使用状態にする

			if (g_aBlock[nCntBlock].nType == BLOCKTYPE_MOVEBLOCK)
			{
				g_aBlock[nCntBlock].nLife = 3;
			}
			else
			{
				g_aBlock[nCntBlock].nLife = 1;
			}

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(pos.x - fWidth, pos.y - fHeight,0.0f);	// 1つ目の頂点情報
			pVtx[1].pos = D3DXVECTOR3(pos.x + fWidth, pos.y - fHeight,0.0f);	// 2つ目の頂点情報
			pVtx[2].pos = D3DXVECTOR3(pos.x - fWidth, pos.y + fHeight,0.0f);	// 3つ目の頂点情報
			pVtx[3].pos = D3DXVECTOR3(pos.x + fWidth, pos.y + fHeight,0.0f);	// 4つ目の頂点情報

			break;
		}
		// 頂点情報分ずらす
		pVtx += 4;	
	}

	// 頂点バッファをアンロック
	g_pVtxBuffBlock->Unlock();

}
//===========================
// ブロックの上の当たり判定
//===========================
bool CollisionBlockOn(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, float fWidth, float fHeight)
{
	// 着地しているかどうか
	bool Check = false;

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (!g_aBlock[nCntBlock].bUse)
		{
			//ブロックが使用されてないとき
			continue;
		}

		//値を保存する
		const float fPlayerHalfWidth = fWidth * HALF;												// プレイヤーの横の半分のサイズ
		const float fBlockHalfWidth = g_aBlock[nCntBlock].fWidth * HALF;							// ブロックの横の半分のサイズ
		const float fBlockHalfHeight = g_aBlock[nCntBlock].fHeight * HALF;							// ブロックの縦の半分のサイズ

		const float fBlockUp = g_aBlock[nCntBlock].pos.y - fBlockHalfHeight;						// ブロックの上
		const float fBlockUnder = g_aBlock[nCntBlock].pos.y + fBlockHalfHeight + fHeight;			// ブロックの下
		const float fBlockLeft = g_aBlock[nCntBlock].pos.x - fBlockHalfWidth - fPlayerHalfWidth;	// ブロックの左
		const float fBlockRight = g_aBlock[nCntBlock].pos.x + fBlockHalfWidth + fPlayerHalfWidth;	// ブロックの右

		// 上下の当たり判定
		if (pPos->x > fBlockLeft &&
			pPos->x < fBlockRight)
		{
			// プレイヤーがブロックの上から下へめり込んでいる時
			if ((pPosOld->y <= fBlockUp &&
				pPos->y >= fBlockUp)
				|| (g_aBlock[nCntBlock].bOldpos.y >= pPos->y &&
				fBlockUp <= pPos->y))
			{
				Check = true;
				// pPlayer->bLanding = true;	// 着地判定(上から下に来る時だけ着地している)
				pPos->y = fBlockUp;				// ブロックの上の位置にプレイヤーを合わせる
				pMove->y = 0.0f;	            // 縦の移動量をなくす
			}
		}
	}

	// 判定を返す
	return Check;			
}
//===========================
// ブロックの下の当たり判定
//===========================
bool CollisionBlockBotton(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, float fWidth, float fHeight) 
{
	// 着地しているかどうか
	bool Check = false;     

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (!g_aBlock[nCntBlock].bUse)
		{
			//ブロックが使用されてないとき
			continue;
		}

		//値を保存する
		const float fPlayerHalfWidth = fWidth * HALF;												// プレイヤーの横の半分のサイズ
		const float fBlockHalfWidth = g_aBlock[nCntBlock].fWidth * HALF;							// ブロックの横の半分のサイズ
		const float fBlockHalfHeight = g_aBlock[nCntBlock].fHeight * HALF;							// ブロックの縦の半分のサイズ

		const float fBlockUp = g_aBlock[nCntBlock].pos.y - fBlockHalfHeight;						// ブロックの上
		const float fBlockUnder = g_aBlock[nCntBlock].pos.y + fBlockHalfHeight + fHeight;			// ブロックの下
		const float fBlockLeft = g_aBlock[nCntBlock].pos.x - fBlockHalfWidth - fPlayerHalfWidth;	// ブロックの左
		const float fBlockRight = g_aBlock[nCntBlock].pos.x + fBlockHalfWidth + fPlayerHalfWidth;	// ブロックの右

		// 上下の当たり判定
		if (pPos->x > fBlockLeft &&
			pPos->x < fBlockRight)
		{
			// プレイヤーがブロックの下から上へめり込んでいる
			if (pPosOld->y >= fBlockUnder &&
				pPos->y <= fBlockUnder)
			{
				pPos->y = fBlockUnder;				// ブロックの下の位置にプレイヤーを合わせる
				pMove->y = 0.0f;					// 縦の横の移動量をなくす
			}
		}

	}

	// 判定を返す
	return Check;			
}
//===========================
// ブロックの左右の当たり判定
//===========================
bool CollisionBlockX(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, float fWidth, float fHeight)
{
	// 判定
	bool bhit = false;

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (!g_aBlock[nCntBlock].bUse)
		{
			//ブロックが使用されてないとき
			continue;
		}

		//値を保存する
		const float fPlayerHalfWidth = fWidth * HALF;												// プレイヤーの横の半分のサイズ
		const float fBlockHalfWidth = g_aBlock[nCntBlock].fWidth * HALF;							// ブロックの横の半分のサイズ
		const float fBlockHalfHeight = g_aBlock[nCntBlock].fHeight * HALF;							// ブロックの縦の半分のサイズ

		const float fBlockUp = g_aBlock[nCntBlock].pos.y - fBlockHalfHeight;						// ブロックの上
		const float fBlockUnder = g_aBlock[nCntBlock].pos.y + fBlockHalfHeight + fHeight;			// ブロックの下
		const float fBlockLeft = g_aBlock[nCntBlock].pos.x - fBlockHalfWidth - fPlayerHalfWidth;	// ブロックの左
		const float fBlockRight = g_aBlock[nCntBlock].pos.x + fBlockHalfWidth + fPlayerHalfWidth;	// ブロックの右

		// 左右の当たり判定
		if (pPos->y > fBlockUp &&
			pPos->y < fBlockUnder)
		{
			// プレイヤーがブロックの縦の範囲内にいる
			if (pPosOld->x <= fBlockLeft &&
				pPos->x >= fBlockLeft)
			{
				// プレイヤーがブロックの左から右へめり込んでいる
				pPos->x = fBlockLeft;									// ブロックの左の位置にプレイヤーを合わせる
				pMove->x = 0.0f;										// 移動量をなくす
			}
			else if (pPosOld->x >= fBlockRight &&
				pPos->x <= fBlockRight)
			{
				// プレイヤーがブロックの右から左へめり込んでいる
				pPos->x = fBlockRight;									// ブロックの右の位置にプレイヤーを合わせる
				pMove->x = 0.0f;										// 横の移動量をなくす
			}

		}
	}

	// 判定を返す
	return bhit;
}
//====================
// ブロックを取得
//====================
Block* GetBlockInfo(void)
{
	return &g_aBlock[0];
}
//====================
// ブロックの数を取得
//====================
int GetBlock()
{
	return g_nCntblock;
}
//===================================
// 配置情報の読み込み処理
//===================================
void LoadBlockText()
{
	// ファイルポインタの宣言
	FILE* pFile;

	//ウェーブ関係
	switch (g_nWave)
	{

	case 0:
		//初期ウェブ
		//ファイル開く
		pFile = fopen("data\\TEXT\\SetBlock000.txt", "r");
		break;

	case 1:

		//ファイル開く
		pFile = fopen("data\\TEXT\\SetBlock001.txt", "r");
		break;

	case 2:
		//ファイルを開く
		pFile = fopen("data\\TEXT\\SetBlock002.txt", "r");

		break;

	case 3:
		//ファイルを開く
		pFile = fopen("data\\TEXT\\SetBlock003.txt", "r");

		break;

	case 4:
		//ファイルを開く
		pFile = fopen("data\\TEXT\\SetBlock004.txt", "r");

		break;

	default:
		pFile = NULL;
		g_bFinish = true;	//ウェーブの終了
		break;
	}

	if (pFile != NULL)
	{// ファイルが開けたら
		// 文字列
		char astr[256] = {};
		char aGomi[5] = {}; // =格納用
		int nData = 0;
		int nBlock = 0;
		int nType = 0;
		D3DXVECTOR3 pos(0.0f, 0.0f, 0.0f);

		float fWidth = 0.0f;
		float fHeight = 0.0f;

		while (1)
		{
			// nDataに代入する
			nData = fscanf(pFile, "%s", &astr[0]);

			if (strcmp(astr, "NUM_BLOCK") == 0)
			{// NUMT_BLOCKを読み取ったら
				// 読み飛ばす
				fscanf(pFile, "%s", &aGomi[0]);

				// ブロックの最大数を読み込む
				fscanf(pFile, "%d", &nBlock);
			}

			if (strcmp(astr, "SETBLOCK") == 0)
			{// SETBLOCKを読み取ったら
				while (g_nCntblock < nBlock)
				{
					// 文字列
					fscanf(pFile, "%s", astr);

					if (strcmp(astr, "POS") == 0)
					{// POSを読み取ったら
						// 読み飛ばす
						fscanf(pFile, "%s", aGomi);

						// 座標を代入
						fscanf(pFile, "%f", &pos.x);
						fscanf(pFile, "%f", &pos.y);
						fscanf(pFile, "%f", &pos.z);

					}

					if (strcmp(astr, "WIDTH") == 0)
					{// WIDTHを読み取ったら
						// 読み飛ばす
						fscanf(pFile, "%s", aGomi);

						// 横幅を代入
						fscanf(pFile, "%f", &fWidth);
					}

					if (strcmp(astr, "HEIGHT") == 0)
					{// HEIGHTを読み取ったら
						// 読み飛ばす
						fscanf(pFile, "%s", aGomi);

						// 高さを代入
						fscanf(pFile, "%f", &fHeight);
					}

					if (strcmp(astr, "TYPE") == 0)
					{// TYPEを読み取ったら
						// 読み飛ばす
						fscanf(pFile, "%s", aGomi);

						// 種類番号を代入
						fscanf(pFile, "%d", &nType);
					}

					if (strcmp(astr, "END_SETBLOCK") == 0)
					{// END_SETPOLYGONを読み取ったら
						// ブロックに情報を代入する
						SetBlock(pos, nType, fWidth, fHeight);

						// インクリメント
						g_nCntblock++;
					}
				}
			}
			if (nData == EOF)
			{// EOFを読み取ったら
				break;
			}
		}

		// ファイルを閉じる
		fclose(pFile);

		//ウェーブのカウントアップ
		g_nWave++;

	}
	else
	{
		// メッセージBOXの表示
		MessageBox(NULL, "開けません(SetBlock.txt)", "エラー", MB_OK);
		pFile = NULL;
		return;
	}


}
//======================
// ウェーブの値を返す
//======================
int GetWave(void)
{
	return g_nWave;
}
//=========================
// ウェーブの終了判定を返す
//=========================
bool GetFinish(void)
{
	return g_bFinish;
}
//====================
// 構造体初期化関数
//====================
void InitStruct()
{
	// ブロック情報の初期化
	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		g_aBlock[nCntBlock].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	 // 座標
		g_aBlock[nCntBlock].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	 // 移動量
		g_aBlock[nCntBlock].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	 // 向き
		g_aBlock[nCntBlock].bUse = false;							 // 使用してない状態にする
		g_aBlock[nCntBlock].fWidth = 0.0f;							 // 横幅
		g_aBlock[nCntBlock].fHeight = 0.0f;							 // 高さ
		g_aBlock[nCntBlock].nType = 0;								 // 種類
		g_aBlock[nCntBlock].nLife = 0;								 // 体力
		g_aBlock[nCntBlock].bHitBlock = false;						 // 当たったかどうか
		g_aBlock[nCntBlock].bLeft = false;							 // 左端に着いたかどうか
		g_aBlock[nCntBlock].bRight = false;							 // 右端に着いたかどうか
		g_aBlock[nCntBlock].nCntBlockstateCount = 0;				 // カウンターの初期化
		g_aBlock[nCntBlock].bOldpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // 座標
	}

}
//=======================================
// ブロックのヒット処理
//=======================================
void HitBlock(int nCntBlock, int nDamage)
{
	g_aBlock[nCntBlock].nLife -= nDamage;

	if (g_aBlock[nCntBlock].nLife <= 0)
	{
		// 爆発
		SetExplosion(g_aBlock[nCntBlock].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		if (g_aBlock[nCntBlock].nType == BLOCKTYPE_NORMAL || g_aBlock[nCntBlock].nType == BLOCKTYPE_MOVEBLOCK)
		{
			g_aBlock[nCntBlock].bUse = false;
		}

		// 弾ブロックを破壊した
		if (g_aBlock[nCntBlock].nType == BLOCKTYPE_BULLETBLOCK)
		{

			g_aBlock[nCntBlock].bUse = false;
		}

		if (g_aBlock[nCntBlock].nType == BLOCKTYPE_VERTICAL)
		{
			// 上方向
			SetBullet(g_aBlock[nCntBlock].pos,
				D3DXVECTOR3(0.0f, 10.0f, 0.0f),
				g_aBlock[nCntBlock].rot,
				30.0f,
				30.0f,
				80.0f, BULLETTYPE_BLOCK);

			// 下方向
			SetBullet(g_aBlock[nCntBlock].pos,
				D3DXVECTOR3(0.0f, -10.0f, 0.0f),
				g_aBlock[nCntBlock].rot,
				30.0f,
				30.0f,
				80.0f, BULLETTYPE_BLOCK);

			g_aBlock[nCntBlock].bUse = false;
		}
		if (g_aBlock[nCntBlock].nType == BLOCKTYPE_HORIZONTAL)
		{
			// 右方向
			SetBullet(g_aBlock[nCntBlock].pos,
				D3DXVECTOR3(10.0f, 0.0f, 0.0f),
				g_aBlock[nCntBlock].rot,
				30.0f,
				30.0f,
				80.0f, BULLETTYPE_BLOCK);

			// 左方向
			SetBullet(g_aBlock[nCntBlock].pos,
				D3DXVECTOR3(-10.0f, 0.0f, 0.0f),
				g_aBlock[nCntBlock].rot,
				30.0f,
				30.0f,
				80.0f, BULLETTYPE_BLOCK);

			g_aBlock[nCntBlock].bUse = false;

			switch (g_aBlock[nCntBlock].nType)
			{
			case  BLOCKTYPE_NORMAL:		// 通常
				// スコアを加算
				AddScore(1000);
				break;

			case BLOCKTYPE_VERTICAL:	// 縦に弾が出るブロック
				// スコアを加算
				AddScore(3000);
				break;


			case BLOCKTYPE_HORIZONTAL:	// 横に弾が出るブロック
				// スコアを加算
				AddScore(3000);
				break;

			case BLOCKTYPE_BULLETBLOCK:	// 残弾数増加のブロック

				// TODO : ここにプレイヤーの弾の残弾数を増やす関数呼ぶ
				AddBulletNum(1);
				break;

			default:
				break;
			}

		}

		g_nCntblock--; // デクリメント
	}
}
