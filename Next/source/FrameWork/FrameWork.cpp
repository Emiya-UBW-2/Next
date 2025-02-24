#include "FrameWork.hpp"

const FrameWork* SingletonBase<FrameWork, "FrameWork">::m_Singleton = nullptr;

void FrameWork::Init()
{
	SetOutApplicationLogValidFlag(FALSE);
	SetMainWindowText("TriMono");
	ChangeWindowMode(TRUE);
	SetUseDirect3DVersion(DX_DIRECT3D_11);
	{
		// DPI�ݒ�
		int DPI = 96;
		GetMonitorDpi(NULL, &DPI);
		if (DPI == 0) {
			DPI = 96;
		}
		m_WindowWidth = m_WindowWidth * 96 / DPI;
		m_WindowHeight = m_WindowHeight * 96 / DPI;
	}
	SetGraphMode(GetWindowWidth(), GetWindowHeight(), 32);		// �𑜓x
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
	SceneController::Create();

	SetPauseActive(false);
	m_TimeScale = 1.f;
}
bool FrameWork::Update()
{
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
	SetDrawScreen(DX_SCREEN_BACK);
	ClearDrawScreen();
	{
		DrawBox(0, 0, GetWindowWidth(), GetWindowHeight(), ColorPalette::Gray085, TRUE);

		int Width = GetWindowHeight() * m_ScreenWidth / m_ScreenHeight;
		DrawExtendGraph(GetWindowWidth() / 2 - Width / 2, 0, GetWindowWidth() / 2 + Width / 2, GetWindowHeight(), BackScreen, FALSE);
		//�f�o�b�O�\��
		//clsDx();
		//printfDx("FPS:%4.1f\n", GetFPS());
		//printfDx("%d\n", MenuEnter);
		//printfDx("�f���^�^�C��:%5.2fs\n", GetDeltaTime());
	}

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
