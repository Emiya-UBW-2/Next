#pragma once

#include "../../FrameWork/Define.hpp"

class Bullet {
	Mathf::Vector3 m_BaseVec;
	Mathf::Vector3 m_AddVec;
	Mathf::Vector3 m_Pos;
	Mathf::Vector3 m_RePos;
	float m_Time = 100.f;
	float m_Size = 5.f;

	int m_Damage = 0;
	int m_ShooterID{};
public:
	const Mathf::Vector3& GetPosition() const { return  m_Pos; }
	const Mathf::Vector3& GetRePos() const { return  m_RePos; }
	const int GetDamage() const { return m_Damage; }

	bool IsActive() const { return m_Time < 2.f; }
	void DisActive() { m_Time = 2.f; }
	int GetShooterID() { return m_ShooterID; }
public:
	void Init(int ShooterID, const Mathf::Vector3& Pos, const Mathf::Vector3& BaseVec, const Mathf::Vector3& AddVec, float Size, int Damage) {
		m_ShooterID = ShooterID;
		m_Pos = Pos;
		m_RePos = m_Pos;
		m_BaseVec = BaseVec;
		m_AddVec = AddVec;
		m_Time = 0.f;
		m_Size = Size;
		m_Damage = Damage;
	}
	void Update();

	void DrawShadow() const;
	void Draw() const;
};

class BulletControl : public SingletonBase<BulletControl, "BulletControl"> {
private:
	friend class SingletonBase<BulletControl, "BulletControl">;
private:
	BulletControl() {
		Init();
	}
	virtual ~BulletControl() {
		Dispose();
	}
private:
	BulletControl(const BulletControl&) = delete;
	BulletControl& operator=(const BulletControl&) = delete;
	BulletControl(BulletControl&&) = delete;
	BulletControl& operator=(BulletControl&&) = delete;
private:
private:
	std::vector<std::unique_ptr<Bullet>> m_Position;
public:
	void Set(int ShooterID, const Mathf::Vector3& Pos, const Mathf::Vector3& BaseVec, const Mathf::Vector3& AddVec, float Size, int Damage);
	std::vector<std::unique_ptr<Bullet>>& SetBulletList() { return m_Position; }
public:
	void Init() {}
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
	}
	void Draw() const {
		for (auto& s : m_Position) {
			if (!s->IsActive()) { continue; }
			s->Draw();
		}
	}
	void Dispose() {}
};
