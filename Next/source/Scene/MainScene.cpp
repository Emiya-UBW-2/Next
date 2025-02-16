#include "MainScene.hpp"

const MainCamera* MainCamera::m_Singleton = nullptr;


void MainGame::InitSub() {
	MainCamera::Create();
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

	CamPosBuf = m_Characters.back().GetPosition();
	CamPosBuf.z = 10.f;
	MainCamera::Instance()->SetCamPos(CamPosBuf);

	m_EffectControl.Init();

	for (int loop = 0; loop < m_BlockPos.size(); ++loop) {
		m_BlockPos.at(loop) = static_cast<float>(GetRand(200) - 100) / 100.f;
	}

	m_gauge.LoadGraph("data/UI/boostgauge.bmp", true);
	m_meter.LoadGraph("data/UI/boostmeter.bmp", true);
	m_FinImage.LoadGraph("data/UI/Fin.png", true);

	m_Font.Create("Agency FB", 12, -1, DX_FONTTYPE_ANTIALIASING_EDGE, DX_CHARSET_DEFAULT, 1);
	m_FontBig.Create("Agency FB", 24, -1, DX_FONTTYPE_ANTIALIASING_EDGE, DX_CHARSET_DEFAULT, 1);
	m_FontResult.Create("BIZ UDゴシック", 24, -1, DX_FONTTYPE_ANTIALIASING_EDGE, DX_CHARSET_DEFAULT, 1);


	for (auto& s : m_ShotSE) {
		s.Create("data/Audio/Shot.wav");
	}
	for (auto& s : m_DamageSE) {
		s.Create("data/Audio/Damage.wav");
	}
	for (auto& s : m_DeathSE) {
		s.Create("data/Audio/Death.wav");
	}
	m_ScoreSE.Create("data/Audio/Score.wav");

	m_BGM.Create("data/Audio/BGM.wav", DX_SOUNDDATATYPE_FILE);

	PlaySoundMem(m_BGM.GetHandle(), DX_PLAYTYPE_BACK);

	m_MainTimer = m_TotalTimer + 2.f;

	InitResult();

	BaseScene::SetNextSceneID(static_cast<SceneID>(EnumSceneID::Title));
}
void MainGame::UpdateSub() {
	if (m_MainTimer <= -3.5f) {
		UpdateResult();
		return;
	}


	if (m_Characters.back().IsAlive() && m_MainTimer > 0.f && (m_MainTimer < m_TotalTimer)) {
		Mathf::Vector3 Vec = m_Characters.back().GetVec();

		if (InputControl::Instance()->GetWKey().IsPress() && m_BoostActive) {
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

		if (InputControl::Instance()->GetAKey().IsPress()) {
			Vec.x = Mathf::Clamp(Vec.x - 0.5f * FrameWork::Instance()->GetDeltaTime(), -0.25f, 0.25f);
		}
		if (InputControl::Instance()->GetSKey().IsPress()) {
			Vec.x = Mathf::Clamp(Vec.x + 0.5f * FrameWork::Instance()->GetDeltaTime(), -0.25f, 0.25f);
		}
		if (!InputControl::Instance()->GetAKey().IsPress() && !InputControl::Instance()->GetSKey().IsPress()) {
			Mathf::Easing(&Vec.x, 0.f, 0.95f);
		}

		if (InputControl::Instance()->GetSKey().IsPress()) {
			Vec.y = Mathf::Clamp(Vec.y - 1.f * FrameWork::Instance()->GetDeltaTime(), 0.85f, 1.5f);
		}
		if (InputControl::Instance()->GetWKey().IsPress() && m_BoostActive) {
			Vec.y = Mathf::Clamp(Vec.y + 1.f * FrameWork::Instance()->GetDeltaTime(), 0.85f, 1.5f);
		}
		if (!InputControl::Instance()->GetSKey().IsPress() && !(InputControl::Instance()->GetWKey().IsPress() && m_BoostActive)) {
			Mathf::Easing(&Vec.y, 1.f, 0.95f);
		}

		if (InputControl::Instance()->GetMainShotKey().IsPress()) {
			if (m_Characters.back().SetBullet(0, m_Characters.back().GetPosition(), m_Characters.back().GetVec().Nomalize() * 2.f)) {
				PlaySoundMem(m_ShotSE.at(m_ShotSENow).GetHandle(), DX_PLAYTYPE_BACK);
				++m_ShotSENow %= static_cast<int>(m_ShotSE.size());
				m_ShotScore++;
			}
		}

		Mathf::Vector3 MP1(static_cast<float>(InputControl::Instance()->GetMouseX() + 16), static_cast<float>(InputControl::Instance()->GetMouseY() + 16), 0.f);

		Mathf::Vector3 PG = m_Characters.back().GetGunPos();
		Mathf::Vector3 P1 = MainCamera::Instance()->GetDisplayPoint(PG);
		Mathf::Vector3 P1X = MainCamera::Instance()->GetDisplayPoint(PG + Mathf::Vector3(1.0f, 0.0f, 0.0f));
		Mathf::Vector3 P1Z = MainCamera::Instance()->GetDisplayPoint(PG + Mathf::Vector3(0.0f, 1.0f, 0.0f));

		Mathf::Vector3 Vector;
		Vector.x = Mathf::Vector3::Dot(Mathf::Vector3::Cross((MP1 - P1).Nomalize(), (P1Z - P1).Nomalize()), Mathf::Vector3::Cross((P1X - P1).Nomalize(), (P1Z - P1).Nomalize()));//Sin
		Vector.y = Mathf::Vector3::Dot((MP1 - P1).Nomalize(), (P1Z - P1).Nomalize());//Cos
		Vector.z = 0.f;

		m_Characters.back().SetGunRad(std::atan2f(Vector.x, Vector.y));
		if (InputControl::Instance()->GetSubShotKey().IsPress()) {
			if (m_Characters.back().SetBullet(1, PG, Vector.Nomalize() * 1.f)) {
				PlaySoundMem(m_ShotSE.at(m_ShotSENow).GetHandle(), DX_PLAYTYPE_BACK);
				++m_ShotSENow %= static_cast<int>(m_ShotSE.size());
				m_ShotScore++;
			}
		}

		m_Characters.back().SetVec(Vec);

		if (InputControl::Instance()->GetWKey().IsPress() && m_BoostActive) {
			if (InputControl::Instance()->GetWKey().IsTrigger()) {
				m_SonicTimer = 0.f;
			}
			else {
				m_SonicTimer += FrameWork::Instance()->GetDeltaTime();
			}
			if (m_SonicTimer > 0.025f) {
				m_SonicTimer -= 0.025f;
				m_EffectControl.SetEffect(EnumEffect::Sonic, m_Characters.back().GetPosition());
			}
		}
	}
	else {
		m_BoostTimer = 1.f;
		m_BoostActive = true;
	}
	Mathf::Easing(&m_BoostMeterRand, m_BoostTimer + static_cast<float>(GetRand(200) - 100) / 100.f * 0.1f, 0.9f);

	for (auto& e : m_Characters) {
		if ((&e - &m_Characters.front()) == (m_Characters.size() - 1)) { continue; }
		if (!(e.IsAlive() && m_MainTimer > 0.f)) { continue; }
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
				PlaySoundMem(m_ShotSE.at(m_ShotSENow).GetHandle(), DX_PLAYTYPE_BACK);
				++m_ShotSENow %= static_cast<int>(m_ShotSE.size());
			}
		}
		interval = Mathf::Max(interval - FrameWork::Instance()->GetDeltaTime(), 0.f);

	}

	m_MainTimer -= FrameWork::Instance()->GetDeltaTime();

	if (m_MainTimer > 0.f) {
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
								e2.SetDamage(b.GetDamage());
								if ((&e - &m_Characters.front()) == (m_Characters.size() - 1)) {
									m_HitScore++;
								}
								//
								if ((&e2 - &m_Characters.front()) == (m_Characters.size() - 1)) {
									m_DamageCoolTime = 1.f;
								}

								if (e2.IsAlive()) {
									PlaySoundMem(m_DamageSE.at(m_DamageSENow).GetHandle(), DX_PLAYTYPE_BACK);
									++m_DamageSENow %= static_cast<int>(m_DamageSE.size());
								}
								else {
									if ((&e - &m_Characters.front()) == (m_Characters.size() - 1)) {
										m_KillScore++;
									}

									PlaySoundMem(m_DeathSE.at(m_DeathSENow).GetHandle(), DX_PLAYTYPE_BACK);
									++m_DeathSENow %= static_cast<int>(m_DeathSE.size());
								}
							}
							b.DisActive();
							m_EffectControl.SetEffect(EnumEffect::Hit, Pos + e2.GetVec().Nomalize() * 0.1f);
							break;
						}
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
					m_EffectControl.SetEffect(EnumEffect::Hit, e.GetPosition() + e.GetVec().Nomalize() * 0.1f);
				}
			}
			if (e.GetPosition().z <= 0.f) {
				auto& interval = m_DeathEffectFlag.at(&e - &m_Characters.front());
				if (interval == false) {
					interval = true;
					m_EffectControl.SetEffect(EnumEffect::Death, e.GetPosition() + e.GetVec().Nomalize() * 0.1f);
					PlaySoundMem(m_DeathSE.at(m_DeathSENow).GetHandle(), DX_PLAYTYPE_BACK);
					++m_DeathSENow %= static_cast<int>(m_DeathSE.size());
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
	m_EffectControl.Update();

	for (auto& e : m_Characters) {
		if (e.CanRespawn() && !e.IsAlive()) {
			e.SetDamage(-MaxHP);
			Mathf::Vector3 Pos = MainCamera::Instance()->GetCamPos() + Mathf::Vector3(0.f, -1.f, 0.f);
			Pos.z = 20.f;
			e.SetPos(Pos);
			if ((&e - &m_Characters.front()) == (m_Characters.size() - 1)) {
				m_RespawnScore++;
			}
		}
		e.Update();
	}
	{
		auto Pos = MainCamera::Instance()->GetCamPos();
		if (m_MainTimer > 0.f) {
			CamPosBuf = m_Characters.back().GetPosition();
			CamPosBuf.x -= 0.75f;
			CamPosBuf.y -= 0.15f;
		}
		CamPosBuf.z = 10.f;
		Mathf::Easing(&Pos, CamPosBuf, 0.9f);
		MainCamera::Instance()->SetCamPos(Pos);
	}

	m_DamageCoolTime = Mathf::Max(m_DamageCoolTime - FrameWork::Instance()->GetDeltaTime(), 0.f);
	if (m_DamageCoolTime == 0.f) {
		m_CureCoolTime += FrameWork::Instance()->GetDeltaTime();
		if (m_CureCoolTime > 0.1f) {
			m_CureCoolTime -= 0.1f;
			if (m_Characters.back().IsAlive()) {
				m_Characters.back().SetDamage(-MaxHP / 100);
			}
		}
	}
	else {
		m_CureCoolTime = 0.f;
	}
	if (m_DamageCoolTime == 0.f) {
		if (std::abs(m_HitPointRe - m_Characters.back().GetHP()) > 1.f) {
			if (m_HitPointRe > m_Characters.back().GetHP()) {
				m_HitPointRe -= FrameWork::Instance()->GetDeltaTime() * 100.f;
			}
			else {
				m_HitPointRe += FrameWork::Instance()->GetDeltaTime() * 100.f;
			}
		}
		else {
			m_HitPointRe = static_cast<float>(m_Characters.back().GetHP());
		}
	}
}
void MainGame::DrawSub() {
	DrawMain();
	if (m_MainTimer >= -3.5f) {
		DrawUI();
	}
	else {
		DrawResult();
	}
}
void MainGame::DisposeSub()
{
	MainCamera::Release();
	for (auto& e : m_Characters) {
		e.Dispose();
	}

	m_EffectControl.Dispose();

	m_gauge.ReleaseGraph();
	m_meter.ReleaseGraph();
	m_FinImage.ReleaseGraph();

	m_Font.ReleaseFont();
	m_FontBig.ReleaseFont();
	m_FontResult.ReleaseFont();
	for (auto& s : m_ShotSE) {
		s.ReleaseSound();
	}
	for (auto& s : m_DamageSE) {
		s.ReleaseSound();
	}
	for (auto& s : m_DeathSE) {
		s.ReleaseSound();
	}
	m_ScoreSE.ReleaseSound();
	m_BGM.ReleaseSound();
}
void MainGame::DrawMain() {
	DrawBox(0, 0, FrameWork::Instance()->GetScreenWidth(), FrameWork::Instance()->GetScreenHeight(), (IsTriMonoMode) ? GetColor(255, 0, 0) : GetColor(192, 0, 0), TRUE);

	//
	for (int loop = -5; loop < 20; ++loop) {
		float timeTemp = static_cast<float>(loop + (int)(MainCamera::Instance()->GetCamPos().y) * 3.f) / 3.f;

		Mathf::Vector3 P1 = MainCamera::Instance()->GetDisplayPoint(MainCamera::Instance()->GetCamPos().x, timeTemp, 0.f);
		Mathf::Vector3 P2 = MainCamera::Instance()->GetDisplayPoint(MainCamera::Instance()->GetCamPos().x + 2.f, timeTemp, 0.f);
		Mathf::Vector3 P3 = MainCamera::Instance()->GetDisplayPoint(MainCamera::Instance()->GetCamPos().x + 2.f, timeTemp + 0.1f, 0.f);
		DrawTriangle(static_cast<int>(P1.x), static_cast<int>(P1.y), static_cast<int>(P2.x), static_cast<int>(P2.y), static_cast<int>(P3.x), static_cast<int>(P3.y), GetColor(0, 0, 0), TRUE);
	}

	//
	for (int loop = -5; loop < 20; ++loop) {
		float timeTemp = (static_cast<float>(loop + (int)(MainCamera::Instance()->GetCamPos().y) * 3.f) + 0.5f) / 3.f;

		Mathf::Vector3 P1 = MainCamera::Instance()->GetDisplayPoint(MainCamera::Instance()->GetCamPos().x, timeTemp, 0.f);
		Mathf::Vector3 P2 = MainCamera::Instance()->GetDisplayPoint(MainCamera::Instance()->GetCamPos().x + 2.f, timeTemp, 0.f);
		Mathf::Vector3 P3 = MainCamera::Instance()->GetDisplayPoint(MainCamera::Instance()->GetCamPos().x + 2.f, timeTemp + 0.1f, 0.f);
		DrawTriangle(static_cast<int>(P1.x), static_cast<int>(P1.y), static_cast<int>(P2.x), static_cast<int>(P2.y), static_cast<int>(P3.x), static_cast<int>(P3.y), GetColor(255, 0, 0), TRUE);
	}

	for (auto& e : m_Characters) {
		e.DrawShadow();
	}


	for (auto& e : m_Characters) {
		e.Draw();
	}

	m_EffectControl.Draw();
}
void MainGame::DrawUI() {
	if (m_MainTimer < 0.f) { return; }

	if (m_MainTimer < m_TotalTimer) {
		DrawFormatString2ToHandle(32, 32, GetColor(255, 255, 255), GetColor(0, 0, 0), m_FontBig.GetHandle(), "%d:%02d.%02d", static_cast<int>(m_MainTimer / 60.f), static_cast<int>(m_MainTimer) % 60, static_cast<int>(m_MainTimer * 100) % 60);
	}

	if (!m_Characters.back().IsAlive()) { return; }

	{
		int x1 = 32;
		int y1 = FrameWork::Instance()->GetScreenHeight() / 2 - 256 / 2;
		int x2 = x1 + 16;
		int y2 = y1 + 256;
		DrawBox(x1, static_cast<int>(y2 - (y2 - y1) * m_HitPointRe / MaxHP), x2, y2, GetColor(255, 0, 0), TRUE);
		DrawBox(x1, static_cast<int>(y2 - (y2 - y1) * m_Characters.back().GetHP() / MaxHP), x2, y2, GetColor(255, 255, 255), TRUE);
		DrawBox(x1, y1, x2, y2, GetColor(0, 0, 0), FALSE);
		DrawBox(x1 + 1, y1 + 1, x2 - 1, y2 - 1, GetColor(0, 0, 0), FALSE);

		DrawFormatString2ToHandle(x1, y2, GetColor(255, 0, 0), GetColor(0, 0, 0), m_Font.GetHandle(), "HP");
	}

	DrawRotaGraph(960 - 32, 720 - 32, 1.0, 0.0, m_gauge.GetHandle(), TRUE);
	DrawRotaGraph(960 - 32, 720 - 32, 1.0, static_cast<double>(Mathf::Deg2Rad((m_BoostMeterRand - 1.f) * 90.f)), m_meter.GetHandle(), TRUE);
	if (!m_BoostActive && (static_cast<int>(m_MainTimer * 100) % 10 < 5)) {
		DrawFormatString2ToHandle(960 - 32 - 128 + 32, 720 - 32 - 128 + 64, GetColor(255, 0, 0), GetColor(0, 0, 0), m_Font.GetHandle(), "OVER HEAT!");
	}
	DrawFormatString2ToHandle(32, FrameWork::Instance()->GetScreenHeight() - 32, GetColor(255, 255, 255), GetColor(0, 0, 0), m_Font.GetHandle(), "AD : 旋回　W : ブースト S : 減速 スペース : 前方射撃 左クリック : 旋回機銃射撃");
}
void MainGame::InitResult()
{
	m_Respawn = 0.f;
	m_Kill = 0.f;
	m_HitRatio = 0.f;
	m_ResultClear = -1;
	//
	m_RespawnScore = 0;
	m_KillScore = 0;
	m_HitScore = 0;
	m_ShotScore = 0;
	for (auto& s : m_ResultAnim) {
		s = -1000.f;
	}
}
void MainGame::UpdateResult() {
	if (m_ResultClear == -1) {
		PlaySoundMem(m_ScoreSE.GetHandle(), DX_PLAYTYPE_LOOP);
		m_ResultClear++;
	}
	if (m_ResultClear == 0) {
		Mathf::Easing(&m_Respawn, static_cast<float>(m_RespawnScore), 0.9f);
		if (std::abs(m_Respawn - static_cast<float>(m_RespawnScore)) < 1.f) {
			m_Respawn = static_cast<float>(m_RespawnScore);
			if (m_ResultTimer == 0.f) {
				StopSoundMem(m_ScoreSE.GetHandle());
			}
			m_ResultTimer += FrameWork::Instance()->GetDeltaTime();
			if (m_ResultTimer > 0.5f) {
				m_ResultTimer = 0.f;
				PlaySoundMem(m_ScoreSE.GetHandle(), DX_PLAYTYPE_LOOP);
				m_ResultClear++;
			}
		}
	}
	if (m_ResultClear == 1) {
		float HitRatio = static_cast<float>((m_ShotScore == 0) ? 0 : (100 * m_HitScore / m_ShotScore));
		Mathf::Easing(&m_HitRatio, HitRatio, 0.9f);
		if (std::abs(m_HitRatio - HitRatio) < 1.f) {
			m_HitRatio = HitRatio;
			if (m_ResultTimer == 0.f) {
				StopSoundMem(m_ScoreSE.GetHandle());
			}
			m_ResultTimer += FrameWork::Instance()->GetDeltaTime();
			if (m_ResultTimer > 0.5f) {
				m_ResultTimer = 0.f;
				PlaySoundMem(m_ScoreSE.GetHandle(), DX_PLAYTYPE_LOOP);
				m_ResultClear++;
			}
		}
	}
	if (m_ResultClear == 2) {
		Mathf::Easing(&m_Kill, static_cast<float>(m_KillScore), 0.9f);
		if (std::abs(m_Kill - static_cast<float>(m_KillScore)) < 1.f) {
			m_Kill = static_cast<float>(m_KillScore);
			if (m_ResultTimer == 0.f) {
				StopSoundMem(m_ScoreSE.GetHandle());
			}
			m_ResultTimer += FrameWork::Instance()->GetDeltaTime();
			if (m_ResultTimer > 0.5f) {
				m_ResultTimer = 0.f;
				PlaySoundMem(m_ScoreSE.GetHandle(), DX_PLAYTYPE_LOOP);
				m_ResultClear++;
			}
		}
	}
	if (m_ResultClear >= 3) {
		if (m_ResultTimer == 0.f) {
			StopSoundMem(m_ScoreSE.GetHandle());
		}
		m_ResultTimer += FrameWork::Instance()->GetDeltaTime();
	}
	if (m_ResultClear == 3) {
		if (m_ResultTimer > 0.5f) {
			//PlaySoundMem(m_ScoreSE, DX_PLAYTYPE_LOOP);
			m_ResultClear = 4;
		}
	}
	if (m_ResultClear == 4 && InputControl::Instance()->GetMenuEnter().IsTrigger()) {
		BaseScene::SetSceneEnd();
	}


	for (int loop = 0; loop < m_ResultAnim.size(); ++loop) {
		if (m_ResultClear >= loop) {
			Mathf::Easing(&m_ResultAnim.at(loop), 0.f, 0.9f);
		}
	}
}
void MainGame::DrawResult()
{
	DrawBox(0, 0, FrameWork::Instance()->GetScreenWidth(), FrameWork::Instance()->GetScreenHeight(), GetColor(0, 0, 0), TRUE);
	DrawGraph(FrameWork::Instance()->GetScreenWidth() - 128 - 32, FrameWork::Instance()->GetScreenHeight() - 64 - 32, m_FinImage.GetHandle(), FALSE);

	int num = 0;
	if (m_ResultClear >= num) {
		DrawFormatString2ToHandle(64 + static_cast<int>(m_ResultAnim[num]), 64 + num * 32, GetColor(255, 255, 255), GetColor(0, 0, 0), m_FontResult.GetHandle(), "Respawn Count : %02d", static_cast<int>(m_Respawn)); num++;
	}
	if (m_ResultClear >= num) {
		DrawFormatString2ToHandle(64 + static_cast<int>(m_ResultAnim[num]), 64 + num * 32, GetColor(255, 255, 255), GetColor(0, 0, 0), m_FontResult.GetHandle(), "Hit Ratio : %02d%%", static_cast<int>(m_HitRatio)); num++;
	}
	if (m_ResultClear >= num) {
		DrawFormatString2ToHandle(64 + static_cast<int>(m_ResultAnim[num]), 64 + num * 32, GetColor(255, 255, 255), GetColor(0, 0, 0), m_FontResult.GetHandle(), "Kill : %02d", static_cast<int>(m_Kill)); num++;
	}
	num++;
	if (m_ResultClear >= num) {
		std::string Result = "";

		if (m_HitRatio > 60.f && m_Kill > 5.f && m_Respawn < 3.f) {
			Result = "S";
		}
		else if (m_HitRatio > 50.f && m_Kill > 4.f && m_Respawn < 5.f) {
			Result = "A";
		}
		else if (m_HitRatio > 40.f && m_Kill > 3.f && m_Respawn < 7.f) {
			Result = "B";
		}
		else {
			Result = "C";
		}
		DrawFormatString2ToHandle(64 + static_cast<int>(m_ResultAnim[num]), 64 + num * 32, GetColor(255, 255, 255), GetColor(0, 0, 0), m_FontResult.GetHandle(), "Rank : %s", Result.c_str()); num++;
	}

	if (m_ResultClear == 4 && (m_ResultTimer > 0.5f + 1.f) && (static_cast<int>(m_ResultTimer * 10) % 10 < 5)) {
		const char* Str = "Space :Return Title";
		int Width = GetDrawStringWidthToHandle(Str, static_cast<int>(strlenDx(Str)), m_FontBig.GetHandle());
		DrawFormatString2ToHandle(FrameWork::Instance()->GetScreenWidth() / 2 - Width / 2, FrameWork::Instance()->GetScreenHeight() * 3 / 4, GetColor(255, 255, 255), GetColor(0, 0, 0), m_FontBig.GetHandle(), Str);
	}

}

void EffectPositionData::Update() {
	this->Size += FrameWork::Instance()->GetDeltaTime();
}
void EffectPositionData::Draw() const {
	switch (EffectType) {
	case EnumEffect::Smoke:
		break;
	case EnumEffect::Sonic:
	{
		if (!IsTriMonoMode) {
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(32.f / this->Size));
		}
		Mathf::Vector3 P1 = MainCamera::Instance()->GetDisplayPoint(this->Pos);
		DrawRotaGraph3(static_cast<int>(P1.x), static_cast<int>(P1.y), 640 / 2, 640 / 2, double(this->Size), double(this->Size / 2), double(Mathf::Deg2Rad(30)), m_Screen.GetHandle(), TRUE);
		if (!IsTriMonoMode) {
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		}
	}
	break;
	case EnumEffect::Death:
	{
		if (!IsTriMonoMode) {
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(32.f / this->Size));
		}
		Mathf::Vector3 P1 = MainCamera::Instance()->GetDisplayPoint(this->Pos);
		DrawRotaGraph3(static_cast<int>(P1.x), static_cast<int>(P1.y), 640 / 2, 640 / 2, double(this->Size), double(this->Size * 0.8f), double(Mathf::Deg2Rad(30)), m_Screen.GetHandle(), TRUE);
		if (!IsTriMonoMode) {
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		}
	}
	break;
	case EnumEffect::Hit:
	{
		float alpha = std::sin(Mathf::Deg2Rad(this->Size / 0.5f * 180.f));
		float scale = this->Size * 0.15f / 0.5f;// std::sin(Mathf::Deg2Rad(this->Size / 0.5f * 180.f)) * 0.25f;
		if (!IsTriMonoMode) {
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255.f * alpha));
		}
		Mathf::Vector3 P1 = MainCamera::Instance()->GetDisplayPoint(this->Pos);
		DrawRotaGraph3(static_cast<int>(P1.x), static_cast<int>(P1.y), 640 / 2, 640 / 2, double(scale), double(scale), double(this->Rad + Mathf::Deg2Rad(30 + this->Rad * this->Size * 3.f * 180.f)), m_Screen2.GetHandle(), TRUE);
		if (!IsTriMonoMode) {
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		}
	}
	break;
	default:
		break;
	}
}

