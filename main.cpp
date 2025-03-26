//=================================
//
// HGS_SPRING ���
// Author:Asuma Nishio
//
//=================================

//******************************************
// �C���N���[�h�t�@�C���錾
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
// �O���[�o���ϐ��錾
//******************************************
LPDIRECT3D9 g_pD3D = NULL;				//�@Direct3D�I�u�W�F�N�g�ւ̃|�C���^
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;	//�@Direct3D�f�o�C�X�ւ̃|�C���^
MODE g_mode = MODE_TITLE;				//�@���݂̃��[�h
bool g_isFullscreen = false;			//�@�E�B���h�E��؂�ւ��邽�߂̃t���O
RECT g_windowRect;						//�@�E�B���h�E��؂�ւ��邽�߂̕ϐ�
LPD3DXFONT g_pFont = NULL;				//�@�t�H���g�ւ̃|�C���^
int g_nCountFPS = 0;					//�@FPS�J�E���^

void DrawEditkey(void);

//************************************************
// �E�B���h�E���t���X�N���[���ɕς��鏈��
//************************************************
void ToggleFullscreen(HWND hWnd)
{
	// ���݂̃E�B���h�E�X�^�C�����擾
	DWORD dwStyle = GetWindowLong(hWnd, GWL_STYLE);

	if (g_isFullscreen)
	{
		// �E�B���h�E���[�h�ɐ؂�ւ�
		SetWindowLong(hWnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPos(hWnd, HWND_TOP, g_windowRect.left, g_windowRect.top,
			g_windowRect.right - g_windowRect.left, g_windowRect.bottom - g_windowRect.top,
			SWP_FRAMECHANGED | SWP_NOACTIVATE);
		ShowWindow(hWnd, SW_NORMAL);
	}
	else
	{
		// �t���X�N���[�����[�h�ɐ؂�ւ�
		GetWindowRect(hWnd, &g_windowRect);
		SetWindowLong(hWnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
		SetWindowPos(hWnd, HWND_TOP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
			SWP_FRAMECHANGED | SWP_NOACTIVATE);
		ShowWindow(hWnd, SW_MAXIMIZE);
	}

	g_isFullscreen = !g_isFullscreen;
}
//===============================
// ���C���֐�
//===============================
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hInstancePrev, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);			// ���������[�N���m�p�̃t���O

	DWORD dwCurrentTime;			// ���ݎ���
	DWORD dwExecLastTime;			// �I������

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),				 // WNDCLASSEX�̃������T�C�Y
		CS_CLASSDC,						 // �E�C���h�E�̃X�^�C��
		WindowProc,						 // �E�C���h�E�v���V�[�W��
		0,								 // 0�ɂ���
		0,								 // 0�ɂ���
		hInstance,						 // �C���X�^���X�n���h��
		LoadIcon(NULL,IDI_APPLICATION),  // �^�X�N�o�[�A�C�R��
		LoadCursor(NULL,IDC_ARROW),      // �}�E�X�J�[�\��
		(HBRUSH)(COLOR_WINDOW + 1),      // �N���C�A���g�̈�̔w�i�F
		NULL,                            // ���j���[�o�[
		CLASS_NAME,                      // �E�C���h�E�N���X�̖��O
		LoadIcon(NULL,IDI_APPLICATION),  // �t�@�C���A�C�R��
	};

	HWND hWnd;  // �E�C���h�E�n���h��(���ʎq)
	MSG msg;    // ���b�Z�[�W���i�[����ϐ�

	// ��ʃT�C�Y�̍\��
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

	// �t���X�N���[�����[�h
	BOOL bWindow = FALSE;
	
	// �E�C���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	// �N���C�A���g�̈�
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	// �E�C���h�E�𐶐�
	hWnd = CreateWindowEx(0,		// �g���E�C���h�E�X�^�C��
	CLASS_NAME,						// �E�C���h�E�N���X�̖��O
	WINDOW_NAME,					// �E�C���h�E�̖��O
	WS_OVERLAPPEDWINDOW,			// �E�C���h�E�X�^�C��
	CW_USEDEFAULT,					// �E�C���h�E�̍����X���W
	CW_USEDEFAULT,					// �E�C���h�E�̍����Y���W
	(rect.right - rect.left),       // �E�C���h�E��
	(rect.bottom - rect.top),       // �E�C���h�E�̍���
	NULL,							   
	NULL,							   
	hInstance,						// �C���X�^���X�n���h��
	NULL);							// �E�C���h�E�쐬�f�[�^

	// ����������
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{
		// ���������������s�����Ƃ�
		return -1;
	}

	// ����\��ݒ�
	timeBeginPeriod(1);
	dwCurrentTime = 0;					// ������
	dwExecLastTime = timeGetTime();		// ���ݎ�����ۑ�

	// �E�C���h�E�̕\��
	ShowWindow(hWnd, SW_SHOWMAXIMIZED); // �E�C���h�E�̕\����Ԃ̐ݒ�
	UpdateWindow(hWnd);					// �N���C�A���g�̈�̍X�V

	// ������
	DWORD dwFrameCount;					// �t���[���J�E���g
	DWORD dwFPSLastTime;				// �Ō��FPS���v����������

	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();

	// ���b�Z�[�W���[�v
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{
			// Windows�̏���
			if (msg.message == WM_QUIT)
			{
				// WM_QUIT���b�Z�[�W���󂯎�����烋�[�v������
				break;
			}
			else
			{
				// ���b�Z�[�W�ݒ�
				TranslateMessage(&msg);	// ���z�L�[���b�Z�[�W�𕶎����b�Z�[�W�֕ϊ�
				DispatchMessage(&msg);	// �E�C���h�E�v���V�[�W���փ��b�Z�[�W�𑗏o
			}
		}
		else
		{

			// DirectX�̏���
			dwCurrentTime = timeGetTime();		// ���ݎ����̎擾

			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{
				// 0.5�b�o��
				// FPS���v��
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);

				dwFPSLastTime = dwCurrentTime;	// �ۑ�

				dwFrameCount = 0;				// 0�ɖ߂�
		
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{
				//  60/1�o��
				dwExecLastTime = dwCurrentTime;	// �����J�n�̎���

				dwFrameCount++;					// �t���[���J�E���g�����Z

				//�X�V����
				Update();

				//�`�揈��
				Draw();

			}
		}
	}

	// �I������
	Uninit();

	// ����\��߂�
	timeEndPeriod(1);

	// �E�C���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return(int)msg.wParam;
}
//===========================================
// �E�C���h�E�v���V�[�W��
//===========================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	const RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT }; // �E�C���h�E�̗̈�
	int nID;
	switch (uMsg)
	{
	case WM_DESTROY: // �E�C���h�E�j�����b�Z�[�W

		//W M_QUIT���b�Z�[�W�𑗂�
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:				// �L�[�����̃��b�Z�[�W
		switch (wParam)
		{
		case VK_ESCAPE:				//[ ESC]�L�[�������ꂽ
			nID = MessageBox(hWnd, "�I�����܂���?", "�I�����b�Z�[�W", MB_YESNO);
			if (nID == IDYES)
			{
				DestroyWindow(hWnd);// �E�C���h�E�j�����b�Z�[�W

			}
			else
			{
				return 0;			// �Ԃ�
			}
			break;

		case VK_F11:
			ToggleFullscreen(hWnd);	// F11�Ńt���X�N���[��
			break;
		}
		break;
	}

	// ����̏������J��Ԃ�
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
//=========================
// ����������
//=========================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;			// �f�B�X�v���C���[�h
	D3DPRESENT_PARAMETERS d3dpp;	// �v���[���e�[�V�����p�����[�^

	// Direct3D�I�u�W�F�N�g�̐���
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}
	// ���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// �f�o�C�X�̃v���[���e�[�V�����̃p�����[�^�[��ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp)); // �p�����[�^�[��0�N���A

	d3dpp.BackBufferWidth = SCREEN_WIDTH;						// �Q�[����ʃT�C�Y(��)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;						// �Q�[����ʃT�C�Y(����)
	d3dpp.BackBufferFormat = d3ddm.Format;						// �o�b�N�o�b�t�@�̌`��
	d3dpp.BackBufferCount = 1;									// �o�b�N�o�b�t�@�̐�
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					// �_�u���o�b�t�@�̐؂�ւ�(�f���M���ɓ���)
	d3dpp.EnableAutoDepthStencil = TRUE;						// �f�v�X�o�b�t�@�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					// �f�v�X�o�b�t�@�Ƃ���16bit���g�p
	d3dpp.Windowed = bWindow;									// �E�C���h�E���[�h
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	// ���t���b�V���V�[�g
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// �C���^�[�o��

	// Direct3D�f�o�C�X�̐���(�`�揈���ƒ��_�������n�[�h�E�F�A�ōs��)
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice)))
	{
		// Direct3D�f�o�C�X�̐���(�`�揈���̓n�[�h�E�F�A,���_������CPU�ōs��)
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice)))
		{
			// Direct3D�f�o�C�X�̐���(�`�揈��,���_������CPU�ōs��)
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

	// �����_�[�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// �T���v���[�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	// �e�N�X�`���X�e�[�W�`���[�g�̐ݒ�
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	//=================
	// �e�평��������
	//=================
	
	// �f�o�b�O�\���p�̃t�H���g��ݒ�
	D3DXCreateFont(g_pD3DDevice, 18, 0, 0, 0,
		FALSE,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH,
		"Terminal", &g_pFont);

	// �L�[�{�[�h�̏���������
	if (FAILED(InitKeyboard(hInstance, hWnd)))
	{
		return E_FAIL;
	}
	// �W���C�p�b�h�̏�����
	if (FAILED(InitJoypad()))
	{
		return E_FAIL;
	}

	// �T�E���h������
	InitSound(hWnd);

	//���[�h�̐ݒ�
	SetMode(g_mode);

	// �t�F�[�h�̐ݒ�
	InitFade(g_mode);

	return S_OK;
}
//==================
// �I������
//==================
void Uninit(void)
{
	//==================
	// �e��I������
	//==================
	
	// �L�[�{�[�h�̏I������
	UninitKeyboard();

	// �W���C�p�b�h�̏I��
	UninitJoypad();

	//�t�F�[�h�̏I��
	UninitFade();

	//�T�E���h�̏I��
	UninitSound();

	// �f�o�b�O�\���p�̃t�H���g�̔j��
	if (g_pFont != NULL)
	{
		g_pFont->Release();
		g_pFont = NULL;
	}

	// Direct3D�f�o�C�X�̔j��
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	// Direct3D�I�u�W�F�N�g�̔j��
	if (g_pD3D != NULL)
	{
		g_pD3D ->Release();
		g_pD3D = NULL;
	}

}
//===================
// �X�V����
//===================
void Update(void)
{
	//========================
	// �e��I�u�W�F�N�g�X�V����
	//========================
	
	// �L�[�{�[�h�̍X�V����
	UpdateKeyboard();

	// �W���C�p�b�h�̍X�V����
	UpdateJoypad();

	switch (g_mode)
	{
	case MODE_TITLE:		// �^�C�g�����
		UpdateTitle();
		break;

	case MODE_TUTORIAL:		// �`���[�g���A�����
		UpdateTutorial();
		break;

	case MODE_GAME:			// �Q�[�����
		UpdateGame();
		break;

	case MODE_RESULT:		// ���U���g���
		UpdateResult();
		break;

	case MODE_EDIT:			// �G�f�B�b�g���
		UpdateEdit();
		break;

	case MODE_RANKING:		// �����L���O���
		UpdateRanking();
		break;
	}

	//�t�F�[�h�̍X�V
	UpdateFade();
}
//===================
// �`�揈��
//===================
void Draw(void)
{
	// ��ʃN���A(�o�b�N�o�b�t�@&Z�o�b�t�@�̃N���A)
	g_pD3DDevice->Clear(0,
		NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// �`��J�n
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{// �`�搬����
		
		//================
		// �`�揈��
		//================
		
		// ���݂̉�ʂ̏I��
		switch (g_mode)
		{
		case MODE_TITLE:	// �^�C�g�����
			DrawTitle();
			break;

		case MODE_TUTORIAL: // �`���[�g���A�����
			DrawTutorial();
			break;

		case MODE_GAME:		// �Q�[�����
			DrawGame();
			break;

		case MODE_RESULT:	// ���U���g
			DrawResult();
			break;

		case MODE_EDIT:		// �G�f�B�b�g���
			DrawEdit();
			DrawEditkey();
			break;

		case MODE_RANKING:	// �����L���O���
			DrawRanking();
			break;
		}

		//�t�F�[�h�̕`��
		DrawFade();
//
//#ifdef _DEBUG
//
//		//FPS�̕\��
//		DrawFPS();
//
//#endif

		// �`��I��
		g_pD3DDevice->EndScene();
	}

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}
//======================
// FPS�̕\��
//======================
void DrawFPS(void)
{
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aString[256];

	// ������ɑ��
	wsprintf(&aString[0], "FPS:%d\n", g_nCountFPS);

	// �e�L�X�g�̕`��
	g_pFont->DrawText(NULL, &aString[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0,0, 255));
}
#if 0
//============================
// �G�̃f�o�b�O�\��
//============================
void DrawEnemySet(int g_nNumEnemy)
{
	// ���[�J���ϐ�
	RECT rect = { 0,20,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aString[256];

	// ������ɑ��
	wsprintf(&aString[0], "�G�̑���:%d\n", g_nNumEnemy);

	// �e�L�X�g�`��
	g_pFont->DrawText(NULL, &aString[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0,0, 255));
}
//==============================
// �v���C���[�̍��W�\��
//==============================
void DrawDebugPlayerPos(void)
{
	// �v���C���[�̎擾
	Player* pPlayer = GetPlayer();

	RECT rectPosx = { 0,40,SCREEN_WIDTH,SCREEN_HEIGHT };
	RECT rectPosy = { 0,60,SCREEN_WIDTH,SCREEN_HEIGHT };
	RECT rectkey = { 0,80,SCREEN_WIDTH,SCREEN_HEIGHT };

	// ������
	char aStringx[256];
	char aStringy[256];
	char aStringKey[256];

	// ������ɑ��
	sprintf(&aStringx[0], "�v���C���[��X���W:%.2f\n", pPlayer->pos.x);
	sprintf(&aStringy[0], "�v���C���[��Y���W:%.2f\n", pPlayer->pos.y);
	wsprintf(&aStringKey[0], "�G�f�B�^�[���[�h��   [F4]\n");

	// �e�L�X�g�`��
	g_pFont->DrawText(NULL, &aStringx[0], -1, &rectPosx, DT_LEFT, D3DCOLOR_RGBA(255, 0,0, 255));
	g_pFont->DrawText(NULL, &aStringy[0], -1, &rectPosy, DT_LEFT, D3DCOLOR_RGBA(255, 0,0, 255));
	g_pFont->DrawText(NULL, &aStringKey[0], -1,&rectkey, DT_LEFT, D3DCOLOR_RGBA(255,0,0,255));	// ���[�h�ύX�L�[

}
//==============================
// �G�f�B�b�g��ʂ̕\��
//==============================
void DrawAllBlock(int g_nCntEdit)
{
	//���[�J���ϐ�
	RECT rectBlock = { 0,180,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStringBlock[256];

	//������ɑ��
	wsprintf(&aStringBlock[0], "�u���b�N�̑���: %d / 2056\n", g_nCntEdit);

	//�e�L�X�g�`��
	g_pFont->DrawText(NULL, &aStringBlock[0], -1, &rectBlock, DT_LEFT, D3DCOLOR_RGBA(255,0,0,255));

}
//==============================
// �G�f�B�b�g��ʂ̕\��
//==============================
void DrawEditPos(void)
{
	// ���̎擾
	EditInfo* pEdit = GetEditInfo();

	RECT rectPosx = { 0,200,SCREEN_WIDTH,SCREEN_HEIGHT };
	RECT rectPosy = { 0,220,SCREEN_WIDTH,SCREEN_HEIGHT };

	//������
	char aStringx[256];
	char aStringy[256];

	//������ɑ��
	sprintf(&aStringx[0], "�u���b�N��X���W:%.2f\n", pEdit->pos.x);
	sprintf(&aStringy[0], "�u���b�N��Y���W:%.2f\n", pEdit->pos.y);

	//�e�L�X�g�`��
	g_pFont->DrawText(NULL, &aStringx[0], -1, &rectPosx, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
	g_pFont->DrawText(NULL, &aStringy[0], -1, &rectPosy, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));

#endif
//======================
// ���[�h�̐ݒ�
//======================
void SetMode(MODE mode)
{
	// ���݂̉�ʂ̏I������
	switch (g_mode)
	{
	case MODE_TITLE:		// �^�C�g�����
		UninitTitle();
		break;

	case MODE_TUTORIAL:		// �`���[�g���A�����
		UninitTutorial();
		break;

	case MODE_GAME:			// �Q�[�����
		UninitGame();
		break;

	case MODE_RESULT:		// ���U���g���
		UninitResult();
		break;

	case MODE_EDIT://�G�f�B�b�g���
		UninitEdit();
		break;

	case MODE_RANKING:		// �����L���O���
		UninitRanking();
		break;

	}

	g_mode = mode;			// ��ʐ؂�ւ�

	// �V������ʂ̏���������
	switch (mode)
	{
	case MODE_TITLE:		// �^�C�g�����
		InitTitle();
		break;

	case MODE_TUTORIAL:		// �`���[�g���A�����
		InitTutorial();
		break;

	case MODE_GAME:			// �Q�[�����
		InitGame();
		break;

	case MODE_RESULT:		// ���U���g���
		InitResult();
		break;

	case MODE_EDIT:			// �G�f�B�b�g���
		InitEdit();
		break;

	case MODE_RANKING:		// �����L���O���
		InitRanking();
		break;

	}
}
//======================
//�f�o�C�X�̎擾
//======================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}
//====================
//���[�h�̎擾
//====================
MODE GetMode(void)
{
	return g_mode;//�ϐ���Ԃ�
}
//==============================
// �G�f�B�b�g��ʂ̕\��
//==============================
void DrawEditkey(void)
{
	int ndata = ReturnPass();


	// ���[�J���ϐ�
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

	// ������
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
		strcpy(aStFile, "data/TEXT/SetBlock.txt"); // �����t�@�C��
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

	// ������ɑ��
	wsprintf(&aString1[0], "[ F7 ] : �t�@�C�������o��  ***< %s >*** \n", &aStFile[0]);
	wsprintf(&aString2[0], "�ړ��L�[         [W/A/S/D]\n");
	wsprintf(&aString3[0], "�e�N�X�`���̐؂�ւ��@[��/��]\n");
	wsprintf(&aString4[0], "�����̑���       [ V/C ]\n");
	wsprintf(&aString5[0], "�����̑���       [ Q/E ]\n");
	wsprintf(&aString6[0], "�t�@�C�������o�� [ F7�L�[ ]\n");
	wsprintf(&aString7[0], "�z�u�̌���       [ Enter�L�[ ]\n");
	wsprintf(&aString8[0], "�t�@�C���p�X�؂�ւ�       [ F4�L�[ ]\n");
	wsprintf(&aString9[0], "�ēǂݍ���       [ F9�L�[ ]\n");
	wsprintf(&aString10[0], "�Q�[����ʂ�    [ F3�L�[ ]\n");

	// �e�L�X�g�`��
	g_pFont->DrawText(NULL, &aString1[0], -1, &rect, DT_LEFT,  D3DCOLOR_RGBA(255, 255, 255, 255));		// �ړ��L�[
	g_pFont->DrawText(NULL, &aString2[0], -1, &rect2, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));		// �ړ��L�[
	g_pFont->DrawText(NULL, &aString3[0], -1, &rect3, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));		// �e�N�X�`���؂�ւ�
	g_pFont->DrawText(NULL, &aString4[0], -1, &rect4, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));		// ��������
	g_pFont->DrawText(NULL, &aString5[0], -1, &rect5, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));		// ��������
	g_pFont->DrawText(NULL, &aString6[0], -1, &rect6, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));		// �����o���L�[
	g_pFont->DrawText(NULL, &aString7[0], -1, &rect7, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));		// �z�u�L�[
	g_pFont->DrawText(NULL, &aString8[0], -1, &rect8, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));		// �z�u�L�[
	g_pFont->DrawText(NULL, &aString9[0], -1, &rect9, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));		// �z�u�L�[
	g_pFont->DrawText(NULL, &aString10[0], -1, &rect10, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));	// ���[�h�ύX�L�[

}
