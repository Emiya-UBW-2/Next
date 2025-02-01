#pragma once

#include "Define.hpp"

#include <cmath>
#include <array>
#include <string>

extern Mathf::Vector3 CamPos;

static Mathf::Vector3 GetDisplayPoint(float X, float Y, float Z) {
	float Rad1 = Mathf::Deg2Rad(-30.f + 90.f);
	float Rad2 = Mathf::Deg2Rad(-30.f);

	Mathf::Vector3 Ret;

	float Xrate = Mathf::Lerp(0.25f, 1.f, X);

	Ret.x = -200.f + std::cos(Rad1) * (1200.f * X) + std::cos(Rad2) * (Y * Xrate * 1000.f);
	Ret.y = -200.f + std::sin(Rad1) * (1200.f * X) + std::sin(Rad2) * (Y * Xrate * 1000.f) - Z * 10.f;
	Ret.z = 0.f;

	return Ret;
}

struct SmokeData
{
	Mathf::Vector3 Pos;
	float Size{};
	float Seed{};
};
class Smoke {
private:
	std::array<SmokeData, 90> m_SmokePoint;
	int m_SmokePointNum = 0;
public:
	void Init(const Mathf::Vector3& Pos) {
		for (auto& s : m_SmokePoint) {
			s.Pos = Pos;
			s.Size = 0.f;
		}
	}
	void Update(const Mathf::Vector3& Pos) {
		for (auto& s : m_SmokePoint) {
			s.Size += FrameWork::Instance()->GetDeltaTime();
		}
		m_SmokePoint.at(m_SmokePointNum).Pos = Pos;
		m_SmokePoint.at(m_SmokePointNum).Size = 0.f;
		++m_SmokePointNum %= static_cast<int>(m_SmokePoint.size());
	}
	void DrawShadow() const {
		for (int loop = 0; loop < static_cast<int>(m_SmokePoint.size()); ++loop) {
			auto& s1 = m_SmokePoint.at(static_cast<size_t>(loop));
			auto& s2 = m_SmokePoint.at(static_cast<size_t>((loop + 1) % static_cast<int>(m_SmokePoint.size())));
			if (s1.Size == 0.f) { continue; }
			Mathf::Vector3 P1 = GetDisplayPoint(s1.Pos.x - CamPos.x, s1.Pos.y - CamPos.y, 0.f - CamPos.z);
			Mathf::Vector3 P2 = GetDisplayPoint(s2.Pos.x - CamPos.x, s2.Pos.y - CamPos.y, 0.f - CamPos.z);
			DrawLine(static_cast<int>(P1.x), static_cast<int>(P1.y), static_cast<int>(P2.x), static_cast<int>(P2.y), GetColor(0, 0, 0), static_cast<int>(s1.Size * 10));
		}
	}
	void Draw() const {
		for (int loop = 0; loop < static_cast<int>(m_SmokePoint.size()); ++loop) {
			auto& s1 = m_SmokePoint.at(static_cast<size_t>(loop));
			auto& s2 = m_SmokePoint.at(static_cast<size_t>((loop + 1) % static_cast<int>(m_SmokePoint.size())));
			if (s1.Size == 0.f) { continue; }
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255.f * 2.f * s1.Size));
			Mathf::Vector3 P1 = GetDisplayPoint(s1.Pos.x - CamPos.x, s1.Pos.y - CamPos.y, s1.Pos.z - CamPos.z);
			Mathf::Vector3 P2 = GetDisplayPoint(s2.Pos.x - CamPos.x, s2.Pos.y - CamPos.y, s2.Pos.z - CamPos.z);
			DrawLine(static_cast<int>(P1.x), static_cast<int>(P1.y), static_cast<int>(P2.x), static_cast<int>(P2.y), GetColor(255, 255, 255), static_cast<int>(s1.Size * 10));
		}
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}
};

class Sonic {
private:
	int m_Screen = 0;
	std::array<SmokeData, 64> m_Position;
	int m_Pos = 0;
public:
	void SetSonic(const Mathf::Vector3& Pos) {
		m_Position.at(m_Pos).Pos = Pos;
		m_Position.at(m_Pos).Size = 0.f;
		++m_Pos %= static_cast<int>(m_Position.size());
	}
public:
	void Init() {
		m_Screen = MakeScreen(640, 640, TRUE);
		SetDrawScreen(m_Screen);
		ClearDrawScreen();
		{
			DrawCircle(640 / 2, 640 / 2, 640 / 2, GetColor(255, 255, 255), FALSE, 15);
		}
		for (auto& s : m_Position) {
			s.Size = 1000.f;
		}
	}
	void Update() {
		for (auto& s : m_Position) {
			if (s.Size > 10.f) { continue; }
			s.Size += FrameWork::Instance()->GetDeltaTime();
		}
	}
	void Draw() const {
		for (auto& s : m_Position) {
			if (s.Size > 10.f) { continue; }

			SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(32.f / s.Size));
			Mathf::Vector3 P1 = GetDisplayPoint(s.Pos.x - CamPos.x, s.Pos.y - CamPos.y, s.Pos.z - CamPos.z);
			DrawRotaGraph3(static_cast<int>(P1.x), static_cast<int>(P1.y), 640 / 2, 640 / 2, double(s.Size), double(s.Size / 2), double(Mathf::Deg2Rad(30)), m_Screen, TRUE);
		}
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}
};

