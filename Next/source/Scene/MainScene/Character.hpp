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
public:
	bool SetBullet(int ID, const Mathf::Vector3& Pos, const Mathf::Vector3& Vec);
	void SetDamage(int Damage);
	void Spawn(const Mathf::Vector3& Pos);
public:
	void SetLRVec(bool IsLeft, bool IsRight);
public:
	void Init(const Mathf::Vector3& Pos, std::string Name, float Ofs);
	void Update();
	void DrawShadow() const;
	void Draw() const;
	void Dispose();
};
