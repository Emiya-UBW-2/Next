#pragma once

#include "../../FrameWork/Define.hpp"
#include <array>

enum class EnumSE {
	None,

	Shot,
	Damage,
	Death,
};

class SoundControl {
private:
	static const SoundControl* m_Singleton;
public:
	static void Create(void) noexcept {
		m_Singleton = new SoundControl();
	}
	static void Release(void) noexcept {
		delete m_Singleton;
	}
	static SoundControl* Instance(void) noexcept {
		if (m_Singleton == nullptr) {
			MessageBox(NULL, "Failed SoundControl Instance Create", "", MB_OK);
			exit(-1);
		}
		// if (m_Singleton == nullptr) { m_Singleton = new SoundControl(); }
		return (SoundControl*)m_Singleton;
	}
private:
	SoundControl() {
		Init();
	}
	~SoundControl() {
		Dispose();
	}
private:
	SoundControl(const SoundControl&) = delete;
	SoundControl& operator=(const SoundControl&) = delete;
	SoundControl(SoundControl&&) = delete;
	SoundControl& operator=(SoundControl&&) = delete;
private:
	std::array<SoundHandle, 10> m_ShotSE{};
	int m_ShotSENow = 0;
	std::array<SoundHandle, 10> m_DamageSE{};
	int m_DamageSENow = 0;
	std::array<SoundHandle, 10> m_DeathSE{};
	int m_DeathSENow = 0;
public:
	void SetSE(int Type);
public:
	void Init() {
		for (auto& s : m_ShotSE) {
			s.Create("data/Audio/Shot.wav");
		}
		for (auto& s : m_DamageSE) {
			s.Create("data/Audio/Damage.wav");
		}
		for (auto& s : m_DeathSE) {
			s.Create("data/Audio/Death.wav");
		}
	}
	void Update() {
	}
	void Dispose() {
		for (auto& s : m_ShotSE) {
			s.ReleaseSound();
		}
		for (auto& s : m_DamageSE) {
			s.ReleaseSound();
		}
		for (auto& s : m_DeathSE) {
			s.ReleaseSound();
		}
	}
};
