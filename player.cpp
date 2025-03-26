//====================================
//
// プレイヤー処理 [player.cpp]
// Author:Asuma Nishio
//
//====================================

#include "player.h"
#include "input.h"
// #include "bullet.h"
// #include "explosion.h"
#include "fade.h"
#include "bullet.h"
//#include "effect.h"
//#include "particle.h"
//#include "sound.h"

//*********************************
// マクロ定義
//*********************************
#define MAX_LIFEBARSIZE_X (400)		// 横幅
#define MAX_LIFEBARSIZE_Y (20)		// 高さ
									   
#define MAX_UNDER_POS	  (700)		// 下に行ける最大Y座標
#define MAX_TOP_POS       (20)		// 上に行ける最大Y座標
#define MAX_RIGHT_POS     (1260)	// 右に行ける最大X座標
#define MAX_LEFT_POS      (20)      // 左に行ける最大X座標

//グローバル変数
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffplayer = NULL;		// 頂点バッファのポインタ
LPDIRECT3DTEXTURE9 g_pTextureplayer = NULL;				// テクスチャのポインタ										   
Player g_aPlayer;										// プレイヤーの情報
														   
int g_nApperCnt = 0;									// プレイヤーの無敵カウント

//=========================
// プレイヤーの初期化処理
//=========================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ

	// 頂点情報のポインタ
	VERTEX_2D* pVtx;		

	// テクスチャ2の読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\player.png",
		&g_pTextureplayer);

	// 構造体変数の初期化
	g_aPlayer.pos =  D3DXVECTOR3(640.0f,450.0f,0.0f);		// 位置の初期化
	g_aPlayer.move = D3DXVECTOR3(0.0f,0.0f,0.0f);			// 移動量の初期化
	g_aPlayer.rot =  D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 向きの初期化
	g_aPlayer.nLife = 100;									// 体力
	g_aPlayer.nCntAnimState = 0;							// ヒット状態のカウントの初期化
	g_aPlayer.bHit = false;									// 当たり判定
	g_aPlayer.nCntState = 0;								// ダメージカウントの初期化
	g_aPlayer.state = PLAYERSTATE_APPEAR;					// プレイヤーの初期状態
	g_aPlayer.bDisp = true;									// プレイヤーの使用判定
	g_nApperCnt = 0;										// 無敵判定のカウントを初期化

	//対角線の長さを算出
	g_aPlayer.fLength = sqrtf((MAX_SIZE_X * MAX_SIZE_X) + (MAX_SIZE_Y * MAX_SIZE_Y)) / 2.0f;
	
	//対角線の角度を算出
	g_aPlayer.fAngle = atan2f(MAX_SIZE_X,MAX_SIZE_Y);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY, FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffplayer,
		NULL);

	//頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffplayer->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos.x = g_aPlayer.pos.x + sinf(g_aPlayer.rot.z + (-D3DX_PI + g_aPlayer.fAngle)) * g_aPlayer.fLength;
	pVtx[0].pos.y = g_aPlayer.pos.y + cosf(g_aPlayer.rot.z + (-D3DX_PI + g_aPlayer.fAngle)) * g_aPlayer.fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_aPlayer.pos.x + sinf(g_aPlayer.rot.z + (D3DX_PI - g_aPlayer.fAngle)) * g_aPlayer.fLength;
	pVtx[1].pos.y = g_aPlayer.pos.y + cosf(g_aPlayer.rot.z + (D3DX_PI - g_aPlayer.fAngle)) * g_aPlayer.fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_aPlayer.pos.x + sinf(g_aPlayer.rot.z + (0.0f - g_aPlayer.fAngle)) * g_aPlayer.fLength;
	pVtx[2].pos.y = g_aPlayer.pos.y + cosf(g_aPlayer.rot.z + (0.0f - g_aPlayer.fAngle)) * g_aPlayer.fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_aPlayer.pos.x + sinf(g_aPlayer.rot.z + (0.0f + g_aPlayer.fAngle)) * g_aPlayer.fLength;
	pVtx[3].pos.y = g_aPlayer.pos.y + cosf(g_aPlayer.rot.z + (0.0f + g_aPlayer.fAngle)) * g_aPlayer.fLength;
	pVtx[3].pos.z = 0.0f;

	//rhwの設定(1.0fで固定)
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(SPLIT_U, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, SPLIT_V);
	pVtx[3].tex = D3DXVECTOR2(SPLIT_U, SPLIT_V);

	//アンロック
	g_pVtxBuffplayer->Unlock();

}
//======================
// プレイヤーの終了処理
//======================
void UninitPlayer(void)
{
	// StopSound();

	// テクスチャの破棄
	if (g_pTextureplayer != NULL)
	{
		g_pTextureplayer->Release();
		g_pTextureplayer = NULL;
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffplayer != NULL)
	{
		g_pVtxBuffplayer->Release();
		g_pVtxBuffplayer = NULL;
	}
}
//=====================
// プレイヤーの更新処理
//=====================
void UpdatePlayer(void)
{
	// 頂点情報のポインタ
	VERTEX_2D* pVtx;	

	// キー入力情報の取得
	GetKeyPlayer();

	// パッド情報を取得
	GetJoypadPlayer();

	// スティック情報の取得
	StickState();

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffplayer->Lock(0, 0, (void**)&pVtx, 0);

	switch (g_aPlayer.state)
	{
	case PLAYERSTATE_NORMAL:
		break;

	case PLAYERSTATE_DAMAGE:
		g_aPlayer.nCntState--;

		if (g_aPlayer.nCntState <= 0)
		{
			g_aPlayer.state = PLAYERSTATE_APPEAR;
			g_aPlayer.nCntState = 80;

			// 頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		}
		break;

	case PLAYERSTATE_APPEAR:
		{
			// 無敵判定
			float farufa = 1.0f;
			g_aPlayer.nCntState--;

			if (g_aPlayer.nCntState % 5 == 0)
			{
				// 透明にする
				farufa = 0.0f;
			}
			else
			{
				// 通常色
				farufa = 1.0f;
			}

			if (g_aPlayer.nCntState <= 0)
			{
				// プレイヤーを通常状態に
				farufa = 1.0f;
				g_aPlayer.state = PLAYERSTATE_NORMAL;
			}

			// 頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, farufa);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, farufa);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, farufa);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, farufa);

			break;
		}
	}

	// アンロック
	g_pVtxBuffplayer->Unlock();

}
//======================
// プレイヤーの描画処理
//======================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// デバイスへのポインタ

	if (g_aPlayer.bDisp)
	{
		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffplayer, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureplayer);

		// プレイヤーの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}

}
//====================
// キー入力情報の取得
//====================
void GetKeyPlayer(void)
{
	VERTEX_2D* pVtx;		// 頂点情報のポインタ
	static float fData = SPLIT_U;
	static float fData2 = 0.0f;

	// Aキーを押したら
	if (GetKeyboardPress(DIK_A) == true)
	{
		fData = SPLIT_U;
		// A単体の処理
		g_aPlayer.move.x -= 2.0f;

		if(GetKeyboardPress(DIK_W) == true)
		{
			// AとWキーを押したら
			g_aPlayer.move.y -= 2.0f;
		}
		else if (GetKeyboardPress(DIK_S) == true)
		{
			// AとSを押したら
			g_aPlayer.move.y += 2.0f;
		}
	}
	// Dを押したら
	else if (GetKeyboardPress(DIK_D) == true)
	{
		fData = -SPLIT_U;

		// D単体の処理
		g_aPlayer.move.x += 2.0f;

		if (GetKeyboardPress(DIK_S) == true)
		{
			// DとSが押された
			g_aPlayer.move.y += 2.0f;

		}
		else if (GetKeyboardPress(DIK_W) == true)
		{
			// DとWが押された
			g_aPlayer.move.y -= 2.0f;

		}
	}
	// Sを押したら
	else if (GetKeyboardPress(DIK_S) == true)
	{
		g_aPlayer.move.y += 2.0f;
	}
	// Wを押したら
	else if (GetKeyboardPress(DIK_W) == true)
	{
		g_aPlayer.move.y -= 2.0f;
	}
	//if (KeyboardTrigger(DIK_SPACE) == true)
	//{
	//	// PlaySound(SOUND_LABEL_PLAYERBULLET);

	//	// 弾の設定
	//	SetBullet(g_aPlayer.pos, D3DXVECTOR3(sinf(g_aPlayer.rot.z + D3DX_PI) * 10.0f, cosf(g_aPlayer.rot.z + D3DX_PI) * 10.0f, 0.0f), g_aPlayer.rot, 30.0f, 30.0f, 70, BULLETTYPE_PLAYER);

	//}
	
	if (KeyboardTrigger(DIK_E) == true && KeyboardTrigger(DIK_Q) == true)
	{
		// 正面に向くようにリセット
		g_aPlayer.rot.z = 0.0f;
	}
	else if (KeyboardTrigger(DIK_E) == true)
	{
		// 回転(右)
		g_aPlayer.rot.z = g_aPlayer.rot.z - (D3DX_PI * 0.5f);
	}
	else if (KeyboardTrigger(DIK_Q) == true)
	{
		// 回転(左)
		g_aPlayer.rot.z = g_aPlayer.rot.z + (D3DX_PI * 0.5f);
	}

	if (KeyboardTrigger(DIK_UP))
	{// 上矢印キー
		// 弾の設定
		SetBullet(g_aPlayer.pos, D3DXVECTOR3(sinf(g_aPlayer.rot.z + D3DX_PI) * 10.0f, cosf(g_aPlayer.rot.z + D3DX_PI) * 10.0f, 0.0f), g_aPlayer.rot, 30.0f, 30.0f, 70, BULLETTYPE_PLAYER);
	}
	else if (KeyboardTrigger(DIK_DOWN))
	{// 下矢印キー
		// 弾の設定
		SetBullet(g_aPlayer.pos, D3DXVECTOR3(-sinf(g_aPlayer.rot.z + D3DX_PI) * 10.0f, -cosf(g_aPlayer.rot.z + D3DX_PI) * 10.0f, 0.0f), g_aPlayer.rot, 30.0f, 30.0f, 70, BULLETTYPE_PLAYER);
	}
	else if (KeyboardTrigger(DIK_LEFT))
	{// 左矢印キー
		// 弾の設定
		SetBullet(g_aPlayer.pos, D3DXVECTOR3(sinf(g_aPlayer.rot.z - D3DX_PI * 0.5f) * 10.0f, cosf(g_aPlayer.rot.z - D3DX_PI * 0.5f) * 10.0f, 0.0f), g_aPlayer.rot, 30.0f, 30.0f, 70, BULLETTYPE_PLAYER);
	}
	else if (KeyboardTrigger(DIK_RIGHT))
	{// 右矢印キー
		// 弾の設定
		SetBullet(g_aPlayer.pos, D3DXVECTOR3(sinf(g_aPlayer.rot.z + D3DX_PI * 0.5f) * 10.0f, -cosf(g_aPlayer.rot.z - D3DX_PI * 0.5f) * 10.0f, 0.0f), g_aPlayer.rot, 30.0f, 30.0f, 70, BULLETTYPE_PLAYER);
	}

	// 位置を更新
	g_aPlayer.pos.x += g_aPlayer.move.x;
	g_aPlayer.pos.y += g_aPlayer.move.y;

	// 右壁
	if (g_aPlayer.pos.x  >= MAX_RIGHT_POS)
	{
		g_aPlayer.pos.x = MAX_RIGHT_POS;
		g_aPlayer.move.x = 0.0f;

	}
	// 左壁
	else if (g_aPlayer.pos.x <= MAX_LEFT_POS)
	{
		g_aPlayer.pos.x = MAX_LEFT_POS;
		g_aPlayer.move.x = 0.0f;
	}

	//下壁
	if (g_aPlayer.pos.y >= MAX_UNDER_POS)
	{
		g_aPlayer.pos.y = MAX_UNDER_POS;
		g_aPlayer.move.y = 0.0f;
	}

	//上壁
	else if (g_aPlayer.pos.y <= MAX_TOP_POS)
	{
		g_aPlayer.pos.y = MAX_TOP_POS;
		g_aPlayer.move.y = 0.0f;
	}


	// 移動量の更新(減衰させる)
	g_aPlayer.move.x += (0.0f - g_aPlayer.move.x) * 0.25f;
	g_aPlayer.move.y += (0.0f - g_aPlayer.move.y) * 0.25f;

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffplayer->Lock(0, 0, (void**)&pVtx, 0);

	if (g_aPlayer.nCntAnimState >= 5)
	{
		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);

		g_aPlayer.nCntAnimState = 0;			// 初期状態に戻す
		g_aPlayer.bHit = false;					// 判定を未使用状態にする
	}

	// 頂点座標の設定
	pVtx[0].pos.x = g_aPlayer.pos.x + sinf(g_aPlayer.rot.z + (-D3DX_PI + g_aPlayer.fAngle)) * g_aPlayer.fLength;
	pVtx[0].pos.y = g_aPlayer.pos.y + cosf(g_aPlayer.rot.z + (-D3DX_PI + g_aPlayer.fAngle)) * g_aPlayer.fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_aPlayer.pos.x + sinf(g_aPlayer.rot.z + (D3DX_PI - g_aPlayer.fAngle)) * g_aPlayer.fLength;
	pVtx[1].pos.y = g_aPlayer.pos.y + cosf(g_aPlayer.rot.z + (D3DX_PI - g_aPlayer.fAngle)) * g_aPlayer.fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_aPlayer.pos.x + sinf(g_aPlayer.rot.z + (0.0f - g_aPlayer.fAngle)) * g_aPlayer.fLength;
	pVtx[2].pos.y = g_aPlayer.pos.y + cosf(g_aPlayer.rot.z + (0.0f - g_aPlayer.fAngle)) * g_aPlayer.fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_aPlayer.pos.x + sinf(g_aPlayer.rot.z + (0.0f + g_aPlayer.fAngle)) * g_aPlayer.fLength;
	pVtx[3].pos.y = g_aPlayer.pos.y + cosf(g_aPlayer.rot.z + (0.0f + g_aPlayer.fAngle)) * g_aPlayer.fLength;
	pVtx[3].pos.z = 0.0f;

	// アンロック
	g_pVtxBuffplayer->Unlock();

}
//=================================
// パッドの処理
//=================================
void GetJoypadPlayer(void)
{
	VERTEX_2D* pVtx;			// 頂点情報のポイタ

	if (JoypadPress(JOYKEY_UP) == true)
	{
		// 上キー単体の処理
		g_aPlayer.move.y -= 2.0f;

		if (JoypadPress(JOYKEY_LEFT) == true)
		{
			// 上キーと左キーが押された
			g_aPlayer.move.x -= 2.0f;
		}
		else if (JoypadPress(JOYKEY_RIGHT) == true)
		{
			// 上キーと右キーが押された
			g_aPlayer.move.x += 2.0f;
		}
	}
	else if (JoypadPress(JOYKEY_DOWN) == true)
	{
		// 下キー単体の処理
		g_aPlayer.move.y += 2.0f;

		if (JoypadPress(JOYKEY_LEFT) == true)
		{
			// 下キーと左キーが押された
			g_aPlayer.move.x -= 2.0f;
		}
		else if (JoypadPress(JOYKEY_RIGHT) == true)
		{
			// 下キーと右キーが押された
			g_aPlayer.move.x += 2.0f;
		}
	}
	else if (JoypadPress(JOYKEY_LEFT) == true)
	{
		// 左キーが押された
		g_aPlayer.move.x -= 2.0f;
	}
	else if (JoypadPress(JOYKEY_RIGHT) == true)
	{
		// 右キーが押された
		g_aPlayer.move.x += 2.0f;
	}

	if (JoypadTrigger(JOYKEY_B) == true)
	{// Bボタンを押した
		// 弾の設定
		SetBullet(g_aPlayer.pos, D3DXVECTOR3(sinf(g_aPlayer.rot.z + D3DX_PI * 0.5f) * 10.0f, -cosf(g_aPlayer.rot.z - D3DX_PI * 0.5f) * 10.0f, 0.0f), g_aPlayer.rot, 30.0f, 30.0f, 70, BULLETTYPE_PLAYER);
	}
	else if (JoypadTrigger(JOYKEY_X) == true)
	{// Xボタンを押した
		// 弾の設定
		SetBullet(g_aPlayer.pos, D3DXVECTOR3(sinf(g_aPlayer.rot.z - D3DX_PI * 0.5f) * 10.0f, cosf(g_aPlayer.rot.z - D3DX_PI * 0.5f) * 10.0f, 0.0f), g_aPlayer.rot, 30.0f, 30.0f, 70, BULLETTYPE_PLAYER);
	}
	else if (JoypadTrigger(JOYKEY_Y) == true)
	{// Yボタンを押した
		// 弾の設定
		SetBullet(g_aPlayer.pos, D3DXVECTOR3(sinf(g_aPlayer.rot.z + D3DX_PI) * 10.0f, cosf(g_aPlayer.rot.z + D3DX_PI) * 10.0f, 0.0f), g_aPlayer.rot, 30.0f, 30.0f, 70, BULLETTYPE_PLAYER);
	}
	else if (JoypadTrigger(JOYKEY_A) == true)
	{// Aボタン
		// 弾の設定
		SetBullet(g_aPlayer.pos, D3DXVECTOR3(-sinf(g_aPlayer.rot.z + D3DX_PI) * 10.0f, -cosf(g_aPlayer.rot.z + D3DX_PI) * 10.0f, 0.0f), g_aPlayer.rot, 30.0f, 30.0f, 70, BULLETTYPE_PLAYER);
	}


	// 位置を更新
	g_aPlayer.pos.x += g_aPlayer.move.x;
	g_aPlayer.pos.y += g_aPlayer.move.y;

	// 移動量の更新(減衰させる)
	g_aPlayer.move.x += (0.0f - g_aPlayer.move.x) * 0.25f;
	g_aPlayer.move.y += (0.0f - g_aPlayer.move.y) * 0.25f;

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffplayer->Lock(0, 0, (void**)&pVtx, 0);

	if (g_aPlayer.nCntAnimState >= 5)
	{
		// 頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		g_aPlayer.nCntAnimState = 0;		// カウンターを0にする
		g_aPlayer.bHit = false;				// 未使用判定
	}

	// 頂点座標の設定
	pVtx[0].pos.x = g_aPlayer.pos.x + sinf(g_aPlayer.rot.z + (-D3DX_PI + g_aPlayer.fAngle)) * g_aPlayer.fLength;
	pVtx[0].pos.y = g_aPlayer.pos.y + cosf(g_aPlayer.rot.z + (-D3DX_PI + g_aPlayer.fAngle)) * g_aPlayer.fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_aPlayer.pos.x + sinf(g_aPlayer.rot.z + (D3DX_PI - g_aPlayer.fAngle)) * g_aPlayer.fLength;
	pVtx[1].pos.y = g_aPlayer.pos.y + cosf(g_aPlayer.rot.z + (D3DX_PI - g_aPlayer.fAngle)) * g_aPlayer.fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_aPlayer.pos.x + sinf(g_aPlayer.rot.z + (0.0f - g_aPlayer.fAngle)) * g_aPlayer.fLength;
	pVtx[2].pos.y = g_aPlayer.pos.y + cosf(g_aPlayer.rot.z + (0.0f - g_aPlayer.fAngle)) * g_aPlayer.fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_aPlayer.pos.x + sinf(g_aPlayer.rot.z + (0.0f + g_aPlayer.fAngle)) * g_aPlayer.fLength;
	pVtx[3].pos.y = g_aPlayer.pos.y + cosf(g_aPlayer.rot.z + (0.0f + g_aPlayer.fAngle)) * g_aPlayer.fLength;
	pVtx[3].pos.z = 0.0f;

	// アンロック
	g_pVtxBuffplayer->Unlock();

}
//================================
// スティック情報
//================================
void StickState(void)
{
	// 頂点情報のポイタ
	VERTEX_2D* pVtx;

	// スティック情報の取得
	XINPUT_STATE* pStick;
	pStick = GetJoyStickAngle();

	if (GetJoyStick() == true)
	{
		if (pStick->Gamepad.sThumbLX > 10922)
		{
			// 右移動
			g_aPlayer.move.x += 2.0f;
		}
		if (pStick->Gamepad.sThumbLX < -10922)
		{
			// 左移動
			g_aPlayer.move.x -= 2.0f;
		}
		if (pStick->Gamepad.sThumbLY > 10922)
		{
			// 上移動
			g_aPlayer.move.y -= 2.0f;
		}
		if (pStick->Gamepad.sThumbLY < -10922)
		{
			// 左移動
			g_aPlayer.move.y += 2.0f;
		}

	}

	// 位置を更新
	g_aPlayer.pos.x += g_aPlayer.move.x;
	g_aPlayer.pos.y += g_aPlayer.move.y;

	// 移動量の更新(減衰させる)
	g_aPlayer.move.x += (0.0f - g_aPlayer.move.x) * 0.25f;
	g_aPlayer.move.y += (0.0f - g_aPlayer.move.y) * 0.25f;

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffplayer->Lock(0, 0, (void**)&pVtx, 0);

	if (g_aPlayer.nCntAnimState >= 5)
	{
		//頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		g_aPlayer.nCntAnimState = 0;		//カウンターを0にする
		g_aPlayer.bHit = false;				//未使用判定
	}

	// 頂点座標の設定
	pVtx[0].pos.x = g_aPlayer.pos.x + sinf(g_aPlayer.rot.z + (-D3DX_PI + g_aPlayer.fAngle)) * g_aPlayer.fLength;
	pVtx[0].pos.y = g_aPlayer.pos.y + cosf(g_aPlayer.rot.z + (-D3DX_PI + g_aPlayer.fAngle)) * g_aPlayer.fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_aPlayer.pos.x + sinf(g_aPlayer.rot.z + (D3DX_PI - g_aPlayer.fAngle)) * g_aPlayer.fLength;
	pVtx[1].pos.y = g_aPlayer.pos.y + cosf(g_aPlayer.rot.z + (D3DX_PI - g_aPlayer.fAngle)) * g_aPlayer.fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_aPlayer.pos.x + sinf(g_aPlayer.rot.z + (0.0f - g_aPlayer.fAngle)) * g_aPlayer.fLength;
	pVtx[2].pos.y = g_aPlayer.pos.y + cosf(g_aPlayer.rot.z + (0.0f - g_aPlayer.fAngle)) * g_aPlayer.fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_aPlayer.pos.x + sinf(g_aPlayer.rot.z + (0.0f + g_aPlayer.fAngle)) * g_aPlayer.fLength;
	pVtx[3].pos.y = g_aPlayer.pos.y + cosf(g_aPlayer.rot.z + (0.0f + g_aPlayer.fAngle)) * g_aPlayer.fLength;
	pVtx[3].pos.z = 0.0f;

	// アンロック
	g_pVtxBuffplayer->Unlock();

}
//===================
//プレイヤーの取得
//===================
Player* GetPlayer(void)
{
	return &g_aPlayer;
}