void Smoke::Update(const Mathf::Vector3& Pos, float Per) {
	for (auto& s : m_SmokePoint) {
		s.Size += FrameWork::Instance()->GetDeltaTime();
	}
	m_SmokePoint.at(m_SmokePointNum).Pos = Pos;
	m_SmokePoint.at(m_SmokePointNum).Size = 0.f;
	m_SmokePoint.at(m_SmokePointNum).Alpha = Per;
	++m_SmokePointNum %= static_cast<int>(m_SmokePoint.size());
}
void Smoke::DrawShadow() const {
	for (int loop = 0; loop < static_cast<int>(m_SmokePoint.size()); ++loop) {
		auto& s1 = m_SmokePoint.at(static_cast<size_t>(loop));
		auto& s2 = m_SmokePoint.at(static_cast<size_t>((loop + 1) % static_cast<int>(m_SmokePoint.size())));
		if (s1.Size == 0.f || s1.Size > 3.f) { continue; }
		Mathf::Vector3 P1 = MainCamera::Instance()->GetDisplayPoint(s1.Pos.x, s1.Pos.y, 0.f);
		Mathf::Vector3 P2 = MainCamera::Instance()->GetDisplayPoint(s2.Pos.x, s2.Pos.y, 0.f);
		if (!IsTriMonoMode) {
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255.f * std::sin(Mathf::Deg2Rad(s1.Size * 180.f)) * s1.Alpha));
		}
		DrawLine(static_cast<int>(P1.x), static_cast<int>(P1.y), static_cast<int>(P2.x), static_cast<int>(P2.y), GetColor(0, 0, 0), static_cast<int>(s1.Size * 2));
	}
	if (!IsTriMonoMode) {
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}
}
void Smoke::Draw() const {
	for (int loop = 0; loop < static_cast<int>(m_SmokePoint.size()); ++loop) {
		auto& s1 = m_SmokePoint.at(static_cast<size_t>(loop));
		auto& s2 = m_SmokePoint.at(static_cast<size_t>((loop + 1) % static_cast<int>(m_SmokePoint.size())));
		if (s1.Size == 0.f || s1.Size > 3.f) { continue; }
		if (!IsTriMonoMode) {
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255.f * std::sin(Mathf::Deg2Rad(s1.Size * 180.f)) * s1.Alpha));
		}
		Mathf::Vector3 P1 = MainCamera::Instance()->GetDisplayPoint(s1.Pos);
		Mathf::Vector3 P2 = MainCamera::Instance()->GetDisplayPoint(s2.Pos);
		DrawLine(static_cast<int>(P1.x), static_cast<int>(P1.y), static_cast<int>(P2.x), static_cast<int>(P2.y), GetColor(255, 255, 255), static_cast<int>(s1.Size * 10));
	}
	if (!IsTriMonoMode) {
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}
}

