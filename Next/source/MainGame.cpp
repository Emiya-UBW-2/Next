#include "MainGame.hpp"

Mathf::Vector3 CamPos;

void MainGame::Init() {

	for (auto& e : m_Characters) {
		int index = static_cast<int>(&e - &m_Characters.front());
		Mathf::Vector3 Pos(static_cast<float>(GetRand(index + 1)) * 0.1f, static_cast<float>(index) * 0.5f, 20.f);
		if (index == (m_Characters.size() - 1)) {
			Pos.y += 1.5f;
			e.Init(Pos, "plane", 0.03f);
		}
		else {
			e.Init(Pos, "enemy", 0.01f);
		}
	}

	m_BoostTimer = 1.f;

	CamPos = m_Characters.back().GetPosition();
	CamPos.z = 10.f;

	m_Sonic.Init();
	m_DeathEffect.Init();
	m_HitEffect.Init();

	for (int loop = 0; loop < m_BlockPos.size(); ++loop) {
		m_BlockPos.at(loop) = static_cast<float>(GetRand(200) - 100) / 100.f;
	}

	m_gauge = LoadGraph("data/UI/boostgauge.bmp", TRUE);
	m_meter = LoadGraph("data/UI/boostmeter.bmp", TRUE);
	m_Font = CreateFontToHandle("Agency FB", 12, -1, DX_FONTTYPE_ANTIALIASING_EDGE, DX_CHARSET_DEFAULT, 1);

	SetCreateSoundDataType(DX_SOUNDDATATYPE_MEMNOPRESS);
	for (auto& s : m_ShotSE) {
		s = LoadSoundMem("data/Audio/Shot.wav");
	}
	for (auto& s : m_DamageSE) {
		s = LoadSoundMem("data/Audio/Damage.wav");
	}
	for (auto& s : m_DeathSE) {
		s = LoadSoundMem("data/Audio/Death.wav");
	}

	SetCreateSoundDataType(DX_SOUNDDATATYPE_FILE);
	m_BGM = LoadSoundMem("data/Audio/BGM.wav");
	SetCreateSoundDataType(DX_SOUNDDATATYPE_MEMNOPRESS);

	PlaySoundMem(m_BGM, DX_PLAYTYPE_BACK);
}

