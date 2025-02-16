#pragma once

#include "../../FrameWork/Define.hpp"

class Bullet {
	Mathf::Vector3 m_BaseVec;
	Mathf::Vector3 m_AddVec;
	Mathf::Vector3 m_Pos;
	Mathf::Vector3 m_RePos;
	float m_Time = 100.f;
	float m_Size = 5.f;

	float m_Wave = 0.f;
	Mathf::Vector3 m_WavePoint;
	int m_Damage = 0;
public:
	const Mathf::Vector3& GetPosition() const { return  m_Pos; }
	const Mathf::Vector3& GetRePos() const { return  m_RePos; }
	const int GetDamage() const { return m_Damage; }

	bool IsActive() const { return m_Time < 2.f; }
	void DisActive() {
		m_Time = 2.f;
	}
public:
	void Init(const Mathf::Vector3& Pos, const Mathf::Vector3& BaseVec, const Mathf::Vector3& AddVec, float Size, int Damage) {
		m_Pos = Pos;
		m_RePos = m_Pos;
		m_BaseVec = BaseVec;
		m_AddVec = AddVec;
		m_Time = 0.f;
		m_Size = Size;
		m_Damage = Damage;
		m_Wave = 0.f;
	}
	void Update();

	void DrawShadow() const;
	void Draw() const;
};