class HitEffect {
private:
	int m_Screen = 0;
	std::array<SmokeData, 16> m_Position;
	int m_Pos = 0;
public:
	void SetHitEffect(const Mathf::Vector3& Pos) {
		m_Position.at(m_Pos).Pos = Pos;
		m_Position.at(m_Pos).Size = 0.f;
		m_Position.at(m_Pos).Seed = Mathf::Deg2Rad(static_cast<float>(-90 + GetRand(180)));
		++m_Pos %= static_cast<int>(m_Position.size());
	}
public:
	void Init() {
		m_Screen = MakeScreen(640, 640, TRUE);
		SetDrawScreen(m_Screen);
		ClearDrawScreen();
		{
			for (int loop = 0; loop < 25; ++loop) {
				//DrawBox(0 + loop, 0 + loop, 640 - loop, 640 - loop, GetColor(255, 255, 255), FALSE);
			}
			DrawCircleAA(640.f / 2, 640.f / 2, 640.f / 2, 5, GetColor(255, 255, 255), FALSE, 50);
		}
		for (auto& s : m_Position) {
			s.Size = 1000.f;
		}
	}
	void Update() {
		for (auto& s : m_Position) {
			if (s.Size > 0.5f) { continue; }
			s.Size += FrameWork::Instance()->GetDeltaTime();
		}
	}
	void Draw() const {
		for (auto& s : m_Position) {
			if (s.Size > 0.5f) { continue; }

			float alpha = std::sin(Mathf::Deg2Rad(s.Size / 0.5f * 180.f));
			float scale = s.Size * 0.15f / 0.5f;// std::sin(Mathf::Deg2Rad(s.Size / 0.5f * 180.f)) * 0.25f;

			SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255.f * alpha));
			Mathf::Vector3 P1 = GetDisplayPoint(s.Pos.x - CamPos.x, s.Pos.y - CamPos.y, s.Pos.z - CamPos.z);
			DrawRotaGraph3(static_cast<int>(P1.x), static_cast<int>(P1.y), 640 / 2, 640 / 2, double(scale), double(scale), double(s.Seed + Mathf::Deg2Rad(30+ s.Seed * s.Size*3.f*180.f)), m_Screen, TRUE);
		}
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}
};

class Bullet {
	Mathf::Vector3 m_Vec;
	Mathf::Vector3 m_Pos;
	Mathf::Vector3 m_RePos;
	float m_Time = 100.f;
	float m_Size = 5.f;

	float m_Wave = 0.f;
	Mathf::Vector3 m_WavePoint;
public:
	const Mathf::Vector3& GetPosition() const { return  m_Pos; }
	const Mathf::Vector3& GetRePos() const { return  m_RePos; }

	bool IsActive() const { return m_Time < 2.f; }
	void DisActive() {
		m_Time = 2.f;
	}
public:
	void Init(const Mathf::Vector3& Pos, const Mathf::Vector3& Vec, float Size) {
		m_Pos = Pos;
		m_RePos = m_Pos;
		m_Vec = Vec;
		m_Time = 0.f;
		m_Size = Size;
		m_Wave = 0.f;
	}
	void Update() {
		m_RePos = m_Pos;
		m_Pos += m_Vec * FrameWork::Instance()->GetDeltaTime();
		m_Time += FrameWork::Instance()->GetDeltaTime();
		if (m_Pos.z < 0.f) {
			if (m_Wave == 0.f) {
				m_WavePoint = m_Pos;
			}
			m_Wave += FrameWork::Instance()->GetDeltaTime();
		}
	}

	void DrawShadow() const {
		if (m_Wave > 0.f) { return; }
		Mathf::Vector3 P1 = GetDisplayPoint(m_Pos.x - CamPos.x, m_Pos.y - CamPos.y, 0.f - CamPos.z);
		DrawCircle(static_cast<int>(P1.x), static_cast<int>(P1.y), static_cast<int>(m_Size), GetColor(0, 0, 0), TRUE);
	}
	void Draw() const {
		if (m_Wave > 0.f) {
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255.f / m_Wave));