void MainGame::Update() {
	if (m_Characters.back().IsAlive()) {
		Mathf::Vector3 Vec = m_Characters.back().GetVec();

		bool UpKey = CheckHitKey(KEY_INPUT_W);
		if (!m_BoostActive) {
			UpKey = false;
		}
		if (UpKey) {
			m_BoostInterval = 0.5f;
			if (m_BoostActive) {
				m_BoostTimer = Mathf::Max(m_BoostTimer - FrameWork::Instance()->GetDeltaTime(), 0.f);
				if (m_BoostTimer == 0.f) {
					m_BoostActive = false;
				}
			}
		}
		else {
			m_BoostInterval = Mathf::Max(m_BoostInterval - FrameWork::Instance()->GetDeltaTime(), 0.f);
			if (m_BoostInterval == 0.f) {
				if (m_BoostActive) {
					m_BoostTimer = Mathf::Min(m_BoostTimer + FrameWork::Instance()->GetDeltaTime(), 1.f);
				}
				else {
					m_BoostTimer = Mathf::Min(m_BoostTimer + FrameWork::Instance()->GetDeltaTime() * 0.5f, 1.f);
				}
			}
			if (m_BoostTimer == 1.f) {
				m_BoostActive = true;
			}
		}

		bool DownKey = CheckHitKey(KEY_INPUT_S);
		bool LeftKey = CheckHitKey(KEY_INPUT_A);
		bool RightKey = CheckHitKey(KEY_INPUT_D);
		bool ShotKey = CheckHitKey(KEY_INPUT_SPACE);
		bool ShotSubKey = (GetMouseInput() & MOUSE_INPUT_LEFT) != 0;
		if (LeftKey) {
			Vec.x = Mathf::Clamp(Vec.x - 0.5f * FrameWork::Instance()->GetDeltaTime(), -0.25f, 0.25f);
		}
		if (RightKey) {
			Vec.x = Mathf::Clamp(Vec.x + 0.5f * FrameWork::Instance()->GetDeltaTime(), -0.25f, 0.25f);
		}
		if (!LeftKey && !RightKey) {
			Mathf::Easing(&Vec.x, 0.f, 0.95f);
		}

		if (DownKey) {
			Vec.y = Mathf::Clamp(Vec.y - 1.f * FrameWork::Instance()->GetDeltaTime(), 0.85f, 1.5f);
		}
		if (UpKey) {
			Vec.y = Mathf::Clamp(Vec.y + 1.f * FrameWork::Instance()->GetDeltaTime(), 0.85f, 1.5f);
		}
		if (!DownKey && !UpKey) {
			Mathf::Easing(&Vec.y, 1.f, 0.95f);
		}

		if (ShotKey) {
			if (m_Characters.back().SetBullet(0, m_Characters.back().GetPosition(), m_Characters.back().GetVec().Nomalize() * 2.f)) {
				PlaySoundMem(m_ShotSE.at(m_ShotSENow), DX_PLAYTYPE_BACK);
				++m_ShotSENow %= static_cast<int>(m_ShotSE.size());
			}
		}

		//マウス座標を取得しておく
		int MouseX;
		int MouseY;
		//マウス座標を取得しておく
		GetMousePoint(&MouseX, &MouseY);

		MouseX = MouseX * FrameWork::Instance()->GetScreenWidth() / FrameWork::Instance()->GetWindowWidth();
		MouseY = MouseY * FrameWork::Instance()->GetScreenHeight() / FrameWork::Instance()->GetWindowHeight();

		Mathf::Vector3 MP1(static_cast<float>(MouseX), static_cast<float>(MouseY), 0.f);

		Mathf::Vector3 PG = m_Characters.back().GetGunPos();
		Mathf::Vector3 P1 = GetDisplayPoint(PG.x - CamPos.x, PG.y - CamPos.y, PG.z - CamPos.z);
		Mathf::Vector3 P1X = GetDisplayPoint(PG.x + 1.f - CamPos.x, PG.y + 0.f - CamPos.y, PG.z - CamPos.z);
		Mathf::Vector3 P1Z = GetDisplayPoint(PG.x + 0.f - CamPos.x, PG.y + 1.f - CamPos.y, PG.z - CamPos.z);

		Mathf::Vector3 Vector;
		Vector.x = Mathf::Vector3::Dot(Mathf::Vector3::Cross((MP1 - P1).Nomalize(), (P1Z - P1).Nomalize()), Mathf::Vector3::Cross((P1X - P1).Nomalize(), (P1Z - P1).Nomalize()));//Sin
		Vector.y = Mathf::Vector3::Dot((MP1 - P1).Nomalize(), (P1Z - P1).Nomalize());//Cos
		Vector.z = 0.f;

		m_Characters.back().SetGunRad(std::atan2f(Vector.x, Vector.y));
		if (ShotSubKey) {
			if (m_Characters.back().SetBullet(1, PG, Vector.Nomalize() * 1.f)) {
				PlaySoundMem(m_ShotSE.at(m_ShotSENow), DX_PLAYTYPE_BACK);
				++m_ShotSENow %= static_cast<int>(m_ShotSE.size());
			}
		}

		m_Characters.back().SetVec(Vec);

		if (UpKey) {
			if (prevUpKey != UpKey) {
				m_SonicTimer = 0.f;
			}
			else {
				m_SonicTimer += FrameWork::Instance()->GetDeltaTime();
			}
			if (m_SonicTimer > 0.025f) {
				m_SonicTimer -= 0.025f;
				m_Sonic.SetSonic(m_Characters.back().GetPosition());
			}
		}

		prevUpKey = UpKey;
	}
	else {
		m_BoostTimer = 1.f;
		m_BoostActive = true;
	}
	Mathf::Easing(&m_BoostMeterRand, m_BoostTimer + static_cast<float>(GetRand(200) - 100) / 100.f * 0.1f, 0.9f);

	for (auto& e : m_Characters) {
		if ((&e - &m_Characters.front()) == (m_Characters.size() - 1)) { continue; }
		Mathf::Vector3 ToP = e.GetPosition() - m_Characters.back().GetPosition();
		Mathf::Vector3 Vec = e.GetVec();
		//Vec.x = 0.f;wwwww

		bool IsBoost = (ToP.y) > 3.f;
		bool IsUp = (ToP.y < 0.f);
		if (!IsBoost && std::abs(ToP.x) > 0.15f) {
			IsUp = false;
		}
		bool IsRight = (ToP.x < 0.f);

		for (auto& e2 : m_Characters) {
			if (&e != &e2) {
				Mathf::Vector3 To = e.GetPosition() - e2.GetPosition();
				if (std::abs(To.x) < 0.05f && std::abs(To.y) < 0.5f) {
					IsRight = To.x > 0;
					IsUp = To.y > 0;
				}
			}
		}

		if (IsRight) {
			Vec.x = Mathf::Clamp(Vec.x + 0.5f * FrameWork::Instance()->GetDeltaTime(), -0.25f, 0.25f);
		}
		else {
			Vec.x = Mathf::Clamp(Vec.x - 0.5f * FrameWork::Instance()->GetDeltaTime(), -0.25f, 0.25f);
		}

		if (IsUp) {
			Mathf::Easing(&Vec.y, IsBoost ? 3.f : 1.5f, 0.95f);
		}
		else {
			Mathf::Easing(&Vec.y, 0.75f, 0.95f);
		}
		e.SetVec(Vec);
		//
		Mathf::Vector3 Vector = m_Characters.back().GetPosition() - e.GetGunPos();
		bool ShotSubKey = (0.5f < Vector.Length()) && (Vector.Length() < 1.f);
		if (Vector.y > 0.f) {
			ShotSubKey = (0.1f < Vector.Length()) && (Vector.Length() < 0.25f);
		}
		e.SetGunRad(std::atan2f(Vector.x, Vector.y));

		auto& interval = m_ShotInterval.at(&e - &m_Characters.front());
		if (ShotSubKey && interval == 0.f) {
			interval = static_cast<float>(GetRand(100)) / 100.f + 1.f;
			if (e.SetBullet(1, e.GetGunPos(), Vector.Nomalize() * 1.f)) {
				PlaySoundMem(m_ShotSE.at(m_ShotSENow), DX_PLAYTYPE_BACK);
				++m_ShotSENow %= static_cast<int>(m_ShotSE.size());
			}
		}
		interval = Mathf::Max(interval - FrameWork::Instance()->GetDeltaTime(), 0.f);

		m_MainTimer += FrameWork::Instance()->GetDeltaTime();
	}
	for (auto& e : m_Characters) {
		for (auto& b : e.GetBulletList()) {
			if (b.IsActive()) {
				SEGMENT_SEGMENT_RESULT Ret;
				for (auto& e2 : m_Characters) {
					if (&e == &e2) { continue; }
					if (!e2.IsAlive()) { continue; }
					Mathf::GetSegmenttoSegment(b.GetRePos(), b.GetPosition(), e2.GetRePos(), e2.GetPosition(), &Ret);
					float len = 0.025f;
					if (Ret.SegA_SegB_MinDist_Square <= (len * len)) {
						Mathf::Vector3 Pos(Ret.SegA_MinDist_Pos.x, Ret.SegA_MinDist_Pos.y, Ret.SegA_MinDist_Pos.z);
						if (e2.CanDamage()) {
							e2.SetDamage(34);
							if (e2.IsAlive()) {
								PlaySoundMem(m_DamageSE.at(m_DamageSENow), DX_PLAYTYPE_BACK);
								++m_DamageSENow %= static_cast<int>(m_DamageSE.size());
							}
							else {
								PlaySoundMem(m_DeathSE.at(m_DeathSENow), DX_PLAYTYPE_BACK);
								++m_DeathSENow %= static_cast<int>(m_DeathSE.size());
							}
						}
						b.DisActive();
						m_HitEffect.SetHitEffect(Pos + e2.GetVec().Nomalize() * 0.1f);
						break;
					}
				}
			}
		}
	}
	for (auto& e : m_Characters) {
		if (!e.IsAlive()) {
			if (e.GetPosition().z > 1.f) {
				auto& interval = m_DeathEffectInterval.at(&e - &m_Characters.front());
				if (interval == 0.f) {
					interval = 0.1f;
					m_HitEffect.SetHitEffect(e.GetPosition() + e.GetVec().Nomalize() * 0.1f);
				}
			}
			if (e.GetPosition().z <= 0.f) {
				auto& interval = m_DeathEffectFlag.at(&e - &m_Characters.front());
				if (interval == false) {
					interval = true;
					m_DeathEffect.SetDeathEffect(e.GetPosition() + e.GetVec().Nomalize() * 0.1f);
				}
			}
		}
		else {
			auto& interval = m_DeathEffectFlag.at(&e - &m_Characters.front());
			interval = false;
		}
		{
			auto& interval = m_DeathEffectInterval.at(&e - &m_Characters.front());
			interval = Mathf::Max(interval - FrameWork::Instance()->GetDeltaTime(), 0.f);
		}
	}
	m_Sonic.Update();
	m_DeathEffect.Update();
	m_HitEffect.Update();

	for (auto& e : m_Characters) {
		if (e.CanRespawn() && !e.IsAlive()) {
			e.SetDamage(-MaxHP);
			Mathf::Vector3 Pos = CamPos + Mathf::Vector3(0.f, -1.f, 0.f);
			Pos.z = 20.f;
			e.SetPos(Pos);
		}
		e.Update();
	}
	{
		Mathf::Vector3 Pos = m_Characters.back().GetPosition();
		Pos.x -= 0.75f;
		Pos.y -= 0.15f;

		Mathf::Easing(&CamPos, Pos, 0.9f);
		CamPos.z = 10.f;
	}
}

