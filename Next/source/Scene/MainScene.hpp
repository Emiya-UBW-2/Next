#pragma once

#include "../FrameWork/Define.hpp"

#include <cmath>
#include <array>
#include <string>

class MainCamera {
private:
	static const MainCamera* m_Singleton;
public:
	static void Create(void) noexcept {
		m_Singleton = new MainCamera();
	}
	static void Release(void) noexcept {
		delete m_Singleton;
	}
	static MainCamera* Instance(void) noexcept {
		if (m_Singleton == nullptr) {
			MessageBox(NULL, "Failed MainCamera Instance Create", "", MB_OK);
			exit(-1);
		}
		// if (m_Singleton == nullptr) { m_Singleton = new MainCamera(); }
		return (MainCamera*)m_Singleton;
	}
private:
	MainCamera() {
	}
	~MainCamera() {
	}
private:
	MainCamera(const MainCamera&) = delete;
	MainCamera& operator=(const MainCamera&) = delete;
	MainCamera(MainCamera&&) = delete;
	MainCamera& operator=(MainCamera&&) = delete;
private:
	Mathf::Vector3 CamPos;
public:
	Mathf::Vector3 GetDisplayPoint(float X, float Y, float Z) const {
		X = X - CamPos.x;
		Y = Y - CamPos.y;
		Z = Z - CamPos.z;
		float Rad1 = Mathf::Deg2Rad(-30.f + 90.f);
		float Rad2 = Mathf::Deg2Rad(-30.f);

		Mathf::Vector3 Ret;

		float Xrate = Mathf::Lerp(0.25f, 1.f, X);

		Ret.x = -200.f + std::cos(Rad1) * (1200.f * X) + std::cos(Rad2) * (Y * Xrate * 1000.f);
		Ret.y = -200.f + std::sin(Rad1) * (1200.f * X) + std::sin(Rad2) * (Y * Xrate * 1000.f) - Z * 10.f;
		Ret.z = 0.f;

		return Ret;
	}
	Mathf::Vector3 GetDisplayPoint(const Mathf::Vector3& Pos) const { return GetDisplayPoint(Pos.x, Pos.y, Pos.z); }
public:
	const Mathf::Vector3& GetCamPos() const { return CamPos; }
	void SetCamPos(const Mathf::Vector3& Pos) { CamPos = Pos; }
};

constexpr int MaxHP = 100;


enum class EnumEffect {
	None,

	Smoke,
	Sonic,
	Death,
	Hit,
};

struct EffectPositionData {
	EnumEffect EffectType{ EnumEffect::None};
	Mathf::Vector3 Pos{};
	float Size{};
	float Alpha{};
	float Rad{};

