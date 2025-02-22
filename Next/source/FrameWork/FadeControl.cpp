#include "FadeControl.hpp"
#include "Define.hpp"

const FadeControl* FadeControl::m_Singleton = nullptr;

void FadeControl::Update() {
	m_FadeTimer = Mathf::Clamp(m_FadeTimer + FrameWork::Instance()->GetDeltaTime(), 0.f, 1.f);
}

void FadeControl::Draw() {
	int Alpha = 0;
	if (m_IsFadeIn) {
		Alpha = static_cast<int>(255 * m_FadeTimer);
	}
	else {
		Alpha = static_cast<int>(255 * (1.f - m_FadeTimer));
	}
	if (Alpha > 0) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, Alpha);
		DrawBox(0, 0, FrameWork::Instance()->GetScreenWidth(), FrameWork::Instance()->GetScreenHeight(), m_Color, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}
}