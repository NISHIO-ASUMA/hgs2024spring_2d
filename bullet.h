//---------------------------
//
//弾の処理
//Author:NISHIO ASUMA
//
//---------------------------

#ifndef _BULLET_H_
#define _BULLET_H_

#include "main.h"


//弾の種類の列挙型
typedef enum
{
	BULLETTYPE_PLAYER = 0,
	BULLETTYPE_BLOCK,
	BULLETTYPE_MAX
}BULLETTYPE;

static const char* BULLET_TEXTURE[BULLETTYPE_MAX] =
{
	"data\\TEXTURE\\bullet001.png",	// プレイヤー
	"data\\TEXTURE\\bullet.png",	// ブロック
};

//プロトタイプ宣言
void InitBullet(void);				//弾の初期化
void UninitBullet(void);			//弾の終了
void UpdateBullet(void);			//弾の更新
void DrawBullet(void);				//弾の描画

void SetBullet(D3DXVECTOR3 pos,D3DXVECTOR3 move, D3DXVECTOR3 rot, float fWidth,float fHeight,int nLife,BULLETTYPE type);//弾の設定処理

#endif

