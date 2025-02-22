#include "FadeControl.hpp"
#include "Define.hpp"

const FadeControl* FadeControl::m_Singleton = nullptr;

void FadeControl::Update() {
	m_FadeTimer = Mathf::Clamp(m_FadeTimer + FrameWork::Instance()->GetDeltaTime(), 0.f, m_FadeTimeMax);
}

void FadeControl::Draw() const {
	int Alpha = 0;
	if (m_IsFadeIn) {
		Alpha = static_cast<int>(255 * m_FadeTimer / m_FadeTimeMax);
	}
	else {
		Alpha = static_cast<int>(255 * (1.f - m_FadeTimer / m_FadeTimeMax));
	}
	if (Alpha > 0) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, Alpha);
		DrawBox(0, 0, FrameWork::Instance()->GetScreenWidth(), FrameWork::Instance()->GetScreenHeight(), m_Color, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}
}