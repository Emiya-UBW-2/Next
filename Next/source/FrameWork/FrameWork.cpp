#include "FrameWork.hpp"


bool IsTriMonoMode = false;

const FrameWork* FrameWork::m_Singleton = nullptr;

void FrameWork::Init()
{
	SetOutApplicationLogValidFlag(FALSE);
	SetMainWindowText("TriMono");
	ChangeWindowMode(TRUE);
	SetUseDirect3DVersion(DX_DIRECT3D_11);
	{
		// DPI設定
		int DPI = 96;
		GetMonitorDpi(NULL, &DPI);
		if (DPI == 0) {
			DPI = 96;
		}
		m_WindowWidth = m_WindowWidth * 96 / DPI;
		m_WindowHeight = m_WindowHeight * 96 / DPI;
	}
	SetGraphMode(GetWindowWidth(), GetWindowHeight(), 32);		// 解像度
	SetUseDirectInputFlag(TRUE);
	SetDirectInputMouseMode(TRUE);
	SetWindowSizeChangeEnableFlag(FALSE, FALSE);
	SetUsePixelLighting(TRUE);
	SetFullSceneAntiAliasingMode(4, 2);
	DxLib_Init();
	SetSysCommandOffFlag(TRUE);
	SetAlwaysRunFlag(FALSE);
	MicroSecondOnLoopStartFrame = GetNowHiPerformanceCount();

	BackScreen = MakeScreen(m_ScreenWidth, m_ScreenHeight, FALSE);
}
bool FrameWork::Flip()
{
	//描画
	SetDrawScreen(DX_SCREEN_BACK);
	ClearDrawScreen();
	{
		DrawBox(0, 0, GetWindowWidth(), GetWindowHeight(), GetColor(32, 32, 32), TRUE);

		int Width = GetWindowHeight() * m_ScreenWidth / m_ScreenHeight;
		DrawExtendGraph(GetWindowWidth() / 2 - Width / 2, 0, GetWindowWidth() / 2 + Width / 2, GetWindowHeight(), BackScreen, FALSE);
		//デバッグ表示
		//clsDx();
		//printfDx("FPS:%4.1f\n", GetFPS());
		//printfDx("%d\n", MenuEnter);
		//printfDx("デルタタイム:%5.2fs\n", GetDeltaTime());
	}

	if ((GetWaitVSyncFlag() == TRUE) != isVsync) {
		SetWaitVSyncFlag(isVsync);
	}
	ScreenFlip();
	//次のフレーム開始
	auto Now = GetNowHiPerformanceCount();
	if ((Now - MicroSecondOnLoopStartFrame) > 1000 * 1000 / 60) {
		MicroSecondDeltaTime = 1000 * 1000 / 60;
	}
	else {
		MicroSecondDeltaTime = Now - MicroSecondOnLoopStartFrame;
	}
	MicroSecondOnLoopStartFrame = Now;
	if (ProcessMessage() != 0) {
		return false;
	}
	return true;
}
void FrameWork::Dispose()
{
	DxLib_End();
}
