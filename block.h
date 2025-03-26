//=============================
//
// ブロックの処理 [block.h]
// Author:NISHIO ASUMA
//
//=============================
#ifndef _BLOCK_H_
#define _BLOCK_H_

//******************************
// インクルードファイル宣言
//******************************
#include "main.h"

//******************************
// マクロ定義
//******************************
#define MAX_BLOCK (256)			 // ブロックの最大数
#define MAX_BLOCKSIZE_X (100.0f) // ブロックのサイズ
#define MAX_BLOCKSIZE_Y (100.0f) // ブロックのサイズ
#define HALF (0.5f)				 // 半分

//******************************
// ブロックの列挙型情報
//******************************
typedef enum
{
	BLOCKTYPE_GROUND = 0,			// 仮1
	BLOCKTYPE_WALL,					// 仮2
	BLOCKTYPE_NORMAL,				// ノーマルブロック
	BLOCKTYPE_VERTICAL,				// 縦消しブロック
	BLOCKTYPE_HORIZONTAL,			// 横消しブロック
	BLOCKTYPE_MAX
}BLOCKTYPE;

//**************************
// ブロック構造体の定義
//**************************
typedef struct
{
	D3DXVECTOR3 pos;			// 位置
	D3DXVECTOR3 move;			// 移動量
	bool bUse;					// 使用しているかどうか
	float fWidth;				// 横幅
	float fHeight;				// 高さ
	int nType;					// 種類
	int nLife;					// 体力
	bool bHitBlock;				// 当たったかどうか
	int nCntBlockstateCount;    // ブロックの状態管理カウンター
	D3DXVECTOR3 bOldpos;		// 前回の位置
}Block;

//**************************
// ブロックのテクスチャ設定
//**************************
static const char* BLOCK_TEXTURE[BLOCKTYPE_MAX] =
{
	"data\\TEXTURE\\Block000.png",			// 地面ブロック
	"data\\TEXTURE\\Block001.png",			// 壁ブロック
	"data\\TEXTURE\\Block_normal.png",		// ノーマルブロック
	"data\\TEXTURE\\Block_vertical.png",	// 縦消しブロック
	"data\\TEXTURE\\Block_horizontal.png",	// 横消しブロック

};

//*************************
// プロトタイプ宣言
//*************************
void InitBlock(void);			// ブロックの初期化
void UninitBlock(void);			// ブロックの終了
void UpdateBlock(void);			// ブロックの更新
void DrawBlock(void);			// ブロックの描画
void SetBlock(D3DXVECTOR3 pos, int nType, float fWidth, float fHeight);		// ブロックの設定処理			
bool CollisionBlockBotton(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, float fWidth, float fHeight); // 現在の位置,前回の位置,移動量,幅,高さ
bool CollisionBlockX(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, float fWidth, float fHeight);  // 現在の位置,前回の位置,移動量,幅,高さ
bool CollisionBlockOn(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, float fWidth, float fHeight); // 現在の位置,前回の位置,移動量,幅,高さ
Block* GetBlockInfo(void);		// ブロック情報
int GetBlock(void);				// ブロック数の取得
void LoadBlockText(void);		// 配置情報の読み込み処理
void HitBlock(int nCntBlock, int nDamage); // ヒット処理

#endif