void Bullet::Update() {
	m_RePos = m_Pos;
	m_Pos += (m_BaseVec + m_AddVec) * FrameWork::Instance()->GetDeltaTime();
	m_Time += FrameWork::Instance()->GetDeltaTime();
	if (m_Pos.z < 0.f) {
		if (m_Wave == 0.f) {
			m_WavePoint = m_Pos;
		}
		m_Wave += FrameWork::Instance()->GetDeltaTime();
	}
}
void Bullet::DrawShadow() const {
	if (m_Wave > 0.f) { return; }
	Mathf::Vector3 PP = m_Pos - m_AddVec.Nomalize() * 0.003f * m_Size;
	Mathf::Vector3 P1 = MainCamera::Instance()->GetDisplayPoint(m_Pos.x, m_Pos.y, 0.f);
	Mathf::Vector3 P2 = MainCamera::Instance()->GetDisplayPoint(PP.x, PP.y, 0.f);
	DrawLine(static_cast<int>(P1.x), static_cast<int>(P1.y), static_cast<int>(P2.x), static_cast<int>(P2.y), GetColor(0, 0, 0), static_cast<int>(m_Size));
	DrawCircle(static_cast<int>(P1.x), static_cast<int>(P1.y), static_cast<int>(m_Size / 2 - 1), GetColor(0, 0, 0), TRUE);
	DrawCircle(static_cast<int>(P2.x), static_cast<int>(P2.y), static_cast<int>(m_Size / 2 - 1), GetColor(0, 0, 0), TRUE);
}
void Bullet::Draw() const {
	if (m_Wave > 0.f) {
		if (!IsTriMonoMode) {
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255.f / m_Wave));
		}

		Mathf::Vector3 P1 = MainCamera::Instance()->GetDisplayPoint(m_WavePoint.x, m_WavePoint.y, 0.f);
		DrawCircle(static_cast<int>(P1.x), static_cast<int>(P1.y), static_cast<int>(m_Wave * 30.f), GetColor(255, 255, 255), FALSE);

		if (!IsTriMonoMode) {
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		}
	}
	else {
		Mathf::Vector3 PP = m_Pos - m_AddVec.Nomalize() * 0.003f * m_Size;
		Mathf::Vector3 P1 = MainCamera::Instance()->GetDisplayPoint(m_Pos);
		Mathf::Vector3 P2 = MainCamera::Instance()->GetDisplayPoint(PP);
		DrawLine(static_cast<int>(P1.x), static_cast<int>(P1.y), static_cast<int>(P2.x), static_cast<int>(P2.y), GetColor(255, 255, 255), static_cast<int>(m_Size));
		DrawCircle(static_cast<int>(P1.x), static_cast<int>(P1.y), static_cast<int>(m_Size / 2 - 1), GetColor(255, 255, 255), TRUE);
		DrawCircle(static_cast<int>(P2.x), static_cast<int>(P2.y), static_cast<int>(m_Size / 2 - 1), GetColor(255, 255, 255), TRUE);
	}
}

