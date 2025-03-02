#pragma once

#include "Define.hpp"

class Switch {
	int m_PressBuffer = 0;
public:
	bool IsTrigger() const { return m_PressBuffer == 1; }
	bool IsPress() const { return m_PressBuffer != 0; }
public:
	void Init() {
		m_PressBuffer = 0;
	}
	void Update(bool value) {
		m_PressBuffer = static_cast<int>(Mathf::Clamp(static_cast<float>(m_PressBuffer + (value ? 1 : -2)), 0.f, 2.f));
	}
}; 

class InputControl : public SingletonBase<InputControl,"InputControl"> {
private:
	friend class SingletonBase<InputControl, "InputControl">;
private:
	InputControl() {
		Init();
	}
	virtual ~InputControl() {}
private:
	InputControl(const InputControl&) = delete;
	InputControl& operator=(const InputControl&) = delete;
	InputControl(InputControl&&) = delete;
	InputControl& operator=(InputControl&&) = delete;
private:
	//
	Switch m_PauseEnter;
	Switch m_LMEnter;
	int m_MouseX{};
	int m_MouseY{};
	int m_MouseWheel{};
public:
	const Switch& GetPauseEnter() const { return m_PauseEnter; }
	const Switch& GetLMEnter() const { return m_LMEnter; }
	const int GetMouseX() const { return m_MouseX; }
	const int GetMouseY() const { return m_MouseY; }
	const int GetMouseWheel() const { return m_MouseWheel; }
public:
	void Init() {
		m_PauseEnter.Init();
		m_LMEnter.Init();
		m_MouseX = 0;
		m_MouseY = 0;
		m_MouseWheel = 0;
	}
	void Update();
};

extern bool IntoMouse(int x1, int y1, int x2, int y2);