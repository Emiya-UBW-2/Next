#pragma once

#include "../../FrameWork/Define.hpp"

#include "Bullet.hpp"

#include <array>
#include <string>

constexpr int MaxHP = 100;

class Character {
	Mathf::Vector3 m_Vec{};
	Mathf::Vector3 m_Pos{};
	Mathf::Vector3 m_GunPos{};
	Mathf::Vector3 m_RePos{};
	float m_GunPosOffset = 0.03f;
	float m_Rad{};
	float m_GunRad{};
	std::array<GraphHandle, 4> m_GraphHandle{};
	GraphHandle m_SubHandle{};
	float time{};
	int m_GraphAnim = 0;

	float m_DamageTime{};

	int m_HitPoint = MaxHP;
	float m_RespawnTime{};

	std::array<Bullet, 64> m_Bullet{};
	std::array<float, 2> ShotInterval{};
	std::array<Mathf::Vector3, 2> SmokeVector{};
public:
	float m_SonicTimer{};
	bool m_BoostActive = true;
	float m_BoostTimer = 1.f;
	float m_BoostInterval{};
	float m_ShotInterval{};
	float m_DeathEffectInterval{};
	int m_DeathEffectFlag{};
public:
	std::array<Bullet, 64>& GetBulletList() { return  m_Bullet; }

	const Mathf::Vector3& GetVec() const { return  m_Vec; }
	const Mathf::Vector3& GetRePos() const { return  m_RePos; }
	const Mathf::Vector3& GetPosition() const { return  m_Pos; }
	const Mathf::Vector3& GetGunPos() const { return  m_GunPos; }

	bool CanDamage() const { return m_DamageTime == 0.f; }
	bool CanRespawn() const { return m_RespawnTime == 0.f; }
	bool IsAlive() const { return m_HitPoint > 0; }
	int GetHP() const { return m_HitPoint; }
	void SetVec(const Mathf::Vector3& Vec) { m_Vec = Vec; }
	void SetGunRad(float rad) { m_GunRad = rad; }
	bool SetBullet(int ID, const Mathf::Vector3& Pos, const Mathf::Vector3& Vec);
	void SetDamage(int Damage) {
		if (Damage > 0 || Damage == -MaxHP) {
			m_DamageTime = 1.f;
		}
		m_HitPoint = static_cast<int>(Mathf::Clamp(static_cast<float>(m_HitPoint - Damage), 0.f, static_cast<float>(MaxHP)));
		if (m_HitPoint == 0) {
			m_RespawnTime = 3.f;
		}
	}
	void SetPos(const Mathf::Vector3& Pos) {
		m_Pos = Pos;
		m_RePos = m_Pos;

		m_Vec.x = 0.f;
		m_Vec.y = 1.f;
		m_Vec.z = 0.f;

		m_Rad = 0.f;

		{
			Mathf::Vector3 Offset = Mathf::Vector3(-0.03f * -std::sin(-m_Rad), -0.03f * std::cos(-m_Rad), 0.f);
			float Per = 0.02f * 1.f / m_Vec.y;
			Mathf::Vector3 Pos0 = m_Pos + Offset + Mathf::Vector3(-Per * std::cos(-m_Rad), -Per * std::sin(-m_Rad), 0.f);
			Mathf::Vector3 Pos1 = m_Pos + Offset + Mathf::Vector3(Per * std::cos(-m_Rad), Per * std::sin(-m_Rad), 0.f);
			SmokeVector.at(0) = Pos0;
			SmokeVector.at(1) = Pos1;
		}
		m_BoostTimer = 1.f;
		m_BoostActive = true;
	}
public:
	void SetLRVec(bool IsLeft, bool IsRight);
public:
	void Init(const Mathf::Vector3& Pos, std::string Name, float Ofs) {
		SetPos(Pos);
		m_GunPosOffset = Ofs;

		m_GraphHandle.at(0).LoadGraph(("data/" + Name + "/0.bmp").c_str(), true);
		m_GraphHandle.at(1).LoadGraph(("data/" + Name + "/1.bmp").c_str(), true);
		m_GraphHandle.at(2).LoadGraph(("data/" + Name + "/2.bmp").c_str(), true);
		m_GraphHandle.at(3).LoadGraph(("data/" + Name + "/1.bmp").c_str(), true);
		m_SubHandle.LoadGraph(("data/" + Name + "/gun.bmp").c_str(), true);

		for (auto& s : ShotInterval) {
			s = 0.f;
		}
		for (auto& b : m_Bullet) {
			b.DisActive();
		}
		m_BoostTimer = 1.f;
		m_BoostActive = true;
	}
	void Update();
	void DrawShadow() const;
	void Draw() const;
	void Dispose() {
		m_GraphHandle.at(0).ReleaseGraph();
		m_GraphHandle.at(1).ReleaseGraph();
		m_GraphHandle.at(2).ReleaseGraph();
		m_GraphHandle.at(3).ReleaseGraph();
		m_SubHandle.ReleaseGraph();
	}
};