void Character::Update() {
	m_RePos = m_Pos;
	if (m_Pos.z > 0.f) {

		m_Pos += m_Vec * FrameWork::Instance()->GetDeltaTime();

	}
	if (!IsAlive()) {
		m_Pos.z -= 10.f * FrameWork::Instance()->GetDeltaTime();
	}
	m_GunPos = m_Pos - m_Vec.Nomalize() * m_GunPosOffset;

	m_Rad = std::atan2f(m_Vec.x, m_Vec.y);

	Mathf::Vector3 Offset = Mathf::Vector3(-0.03f * -std::sin(-m_Rad), -0.03f * std::cos(-m_Rad), 0.f);

	float Per = 0.02f * 1.f / m_Vec.y;
	m_Smoke.at(0).Update(m_Pos + Offset + Mathf::Vector3(-Per * std::cos(-m_Rad), -Per * std::sin(-m_Rad), 0.f), IsAlive() ? 1.f : 0.f);
	m_Smoke.at(1).Update(m_Pos + Offset + Mathf::Vector3(Per * std::cos(-m_Rad), Per * std::sin(-m_Rad), 0.f), IsAlive() ? 1.f : 0.f);

	for (auto& s : ShotInterval) {
		s = Mathf::Max(s - FrameWork::Instance()->GetDeltaTime(), 0.f);
	}
	m_DamageTime = Mathf::Max(m_DamageTime - FrameWork::Instance()->GetDeltaTime(), 0.f);
	m_RespawnTime = Mathf::Max(m_RespawnTime - FrameWork::Instance()->GetDeltaTime(), 0.f);
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
void Character::DrawShadow() const {
	if (!IsAlive()) {
		if (m_Pos.z < 0.f) { return; }
	}
	for (auto& s : m_Smoke) {
		s.DrawShadow();
	}

	for (auto& b : m_Bullet) {
		if (b.IsActive()) {
			b.DrawShadow();
		}
	}

	if (!IsAlive()) {
		if (!IsTriMonoMode) {
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255.f * 2.f * m_Pos.z / 20.f));
		}
	}


	Mathf::Vector3 P1 = MainCamera::Instance()->GetDisplayPoint(m_Pos.x, m_Pos.y, 0.f);
	SetDrawBright(0, 0, 0);
	DrawRotaGraph(static_cast<int>(P1.x), static_cast<int>(P1.y), 1.0, static_cast<double>(m_Rad * 1.5f), m_GraphHandle.at(m_GraphAnim).GetHandle(), TRUE);
	SetDrawBright(255, 255, 255);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}
