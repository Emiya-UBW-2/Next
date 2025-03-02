#include "InputControl.hpp"

const InputControl* SingletonBase<InputControl, "InputControl">::m_Singleton = nullptr;

void InputControl::Update() {
	m_PauseEnter.Update(CheckHitKey(KEY_INPUT_TAB));
	m_LMEnter.Update((GetMouseInput() & MOUSE_INPUT_LEFT) != 0);
	//マウス座標を取得しておく
	GetMousePoint(&m_MouseX, &m_MouseY);
	SetMouseDispFlag(TRUE);
	m_MouseX = FrameWork::Instance()->GetScreenWidth() / 2 + ((m_MouseX - FrameWork::Instance()->GetWindowWidth() / 2) * FrameWork::Instance()->GetScreenHeight() / FrameWork::Instance()->GetWindowHeight());
	m_MouseY = FrameWork::Instance()->GetScreenHeight() / 2 + ((m_MouseY - FrameWork::Instance()->GetWindowHeight() / 2) * FrameWork::Instance()->GetScreenHeight() / FrameWork::Instance()->GetWindowHeight());
	m_MouseWheel = GetMouseWheelRotVol(TRUE);
}

bool IntoMouse(int x1, int y1, int x2, int y2) {
	return Mathf::HitPointToRectangle(InputControl::Instance()->GetMouseX(), InputControl::Instance()->GetMouseY(), x1, y1, x2, y2);
}

