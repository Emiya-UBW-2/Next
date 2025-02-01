#include "FrameWork.hpp"

const FrameWork* FrameWork::m_Singleton = nullptr;

void FrameWork::Init()
{
	SetOutApplicationLogValidFlag(FALSE);
	SetMainWindowText("game title");
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
	SetAlwaysRunFlag(TRUE);
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
	MicroSecondDeltaTime = Now - MicroSecondOnLoopStartFrame;
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