void MainGame::DrawMain() {
	DrawBox(0, 0, FrameWork::Instance()->GetScreenWidth(), FrameWork::Instance()->GetScreenHeight(), GetColor(192, 0, 0), TRUE);

	//
	for (int loop = -5; loop < 20; ++loop) {
		float timeTemp = static_cast<float>(loop + (int)(CamPos.y) * 3.f) / 3.f;

		Mathf::Vector3 P1 = GetDisplayPoint(0.f, timeTemp - CamPos.y, 0.f - CamPos.z);
		Mathf::Vector3 P2 = GetDisplayPoint(2.f, timeTemp - CamPos.y, 0.f - CamPos.z);
		Mathf::Vector3 P3 = GetDisplayPoint(2.f, timeTemp - CamPos.y + 0.1f, 0.f - CamPos.z);
		DrawTriangle(static_cast<int>(P1.x), static_cast<int>(P1.y), static_cast<int>(P2.x), static_cast<int>(P2.y), static_cast<int>(P3.x), static_cast<int>(P3.y), GetColor(0, 0, 0), TRUE);
	}

	//
	for (int loop = -5; loop < 20; ++loop) {
		float timeTemp = (static_cast<float>(loop + (int)(CamPos.y) * 3.f) + 0.5f) / 3.f;

		Mathf::Vector3 P1 = GetDisplayPoint(0.f, timeTemp - CamPos.y, 0.f - CamPos.z);
		Mathf::Vector3 P2 = GetDisplayPoint(2.f, timeTemp - CamPos.y, 0.f - CamPos.z);
		Mathf::Vector3 P3 = GetDisplayPoint(2.f, timeTemp - CamPos.y + 0.1f, 0.f - CamPos.z);
		DrawTriangle(static_cast<int>(P1.x), static_cast<int>(P1.y), static_cast<int>(P2.x), static_cast<int>(P2.y), static_cast<int>(P3.x), static_cast<int>(P3.y), GetColor(255, 0, 0), TRUE);
	}

	for (auto& e : m_Characters) {
		e.DrawShadow();
	}


	for (auto& e : m_Characters) {
		e.Draw();
	}

	m_Sonic.Draw();
	m_DeathEffect.Draw();
	m_HitEffect.Draw();

}

