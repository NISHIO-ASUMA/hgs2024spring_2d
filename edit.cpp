//==================================
//
// エディター画面処理 [ edit.h ]
// Author:Asuma Nishio
//
//==================================

//***********************************************
// インクルードファイル宣言
//***********************************************
#include "edit.h"
#include "input.h"
#include <stdio.h>
#include "block.h"
#include "fade.h"

//***********************************************
// マクロ定義
//***********************************************
#define EDIT_MOVE (10.0f)		// 配置時の移動量

//***********************************************
// 書き出しパスの列挙型
//***********************************************
typedef enum
{
	FILLPASS_0 = 0,
	FILLPASS_1,
	FILLPASS_2,
	FILLPASS_3,
	FILLPASS_4,
	FILLPASS_5,
	FILLPASS_6,
	FILLPASS_MAX
}FILLPASS;

const char* TEXT_FILEPASS[FILLPASS_MAX] = 
{
	"data\\TEXT\\SetBlock000.txt",
	"data\\TEXT\\SetBlock001.txt",
	"data\\TEXT\\SetBlock002.txt",
	"data\\TEXT\\SetBlock003.txt",
	"data\\TEXT\\SetBlock004.txt",
	"data\\TEXT\\SetBlock005.txt",
	"data\\TEXT\\SetBlock006.txt",

};

//***********************************************
// エディター構造体の宣言
//***********************************************
typedef struct
{
	D3DXVECTOR3 pos;	// 位置
	int nType;			// 種類
	float fWidth;		// 横幅
	float fHeight;		// 高さ
	bool bUse;			// 使用状態
}EditInfo;