void Character::Draw() const {
	for (auto& s : m_Smoke) {
		s.Draw();
	}

	for (auto& b : m_Bullet) {
		if (b.IsActive()) {
			b.Draw();
		}
	}

	if (!IsAlive()) {
		if (!IsTriMonoMode) {
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255.f * 2.f * m_Pos.z / 20.f));
		}
	}

	if (CanDamage() || (static_cast<int>(m_DamageTime * 100) % 10 < 5)) {
		SetDrawBright(255, 255 * m_HitPoint / MaxHP, 255 * m_HitPoint / MaxHP);

		Mathf::Vector3 P2 = MainCamera::Instance()->GetDisplayPoint(m_GunPos);
		DrawRotaGraph(static_cast<int>(P2.x), static_cast<int>(P2.y), 1.0, static_cast<double>(m_GunRad + Mathf::Deg2Rad(-30.f)), m_SubHandle.GetHandle(), TRUE);

		Mathf::Vector3 P1 = MainCamera::Instance()->GetDisplayPoint(m_Pos);
		DrawRotaGraph(static_cast<int>(P1.x), static_cast<int>(P1.y), 1.0, static_cast<double>(m_Rad * 1.5f), m_GraphHandle.at(m_GraphAnim).GetHandle(), TRUE);
		SetDrawBright(255, 255, 255);
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}