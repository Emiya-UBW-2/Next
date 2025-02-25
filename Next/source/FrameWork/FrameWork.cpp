#include "FrameWork.hpp"

const FrameWork* SingletonBase<FrameWork, "FrameWork">::m_Singleton = nullptr;

void FrameWork::Init()
{
	SetOutApplicationLogValidFlag(FALSE);
	SetMainWindowText("Starting Game...");
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);
	ChangeWindowMode(TRUE);
	SetUseDirect3DVersion(DX_DIRECT3D_11);
	{
		//�f�t�H���g�𑜓x
		m_WindowWidth = 1280;
		m_WindowHeight = 720;
		// DPI�ݒ�
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
	SetGraphMode(GetWindowWidth(), GetWindowHeight(), 32);		// �𑜓x
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
	SetWindowSetting(WindowSetting::WindowMode);
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
		//�ŏ����ɂ�����̂ł���ȍ~�͂�������X�L�b�v
		if (ProcessMessage() != 0) {
			return false;
		}
		return true;
	}
	if (m_WindowSetting == WindowSetting::WindowMode) {
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
	//�`��
	SetDrawScreen(BackScreen);
	ClearDrawScreen();
	{
		SceneController::Instance()->Draw();
		FadeControl::Instance()->Draw();
	}
	//�`��
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
		//�f�o�b�O�\��
		//clsDx();
		//printfDx("FPS:%4.1f\n", GetFPS());
		//printfDx("%d\n", MenuEnter);
		//printfDx("�f���^�^�C��:%5.2fs\n", GetDeltaTime());
	}
	SetDrawMode(DX_DRAWMODE_NEAREST);
	if ((GetWaitVSyncFlag() == TRUE) != isVsync) {
		SetWaitVSyncFlag(isVsync);
	}
	ScreenFlip();
	//���̃t���[���J�n
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
	DxLib_End();
}