//***********************************************
// グローバル変数宣言
//***********************************************
int g_nCntEdit;								// 配置した数をカウント
EditInfo g_aEditInfo[MAX_BLOCK];			// 構造体情報
LPDIRECT3DTEXTURE9 g_apTextureEdit[BLOCKTYPE_MAX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEdit = NULL;			// 頂点バッファのポインタ
int nFillPass = 0;

void SaveEdit(void);			//ファイル書き出し
void ReloadEdit();				// 再読み込み
//========================
// エディター初期化処理
//========================
void InitEdit(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ

	// 構造体変数の初期化
	for (int nCnt = 0; nCnt < MAX_BLOCK; nCnt++)
	{
		g_aEditInfo[nCnt].nType = 0;							// 種類
		g_aEditInfo[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
		g_aEditInfo[nCnt].fHeight = 20.0f;						// 高さ
		g_aEditInfo[nCnt].fWidth = 20.0f;						// 横幅
		g_aEditInfo[nCnt].bUse = false;							// 使用判定
	}

	// 最初の番号だけ使用判定
	g_aEditInfo[0].bUse = true;
	g_nCntEdit = 0;
	nFillPass = FILLPASS_0; // 初期パス

	// 頂点情報のポインタ
	VERTEX_2D* pVtx;			

	for (int nCnt = 0; nCnt < BLOCKTYPE_MAX; nCnt++)
	{
		//敵のテクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			BLOCK_TEXTURE[nCnt],
			&g_apTextureEdit[nCnt]);
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BLOCK,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEdit,
		NULL);

	//頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffEdit->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEdit = 0; nCntEdit < MAX_BLOCK; nCntEdit++)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

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

		pVtx += 4;
	}

	// アンロック
	g_pVtxBuffEdit->Unlock();
}
//========================
// エディター終了処理
//========================
void UninitEdit(void)
{
	// テクスチャの破棄
	for (int nCntEdit = 0; nCntEdit < BLOCKTYPE_MAX; nCntEdit++)
	{
		if (g_apTextureEdit[nCntEdit] != NULL)
		{
			g_apTextureEdit[nCntEdit]->Release();
			g_apTextureEdit[nCntEdit] = NULL;
		}
	}
	// 頂点バッファの破棄
	if (g_pVtxBuffEdit != NULL)
	{
		g_pVtxBuffEdit->Release();
		g_pVtxBuffEdit = NULL;
	}
}
//========================
//　エディター更新処理
//========================
void UpdateEdit(void)
{
	// 移動処理
	if (GetKeyboardPress(DIK_A) == true)
	{
		// A単体の処理
		g_aEditInfo[g_nCntEdit].pos.x -= EDIT_MOVE;
	}
	else if (GetKeyboardPress(DIK_D) == true)
	{
		// D単体の処理
		g_aEditInfo[g_nCntEdit].pos.x += EDIT_MOVE;
	}
	else if (GetKeyboardPress(DIK_W) == true)
	{
		// W単体の処理
		g_aEditInfo[g_nCntEdit].pos.y -= EDIT_MOVE;
	}
	else if (GetKeyboardPress(DIK_S) == true)
	{
		// S単体の処理
		g_aEditInfo[g_nCntEdit].pos.y += EDIT_MOVE;
	}

	if (KeyboardTrigger(DIK_UP) == true)
	{// 種類切り替え
		if (g_aEditInfo[g_nCntEdit].nType < BLOCKTYPE_MAX - 1)
		{
			// 最大数より小さいとき
			g_aEditInfo[g_nCntEdit].nType++;
		}

	}
	else if (KeyboardTrigger(DIK_DOWN) == true)
	{// 種類切り替え
		if (g_aEditInfo[g_nCntEdit].nType > BLOCKTYPE_BULLETBLOCK)
		{
			// 0より大きいとき
			g_aEditInfo[g_nCntEdit].nType--;
		}
	}

	// 配置処理
	if (KeyboardTrigger(DIK_RETURN) == true)
	{
		// 次の敵の位置を配置した敵と同じにする
		g_aEditInfo[g_nCntEdit + 1].pos = g_aEditInfo[g_nCntEdit].pos;

		// 種類を同じにする
		g_aEditInfo[g_nCntEdit + 1].nType = g_aEditInfo[g_nCntEdit].nType;

		// 横幅,高さを同じにする
		g_aEditInfo[g_nCntEdit + 1].fHeight = g_aEditInfo[g_nCntEdit].fHeight;

		g_aEditInfo[g_nCntEdit + 1].fWidth = g_aEditInfo[g_nCntEdit].fWidth;

		// 使用判定にする
		g_aEditInfo[g_nCntEdit + 1].bUse = true;	

		// インクリメント
		g_nCntEdit++;
	}

	if (g_nCntEdit > 0)
	{// 0以上なら
		if (KeyboardTrigger(DIK_BACKSPACE))
		{// BACKSPACEキーが押された

			// 未使用判定にする
			g_aEditInfo[g_nCntEdit].bUse = false;

			// デクリメント
			g_nCntEdit--;
		}
	}

	if (KeyboardTrigger(DIK_Q))
	{
		// 横幅増加
		g_aEditInfo[g_nCntEdit].fWidth += 5.0f;
	}
	else if (KeyboardTrigger(DIK_E))
	{
		// 横幅減少
		g_aEditInfo[g_nCntEdit].fWidth -= 5.0f;

		if (g_aEditInfo[g_nCntEdit].fWidth <= 5.0f)
		{// 5以下なら
			g_aEditInfo[g_nCntEdit].fWidth = 5.0f;
		}
	}

	if (KeyboardTrigger(DIK_Z))
	{
		// 高さ増加
		g_aEditInfo[g_nCntEdit].fHeight += 5.0f;
	}
	else if (KeyboardTrigger(DIK_C))
	{
		// 高さ減少
		g_aEditInfo[g_nCntEdit].fHeight -= 5.0f;

		if (g_aEditInfo[g_nCntEdit].fHeight <= 5.0f)
		{// 5以下なら
			g_aEditInfo[g_nCntEdit].fHeight = 5.0f;
		}
	}

	if (KeyboardTrigger(DIK_F4))
	{
		// ファイルパスを切り替え
		nFillPass = (nFillPass + 1) % FILLPASS_MAX;

	}

	// 書き出し処理
	if (KeyboardTrigger(DIK_F7) == true)
	{
		//保存する
		SaveEdit();
	}

	if (KeyboardTrigger(DIK_F9))
	{
		ReloadEdit();
	}

	if (KeyboardTrigger(DIK_F3))
	{
		// 画面遷移
		SetFade(MODE_GAME);
	}

	// 頂点情報のポインタ
	VERTEX_2D* pVtx;	

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffEdit->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEdit = 0; nCntEdit < MAX_BLOCK; nCntEdit++)
	{
		if (g_aEditInfo[nCntEdit].bUse)
		{
			// 頂点情報の設定
			pVtx[0].pos = D3DXVECTOR3(g_aEditInfo[nCntEdit].pos.x - g_aEditInfo[nCntEdit].fWidth, g_aEditInfo[nCntEdit].pos.y - g_aEditInfo[nCntEdit].fHeight, 0.0f); // 1つ目の頂点情報
			pVtx[1].pos = D3DXVECTOR3(g_aEditInfo[nCntEdit].pos.x + g_aEditInfo[nCntEdit].fWidth, g_aEditInfo[nCntEdit].pos.y - g_aEditInfo[nCntEdit].fHeight, 0.0f); // 2つ目の頂点情報
			pVtx[2].pos = D3DXVECTOR3(g_aEditInfo[nCntEdit].pos.x - g_aEditInfo[nCntEdit].fWidth, g_aEditInfo[nCntEdit].pos.y + g_aEditInfo[nCntEdit].fHeight, 0.0f); // 3つ目の頂点情報
			pVtx[3].pos = D3DXVECTOR3(g_aEditInfo[nCntEdit].pos.x + g_aEditInfo[nCntEdit].fWidth, g_aEditInfo[nCntEdit].pos.y + g_aEditInfo[nCntEdit].fHeight, 0.0f); // 4つ目の頂点情報
		}

		pVtx += 4;
	}
	// アンロック
	g_pVtxBuffEdit->Unlock();

}
//========================
// エディター描画処理
//========================
void DrawEdit(void)
{
	// デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffEdit, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntEdit = 0; nCntEdit < MAX_BLOCK; nCntEdit++)
	{
		if (g_aEditInfo[nCntEdit].bUse)
		{// 使用されてる
			// 種類を保存する
			int nType = g_aEditInfo[nCntEdit].nType;

			// テクスチャの設定
			pDevice->SetTexture(0, g_apTextureEdit[nType]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEdit * 4, 2);

		}
	}
}
//========================
// 書き出し処理
//========================
void SaveEdit(void)
{
	FILE* pFile;	// ファイルポインタを宣言

	//ファイルを開く
	pFile = fopen(TEXT_FILEPASS[nFillPass],"w");

	if (pFile != NULL)
	{// 書き出し開始
		int nCnt = 0; // カウント用変数

		// タイトル関係
		fprintf(pFile, "//**************************************************\n");
		fprintf(pFile, "//                  ブロック配置情報              \n");
		fprintf(pFile, "//**************************************************\n\n");

		// 配置数,種類数
		fprintf(pFile, "NUM_BLOCK =   %d   # 総配置数\n\n", g_nCntEdit);

		while (nCnt < g_nCntEdit)
		{// nCntが下の分だけ書き出す

			// ブロックごとの配置情報
			fprintf(pFile, "//-------------------------------\n");
			fprintf(pFile, "//   [ %d ] 番目のブロック\n", nCnt);
			fprintf(pFile, "//-------------------------------\n");
			fprintf(pFile, "SETBLOCK\n");
			fprintf(pFile, "POS  = %.2f %.2f %.2f # 座標\n", g_aEditInfo[nCnt].pos.x, g_aEditInfo[nCnt].pos.y, g_aEditInfo[nCnt].pos.z);
			fprintf(pFile, "WIDTH = %.2f  # 横幅\n", g_aEditInfo[nCnt].fWidth);
			fprintf(pFile, "HEIGHT = %.2f # 高さ\n", g_aEditInfo[nCnt].fHeight);
			fprintf(pFile, "TYPE = %d     # 種類番号\n", g_aEditInfo[nCnt].nType);
			fprintf(pFile, "END_SETBLOCK\n\n");

			// 加算
			nCnt++;
		}
	}
	else
	{
		// ファイルが開けなかった時
		// メッセージBOXの表示
		MessageBox(NULL, "開けません", "エラー", MB_OK);

		return;
	}

	//ファイルを閉じる
	fclose(pFile);
}
//========================
// 再読み込み処理
//========================
void ReloadEdit()
{
	// ファイルポインタを宣言
	FILE* pFile;

	// 任意のファイルを開く
	pFile = fopen(TEXT_FILEPASS[nFillPass], "r");

	if (pFile != NULL)
	{// ファイルが開けたら

		// 構造体を初期化
		for (int nCnt = 0; nCnt < MAX_BLOCK; nCnt++)
		{
			g_aEditInfo[nCnt].nType = 0;							// 種類
			g_aEditInfo[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
			g_aEditInfo[nCnt].fHeight = 20.0f;						// 高さ
			g_aEditInfo[nCnt].fWidth = 20.0f;						// 横幅
			g_aEditInfo[nCnt].bUse = false;							// 使用判定
		}

		// ローカル変数---------------------------------
		int nData = 0; // ファイル読み込みの返り値
		int nCnt = 0; // カウント用変数
		char aString[256] = {}; // 文字列を読み込む
		char aGomi[5] = {}; // =を格納する文字列
		//-----------------------------------------------

		while (1)
		{
			// nDataに代入する
			nData = fscanf(pFile, "%s", &aString[0]);

			if (strcmp(aString, "NUM_BLOCK") == 0)
			{// NUM_BLOCKを読み取ったら
				// 読み飛ばす
				fscanf(pFile, "%s", &aGomi[0]);

				// ブロックの配置された数を読み込む
				fscanf(pFile, "%d", &g_nCntEdit);
			}

			if (strcmp(aString, "SETBLOCK") == 0)
			{// START_BLOCKSETを読み取ったら

				while (1)
				{
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(aString, "POS") == 0)
					{// POSを読み取ったら
						// 読み飛ばす
						fscanf(pFile, "%s", &aGomi[0]);

						// 座標を代入
						fscanf(pFile, "%f", &g_aEditInfo[nCnt].pos.x);
						fscanf(pFile, "%f", &g_aEditInfo[nCnt].pos.y);
						fscanf(pFile, "%f", &g_aEditInfo[nCnt].pos.z);

					}

					if (strcmp(aString, "WIDTH") == 0)
					{// WIDTHを読み取ったら
						// 読み飛ばす
						fscanf(pFile, "%s", &aGomi[0]);

						// 横幅を代入
						fscanf(pFile, "%f", &g_aEditInfo[nCnt].fWidth);

					}

					if (strcmp(aString, "HEIGHT") == 0)
					{// HEIGHTを読み取ったら
						// 読み飛ばす
						fscanf(pFile, "%s", &aGomi[0]);

						// 横幅を代入
						fscanf(pFile, "%f", &g_aEditInfo[nCnt].fHeight);

					}

					if (strcmp(aString, "TYPE") == 0)
					{// TYPEを読み取ったら
						// 読み飛ばす
						fscanf(pFile, "%s", &aGomi[0]);

						// 現在のブロックの種類番号を代入
						fscanf(pFile, "%d", &g_aEditInfo[nCnt].nType);
					}

					if (strcmp(aString, "END_SETBLOCK") == 0)
					{// END_BLOCKSETを読み取ったら
						// 使用判定にする
						g_aEditInfo[nCnt].bUse = true;

						// インクリメントして次のブロック情報へ
						nCnt++;

						break;
					}
				}
			}

			if (nData == EOF)
			{// EOFを読み取ったら
				// 判定を初期状態に戻す
				g_aEditInfo[g_nCntEdit].bUse = true;

				break;
			}
		}
	}
	else
	{
		// メッセージBOXの表示
		MessageBox(NULL, "再読み込み失敗(ReloadTextFile)", "エラー", MB_OK);
		pFile = NULL;
		return ;
	}

	// ファイルを閉じる
	fclose(pFile);


}

int GeteditBlock()
{
	return g_nCntEdit;
}
int ReturnPass()
{
	return nFillPass;
}