#pragma once
#include "DxLib.h"

class FadeControl {
private:
	static const FadeControl* m_Singleton;
public:
	static void Create(void) noexcept {
		m_Singleton = new FadeControl();
	}
	static void Release(void) noexcept {
		delete m_Singleton;
	}
	static FadeControl* Instance(void) noexcept {
		if (m_Singleton == nullptr) {
			MessageBox(NULL, "Failed FadeControl Instance Create", "", MB_OK);
			exit(-1);
		}
		// if (m_Singleton == nullptr) { m_Singleton = new FadeControl(); }
		return (FadeControl*)m_Singleton;
	}
private:
	FadeControl() {
		Init();
	}
	~FadeControl() {
	}
private:
	FadeControl(const FadeControl&) = delete;
	FadeControl& operator=(const FadeControl&) = delete;
	FadeControl(FadeControl&&) = delete;
	FadeControl& operator=(FadeControl&&) = delete;
private:
	float m_FadeTimer = 0.f;
	bool m_IsFadeIn{ true };
	float m_FadeTimeMax = 1.f;
	unsigned int m_Color{};
public:
	void SetFadeIn(unsigned int Color, float FadeTime) {
		m_FadeTimer = 0.f;
		m_FadeTimeMax = FadeTime;
		m_IsFadeIn = true;
		m_Color = Color;
	}
	void SetFadeOut(unsigned int Color, float FadeTime) {
		m_FadeTimer = 0.f;
		m_FadeTimeMax = FadeTime;
		m_IsFadeIn = false;
		m_Color = Color;
	}
	bool IsFading() const {
		return (0 < m_FadeTimer && m_FadeTimer < m_FadeTimeMax);
	}
public:
	void Init() {
		m_FadeTimer = m_FadeTimeMax = 1.f;
		m_IsFadeIn = true;
	}
	void Update();
	void Draw() const;
};