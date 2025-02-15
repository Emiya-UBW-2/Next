#include "InputControl.hpp"

const InputControl* InputControl::m_Singleton = nullptr;

void InputControl::Update() {
	m_MenuEnter.Update(CheckHitKey(KEY_INPUT_SPACE));
	m_LMEnter.Update((GetMouseInput() & MOUSE_INPUT_LEFT) != 0);
	//マウス座標を取得しておく
	GetMousePoint(&m_MouseX, &m_MouseY);

	m_MouseX = m_MouseX * FrameWork::Instance()->GetScreenWidth() / FrameWork::Instance()->GetWindowWidth();
	m_MouseY = m_MouseY * FrameWork::Instance()->GetScreenHeight() / FrameWork::Instance()->GetWindowHeight();
}