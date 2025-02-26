#include "Effect.hpp"

#include "Camera.hpp"

const EffectControl* SingletonBase<EffectControl, "EffectControl">::m_Singleton = nullptr;
//
class EffectSmoke : public EffectBase {
	const float MaxTime = 1.f;
private:
	bool IsActiveSub() const override { return (this->Time <= MaxTime); }
	void SetSub() override {}
	void UpdateSub() override {
		this->Time += FrameWork::Instance()->GetDeltaTime();
	}
	void DrawShadowSub() const override {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255.f * std::sin(Mathf::Deg2Rad(this->Time / MaxTime * 180.f))));
		Mathf::Vector3 P1 = MainCamera::Instance()->GetDisplayPoint(this->Pos.x, this->Pos.y, 0.f);
		Mathf::Vector3 P2 = MainCamera::Instance()->GetDisplayPoint(this->PrevPos.x, this->PrevPos.y, 0.f);
		DrawLine(static_cast<int>(P1.x), static_cast<int>(P1.y), static_cast<int>(P2.x), static_cast<int>(P2.y), ColorPalette::Black, static_cast<int>(this->Time / MaxTime * 2));
	}
	void DrawSub() const override {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255.f * std::sin(Mathf::Deg2Rad(this->Time / MaxTime * 180.f))));
		Mathf::Vector3 P1 = MainCamera::Instance()->GetDisplayPoint(this->Pos);
		Mathf::Vector3 P2 = MainCamera::Instance()->GetDisplayPoint(this->PrevPos);
		DrawLine(static_cast<int>(P1.x), static_cast<int>(P1.y), static_cast<int>(P2.x), static_cast<int>(P2.y), ColorPalette::White, static_cast<int>(this->Time / MaxTime * 10));
	}
};
class EffectSonic : public EffectBase {
	const float MaxTime = 3.f;
private:
	bool IsActiveSub() const override { return (this->Time <= MaxTime); }
	void SetSub() override {}
	void UpdateSub() override {
		this->Time += FrameWork::Instance()->GetDeltaTime();
	}
	void DrawShadowSub() const override {}
	void DrawSub() const override {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(32.f / this->Time));
		Mathf::Vector3 P1 = MainCamera::Instance()->GetDisplayPoint(this->Pos);

		double Rate = static_cast<double>(static_cast<float>(FrameWork::Instance()->GetScreenWidth()) / 960.f);
		DrawRotaGraph3(static_cast<int>(P1.x), static_cast<int>(P1.y), 128 / 2, 128 / 2, double(this->Time * 640 / 128) * Rate, double(this->Time / 2 * 640 / 128) * Rate, double(Mathf::Deg2Rad(30)), m_Screen.GetHandle(), TRUE);
	}
};
class EffectDeath : public EffectBase {
	const float MaxTime = 3.f;
private:
	bool IsActiveSub() const override { return (this->Time <= MaxTime); }
	void SetSub() override {}
	void UpdateSub() override {
		this->Time += FrameWork::Instance()->GetDeltaTime();
	}
	void DrawShadowSub() const override {}
	void DrawSub() const override {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(32.f / this->Time));
		Mathf::Vector3 P1 = MainCamera::Instance()->GetDisplayPoint(this->Pos);
		double Rate = static_cast<double>(static_cast<float>(FrameWork::Instance()->GetScreenWidth()) / 960.f);
		DrawRotaGraph3(static_cast<int>(P1.x), static_cast<int>(P1.y), 128 / 2, 128 / 2, double(this->Time * 640 / 128) * Rate, double(this->Time * 0.8f * 640 / 128) * Rate, double(Mathf::Deg2Rad(30)), m_Screen.GetHandle(), TRUE);
	}
};
class EffectHit : public EffectBase {
	float Rad{};
	const float MaxTime = 0.5f;
private:
	bool IsActiveSub() const override { return (this->Time <= MaxTime); }
	void SetSub() override {
		this->Rad = Mathf::Deg2Rad(static_cast<float>(-90 + GetRand(180)));
	}
	void UpdateSub() override {
		this->Time += FrameWork::Instance()->GetDeltaTime();
		this->Rad += Mathf::Deg2Rad(this->Rad * FrameWork::Instance()->GetDeltaTime() * 3.f * 180.f);
	}
	void DrawShadowSub() const override {}
	void DrawSub() const override {
		double Rate = static_cast<double>(static_cast<float>(FrameWork::Instance()->GetScreenWidth()) / 960.f);
		float alpha = std::sin(Mathf::Deg2Rad(this->Time / MaxTime * 180.f));
		float scale = this->Time / MaxTime * 0.15f * 640 / 128 * Rate;
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255.f * alpha));
		Mathf::Vector3 P1 = MainCamera::Instance()->GetDisplayPoint(this->Pos);
		DrawRotaGraph3(static_cast<int>(P1.x), static_cast<int>(P1.y), 128 / 2, 128 / 2, double(scale), double(scale), double(this->Rad), m_Screen.GetHandle(), TRUE);
	}
};
//
void EffectControl::SetEffect(int Type, const Mathf::Vector3& Pos, const Mathf::Vector3& PrevPos) {
	switch ((EnumEffect)Type) {
	case EnumEffect::Smoke:
		m_Pool.emplace_back(std::make_unique<EffectSmoke>());
		m_Pool.back()->Init(Pos, PrevPos, m_EffectGraph.at(1));
		break;
	case EnumEffect::Sonic:
		m_Pool.emplace_back(std::make_unique<EffectSonic>());
		m_Pool.back()->Init(Pos, PrevPos, m_EffectGraph.at(1));
		break;
	case EnumEffect::Death:
		m_Pool.emplace_back(std::make_unique<EffectDeath>());
		m_Pool.back()->Init(Pos, PrevPos, m_EffectGraph.at(1));
		break;
	case EnumEffect::Hit:
		m_Pool.emplace_back(std::make_unique<EffectHit>());
		m_Pool.back()->Init(Pos, PrevPos, m_EffectGraph.at(0));
		break;
	default:
		break;
	}
}