#pragma once

#include "../FrameWork/Define.hpp"

#include "MainScene/Bullet.hpp"
#include "MainScene/Character.hpp"

class MainGame : public BaseScene {
	using BaseScene::BaseScene;
public:
	std::array<Character, 8> m_Characters{};

	float m_MainTimer = 0.f;
	const float m_TotalTimer = 60.f * 2.f;
	bool IsGoingNextScene = false;

	GraphHandle m_FinImage;
	GraphHandle m_gauge;
	GraphHandle m_meter;

	float m_BoostMeterRand{};

	Mathf::Vector3 CamPosBuf;

	float m_CureCoolTime = 0.f;
	float m_DamageCoolTime = 0.f;
	float m_HitPointRe = 0.f;

	float m_PausePer = 0.f;
	float m_PauseTimer = 0.f;
	int m_PauseSelect = 0;
	int m_PauseOpen = 0;
	int m_OptionSelect = 0;
	bool OnMousePause[2][3] = {};
protected:
	void InitSub() override;
	void UpdateSub() override;
	void DrawSub() override;
	void DisposeSub() override;

	std::unique_ptr<BaseScene> MakeNextScene() override;
private:
	void DrawMain();
	void DrawUI();
	void DrawPauseUI();
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
	bool OnMouseResultSkip = false;
private:
	void InitResult();
	void UpdateResult();
	void DrawResult();
	bool IsResultActive() const { return m_MainTimer <= -3.5f; }
private:
	bool IsInGame() const { return 0.f < m_MainTimer; }
	bool IsMainGame() const { return 0.f < m_MainTimer && m_MainTimer < m_TotalTimer; }
};