			Mathf::Vector3 P1 = GetDisplayPoint(m_WavePoint.x - CamPos.x, m_WavePoint.y - CamPos.y, 0.f - CamPos.z);
			DrawCircle(static_cast<int>(P1.x), static_cast<int>(P1.y), static_cast<int>(m_Wave*30.f), GetColor(255, 255, 255), FALSE);

			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		}
		else {
			Mathf::Vector3 P1 = GetDisplayPoint(m_Pos.x - CamPos.x, m_Pos.y - CamPos.y, m_Pos.z - CamPos.z);
			DrawCircle(static_cast<int>(P1.x), static_cast<int>(P1.y), static_cast<int>(m_Size), GetColor(255, 255, 255), TRUE);
		}
	}
};


class Character {
	Mathf::Vector3 m_Vec{};
	Mathf::Vector3 m_Pos{};
	Mathf::Vector3 m_GunPos{};
	Mathf::Vector3 m_RePos{};
	float m_GunPosOffset = 0.03f;
	float m_Rad = 0.f;
	float m_GunRad = 0.f;
	std::array<Smoke, 2> m_Smoke{};
	std::array<int, 4> m_GraphHandle{};
	int m_SubHandle{};
	float time = 0.f;
	int m_GraphAnim = 0;

	std::array<Bullet, 64> m_Bullet{};
	std::array<float, 2> ShotInterval{};
public:
	std::array<Bullet, 64>& GetBulletList() { return  m_Bullet; }

	const Mathf::Vector3& GetVec() const { return  m_Vec; }
	const Mathf::Vector3& GetRePos() const { return  m_RePos; }
	const Mathf::Vector3& GetPosition() const { return  m_Pos; }
	const Mathf::Vector3& GetGunPos() const { return  m_GunPos; }

	void SetVec(const Mathf::Vector3& Vec) { m_Vec = Vec; }
	void SetGunRad(float rad) { m_GunRad = rad; }

	void SetBullet(int ID, const Mathf::Vector3& Pos, const Mathf::Vector3& Vec) {
		if (ShotInterval.at(ID) == 0.f) {
			Mathf::Vector3 CaseOffset(static_cast<float>(GetRand(100)-50)/100.f*0.01f, 0.f, 0.f);
			Mathf::Vector3 CaseSpeed(0.f,0.f,-10.f);
			for (auto& b : m_Bullet) {
				if (!b.IsActive()) {
					b.Init(Pos, m_Vec + Vec, 2.f);
					ShotInterval.at(ID) = 0.1f;
					break;
				}
			}
		}
	}
public:
	void Init(const Mathf::Vector3& Pos, std::string Name, float Ofs) {
		m_Pos = Pos;
		m_RePos = m_Pos;
		m_GunPosOffset = Ofs;

		m_Vec.x = 0.f;
		m_Vec.y = 1.f;
		m_Vec.z = 0.f;

		m_Rad = 0.f;
		m_Smoke.at(0).Init(m_Pos + Mathf::Vector3(-0.02f, 0.0f, 0.f));
		m_Smoke.at(1).Init(m_Pos + Mathf::Vector3(0.02f, 0.0f, 0.f));

		m_GraphHandle.at(0) = LoadGraph(("data/" + Name + "/0.bmp").c_str(), TRUE);
		m_GraphHandle.at(1) = LoadGraph(("data/" + Name + "/1.bmp").c_str(), TRUE);
		m_GraphHandle.at(2) = LoadGraph(("data/" + Name + "/2.bmp").c_str(), TRUE);
		m_GraphHandle.at(3) = LoadGraph(("data/" + Name + "/1.bmp").c_str(), TRUE);
		m_SubHandle = LoadGraph(("data/" + Name + "/gun.bmp").c_str(), TRUE);

		for (auto& s : ShotInterval) {
			s = 0.f;
		}
		for (auto& b : m_Bullet) {
			b.DisActive();
		}
	}
	void Update() {
		m_RePos = m_Pos;
		m_Pos += m_Vec * FrameWork::Instance()->GetDeltaTime();
		m_GunPos = m_Pos - m_Vec.Nomalize() * m_GunPosOffset;

		m_Rad = std::atan2f(m_Vec.x, m_Vec.y);

		Mathf::Vector3 Offset = Mathf::Vector3(-0.03f * -std::sin(-m_Rad), -0.03f * std::cos(-m_Rad), 0.f);

		float Per = 0.02f * 1.f/m_Vec.y;
		m_Smoke.at(0).Update(m_Pos + Offset + Mathf::Vector3(-Per * std::cos(-m_Rad), -Per * std::sin(-m_Rad), 0.f));
		m_Smoke.at(1).Update(m_Pos + Offset + Mathf::Vector3(Per * std::cos(-m_Rad), Per * std::sin(-m_Rad), 0.f));

		for (auto& s : ShotInterval) {
			s = Mathf::Max(s - FrameWork::Instance()->GetDeltaTime(), 0.f);
		}
		time += FrameWork::Instance()->GetDeltaTime();
		float timeRate = (1.f / m_Vec.Length()) * 0.07f;
		if (time > timeRate) {
			time -= timeRate;
			++m_GraphAnim %= static_cast<int>(m_GraphHandle.size());
		}

		for (auto& b : m_Bullet) {
			if (b.IsActive()) {
				b.Update();
			}
		}
	}

