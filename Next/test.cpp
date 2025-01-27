#define NOMINMAX

#include "DxLib.h"

static const int draw_x = 960;			//ウィンドウサイズX
static const int draw_y = 720;			//ウィンドウサイズY

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	SetOutApplicationLogValidFlag(FALSE);           /*log*/
	SetMainWindowText("game title");                /*タイトル*/
	ChangeWindowMode(TRUE);                         /*窓表示*/
	SetUseDirect3DVersion(DX_DIRECT3D_11);          /*directX ver*/
	SetGraphMode(draw_x, draw_y, 32);               /*解像度*/
	SetUseDirectInputFlag(TRUE);                    /*DirectInput使用*/
	SetDirectInputMouseMode(TRUE);                  /*DirectInputマウス使用*/
	SetWindowSizeChangeEnableFlag(FALSE, FALSE);    /*ウインドウサイズを手動変更不可、ウインドウサイズに合わせて拡大もしないようにする*/
	SetUsePixelLighting(TRUE);                      /*ピクセルライティングの使用*/
	SetFullSceneAntiAliasingMode(4, 2);             /*アンチエイリアス*/
	SetWaitVSyncFlag(TRUE);                         /*垂直同期*/
	DxLib_Init();                                   /*ＤＸライブラリ初期化処理*/
	SetSysCommandOffFlag(TRUE);                     /*タスクスイッチを有効にするかどうかを設定する*/
	SetAlwaysRunFlag(TRUE);                         /*background*/

	//メインループ開始
	while (ProcessMessage() == 0) {
		LONGLONG WaitTime = GetNowHiPerformanceCount();
		//描画
		SetDrawScreen(DX_SCREEN_BACK);
		ClearDrawScreen();
		{
			//デバッグ表示
			clsDx();
			printfDx("FPS:%4.1f\n", GetFPS());
			printfDx("処理時間:%5.2fms\n", (float)(GetNowHiPerformanceCount() - WaitTime) / 1000.f);
		}
		ScreenFlip();
	}
	DxLib_End();
	return 0;// ソフトの終了 
}
