#include "InputControl.hpp"

const InputControl* SingletonBase<InputControl, "InputControl">::m_Singleton = nullptr;

void InputControl::Update() {
	m_PauseEnter.Update(CheckHitKey(KEY_INPUT_TAB));
	m_MenuEnter.Update(CheckHitKey(KEY_INPUT_SPACE));
	m_LMEnter.Update((GetMouseInput() & MOUSE_INPUT_LEFT) != 0);
	//マウス座標を取得しておく
	GetMousePoint(&m_MouseX, &m_MouseY);

	m_MouseX = m_MouseX * FrameWork::Instance()->GetScreenWidth() / FrameWork::Instance()->GetWindowWidth();
	m_MouseY = m_MouseY * FrameWork::Instance()->GetScreenHeight() / FrameWork::Instance()->GetWindowHeight();
	//
	m_WKey.Update(CheckHitKey(KEY_INPUT_W));
	m_AKey.Update(CheckHitKey(KEY_INPUT_A));
	m_SKey.Update(CheckHitKey(KEY_INPUT_S));
	m_DKey.Update(CheckHitKey(KEY_INPUT_D));
	m_MainShotKey.Update(CheckHitKey(KEY_INPUT_SPACE));
	m_SubShotKey.Update((GetMouseInput() & MOUSE_INPUT_LEFT) != 0);
}