	void DrawShadow() const {
		for (auto& s : m_Smoke) {
			s.DrawShadow();
		}

		for (auto& b : m_Bullet) {
			if (b.IsActive()) {
				b.DrawShadow();
			}
		}

		Mathf::Vector3 P1 = GetDisplayPoint(m_Pos.x - CamPos.x, m_Pos.y - CamPos.y, 0.f - CamPos.z);
		SetDrawBright(0, 0, 0);
		DrawRotaGraph(static_cast<int>(P1.x), static_cast<int>(P1.y), 1.0, static_cast<double>(m_Rad * 1.5f), m_GraphHandle.at(m_GraphAnim), TRUE);
		SetDrawBright(255, 255, 255);
	}
	void Draw() const {
		for (auto& s : m_Smoke) {
			s.Draw();
		}

		for (auto& b : m_Bullet) {
			if (b.IsActive()) {
				b.Draw();
			}
		}

		Mathf::Vector3 P2 = GetDisplayPoint(m_GunPos.x - CamPos.x, m_GunPos.y - CamPos.y, m_GunPos.z - CamPos.z);
		DrawRotaGraph(static_cast<int>(P2.x), static_cast<int>(P2.y), 1.0, static_cast<double>(m_GunRad + Mathf::Deg2Rad(-30.f)), m_SubHandle, TRUE);

		Mathf::Vector3 P1 = GetDisplayPoint(m_Pos.x - CamPos.x, m_Pos.y - CamPos.y, m_Pos.z - CamPos.z);
		DrawRotaGraph(static_cast<int>(P1.x), static_cast<int>(P1.y), 1.0, static_cast<double>(m_Rad * 1.5f), m_GraphHandle.at(m_GraphAnim), TRUE);

	}
};

class MainGame {
public:
	Character m_Character;
	Sonic m_Sonic;

	HitEffect m_HitEffect;
	std::array<Character,8> m_Enemy;

	float m_SonicTimer = 0.f;
	bool prevUpKey = false;

	std::array<float, 100> m_BlockPos;
public:
	MainGame() {}
	~MainGame() {}
public:
	void Init() {
		{
			Mathf::Vector3 Pos(0.f, 0.f, 20.f);
			m_Character.Init(Pos, "plane", 0.03f);
			m_Sonic.Init();
		}
		CamPos = m_Character.GetPosition();
		CamPos.z = 10.f;

		for (auto& e : m_Enemy) {
			Mathf::Vector3 Pos(&e - &m_Enemy.front(), 0.f, 20.f); Pos.x /= 10.f;
			e.Init(Pos, "enemy",0.01f);
		}

		m_HitEffect.Init();

		for (int loop = 0; loop < m_BlockPos.size(); ++loop) {
			m_BlockPos.at(loop) = static_cast<float>(GetRand(200)-100) / 100.f;
		}
	}
	void Update();
	void Draw() {
		DrawBox(0, 0, FrameWork::Instance()->GetScreenWidth(), FrameWork::Instance()->GetScreenHeight(), GetColor(255, 0, 0), TRUE);

		//
		for (auto& e : m_Enemy) {
			e.DrawShadow();
		}
		m_Character.DrawShadow();


		for (int loop = -5; loop < 10; ++loop) {
			float timeTemp = static_cast<float>(loop + (int)(CamPos.y) * 3.f) / 3.f;

			Mathf::Vector3 P1 = GetDisplayPoint(0.f, timeTemp - CamPos.y, 0.f - CamPos.z);
			Mathf::Vector3 P2 = GetDisplayPoint(2.f, timeTemp - CamPos.y, 0.f - CamPos.z);
			Mathf::Vector3 P3 = GetDisplayPoint(2.f, timeTemp - CamPos.y + 0.2f, 0.f - CamPos.z);
			DrawTriangle(static_cast<int>(P1.x), static_cast<int>(P1.y), static_cast<int>(P2.x), static_cast<int>(P2.y), static_cast<int>(P3.x), static_cast<int>(P3.y), GetColor(0, 0, 0), TRUE);
		}

		for (auto& e : m_Enemy) {
			e.Draw();
		}
		m_Character.Draw();

		m_Sonic.Draw();
		m_HitEffect.Draw();
	}
private:
};
