//=================================
//
// HGS_SPRING 基盤
// Author:Asuma Nishio
//
//=================================

//******************************************
// インクルードファイル宣言
//******************************************
#include <time.h>
#include "main.h"
#include <stdlib.h>
#include <crtdbg.h>
#include <stdio.h>
#include "input.h"
#include "title.h"
#include "tutorial.h"
#include "game.h"
#include "result.h"
#include "ranking.h"
#include "fade.h"
#include "edit.h"
#include "sound.h"

//******************************************
// グローバル変数宣言
//******************************************
LPDIRECT3D9 g_pD3D = NULL;				//　Direct3Dオブジェクトへのポインタ
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;	//　Direct3Dデバイスへのポインタ
MODE g_mode = MODE_TITLE;				//　現在のモード
bool g_isFullscreen = false;			//　ウィンドウを切り替えるためのフラグ
RECT g_windowRect;						//　ウィンドウを切り替えるための変数
LPD3DXFONT g_pFont = NULL;				//　フォントへのポインタ
int g_nCountFPS = 0;					//　FPSカウンタ

void DrawEditkey(void);

//************************************************
// ウィンドウをフルスクリーンに変える処理
//************************************************
void ToggleFullscreen(HWND hWnd)
{
	// 現在のウィンドウスタイルを取得
	DWORD dwStyle = GetWindowLong(hWnd, GWL_STYLE);

	if (g_isFullscreen)
	{
		// ウィンドウモードに切り替え
		SetWindowLong(hWnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPos(hWnd, HWND_TOP, g_windowRect.left, g_windowRect.top,
			g_windowRect.right - g_windowRect.left, g_windowRect.bottom - g_windowRect.top,
			SWP_FRAMECHANGED | SWP_NOACTIVATE);
		ShowWindow(hWnd, SW_NORMAL);
	}
	else
	{
		// フルスクリーンモードに切り替え
		GetWindowRect(hWnd, &g_windowRect);
		SetWindowLong(hWnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
		SetWindowPos(hWnd, HWND_TOP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
			SWP_FRAMECHANGED | SWP_NOACTIVATE);
		ShowWindow(hWnd, SW_MAXIMIZE);
	}

	g_isFullscreen = !g_isFullscreen;
}
//===============================
// メイン関数
//===============================
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hInstancePrev, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);			// メモリリーク検知用のフラグ

	DWORD dwCurrentTime;			// 現在時刻
	DWORD dwExecLastTime;			// 終了時刻

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),				 // WNDCLASSEXのメモリサイズ
		CS_CLASSDC,						 // ウインドウのスタイル
		WindowProc,						 // ウインドウプロシージャ
		0,								 // 0にする
		0,								 // 0にする
		hInstance,						 // インスタンスハンドル
		LoadIcon(NULL,IDI_APPLICATION),  // タスクバーアイコン
		LoadCursor(NULL,IDC_ARROW),      // マウスカーソル
		(HBRUSH)(COLOR_WINDOW + 1),      // クライアント領域の背景色
		NULL,                            // メニューバー
		CLASS_NAME,                      // ウインドウクラスの名前
		LoadIcon(NULL,IDI_APPLICATION),  // ファイルアイコン
	};

	HWND hWnd;  // ウインドウハンドル(識別子)
	MSG msg;    // メッセージを格納する変数

	// 画面サイズの構造
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

	// フルスクリーンモード
	BOOL bWindow = FALSE;
	
	// ウインドウクラスの登録
	RegisterClassEx(&wcex);

	// クライアント領域
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	// ウインドウを生成
	hWnd = CreateWindowEx(0,		// 拡張ウインドウスタイル
	CLASS_NAME,						// ウインドウクラスの名前
	WINDOW_NAME,					// ウインドウの名前
	WS_OVERLAPPEDWINDOW,			// ウインドウスタイル
	CW_USEDEFAULT,					// ウインドウの左上のX座標
	CW_USEDEFAULT,					// ウインドウの左上のY座標
	(rect.right - rect.left),       // ウインドウ幅
	(rect.bottom - rect.top),       // ウインドウの高さ
	NULL,							   
	NULL,							   
	hInstance,						// インスタンスハンドル
	NULL);							// ウインドウ作成データ

	// 初期化処理
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{
		// 初期化処理が失敗したとき
		return -1;
	}

	// 分解能を設定
	timeBeginPeriod(1);
	dwCurrentTime = 0;					// 初期化
	dwExecLastTime = timeGetTime();		// 現在時刻を保存

	// ウインドウの表示
	ShowWindow(hWnd, SW_SHOWMAXIMIZED); // ウインドウの表示状態の設定
	UpdateWindow(hWnd);					// クライアント領域の更新

	// 初期化
	DWORD dwFrameCount;					// フレームカウント
	DWORD dwFPSLastTime;				// 最後にFPSを計測した時刻

	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();

	// メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{
			// Windowsの処理
			if (msg.message == WM_QUIT)
			{
				// WM_QUITメッセージを受け取ったらループ抜ける
				break;
			}
			else
			{
				// メッセージ設定
				TranslateMessage(&msg);	// 仮想キーメッセージを文字メッセージへ変換
				DispatchMessage(&msg);	// ウインドウプロシージャへメッセージを送出
			}
		}
		else
		{

			// DirectXの処理
			dwCurrentTime = timeGetTime();		// 現在時刻の取得

			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{
				// 0.5秒経過
				// FPSを計測
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);

				dwFPSLastTime = dwCurrentTime;	// 保存

				dwFrameCount = 0;				// 0に戻す
		
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{
				//  60/1経過
				dwExecLastTime = dwCurrentTime;	// 処理開始の時刻

				dwFrameCount++;					// フレームカウントを加算

				//更新処理
				Update();

				//描画処理
				Draw();

			}
		}
	}

	// 終了処理
	Uninit();

	// 分解能を戻す
	timeEndPeriod(1);

	// ウインドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return(int)msg.wParam;
}
//===========================================
// ウインドウプロシージャ
//===========================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	const RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT }; // ウインドウの領域
	int nID;
	switch (uMsg)
	{
	case WM_DESTROY: // ウインドウ破棄メッセージ

		//W M_QUITメッセージを送る
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:				// キー押下のメッセージ
		switch (wParam)
		{
		case VK_ESCAPE:				//[ ESC]キーが押された
			nID = MessageBox(hWnd, "終了しますか?", "終了メッセージ", MB_YESNO);
			if (nID == IDYES)
			{
				DestroyWindow(hWnd);// ウインドウ破棄メッセージ

			}
			else
			{
				return 0;			// 返す
			}
			break;

		case VK_F11:
			ToggleFullscreen(hWnd);	// F11でフルスクリーン
			break;
		}
		break;
	}

	// 既定の処理を繰り返す
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
//=========================
// 初期化処理
//=========================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;			// ディスプレイモード
	D3DPRESENT_PARAMETERS d3dpp;	// プレゼンテーションパラメータ

	// Direct3Dオブジェクトの生成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}
	// 現在のディスプレイモードを取得
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションのパラメーターを設定
	ZeroMemory(&d3dpp, sizeof(d3dpp)); // パラメーターの0クリア

	d3dpp.BackBufferWidth = SCREEN_WIDTH;						// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;						// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;						// バックバッファの形式
	d3dpp.BackBufferCount = 1;									// バックバッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					// ダブルバッファの切り替え(映像信号に同期)
	d3dpp.EnableAutoDepthStencil = TRUE;						// デプスバッファとステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					// デプスバッファとして16bitを使用
	d3dpp.Windowed = bWindow;									// ウインドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	// リフレッシュシート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// インターバル

	// Direct3Dデバイスの生成(描画処理と頂点処理をハードウェアで行う)
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice)))
	{
		// Direct3Dデバイスの生成(描画処理はハードウェア,頂点処理をCPUで行う)
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice)))
		{
			// Direct3Dデバイスの生成(描画処理,頂点処理をCPUで行う)
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	// レンダーステートの設定
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// サンプラーステートの設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	// テクスチャステージチャートの設定
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	//=================
	// 各種初期化処理
	//=================
	
	// デバッグ表示用のフォントを設定
	D3DXCreateFont(g_pD3DDevice, 18, 0, 0, 0,
		FALSE,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH,
		"Terminal", &g_pFont);

	// キーボードの初期化処理
	if (FAILED(InitKeyboard(hInstance, hWnd)))
	{
		return E_FAIL;
	}
	// ジョイパッドの初期化
	if (FAILED(InitJoypad()))
	{
		return E_FAIL;
	}

	// サウンド初期化
	InitSound(hWnd);

	//モードの設定
	SetMode(g_mode);

	// フェードの設定
	InitFade(g_mode);

	return S_OK;
}
//==================
// 終了処理
//==================
void Uninit(void)
{
	//==================
	// 各種終了処理
	//==================
	
	// キーボードの終了処理
	UninitKeyboard();

	// ジョイパッドの終了
	UninitJoypad();

	//フェードの終了
	UninitFade();

	//サウンドの終了
	UninitSound();

	// デバッグ表示用のフォントの破棄
	if (g_pFont != NULL)
	{
		g_pFont->Release();
		g_pFont = NULL;
	}

	// Direct3Dデバイスの破棄
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	// Direct3Dオブジェクトの破棄
	if (g_pD3D != NULL)
	{
		g_pD3D ->Release();
		g_pD3D = NULL;
	}

}
//===================
// 更新処理
//===================
void Update(void)
{
	//========================
	// 各種オブジェクト更新処理
	//========================
	
	// キーボードの更新処理
	UpdateKeyboard();

	// ジョイパッドの更新処理
	UpdateJoypad();

	switch (g_mode)
	{
	case MODE_TITLE:		// タイトル画面
		UpdateTitle();
		break;

	case MODE_TUTORIAL:		// チュートリアル画面
		UpdateTutorial();
		break;

	case MODE_GAME:			// ゲーム画面
		UpdateGame();
		break;

	case MODE_RESULT:		// リザルト画面
		UpdateResult();
		break;

	case MODE_EDIT:			// エディット画面
		UpdateEdit();
		break;

	case MODE_RANKING:		// ランキング画面
		UpdateRanking();
		break;
	}

	//フェードの更新
	UpdateFade();
}
//===================
// 描画処理
//===================
void Draw(void)
{
	// 画面クリア(バックバッファ&Zバッファのクリア)
	g_pD3DDevice->Clear(0,
		NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// 描画開始
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{// 描画成功時
		
		//================
		// 描画処理
		//================
		
		// 現在の画面の終了
		switch (g_mode)
		{
		case MODE_TITLE:	// タイトル画面
			DrawTitle();
			break;

		case MODE_TUTORIAL: // チュートリアル画面
			DrawTutorial();
			break;

		case MODE_GAME:		// ゲーム画面
			DrawGame();
			break;

		case MODE_RESULT:	// リザルト
			DrawResult();
			break;

		case MODE_EDIT:		// エディット画面
			DrawEdit();
			DrawEditkey();
			break;

		case MODE_RANKING:	// ランキング画面
			DrawRanking();
			break;
		}

		//フェードの描画
		DrawFade();
//
//#ifdef _DEBUG
//
//		//FPSの表示
//		DrawFPS();
//
//#endif

		// 描画終了
		g_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}
//======================
// FPSの表示
//======================
void DrawFPS(void)
{
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aString[256];

	// 文字列に代入
	wsprintf(&aString[0], "FPS:%d\n", g_nCountFPS);

	// テキストの描画
	g_pFont->DrawText(NULL, &aString[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0,0, 255));
}
#if 0
//============================
// 敵のデバッグ表示
//============================
void DrawEnemySet(int g_nNumEnemy)
{
	// ローカル変数
	RECT rect = { 0,20,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aString[256];

	// 文字列に代入
	wsprintf(&aString[0], "敵の総数:%d\n", g_nNumEnemy);

	// テキスト描画
	g_pFont->DrawText(NULL, &aString[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0,0, 255));
}
//==============================
// プレイヤーの座標表示
//==============================
void DrawDebugPlayerPos(void)
{
	// プレイヤーの取得
	Player* pPlayer = GetPlayer();

	RECT rectPosx = { 0,40,SCREEN_WIDTH,SCREEN_HEIGHT };
	RECT rectPosy = { 0,60,SCREEN_WIDTH,SCREEN_HEIGHT };
	RECT rectkey = { 0,80,SCREEN_WIDTH,SCREEN_HEIGHT };

	// 文字列
	char aStringx[256];
	char aStringy[256];
	char aStringKey[256];

	// 文字列に代入
	sprintf(&aStringx[0], "プレイヤーのX座標:%.2f\n", pPlayer->pos.x);
	sprintf(&aStringy[0], "プレイヤーのY座標:%.2f\n", pPlayer->pos.y);
	wsprintf(&aStringKey[0], "エディターモードへ   [F4]\n");

	// テキスト描画
	g_pFont->DrawText(NULL, &aStringx[0], -1, &rectPosx, DT_LEFT, D3DCOLOR_RGBA(255, 0,0, 255));
	g_pFont->DrawText(NULL, &aStringy[0], -1, &rectPosy, DT_LEFT, D3DCOLOR_RGBA(255, 0,0, 255));
	g_pFont->DrawText(NULL, &aStringKey[0], -1,&rectkey, DT_LEFT, D3DCOLOR_RGBA(255,0,0,255));	// モード変更キー

}
//==============================
// エディット画面の表示
//==============================
void DrawAllBlock(int g_nCntEdit)
{
	//ローカル変数
	RECT rectBlock = { 0,180,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStringBlock[256];

	//文字列に代入
	wsprintf(&aStringBlock[0], "ブロックの総数: %d / 2056\n", g_nCntEdit);

	//テキスト描画
	g_pFont->DrawText(NULL, &aStringBlock[0], -1, &rectBlock, DT_LEFT, D3DCOLOR_RGBA(255,0,0,255));

}
//==============================
// エディット画面の表示
//==============================
void DrawEditPos(void)
{
	// 情報の取得
	EditInfo* pEdit = GetEditInfo();

	RECT rectPosx = { 0,200,SCREEN_WIDTH,SCREEN_HEIGHT };
	RECT rectPosy = { 0,220,SCREEN_WIDTH,SCREEN_HEIGHT };

	//文字列
	char aStringx[256];
	char aStringy[256];

	//文字列に代入
	sprintf(&aStringx[0], "ブロックのX座標:%.2f\n", pEdit->pos.x);
	sprintf(&aStringy[0], "ブロックのY座標:%.2f\n", pEdit->pos.y);

	//テキスト描画
	g_pFont->DrawText(NULL, &aStringx[0], -1, &rectPosx, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
	g_pFont->DrawText(NULL, &aStringy[0], -1, &rectPosy, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));

#endif
//======================
// モードの設定
//======================
void SetMode(MODE mode)
{
	// 現在の画面の終了処理
	switch (g_mode)
	{
	case MODE_TITLE:		// タイトル画面
		UninitTitle();
		break;

	case MODE_TUTORIAL:		// チュートリアル画面
		UninitTutorial();
		break;

	case MODE_GAME:			// ゲーム画面
		UninitGame();
		break;

	case MODE_RESULT:		// リザルト画面
		UninitResult();
		break;

	case MODE_EDIT://エディット画面
		UninitEdit();
		break;

	case MODE_RANKING:		// ランキング画面
		UninitRanking();
		break;

	}

	g_mode = mode;			// 画面切り替え

	// 新しい画面の初期化処理
	switch (mode)
	{
	case MODE_TITLE:		// タイトル画面
		InitTitle();
		break;

	case MODE_TUTORIAL:		// チュートリアル画面
		InitTutorial();
		break;

	case MODE_GAME:			// ゲーム画面
		InitGame();
		break;

	case MODE_RESULT:		// リザルト画面
		InitResult();
		break;

	case MODE_EDIT:			// エディット画面
		InitEdit();
		break;

	case MODE_RANKING:		// ランキング画面
		InitRanking();
		break;

	}
}
//======================
//デバイスの取得
//======================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}
//====================
//モードの取得
//====================
MODE GetMode(void)
{
	return g_mode;//変数を返す
}
//==============================
// エディット画面の表示
//==============================
void DrawEditkey(void)
{
	int ndata = ReturnPass();


	// ローカル変数
	RECT rect = { 0,20,SCREEN_WIDTH,SCREEN_HEIGHT };
	RECT rect8 = { 0,40,SCREEN_WIDTH,SCREEN_HEIGHT };
	RECT rect2 = { 0,60,SCREEN_WIDTH,SCREEN_HEIGHT };
	RECT rect3 = { 0,80,SCREEN_WIDTH,SCREEN_HEIGHT };
	RECT rect4 = { 0,100,SCREEN_WIDTH,SCREEN_HEIGHT };
	RECT rect5 = { 0,120,SCREEN_WIDTH,SCREEN_HEIGHT };
	RECT rect6 = { 0,140,SCREEN_WIDTH,SCREEN_HEIGHT };
	RECT rect7 = { 0,160,SCREEN_WIDTH,SCREEN_HEIGHT };
	RECT rect9 = { 0,180,SCREEN_WIDTH,SCREEN_HEIGHT };
	RECT rect10 = { 0,280,SCREEN_WIDTH,SCREEN_HEIGHT };

	// 文字列
	char aString1[256];
	char aString2[256];
	char aString3[256];
	char aString4[256];
	char aString5[256];
	char aString6[256];
	char aString7[256];
	char aString8[256];
	char aString9[256];
	char aString10[256];

	char aStFile[256];

	switch (ndata)
	{
	case 0:
		strcpy(aStFile, "data/TEXT/SetBlock.txt"); // 初期ファイル
		break;

	case 1:
		strcpy(aStFile, "data/TEXT/SetBlock001.txt"); // 
		break;

	case 2:
		strcpy(aStFile, "data/TEXT/SetBlock002.txt"); // 
		break;


	case 3:
		strcpy(aStFile, "data/TEXT/SetBlock003.txt"); // 
		break;

	case 4:
		strcpy(aStFile, "data/TEXT/SetBlock004.txt"); // 
		break;

	default:
		break;
	}

	// 文字列に代入
	wsprintf(&aString1[0], "[ F7 ] : ファイル書き出し  ***< %s >*** \n", &aStFile[0]);
	wsprintf(&aString2[0], "移動キー         [W/A/S/D]\n");
	wsprintf(&aString3[0], "テクスチャの切り替え　[↑/↓]\n");
	wsprintf(&aString4[0], "高さの増減       [ V/C ]\n");
	wsprintf(&aString5[0], "横幅の増減       [ Q/E ]\n");
	wsprintf(&aString6[0], "ファイル書き出し [ F7キー ]\n");
	wsprintf(&aString7[0], "配置の決定       [ Enterキー ]\n");
	wsprintf(&aString8[0], "ファイルパス切り替え       [ F4キー ]\n");
	wsprintf(&aString9[0], "再読み込み       [ F9キー ]\n");
	wsprintf(&aString10[0], "ゲーム画面へ    [ F3キー ]\n");

	// テキスト描画
	g_pFont->DrawText(NULL, &aString1[0], -1, &rect, DT_LEFT,  D3DCOLOR_RGBA(255, 255, 255, 255));		// 移動キー
	g_pFont->DrawText(NULL, &aString2[0], -1, &rect2, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));		// 移動キー
	g_pFont->DrawText(NULL, &aString3[0], -1, &rect3, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));		// テクスチャ切り替え
	g_pFont->DrawText(NULL, &aString4[0], -1, &rect4, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));		// 高さ調整
	g_pFont->DrawText(NULL, &aString5[0], -1, &rect5, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));		// 横幅調整
	g_pFont->DrawText(NULL, &aString6[0], -1, &rect6, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));		// 書き出しキー
	g_pFont->DrawText(NULL, &aString7[0], -1, &rect7, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));		// 配置キー
	g_pFont->DrawText(NULL, &aString8[0], -1, &rect8, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));		// 配置キー
	g_pFont->DrawText(NULL, &aString9[0], -1, &rect9, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));		// 配置キー
	g_pFont->DrawText(NULL, &aString10[0], -1, &rect10, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));	// モード変更キー

}
