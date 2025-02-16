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
	//
	Switch m_MenuEnter;
	Switch m_LMEnter;
	int m_MouseX{};
	int m_MouseY{};
	//
	Switch m_WKey;
	Switch m_AKey;
	Switch m_SKey;
	Switch m_DKey;
	Switch m_MainShotKey;
	Switch m_SubShotKey;
public:
	const Switch& GetMenuEnter() const { return m_MenuEnter; }
	const Switch& GetLMEnter() const { return m_LMEnter; }
	const int GetMouseX() const { return m_MouseX; }
	const int GetMouseY() const { return m_MouseY; }
	//
	const Switch& GetWKey() const { return m_WKey; }
	const Switch& GetAKey() const { return m_AKey; }
	const Switch& GetSKey() const { return m_SKey; }
	const Switch& GetDKey() const { return m_DKey; }
	const Switch& GetMainShotKey() const { return m_MainShotKey; }
	const Switch& GetSubShotKey() const { return m_SubShotKey; }
public:
	void Init() {
		m_MenuEnter.Init();
		m_LMEnter.Init();
		m_MouseX = 0;
		m_MouseY = 0;
		m_WKey.Init();
		m_AKey.Init();
		m_SKey.Init();
		m_DKey.Init();
		m_MainShotKey.Init();
		m_SubShotKey.Init();
	}
	void Update();
};
