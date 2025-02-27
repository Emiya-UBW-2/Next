#include "FrameWork.hpp"

const FrameWork* SingletonBase<FrameWork, "FrameWork">::m_Singleton = nullptr;

void FrameWork::UpdateWindowSetting() {
	switch (SaveData::Instance()->GetWindowSetting()) {
	case WindowSetting::WindowMode:
		m_WindowWidth = m_WindowModeWidth;
		m_WindowHeight = m_WindowModeHeight;
		ChangeWindowMode(TRUE);
		SetWindowSize(m_WindowWidth, m_WindowHeight);
		SetWindowStyleMode(0);
		{
			// DPI設定
			int DPI = 96;
			GetMonitorDpi(NULL, &DPI);
			if (DPI == 0) {
				DPI = 96;
			}
			// DPIを反映するデスクトップサイズ
			int DispXSize = static_cast<int>(GetSystemMetrics(SM_CXSCREEN)) * 96 / DPI;
			int DispYSize = static_cast<int>(GetSystemMetrics(SM_CYSCREEN)) * 96 / DPI;
			SetWindowPosition((DispXSize - m_WindowWidth) / 2, (DispYSize - m_WindowHeight) / 2);
		}
		SetWindowSizeExtendRate(1.0, 1.0);
		break;
	case WindowSetting::BorderLess:
		m_WindowWidth = m_FullScreenModeWidth;
		m_WindowHeight = m_FullScreenModeHeight;
		ChangeWindowMode(TRUE);
		SetWindowStyleMode(2);
		SetWindowPosition(0, 0);
		{
			// DPI設定
			int DPI = 96;
			GetMonitorDpi(NULL, &DPI);
			if (DPI == 0) {
				DPI = 96;
			}
			// DPIを反映するデスクトップサイズ
			int DispXSize = static_cast<int>(GetSystemMetrics(SM_CXSCREEN)) * 96 / DPI;
			int DispYSize = static_cast<int>(GetSystemMetrics(SM_CYSCREEN)) * 96 / DPI;
			SetWindowSizeExtendRate(static_cast<double>(DispXSize) / static_cast<double>(m_WindowWidth), static_cast<double>(DispYSize) / static_cast<double>(m_WindowHeight));
		}
		break;
	case WindowSetting::FullScreen:
		m_WindowWidth = m_FullScreenModeWidth;
		m_WindowHeight = m_FullScreenModeHeight;
		SetFullScreenResolutionMode(DX_FSRESOLUTIONMODE_NATIVE);
		SetFullScreenScalingMode(DX_FSSCALINGMODE_NEAREST);
		ChangeWindowMode(FALSE);
		SetWindowStyleMode(2);
		SetWindowPosition(0, 0);
		SetWindowSizeExtendRate(1.0, 1.0);
		break;
	default:
		break;
	}
}

void FrameWork::Init()
{
	SaveData::Create();
	SetOutApplicationLogValidFlag(FALSE);
	SetMainWindowText("Starting Game...");
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);
	ChangeWindowMode(TRUE);
	SetUseDirect3DVersion(DX_DIRECT3D_11);
	{
		//デフォルト解像度
		m_WindowWidth = 1280;
		m_WindowHeight = 720;
		//スクリーン解像度
		m_ScreenWidth = 960;
		m_ScreenHeight = 720;
		// DPI設定
		int DPI = 96;
		GetMonitorDpi(NULL, &DPI);
		if (DPI == 0) {
			DPI = 96;
		}
		m_WindowWidth = m_WindowWidth * 96 / DPI;
		m_WindowHeight = m_WindowHeight * 96 / DPI;

		m_FullScreenModeWidth = m_WindowWidth;
		m_FullScreenModeHeight = m_WindowHeight;
		m_WindowModeWidth = m_WindowWidth;
		m_WindowModeHeight = m_WindowHeight;
	}
	SetGraphMode(GetWindowWidth(), GetWindowHeight(), 32);		// 解像度
	SetUseDirectInputFlag(TRUE);
	SetDirectInputMouseMode(TRUE);
	//SetWindowSizeChangeEnableFlag(FALSE, FALSE);
	DxLib_Init();
	SetSysCommandOffFlag(TRUE);
	SetAlwaysRunFlag(TRUE);
	MicroSecondOnLoopStartFrame = GetNowHiPerformanceCount();
	//
	BackScreen = MakeScreen(m_ScreenWidth, m_ScreenHeight, FALSE);
	//
	SetPauseActive(false);
	UpdateWindowSetting();
	SetMainWindowText("Game");
	//
	InputControl::Create();
	FadeControl::Create();
	SceneController::Create();
}
bool FrameWork::Update()
{
	int Width, Height;
	GetWindowSize(&Width, &Height);
	if (Width <= 0 || Height <= 0) {
		//最小化にあたるのでこれ以降はいったんスキップ
		if (ProcessMessage() != 0) {
			return false;
		}
		return true;
	}
	if (SaveData::Instance()->GetWindowSetting() == WindowSetting::WindowMode) {
		m_WindowWidth = Width;
		m_WindowHeight = Height;
		m_WindowModeWidth = m_WindowWidth;
		m_WindowModeHeight = m_WindowHeight;
	}
	InputControl::Instance()->Update();
	if (m_IsPauseEnable) {
		if (InputControl::Instance()->GetPauseEnter().IsTrigger()) {
			m_IsPauseActive ^= 1;
			SetPauseActive(m_IsPauseActive);
		}
	}
	else {
		SetPauseActive(false);
	}
	FadeControl::Instance()->Update();
	SceneController::Instance()->Update();
	//描画
	SetDrawScreen(BackScreen);
	ClearDrawScreen();
	{
		SceneController::Instance()->Draw();
		FadeControl::Instance()->Draw();
	}
	//描画
	SetDrawMode(DX_DRAWMODE_NEAREST);
	SetDrawScreen(DX_SCREEN_BACK);
	ClearDrawScreen();
	{
		DrawBox(0, 0, GetWindowWidth(), GetWindowHeight(), ColorPalette::Gray085, TRUE);

		double rate = static_cast<double>(Mathf::Min(
			static_cast<float>(GetWindowHeight()) / static_cast<float>(m_ScreenHeight),
			static_cast<float>(GetWindowWidth()) / static_cast<float>(m_ScreenWidth)
		));
		DrawRotaGraph(GetWindowWidth() / 2, GetWindowHeight()/2, rate ,0.0, BackScreen, FALSE);
		//デバッグ表示
		clsDx();
		//printfDx("FPS:%4.1f\n", GetFPS());
		//printfDx("デルタタイム:%5.2fs\n", GetDeltaTime());
	}
	SetDrawMode(DX_DRAWMODE_NEAREST);
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
	SceneController::Release();
	InputControl::Release();
	FadeControl::Release();
	SaveData::Release();
	DxLib_End();
}
