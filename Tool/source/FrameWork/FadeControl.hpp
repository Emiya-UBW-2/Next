#pragma once
#include "Define.hpp"

class FadeControl : public SingletonBase<FadeControl, "FadeControl"> {
private:
	friend class SingletonBase<FadeControl, "FadeControl">;
private:
	FadeControl() {
		Init();
	}
	virtual ~FadeControl() {}
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