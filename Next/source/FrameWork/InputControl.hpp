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

class InputControl {
private:
	static const InputControl* m_Singleton;
public:
	static void Create(void) noexcept {
		m_Singleton = new InputControl();
	}
	static void Release(void) noexcept {
		delete m_Singleton;
	}
	static InputControl* Instance(void) noexcept {
		if (m_Singleton == nullptr) {
			MessageBox(NULL, "Failed InputControl Instance Create", "", MB_OK);
			exit(-1);
		}
		// if (m_Singleton == nullptr) { m_Singleton = new InputControl(); }
		return (InputControl*)m_Singleton;
	}
private:
	InputControl() {
		Init();
	}
	~InputControl() {
	}
private:
	InputControl(const InputControl&) = delete;
	InputControl& operator=(const InputControl&) = delete;
	InputControl(InputControl&&) = delete;
	InputControl& operator=(InputControl&&) = delete;
private:
	Switch MenuEnter;
	Switch LMEnter;
public:
	const Switch& GetMenuEnter() const { return MenuEnter; }
	const Switch& GetLMEnter() const { return LMEnter; }
public:
	void Init() {
		MenuEnter.Init();
		LMEnter.Init();
	}
	void Update() {
		MenuEnter.Update(CheckHitKey(KEY_INPUT_SPACE));
		LMEnter.Update((GetMouseInput() & MOUSE_INPUT_LEFT) != 0);
	}
};
