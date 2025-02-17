#pragma once

#include "../../FrameWork/Define.hpp"
#include <array>

enum class EnumEffect {
	None,

	Smoke,
	Sonic,
	Death,
	Hit,
};

class EffectControl {
private:
	static const EffectControl* m_Singleton;
public:
	static void Create(void) noexcept {
		m_Singleton = new EffectControl();
	}
	static void Release(void) noexcept {
		delete m_Singleton;
	}
	static EffectControl* Instance(void) noexcept {
		if (m_Singleton == nullptr) {
			MessageBox(NULL, "Failed EffectControl Instance Create", "", MB_OK);
			exit(-1);
		}
		// if (m_Singleton == nullptr) { m_Singleton = new EffectControl(); }
		return (EffectControl*)m_Singleton;
	}
private:
	EffectControl() {
		Init();
	}
	~EffectControl() {
		Dispose();
	}
private:
	EffectControl(const EffectControl&) = delete;
	EffectControl& operator=(const EffectControl&) = delete;
	EffectControl(EffectControl&&) = delete;
	EffectControl& operator=(EffectControl&&) = delete;
private:
	struct EffectPositionData {
		int EffectType{};
		Mathf::Vector3 Pos{};
		Mathf::Vector3 PrevPos{};
		float Size{};
		float Alpha{};
		float Rad{};

		GraphHandle m_Screen;
		GraphHandle m_Screen2;//TODO
	public:
		bool IsActive() const;
	public:
		void Init(const GraphHandle& Graph1, const GraphHandle& Graph2) {
			Size = 1000.f;
			m_Screen = Graph1;
			m_Screen2 = Graph2;
		}
		void Set(int Type, const Mathf::Vector3& Pos, const Mathf::Vector3& PrevPos);
		void Update();
		void DrawShadow() const;
		void Draw() const;
	};
private:
	GraphHandle m_Screen;
	GraphHandle m_Screen2;
	std::array<EffectPositionData, 900> m_Position;
	int m_Pos = 0;
public:
	void SetEffect(int Type, const Mathf::Vector3& Pos, const Mathf::Vector3& PrevPos) {
		m_Position.at(m_Pos).Set(Type, Pos, PrevPos);
		++m_Pos %= static_cast<int>(m_Position.size());
	}
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
		//
		for (auto& s : m_Position) {
			s.Init(m_Screen, m_Screen2);
		}
	}
	void Update() {
		for (auto& s : m_Position) {
			if (!s.IsActive()) { continue; }
			s.Update();
		}
	}
	void DrawShadow() const {
		for (auto& s : m_Position) {
			if (!s.IsActive()) { continue; }
			s.DrawShadow();
		}
	}
	void Draw() const {
		for (auto& s : m_Position) {
			if (!s.IsActive()) { continue; }
			s.Draw();
		}
	}
	void Dispose() {
		m_Screen.ReleaseGraph();
		m_Screen2.ReleaseGraph();
	}
};
