#include "FrameWork.hpp"

const FrameWork* FrameWork::m_Singleton = nullptr;

void FrameWork::Init()
{
	SetOutApplicationLogValidFlag(FALSE);
	SetMainWindowText("TriMono");
	ChangeWindowMode(TRUE);
	SetUseDirect3DVersion(DX_DIRECT3D_11);
	SetGraphMode(GetWindowWidth(), GetWindowHeight(), 32);
	SetUseDirectInputFlag(TRUE);
	SetDirectInputMouseMode(TRUE);
	SetWindowSizeChangeEnableFlag(FALSE, FALSE);
	SetUsePixelLighting(TRUE);
	SetFullSceneAntiAliasingMode(4, 2);
	DxLib_Init();
	SetSysCommandOffFlag(TRUE);
	SetAlwaysRunFlag(FALSE);
	MicroSecondOnLoopStartFrame = GetNowHiPerformanceCount();
}
bool FrameWork::Flip()
{
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