void MainGame::DrawUI() {
	if (!m_Characters.back().IsAlive()) { return; }

	DrawRotaGraph(960 - 32, 720 - 32, 1.0, 0.0, m_gauge, TRUE);
	DrawRotaGraph(960 - 32, 720 - 32, 1.0, static_cast<double>(Mathf::Deg2Rad((m_BoostMeterRand -1.f) * 90.f)), m_meter, TRUE);
	if (!m_BoostActive && (static_cast<int>(m_MainTimer *10)%10<5)) {
		DrawFormatString2ToHandle(960 - 32 - 128 + 32, 720 - 32 - 128 + 64, GetColor(255, 0, 0), GetColor(0, 0, 0), m_Font, "OVER HEAT!");
	}
	/*
	DrawBox(0, 0, 255, 255, GetColor(0, 255, 0), TRUE);
	DrawRotaGraph(128, 128, 1.0, 0.0, m_gauge, TRUE);
	for (int loop = 0; loop < 90; loop++) {
		if (loop % 5 == 0) {
			int x1 = 128 - sin(Mathf::Deg2Rad(loop)) * (128 - 5);
			int y1 = 128 - cos(Mathf::Deg2Rad(loop)) * (128 - 5);
			int x2 = 128 - sin(Mathf::Deg2Rad(loop)) * (128 - 10);
			int y2 = 128 - cos(Mathf::Deg2Rad(loop)) * (128 - 10);
			DrawLine(x1, y1, x2, y2, GetColor(0, 0, 0), 1);
		}
		if (loop % 30 == 0) {
			int x1 = 128 - sin(Mathf::Deg2Rad(loop)) * (128 - 5);
			int y1 = 128 - cos(Mathf::Deg2Rad(loop)) * (128 - 5);
			int x2 = 128 - sin(Mathf::Deg2Rad(loop)) * (128 - 15);
			int y2 = 128 - cos(Mathf::Deg2Rad(loop)) * (128 - 15);
			DrawLine(x1, y1, x2, y2, GetColor(0, 0, 0), 1);
		}
	}
	//*/
}
