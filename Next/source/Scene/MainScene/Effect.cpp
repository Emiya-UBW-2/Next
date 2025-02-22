#include "Effect.hpp"

#include "Camera.hpp"

const EffectControl* SingletonBase<EffectControl, "EffectControl">::m_Singleton = nullptr;
//
bool EffectControl::EffectPositionData::IsActive() const {
	switch ((EnumEffect)EffectType) {
	case EnumEffect::Smoke:
		return (this->Size <= 3.f);
	case EnumEffect::Sonic:
		return (this->Size <= 3.f);
		break;
	case EnumEffect::Death:
		return (this->Size <= 3.f);
		break;
	case EnumEffect::Hit:
		return (this->Size <= 0.5f);
		break;
	default:
		break;
	}
	return false;
}
void EffectControl::EffectPositionData::Set(int Type, const Mathf::Vector3& Pos, const Mathf::Vector3& PrevPos) {
	this->EffectType = Type;
	switch ((EnumEffect)EffectType) {
	case EnumEffect::Smoke:
	{
		this->PrevPos = PrevPos;
		this->Pos = Pos;
		this->Size = 0.f;
	}
	break;
	case EnumEffect::Sonic:
	{
		this->Pos = Pos;
		this->Size = 0.f;
	}
	break;
	case EnumEffect::Death:
	{
		this->Pos = Pos;
		this->Size = 0.f;
	}
	break;
	case EnumEffect::Hit:
	{
		this->Pos = Pos;
		this->Size = 0.f;
		this->Rad = Mathf::Deg2Rad(static_cast<float>(-90 + GetRand(180)));
	}
	break;
	default:
		break;
	}
}
void EffectControl::EffectPositionData::Update() {
	this->Size += FrameWork::Instance()->GetDeltaTime();
}
void EffectControl::EffectPositionData::DrawShadow() const {
	switch ((EnumEffect)EffectType) {
	case EnumEffect::Smoke:
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255.f * std::sin(Mathf::Deg2Rad(this->Size * 180.f))));
		Mathf::Vector3 P1 = MainCamera::Instance()->GetDisplayPoint(this->Pos.x, this->Pos.y, 0.f);
		Mathf::Vector3 P2 = MainCamera::Instance()->GetDisplayPoint(this->PrevPos.x, this->PrevPos.y, 0.f);
		DrawLine(static_cast<int>(P1.x), static_cast<int>(P1.y), static_cast<int>(P2.x), static_cast<int>(P2.y), ColorPalette::Black, static_cast<int>(this->Size * 2));
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}
	break;
	case EnumEffect::Sonic:
		break;
	case EnumEffect::Death:
		break;
	case EnumEffect::Hit:
		break;
	default:
		break;
	}
}
void EffectControl::EffectPositionData::Draw() const {
	switch ((EnumEffect)EffectType) {
	case EnumEffect::Smoke:
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255.f * std::sin(Mathf::Deg2Rad(this->Size * 180.f))));
		Mathf::Vector3 P1 = MainCamera::Instance()->GetDisplayPoint(this->Pos);
		Mathf::Vector3 P2 = MainCamera::Instance()->GetDisplayPoint(this->PrevPos);
		DrawLine(static_cast<int>(P1.x), static_cast<int>(P1.y), static_cast<int>(P2.x), static_cast<int>(P2.y), ColorPalette::White, static_cast<int>(this->Size * 10));
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}
	break;
	case EnumEffect::Sonic:
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(32.f / this->Size));
		Mathf::Vector3 P1 = MainCamera::Instance()->GetDisplayPoint(this->Pos);
		DrawRotaGraph3(static_cast<int>(P1.x), static_cast<int>(P1.y), 640 / 2, 640 / 2, double(this->Size), double(this->Size / 2), double(Mathf::Deg2Rad(30)), m_Screen.GetHandle(), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}
	break;
	case EnumEffect::Death:
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(32.f / this->Size));
		Mathf::Vector3 P1 = MainCamera::Instance()->GetDisplayPoint(this->Pos);
		DrawRotaGraph3(static_cast<int>(P1.x), static_cast<int>(P1.y), 640 / 2, 640 / 2, double(this->Size), double(this->Size * 0.8f), double(Mathf::Deg2Rad(30)), m_Screen.GetHandle(), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}
	break;
	case EnumEffect::Hit:
	{
		float alpha = std::sin(Mathf::Deg2Rad(this->Size / 0.5f * 180.f));
		float scale = this->Size * 0.15f / 0.5f;// std::sin(Mathf::Deg2Rad(this->Size / 0.5f * 180.f)) * 0.25f;
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255.f * alpha));
		Mathf::Vector3 P1 = MainCamera::Instance()->GetDisplayPoint(this->Pos);
		DrawRotaGraph3(static_cast<int>(P1.x), static_cast<int>(P1.y), 640 / 2, 640 / 2, double(scale), double(scale), double(this->Rad + Mathf::Deg2Rad(30 + this->Rad * this->Size * 3.f * 180.f)), m_Screen2.GetHandle(), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}
	break;
	default:
		break;
	}
}