	GraphHandle m_Screen;
	GraphHandle m_Screen2;//TODO
public:
	bool IsActive() const {
		switch (EffectType) {
		case EnumEffect::Smoke:
			break;
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
public:
	void Init(const GraphHandle& Graph1, const GraphHandle& Graph2) {
		Size = 1000.f;
		m_Screen = Graph1;
		m_Screen2 = Graph2;
	}
	void Set(EnumEffect Type, const Mathf::Vector3& Pos) {
		this->EffectType = Type;
		switch (EffectType) {
		case EnumEffect::Smoke:
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
	void Update();
	void DrawShadow() const {
		switch (EffectType) {
		case EnumEffect::Smoke:
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
	void Draw() const;
};

class Smoke {
private:
	std::array<EffectPositionData, 90> m_SmokePoint;
	int m_SmokePointNum = 0;
public:
	void Init(const Mathf::Vector3& Pos) {
		for (auto& s : m_SmokePoint) {
			s.Pos = Pos;
			s.Size = 0.f;
		}
	}
	void Update(const Mathf::Vector3& Pos, float Per);
	void DrawShadow() const;
	void Draw() const;
};


class EffectControl {
private:
	GraphHandle m_Screen;
	GraphHandle m_Screen2;
	std::array<EffectPositionData, 64> m_Position;
	int m_Pos = 0;
public:
	void SetEffect(EnumEffect Type, const Mathf::Vector3& Pos) {
		m_Position.at(m_Pos).Set(Type, Pos);
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
			DrawCircle(640 / 2, 640 / 2, 640 / 2, GetColor(255, 255, 255), FALSE, 15);
		}
		//Hit
		m_Screen2.Create(640, 640, true);
		SetDrawScreen(m_Screen2.GetHandle());
		ClearDrawScreen();
		{
			DrawCircleAA(640.f / 2, 640.f / 2, 640.f / 2, 5, GetColor(255, 255, 255), FALSE, 50);
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
			if(!s.IsActive()) { continue; }
			s.Draw();
		}
	}
	void Dispose() {
		m_Screen.ReleaseGraph();
		m_Screen2.ReleaseGraph();
	}
};

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

class Character {
	Mathf::Vector3 m_Vec{};
	Mathf::Vector3 m_Pos{};
	Mathf::Vector3 m_GunPos{};
	Mathf::Vector3 m_RePos{};
	float m_GunPosOffset = 0.03f;
	float m_Rad = 0.f;
	float m_GunRad = 0.f;
	std::array<Smoke, 2> m_Smoke{};
	std::array<GraphHandle, 4> m_GraphHandle{};
	GraphHandle m_SubHandle{};
	float time = 0.f;
	int m_GraphAnim = 0;

	float m_DamageTime = 0.f;

	int m_HitPoint = MaxHP;
	float m_RespawnTime = 0.f;

	std::array<Bullet, 64> m_Bullet{};
	std::array<float, 2> ShotInterval{};
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

	bool SetBullet(int ID, const Mathf::Vector3& Pos, const Mathf::Vector3& Vec) {
		if (ShotInterval.at(ID) == 0.f) {
			Mathf::Vector3 CaseOffset(static_cast<float>(GetRand(100) - 50) / 100.f * 0.01f, 0.f, 0.f);
			Mathf::Vector3 CaseSpeed(0.f, 0.f, -10.f);
			for (auto& b : m_Bullet) {
				if (!b.IsActive()) {
					b.Init(Pos, m_Vec, Vec, 10.f, (ID == 0) ? 100 : 34);
					ShotInterval.at(ID) = 0.25f;
					return true;
				}
			}
		}
		return false;
	}

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
		m_Smoke.at(0).Init(m_Pos + Mathf::Vector3(-0.02f, 0.0f, 0.f));
		m_Smoke.at(1).Init(m_Pos + Mathf::Vector3(0.02f, 0.0f, 0.f));
	}
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


class MainGame : public BaseScene {
	using BaseScene::BaseScene;
public:
	EffectControl m_EffectControl;
	std::array<Character, 8> m_Characters{};

	std::array<float, 8> m_ShotInterval{};
	std::array<float, 8> m_DeathEffectInterval{};
	std::array<int, 8> m_DeathEffectFlag{};

	float m_MainTimer = 0.f;
	float m_TotalTimer = 60.f * 2.f;

	float m_SonicTimer = 0.f;

	std::array<float, 100> m_BlockPos{};

	bool m_BoostActive = true;
	float m_BoostTimer = 1.f;
	float m_BoostMeterRand = 0.f;
	float m_BoostInterval = 0.f;

	GraphHandle m_FinImage;

	GraphHandle m_gauge;
	GraphHandle m_meter;
	FontHandle m_Font;
	FontHandle m_FontBig;
	FontHandle m_FontResult;

	SoundHandle m_BGM;
	std::array<SoundHandle, 10> m_ShotSE{};
	int m_ShotSENow = 0;
	std::array<SoundHandle, 10> m_DamageSE{};
	int m_DamageSENow = 0;
	std::array<SoundHandle, 10> m_DeathSE{};
	int m_DeathSENow = 0;

	SoundHandle m_ScoreSE{};

	Mathf::Vector3 CamPosBuf;

	float m_CureCoolTime = 0.f;
	float m_DamageCoolTime = 0.f;
	float m_HitPointRe = 0.f;
protected:
	void InitSub();
	void UpdateSub();
	void DrawSub();
	void DisposeSub();
private:
	void DrawMain();
	void DrawUI();
private:
	float m_Respawn = 0.f;
	float m_Kill = 0.f;
	float m_HitRatio = 0.f;

	int m_RespawnScore = 0;
	int m_KillScore = 0;
	int m_HitScore = 0;
	int m_ShotScore = 0;

	float m_ResultTimer = 0.f;
	std::array<float, 5> m_ResultAnim{};
	int m_ResultClear = 0;
private:
	void InitResult();
	void UpdateResult();
	void DrawResult();
};
