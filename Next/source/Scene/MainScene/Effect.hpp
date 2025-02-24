#pragma once

#include "../../FrameWork/Define.hpp"
#include <vector>

enum class EnumEffect {
	Smoke,
	Sonic,
	Death,
	Hit,
};

class EffectBase {
protected:
	Mathf::Vector3 Pos{};
	Mathf::Vector3 PrevPos{};
	float Time{};
	float Alpha{};

	GraphHandle m_Screen;
	GraphHandle m_Screen2;//TODO
protected:
	virtual bool IsActiveSub() const = 0;
	virtual void SetSub() = 0;
	virtual void UpdateSub() = 0;
	virtual void DrawShadowSub() const = 0;
	virtual void DrawSub() const = 0;
public:
	EffectBase() {}
	virtual ~EffectBase() {}
public:
	bool IsActive() const { return IsActiveSub(); }
	void Init(const Mathf::Vector3& Pos, const Mathf::Vector3& PrevPos, const GraphHandle& Graph1, const GraphHandle& Graph2) {
		m_Screen = Graph1;
		m_Screen2 = Graph2;
		this->PrevPos = PrevPos;
		this->Pos = Pos;
		this->Time = 0.f;
		SetSub();
	}
	void Update() { UpdateSub(); }
	void DrawShadow() const { DrawShadowSub(); }
	void Draw() const { DrawSub(); }
};

class EffectControl : public SingletonBase<EffectControl, "EffectControl"> {
private:
	friend class SingletonBase<EffectControl, "EffectControl">;
private:
	EffectControl() {
		Init();
	}
	virtual ~EffectControl() {
		Dispose();
	}
private:
	EffectControl(const EffectControl&) = delete;
	EffectControl& operator=(const EffectControl&) = delete;
	EffectControl(EffectControl&&) = delete;
	EffectControl& operator=(EffectControl&&) = delete;
private:
private:
	GraphHandle m_Screen;
	GraphHandle m_Screen2;
	std::vector<std::unique_ptr<EffectBase>> m_Position;
public:
	void SetEffect(int Type, const Mathf::Vector3& Pos, const Mathf::Vector3& PrevPos);
public:
	void Init() {
		//Sonic
		//Death
		m_Screen.Create(640, 640, true);
		SetDrawScreen(m_Screen.GetHandle());
		ClearDrawScreen();
		{
			DrawCircle(640 / 2, 640 / 2, 640 / 2, ColorPalette::White, FALSE, 15);
		}
		//Hit
		m_Screen2.Create(640, 640, true);
		SetDrawScreen(m_Screen2.GetHandle());
		ClearDrawScreen();
		{
			DrawCircleAA(640.f / 2, 640.f / 2, 640.f / 2, 5, ColorPalette::White, FALSE, 50);
		}
	}
	void Update() {
		for (int loop = 0, max = static_cast<int>(m_Position.size()); loop < max; ++loop) {
			auto& s = m_Position.at(loop);
			s->Update();
			if (!s->IsActive()) {
				std::swap(s, m_Position.back());
				m_Position.pop_back();
				--loop;
				--max;
			}
		}
	}
	void DrawShadow() const {
		for (auto& s : m_Position) {
			if (!s->IsActive()) { continue; }
			s->DrawShadow();
		}
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}
	void Draw() const {
		for (auto& s : m_Position) {
			if (!s->IsActive()) { continue; }
			s->Draw();
		}
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}
	void Dispose() {
		m_Screen.ReleaseGraph();
		m_Screen2.ReleaseGraph();
	}
};
