//=================================
//
// HGS_SPRING ���
// Author:Asuma Nishio
//
//=================================
#ifndef _MAIN_H_	// ���̃}�N����`������ĂȂ�������
#define _MAIN_H_	// 2�d�C���N���[�h�h�~�̃}�N����`

//******************************************
// �C���N���[�h�t�@�C���錾
//******************************************
#include <windows.h>
#include "d3dx9.h"						// �`�揈���ɕK�v
#define DIRECTINPUT_VERSION (0x0800)	// �r���h���x���Ώ��p�̃}�N��
#include "dinput.h"						// ���͏����ɕK�v
#include "Xinput.h"						// �W���C�p�b�h�����ɕK�v
#include "xaudio2.h"

//******************************************
// ���C�u�����̃����N
//******************************************
#pragma comment(lib,"d3d9.lib")			// �`�揈���ɕK�v
#pragma comment(lib,"d3dx9.lib")		// [d3d9.lib]�̊g�����C�u����
#pragma comment(lib,"dxguid.lib")		// DirectX�R���|�[�l���g(���i)�g�p�ɕK�v
#pragma comment(lib,"dinput8.lib")		// ���͏����ɕK�v
#pragma comment(lib,"xinput.lib")		// �W���C�p�b�h�����ɕK�v
#pragma comment(lib,"Winmm.lib")

//******************************************
// �}�N����`
//******************************************
#define CLASS_NAME "WindowClass"		// �E�C���h�E�N���X�̖��O
#define WINDOW_NAME "HGS_2024"			// �E�C���h�E�̖��O(�L���v�V�����ɕ\��)
#define SCREEN_WIDTH (1280)				// �E�C���h�E�̕�
#define SCREEN_HEIGHT (720)				// �E�C���h�E�̍���
#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1) // ���_�t�H�[�}�b�g[2D]

//******************************************
// ��ʂ̎�ނ̗񋓌^
//******************************************
typedef enum
{
	MODE_TITLE = 0,				// �^�C�g��
	MODE_TUTORIAL,				// �`���[�g���A��
	MODE_GAME,					// �Q�[��
	MODE_RESULT,				// ���U���g
	MODE_EDIT,					// �G�f�B�b�g
	MODE_RANKING,				// �����L���O
	MODE_MAX,
}MODE;

//******************************************
// ���_���[2D]�̍\����
//******************************************
typedef struct
{
	D3DXVECTOR3 pos;			// ���_���W
	float rhw;					// ���W�ϊ��p�W��(1.0f�ŌŒ�)
	D3DCOLOR col;				// ���_�J���[
	D3DXVECTOR2 tex;			// �e�N�X�`�����W
}VERTEX_2D;

//******************************************
// �v���g�^�C�v�錾
//******************************************
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);	// �E�B���h�E�v���V�[�W��
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);							// ����������
void Uninit(void);																	// �I������
void Update(void);																	// �X�V����
void Draw(void);																	// �`�揈��
LPDIRECT3DDEVICE9 GetDevice(void);													// �f�o�C�X�̎擾
void SetMode(MODE mode);															// ���[�h�̐ݒ�
MODE GetMode(void);																	// ���[�h�̎擾
void DrawFPS(void);																	// �f�o�b�O�\��
void DrawEnemySet(int g_nNumEnemy);													// �G�̑����̕\��
void ToggleFullscreen(HWND hWnd);													// �E�B���h�E���t���X�N���[���ɂ�����@
void DrawDebugPlayerPos(void);														// �v���C���[�̍��W�̕\��
void DrawEditkey(void);																// �G�f�B�^�[��ʂ̃L�[�\��
void DrawAllBlock(int g_nCntEdit);													// �z�u���̕\��
void DrawEditPos(void);																// �G�f�B�^��ʂ̍��W�\��

#endif