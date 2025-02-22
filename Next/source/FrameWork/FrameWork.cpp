#include "FrameWork.hpp"

const FrameWork* SingletonBase<FrameWork, "FrameWork">::m_Singleton = nullptr;

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
	//
	InputControl::Create();
	FadeControl::Create();
	//
	BackScreen = MakeScreen(m_ScreenWidth, m_ScreenHeight, FALSE);
	//
	_SceneController = new SceneController;
	_SceneController->Init();

	m_IsPauseActive = false;
}
bool FrameWork::Update()
{
	InputControl::Instance()->Update();
	bool isUpdate = true;
	if (m_IsPauseEnable) {
		if (InputControl::Instance()->GetPauseEnter().IsTrigger()) {
			m_IsPauseActive ^= 1;
		}
		isUpdate = !m_IsPauseActive;
	}
	else {
		m_IsPauseActive = false;
	}
	if (isUpdate) {
		FadeControl::Instance()->Update();
		_SceneController->Update();
	}
	//描画
	SetDrawScreen(BackScreen);
	ClearDrawScreen();
	{
		_SceneController->Draw();
		FadeControl::Instance()->Draw();
	}
	//描画
	SetDrawScreen(DX_SCREEN_BACK);
	ClearDrawScreen();
	{
		DrawBox(0, 0, GetWindowWidth(), GetWindowHeight(), ColorPalette::Gray085, TRUE);

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
	_SceneController->Dispose();
	InputControl::Release();
	FadeControl::Release();
	delete _SceneController;
	DxLib_End();
}
