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
	void Init(const Mathf::Vector3& Pos, const Mathf::Vector3& PrevPos, const GraphHandle& Graph) {
		m_Screen = Graph;
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
	std::vector<GraphHandle> m_EffectGraph;
	std::vector<std::unique_ptr<EffectBase>> m_Pool;
public:
	void SetEffect(int Type, const Mathf::Vector3& Pos, const Mathf::Vector3& PrevPos);
public:
	void Init() {
		//Sonic
		//Death
		int Handles[5]{};
		LoadDivGraph("data/Effect.png", 5, 5, 1, 128, 128, Handles);
		m_EffectGraph.resize(5);
		for (int i = 0, max = static_cast<int>(m_EffectGraph.size()); i < max; ++i) {
			m_EffectGraph.at(i).SetHandle(Handles[i]);
		}

		m_Pool.reserve(128);
	}
	void Update() {
		for (int loop = 0, max = static_cast<int>(m_Pool.size()); loop < max; ++loop) {
			auto& s = m_Pool.at(loop);
			s->Update();
			if (!s->IsActive()) {
				std::swap(s, m_Pool.back());
				m_Pool.pop_back();
				--loop;
				--max;
			}
		}
	}
	void DrawShadow() const {
		for (auto& s : m_Pool) {
			if (!s->IsActive()) { continue; }
			s->DrawShadow();
		}
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}
	void Draw() const {
		for (auto& s : m_Pool) {
			if (!s->IsActive()) { continue; }
			s->Draw();
		}
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}
	void Dispose() {
		m_Pool.clear();
		m_Pool.shrink_to_fit();
		for (auto& e : m_EffectGraph) {
			e.ReleaseGraph();
		}
		m_EffectGraph.clear();
	}
};
