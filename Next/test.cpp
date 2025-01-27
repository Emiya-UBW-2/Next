#define NOMINMAX

#include "DxLib.h"

static const int draw_x = 960;			//�E�B���h�E�T�C�YX
static const int draw_y = 720;			//�E�B���h�E�T�C�YY

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	SetOutApplicationLogValidFlag(FALSE);           /*log*/
	SetMainWindowText("game title");                /*�^�C�g��*/
	ChangeWindowMode(TRUE);                         /*���\��*/
	SetUseDirect3DVersion(DX_DIRECT3D_11);          /*directX ver*/
	SetGraphMode(draw_x, draw_y, 32);               /*�𑜓x*/
	SetUseDirectInputFlag(TRUE);                    /*DirectInput�g�p*/
	SetDirectInputMouseMode(TRUE);                  /*DirectInput�}�E�X�g�p*/
	SetWindowSizeChangeEnableFlag(FALSE, FALSE);    /*�E�C���h�E�T�C�Y���蓮�ύX�s�A�E�C���h�E�T�C�Y�ɍ��킹�Ċg������Ȃ��悤�ɂ���*/
	SetUsePixelLighting(TRUE);                      /*�s�N�Z�����C�e�B���O�̎g�p*/
	SetFullSceneAntiAliasingMode(4, 2);             /*�A���`�G�C���A�X*/
	SetWaitVSyncFlag(TRUE);                         /*��������*/
	DxLib_Init();                                   /*�c�w���C�u��������������*/
	SetSysCommandOffFlag(TRUE);                     /*�^�X�N�X�C�b�`��L���ɂ��邩�ǂ�����ݒ肷��*/
	SetAlwaysRunFlag(TRUE);                         /*background*/

	//���C�����[�v�J�n
	while (ProcessMessage() == 0) {
		LONGLONG WaitTime = GetNowHiPerformanceCount();
		//�`��
		SetDrawScreen(DX_SCREEN_BACK);
		ClearDrawScreen();
		{
			//�f�o�b�O�\��
			clsDx();
			printfDx("FPS:%4.1f\n", GetFPS());
			printfDx("��������:%5.2fms\n", (float)(GetNowHiPerformanceCount() - WaitTime) / 1000.f);
		}
		ScreenFlip();
	}
	DxLib_End();
	return 0;// �\�t�g�̏I�� 
}
