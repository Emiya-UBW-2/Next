#include "MainScene.hpp"

#include "MainScene/Camera.hpp"
#include "MainScene/Effect.hpp"
#include "TitleScene.hpp"
//
void MainGame::InitSub() {
	MainCamera::Create();
	EffectControl::Create();
	BulletControl::Create();

	SoundPool::Instance()->Add(10, SoundType::SE, "data/Audio/Shot.wav");
	SoundPool::Instance()->Add(10, SoundType::SE, "data/Audio/Damage.wav");
	SoundPool::Instance()->Add(10, SoundType::SE, "data/Audio/Death.wav");
	SoundPool::Instance()->Add(1, SoundType::SE, "data/Audio/Score.wav");
	SoundPool::Instance()->Add(1, SoundType::BGM, "data/Audio/BGM.wav", DX_SOUNDDATATYPE_FILE);

	for (auto& e : m_Characters) {
		int index = static_cast<int>(&e - &m_Characters.front());
		Mathf::Vector3 Pos(static_cast<float>(GetRand(index + 1)) * 0.1f, static_cast<float>(index) * 0.5f, 20.f);
		if (index == (m_Characters.size() - 1)) {
			Pos.y += 1.5f;
			e.Init(index, Pos, "plane", 0.03f);
		}
		else {
			e.Init(index, Pos, "enemy", 0.01f);
		}
	}
	//
	auto& PlayerChara = m_Characters.back();
	//
	CamPosBuf = PlayerChara.GetPosition();
	CamPosBuf.z = 10.f;
	MainCamera::Instance()->SetCamPos(CamPosBuf);
	//
	m_gauge.LoadGraph("data/UI/boostgauge.bmp", true);
	m_meter.LoadGraph("data/UI/boostmeter.bmp", true);
	m_FinImage.LoadGraph("data/UI/Fin.png", true);
	//
	InitResult();
	InitPauseUI();
	//
	SoundPool::Instance()->Play(DX_PLAYTYPE_BACK, FALSE, SoundType::BGM, "data/Audio/BGM.wav", DX_SOUNDDATATYPE_FILE);
	m_MainTimer = m_TotalTimer + 2.f;
	FadeControl::Instance()->SetFadeOut(ColorPalette::Black, 1.f);
}
void MainGame::UpdateSub() {
	FrameWork::Instance()->SetPauseEnable(IsMainGame());
	if (IsResultActive()) {
		UpdateResult();
		return;
	}
	UpdatePauseUI();
	if (FrameWork::Instance()->GetIsPauseActive()) {
		SoundPool::Instance()->StopAll(SoundType::BGM, "data/Audio/BGM.wav", DX_SOUNDDATATYPE_FILE);
		return;
	}
	SoundPool::Instance()->Play(DX_PLAYTYPE_BACK, FALSE, SoundType::BGM, "data/Audio/BGM.wav", DX_SOUNDDATATYPE_FILE);
	auto& PlayerChara = m_Characters.back();
	for (auto& e : m_Characters) {
		if (IsMainGame()) {
			if ((e.GetID() == PlayerChara.GetID())) {
				//自機
				if (e.IsAlive()) {
					bool IsBoost = InputControl::Instance()->GetWKey().IsPress() && e.m_BoostActive;
					bool IsSlow = InputControl::Instance()->GetSKey().IsPress();

					bool IsRight = InputControl::Instance()->GetDKey().IsPress();
					bool IsLeft = InputControl::Instance()->GetAKey().IsPress();

					if (IsBoost) {
						e.m_BoostInterval = 0.5f;
						if (e.m_BoostActive) {
							e.m_BoostTimer = Mathf::Max(e.m_BoostTimer - FrameWork::Instance()->GetDeltaTime(), 0.f);
							if (e.m_BoostTimer == 0.f) {
								e.m_BoostActive = false;
							}
						}
					}
					else {
						e.m_BoostInterval = Mathf::Max(e.m_BoostInterval - FrameWork::Instance()->GetDeltaTime(), 0.f);
						if (e.m_BoostInterval == 0.f) {
							if (e.m_BoostActive) {
								e.m_BoostTimer = Mathf::Min(e.m_BoostTimer + FrameWork::Instance()->GetDeltaTime(), 1.f);
							}
							else {
								e.m_BoostTimer = Mathf::Min(e.m_BoostTimer + FrameWork::Instance()->GetDeltaTime() * 0.5f, 1.f);
							}
						}
						if (e.m_BoostTimer == 1.f) {
							e.m_BoostActive = true;
						}
					}

					e.SetLRVec(IsLeft, IsRight);
					Mathf::Vector3 Vec = e.GetVec();
					if (IsSlow) {
						Vec.y = Mathf::Clamp(Vec.y - 1.f * FrameWork::Instance()->GetDeltaTime(), 0.85f, 1.5f);
					}
					if (IsBoost) {
						Vec.y = Mathf::Clamp(Vec.y + 1.f * FrameWork::Instance()->GetDeltaTime(), 0.85f, 1.5f);
					}
					if (!IsSlow && !IsBoost) {
						Mathf::Easing(&Vec.y, 1.f, 0.95f);
					}
					e.SetVec(Vec);

					if (InputControl::Instance()->GetMainShotKey().IsPress()) {
						if (e.SetBullet(0, e.GetPosition(), e.GetVec().Nomalize() * 2.f)) {
							m_ShotScore++;
						}
					}

					Mathf::Vector3 MP1(static_cast<float>(InputControl::Instance()->GetMouseX()), static_cast<float>(InputControl::Instance()->GetMouseY()), 0.f);

					Mathf::Vector3 PG = e.GetGunPos();
					Mathf::Vector3 P1 = MainCamera::Instance()->GetDisplayPoint(PG);
					Mathf::Vector3 P1X = MainCamera::Instance()->GetDisplayPoint(PG + Mathf::Vector3(1.0f, 0.0f, 0.0f));
					Mathf::Vector3 P1Z = MainCamera::Instance()->GetDisplayPoint(PG + Mathf::Vector3(0.0f, 1.0f, 0.0f));

					Mathf::Vector3 Vector;
					Vector.x = Mathf::Vector3::Dot(Mathf::Vector3::Cross((MP1 - P1).Nomalize(), (P1Z - P1).Nomalize()), Mathf::Vector3::Cross((P1X - P1).Nomalize(), (P1Z - P1).Nomalize()));//Sin
					Vector.y = Mathf::Vector3::Dot((MP1 - P1).Nomalize(), (P1Z - P1).Nomalize());//Cos
					Vector.z = 0.f;

					e.SetGunRad(std::atan2f(Vector.x, Vector.y));
					if (InputControl::Instance()->GetSubShotKey().IsPress()) {
						if (e.SetBullet(1, PG, Vector.Nomalize() * 1.f)) {
							m_ShotScore++;
						}
					}


					if (IsBoost) {
						if (InputControl::Instance()->GetWKey().IsTrigger()) {
							e.m_SonicTimer = 0.f;
						}
						else {
							e.m_SonicTimer += FrameWork::Instance()->GetDeltaTime();
						}
						if (e.m_SonicTimer > 0.025f) {
							e.m_SonicTimer -= 0.025f;
							EffectControl::Instance()->SetEffect((int)EnumEffect::Sonic, e.GetPosition(), e.GetPosition());
						}
					}
				}
			}
			else {
				//AI
				if (e.IsAlive()) {
					Mathf::Vector3 ToP = e.GetPosition() - PlayerChara.GetPosition();

					bool IsBoost = (ToP.y) > 3.f;
					bool IsUp = (ToP.y < 0.f);
					if (!IsBoost && std::abs(ToP.x) > 0.15f) {
						IsUp = false;
					}
					bool IsRight = (ToP.x < 0.f);

					for (auto& e2 : m_Characters) {
						if (e.GetID() != e2.GetID()) {
							Mathf::Vector3 To = e.GetPosition() - e2.GetPosition();
							if (std::abs(To.x) < 0.05f && std::abs(To.y) < 0.5f) {
								IsRight = To.x > 0;
								IsUp = To.y > 0;
							}
						}
					}

					e.SetLRVec(!IsRight, IsRight);
					Mathf::Vector3 Vec = e.GetVec();
					Mathf::Easing(&Vec.y, IsUp ? (IsBoost ? 3.f : 1.5f) : 0.75f, 0.95f);
					e.SetVec(Vec);
					//
					Mathf::Vector3 Vector = PlayerChara.GetPosition() - e.GetGunPos();
					bool ShotSubKey = (0.5f < Vector.Length()) && (Vector.Length() < 1.f);
					if (Vector.y > 0.f) {
						ShotSubKey = (0.1f < Vector.Length()) && (Vector.Length() < 0.25f);
					}
					e.SetGunRad(std::atan2f(Vector.x, Vector.y));

					if (ShotSubKey) {
						if (e.m_ShotInterval == 0.f) {
							e.m_ShotInterval = static_cast<float>(GetRand(100)) / 100.f + 1.f;
							e.SetBullet(1, e.GetGunPos(), Vector.Nomalize() * 1.f);
						}
					}
					e.m_ShotInterval = Mathf::Max(e.m_ShotInterval - FrameWork::Instance()->GetDeltaTime(), 0.f);
				}
			}
		}
		//リスポーン
		if (e.CanRespawn() && !e.IsAlive()) {
			e.SetDamage(-MaxHP);
			Mathf::Vector3 Pos = MainCamera::Instance()->GetCamPos() + Mathf::Vector3(0.f, -1.f, 0.f);
			Pos.z = 20.f;
			e.Spawn(Pos);
			if (e.GetID() == PlayerChara.GetID()) {
				m_RespawnScore++;
			}
		}
		e.Update();
	}
	//弾判定
	if (IsInGame()) {
		for (auto& b : BulletControl::Instance()->SetBulletList()) {
			if (b->IsActive()) {
				for (auto& e2 : m_Characters) {
					if (b->GetShooterID() == e2.GetID()) { continue; }
					if (!e2.IsAlive()) { continue; }
					SEGMENT_SEGMENT_RESULT Ret;
					Mathf::GetSegmenttoSegment(b->GetRePos(), b->GetPosition(), e2.GetRePos(), e2.GetPosition(), &Ret);
					float len = 0.025f;
					if (Ret.SegA_SegB_MinDist_Square <= (len * len)) {
						Mathf::Vector3 Pos(Ret.SegA_MinDist_Pos.x, Ret.SegA_MinDist_Pos.y, Ret.SegA_MinDist_Pos.z);
						if (e2.CanDamage()) {
							e2.SetDamage(b->GetDamage());
							if (e2.IsAlive()) {
								SoundPool::Instance()->Play(DX_PLAYTYPE_BACK, TRUE, SoundType::SE, "data/Audio/Damage.wav");
							}
							else {
								SoundPool::Instance()->Play(DX_PLAYTYPE_BACK, TRUE, SoundType::SE, "data/Audio/Death.wav");
							}
							if (b->GetShooterID() == PlayerChara.GetID()) {
								m_HitScore++;
								if (!e2.IsAlive()) {
									m_KillScore++;
								}
							}
							if (e2.GetID() == PlayerChara.GetID()) {
								m_DamageCoolTime = 1.f;
							}
						}
						b->DisActive();
						EffectControl::Instance()->SetEffect((int)EnumEffect::Hit, Pos + e2.GetVec().Nomalize() * 0.1f, Pos);
						break;
					}
				}
			}
		}
	}
	BulletControl::Instance()->Update();
	//
	m_MainTimer -= FrameWork::Instance()->GetDeltaTime();
	EffectControl::Instance()->Update();
	//カメラ座標の更新
	{
		Mathf::Vector3 Pos = MainCamera::Instance()->GetCamPos();
		if (IsInGame()) {
			CamPosBuf = PlayerChara.GetPosition();
			CamPosBuf.x -= 0.75f;
			CamPosBuf.y -= 0.15f;
		}
		CamPosBuf.z = 10.f;
		Mathf::Easing(&Pos, CamPosBuf, 0.9f);
		MainCamera::Instance()->SetCamPos(Pos);
	}
	//自機だけダメージ回復
	{
		m_DamageCoolTime = Mathf::Max(m_DamageCoolTime - FrameWork::Instance()->GetDeltaTime(), 0.f);
		if (m_DamageCoolTime == 0.f) {
			m_CureCoolTime += FrameWork::Instance()->GetDeltaTime();
			if (m_CureCoolTime > 0.1f) {
				m_CureCoolTime -= 0.1f;
				if (PlayerChara.IsAlive()) {
					PlayerChara.SetDamage(-MaxHP / 100);
				}
			}
		}
		else {
			m_CureCoolTime = 0.f;
		}
	}
	//UI
	Mathf::Easing(&m_BoostMeterRand, PlayerChara.m_BoostTimer + static_cast<float>(GetRand(200) - 100) / 100.f * 0.1f, 0.9f);
	if (m_DamageCoolTime == 0.f) {
		if (std::abs(m_HitPointRe - PlayerChara.GetHP()) > 1.f) {
			if (m_HitPointRe > PlayerChara.GetHP()) {
				m_HitPointRe -= FrameWork::Instance()->GetDeltaTime() * 100.f;
			}
			else {
				m_HitPointRe += FrameWork::Instance()->GetDeltaTime() * 100.f;
			}
		}
		else {
			m_HitPointRe = static_cast<float>(PlayerChara.GetHP());
		}
	}
}
void MainGame::DrawSub() const {
	DrawMain();
	if (!IsResultActive()) {
		if (!FrameWork::Instance()->GetIsPauseActive()) {
			DrawUI();
		}
		DrawPauseUI();
	}
	else {
		DrawResult();
	}
}
void MainGame::DisposeSub() {
	SoundPool::Instance()->Del(SoundType::BGM, "data/Audio/BGM.wav", DX_SOUNDDATATYPE_FILE);

	MainCamera::Release();
	for (auto& e : m_Characters) {
		e.Dispose();
	}
	EffectControl::Release();
	BulletControl::Release();
	//
	m_gauge.ReleaseGraph();
	m_meter.ReleaseGraph();
	m_FinImage.ReleaseGraph();
}
std::unique_ptr<BaseScene> MainGame::MakeNextScene()
{
	return std::make_unique<TitleScene>();
}
void MainGame::DrawMain() const {
	//背景
	DrawBox(0, 0, FrameWork::Instance()->GetScreenWidth(), FrameWork::Instance()->GetScreenHeight(), ColorPalette::Red075, TRUE);
	for (int loop = -5; loop < 20; ++loop) {
		{
			float timeTemp = static_cast<float>(loop + (int)(MainCamera::Instance()->GetCamPos().y) * 3.f) / 3.f;

			Mathf::Vector3 P1 = MainCamera::Instance()->GetDisplayPoint(MainCamera::Instance()->GetCamPos().x, timeTemp, 0.f);
			Mathf::Vector3 P2 = MainCamera::Instance()->GetDisplayPoint(MainCamera::Instance()->GetCamPos().x + 2.f, timeTemp, 0.f);
			Mathf::Vector3 P3 = MainCamera::Instance()->GetDisplayPoint(MainCamera::Instance()->GetCamPos().x + 2.f, timeTemp + 0.1f, 0.f);
			DrawTriangle(static_cast<int>(P1.x), static_cast<int>(P1.y), static_cast<int>(P2.x), static_cast<int>(P2.y), static_cast<int>(P3.x), static_cast<int>(P3.y), ColorPalette::Black, TRUE);
		}
		{
			float timeTemp = (static_cast<float>(loop + (int)(MainCamera::Instance()->GetCamPos().y) * 3.f) + 0.5f) / 3.f;

			Mathf::Vector3 P1 = MainCamera::Instance()->GetDisplayPoint(MainCamera::Instance()->GetCamPos().x, timeTemp, 0.f);
			Mathf::Vector3 P2 = MainCamera::Instance()->GetDisplayPoint(MainCamera::Instance()->GetCamPos().x + 2.f, timeTemp, 0.f);
			Mathf::Vector3 P3 = MainCamera::Instance()->GetDisplayPoint(MainCamera::Instance()->GetCamPos().x + 2.f, timeTemp + 0.1f, 0.f);
			DrawTriangle(static_cast<int>(P1.x), static_cast<int>(P1.y), static_cast<int>(P2.x), static_cast<int>(P2.y), static_cast<int>(P3.x), static_cast<int>(P3.y), ColorPalette::Red, TRUE);
		}
	}
	//影描画
	BulletControl::Instance()->DrawShadow();
	for (auto& e : m_Characters) {
		e.DrawShadow();
	}
	EffectControl::Instance()->DrawShadow();
	//オブジェクト描画
	BulletControl::Instance()->Draw();
	for (auto& e : m_Characters) {
		e.Draw();
	}
	EffectControl::Instance()->Draw();
}
void MainGame::DrawUI() const {
	auto& PlayerChara = m_Characters.back();

	if (!IsInGame()) { return; }

	if (IsMainGame()) {
		DrawFormatString2ToHandle(32, 32, ColorPalette::White, ColorPalette::Black, FontPool::Instance()->Get("Agency FB", 24, -1, DX_FONTTYPE_ANTIALIASING_EDGE, DX_CHARSET_DEFAULT, 1)->GetHandle(), "%d:%02d.%02d", static_cast<int>(m_MainTimer / 60.f), static_cast<int>(m_MainTimer) % 60, static_cast<int>(m_MainTimer * 100) % 60);
	}
	else if (IsInGame()) {
		//開始前カウントダウン
		int Width = GetDrawFormatStringWidthToHandle(FontPool::Instance()->Get("Agency FB", 24, -1, DX_FONTTYPE_ANTIALIASING_EDGE, DX_CHARSET_DEFAULT, 1)->GetHandle(), "%05.2f", m_MainTimer - m_TotalTimer);
		DrawFormatString2ToHandle(FrameWork::Instance()->GetScreenWidth() / 2 - Width / 2, FrameWork::Instance()->GetScreenHeight() / 3 - 24 / 2, ColorPalette::White, ColorPalette::Black, FontPool::Instance()->Get("Agency FB", 24, -1, DX_FONTTYPE_ANTIALIASING_EDGE, DX_CHARSET_DEFAULT, 1)->GetHandle(), "%05.2f", m_MainTimer - m_TotalTimer);
	}

	if (!PlayerChara.IsAlive()) { return; }

	{
		int x1 = 32;
		int y1 = FrameWork::Instance()->GetScreenHeight() / 2 - 256 / 2;
		int x2 = x1 + 16;
		int y2 = y1 + 256;
		DrawBox(x1, static_cast<int>(y2 - (y2 - y1) * m_HitPointRe / MaxHP), x2, y2, ColorPalette::Red, TRUE);
		DrawBox(x1, static_cast<int>(y2 - (y2 - y1) * PlayerChara.GetHP() / MaxHP), x2, y2, ColorPalette::White, TRUE);
		DrawBox(x1, y1, x2, y2, ColorPalette::Black, FALSE);
		DrawBox(x1 + 1, y1 + 1, x2 - 1, y2 - 1, ColorPalette::Black, FALSE);

		DrawFormatString2ToHandle(x1, y2, ColorPalette::Red, ColorPalette::Black, FontPool::Instance()->Get("Agency FB", 12, -1, DX_FONTTYPE_ANTIALIASING_EDGE, DX_CHARSET_DEFAULT, 1)->GetHandle(), "HP");
	}

	double Rate = static_cast<double>(static_cast<float>(FrameWork::Instance()->GetScreenWidth()) / 960.f);
	DrawRotaGraph(FrameWork::Instance()->GetScreenWidth() - 32, FrameWork::Instance()->GetScreenHeight() - 32, Rate, 0.0, m_gauge.GetHandle(), TRUE);
	DrawRotaGraph(FrameWork::Instance()->GetScreenWidth() - 32, FrameWork::Instance()->GetScreenHeight() - 32, Rate, static_cast<double>(Mathf::Deg2Rad((m_BoostMeterRand - 1.f) * 90.f)), m_meter.GetHandle(), TRUE);
	if (!PlayerChara.m_BoostActive && (static_cast<int>(m_MainTimer * 100) % 10 < 5)) {
		DrawFormatString2ToHandle(FrameWork::Instance()->GetScreenWidth() - 32 - 128 + 32, FrameWork::Instance()->GetScreenHeight() - 32 - 128 + 64, ColorPalette::Red, ColorPalette::Black, FontPool::Instance()->Get("Agency FB", 12, -1, DX_FONTTYPE_ANTIALIASING_EDGE, DX_CHARSET_DEFAULT, 1)->GetHandle(), "OVER HEAT!");
	}
	DrawFormatString2ToHandle(32, FrameWork::Instance()->GetScreenHeight() - 32, ColorPalette::White, ColorPalette::Black, FontPool::Instance()->Get("Agency FB", 12, -1, DX_FONTTYPE_ANTIALIASING_EDGE, DX_CHARSET_DEFAULT, 1)->GetHandle(), "AD : 旋回　W : ブースト S : 減速 スペース : 前方射撃 左クリック : 旋回機銃射撃");
}

void MainGame::InitPauseUI(){
}
void MainGame::UpdatePauseUI(){
	m_PausePer = Mathf::Clamp(m_PausePer + (FrameWork::Instance()->GetIsPauseActive() ? FrameWork::Instance()->GetFixedDeltaTime() : -FrameWork::Instance()->GetFixedDeltaTime()) / 0.25f, 0.f, 1.f);
	bool OnMouse = false;
	if (FrameWork::Instance()->GetIsPauseActive()) {
		switch (m_PauseOpen) {
		case 0:
		{
			int Now = 0;
			{
				bool prev = OnMousePause[m_PauseOpen][Now];
				const char* Str = "Return Title";
				int Width = GetDrawStringWidthToHandle(Str, static_cast<int>(strlenDx(Str)), FontPool::Instance()->Get("Agency FB", 24, -1, DX_FONTTYPE_ANTIALIASING_EDGE, DX_CHARSET_DEFAULT, 1)->GetHandle());
				OnMousePause[m_PauseOpen][Now] = IntoMouse(32, FrameWork::Instance()->GetScreenHeight() - 32 - 24 - 28 * (2 - Now),
					32 + Width, FrameWork::Instance()->GetScreenHeight() - 32 - 24 - 28 * (2 - Now) + 24);
				if (OnMousePause[m_PauseOpen][Now]) {
					m_PauseSelect = Now;
					if (prev != OnMousePause[m_PauseOpen][Now]) {
						SoundPool::Instance()->Play(DX_PLAYTYPE_BACK, TRUE, SoundType::SE, "data/Audio/sel.wav");
					}
					if (InputControl::Instance()->GetLMEnter().IsTrigger()) {
						OnMouse = true;
					}
				}
			}
			Now++;
			{
				bool prev = OnMousePause[m_PauseOpen][Now];
				const char* Str = "Option";
				int Width = GetDrawStringWidthToHandle(Str, static_cast<int>(strlenDx(Str)), FontPool::Instance()->Get("Agency FB", 24, -1, DX_FONTTYPE_ANTIALIASING_EDGE, DX_CHARSET_DEFAULT, 1)->GetHandle());
				OnMousePause[m_PauseOpen][Now] = IntoMouse(32, FrameWork::Instance()->GetScreenHeight() - 32 - 24 - 28 * (2 - Now),
					32 + Width, FrameWork::Instance()->GetScreenHeight() - 32 - 24 - 28 * (2 - Now) + 24);
				if (OnMousePause[m_PauseOpen][Now]) {
					m_PauseSelect = Now;
					if (prev != OnMousePause[m_PauseOpen][Now]) {
						SoundPool::Instance()->Play(DX_PLAYTYPE_BACK, TRUE, SoundType::SE, "data/Audio/sel.wav");
					}
					if (InputControl::Instance()->GetLMEnter().IsTrigger()) {
						OnMouse = true;
					}
				}
			}
			Now++;
			{
				bool prev = OnMousePause[m_PauseOpen][Now];
				const char* Str = "Return Game";
				int Width = GetDrawStringWidthToHandle(Str, static_cast<int>(strlenDx(Str)), FontPool::Instance()->Get("Agency FB", 24, -1, DX_FONTTYPE_ANTIALIASING_EDGE, DX_CHARSET_DEFAULT, 1)->GetHandle());
				OnMousePause[m_PauseOpen][Now] = IntoMouse(32, FrameWork::Instance()->GetScreenHeight() - 32 - 24 - 28 * (2 - Now),
					32 + Width, FrameWork::Instance()->GetScreenHeight() - 32 - 24 - 28 * (2 - Now) + 24);
				if (OnMousePause[m_PauseOpen][Now]) {
					m_PauseSelect = Now;
					if (prev != OnMousePause[m_PauseOpen][Now]) {
						SoundPool::Instance()->Play(DX_PLAYTYPE_BACK, TRUE, SoundType::SE, "data/Audio/sel.wav");
					}
					if (InputControl::Instance()->GetLMEnter().IsTrigger()) {
						OnMouse = true;
					}
				}
			}
			Now++;
		}
		break;
		case 1:
		{
			int Now = 0;
			{
				bool prev = OnMousePause[m_PauseOpen][Now];
				const char* Str = "Window Mode:";
				int Width = GetDrawStringWidthToHandle(Str, static_cast<int>(strlenDx(Str)), FontPool::Instance()->Get("Agency FB", 24, -1, DX_FONTTYPE_ANTIALIASING_EDGE, DX_CHARSET_DEFAULT, 1)->GetHandle());
				OnMousePause[m_PauseOpen][Now] = IntoMouse(32 + 32, FrameWork::Instance()->GetScreenHeight() - 32 - 24 - 28 * (3 - Now),
					32 + 32 + Width, FrameWork::Instance()->GetScreenHeight() - 32 - 24 - 28 * (3 - Now) + 24);
				if (OnMousePause[m_PauseOpen][Now]) {
					m_OptionSelect = Now;
					if (prev != OnMousePause[m_PauseOpen][Now]) {
						SoundPool::Instance()->Play(DX_PLAYTYPE_BACK, TRUE, SoundType::SE, "data/Audio/sel.wav");
					}
					if (InputControl::Instance()->GetLMEnter().IsTrigger()) {
						OnMouse = true;
					}
				}
			}
			Now++;
			{
				bool prev = OnMousePause[m_PauseOpen][Now];
				const char* Str = "BGM:";
				int Width = GetDrawStringWidthToHandle(Str, static_cast<int>(strlenDx(Str)), FontPool::Instance()->Get("Agency FB", 24, -1, DX_FONTTYPE_ANTIALIASING_EDGE, DX_CHARSET_DEFAULT, 1)->GetHandle());
				OnMousePause[m_PauseOpen][Now] = IntoMouse(32 + 32, FrameWork::Instance()->GetScreenHeight() - 32 - 24 - 28 * (3 - Now),
					32 + 32 + Width, FrameWork::Instance()->GetScreenHeight() - 32 - 24 - 28 * (3 - Now) + 24);
				if (OnMousePause[m_PauseOpen][Now]) {
					m_OptionSelect = Now;
					if (prev != OnMousePause[m_PauseOpen][Now]) {
						SoundPool::Instance()->Play(DX_PLAYTYPE_BACK, TRUE, SoundType::SE, "data/Audio/sel.wav");
					}
					if (InputControl::Instance()->GetLMEnter().IsTrigger()) {
						OnMouse = true;
					}
				}
			}
			Now++;
			{
				bool prev = OnMousePause[m_PauseOpen][Now];
				const char* Str = "SE:";
				int Width = GetDrawStringWidthToHandle(Str, static_cast<int>(strlenDx(Str)), FontPool::Instance()->Get("Agency FB", 24, -1, DX_FONTTYPE_ANTIALIASING_EDGE, DX_CHARSET_DEFAULT, 1)->GetHandle());
				OnMousePause[m_PauseOpen][Now] = IntoMouse(32 + 32, FrameWork::Instance()->GetScreenHeight() - 32 - 24 - 28 * (3 - Now),
					32 + 32 + Width, FrameWork::Instance()->GetScreenHeight() - 32 - 24 - 28 * (3 - Now) + 24);
				if (OnMousePause[m_PauseOpen][Now]) {
					m_OptionSelect = Now;
					if (prev != OnMousePause[m_PauseOpen][Now]) {
						SoundPool::Instance()->Play(DX_PLAYTYPE_BACK, TRUE, SoundType::SE, "data/Audio/sel.wav");
					}
					if (InputControl::Instance()->GetLMEnter().IsTrigger()) {
						OnMouse = true;
					}
				}
			}
			Now++;
			{
				bool prev = OnMousePause[m_PauseOpen][Now];
				const char* Str = "Return";
				int Width = GetDrawStringWidthToHandle(Str, static_cast<int>(strlenDx(Str)), FontPool::Instance()->Get("Agency FB", 24, -1, DX_FONTTYPE_ANTIALIASING_EDGE, DX_CHARSET_DEFAULT, 1)->GetHandle());
				OnMousePause[m_PauseOpen][Now] = IntoMouse(32 + 32, FrameWork::Instance()->GetScreenHeight() - 32 - 24 - 28 * (3 - Now),
					32 + 32 + Width, FrameWork::Instance()->GetScreenHeight() - 32 - 24 - 28 * (3 - Now) + 24);
				if (OnMousePause[m_PauseOpen][Now]) {
					m_OptionSelect = Now;
					if (prev != OnMousePause[m_PauseOpen][Now]) {
						SoundPool::Instance()->Play(DX_PLAYTYPE_BACK, TRUE, SoundType::SE, "data/Audio/sel.wav");
					}
					if (InputControl::Instance()->GetLMEnter().IsTrigger()) {
						OnMouse = true;
					}
				}
			}
			Now++;
		}
		break;
		default:
			break;
		}

		m_PauseTimer += FrameWork::Instance()->GetFixedDeltaTime();
		switch (m_PauseOpen) {
		case 0:
		{
			if (InputControl::Instance()->GetSKey().IsTrigger()) {
				++m_PauseSelect;
				SoundPool::Instance()->Play(DX_PLAYTYPE_BACK, TRUE, SoundType::SE, "data/Audio/sel.wav");
			}
			if (InputControl::Instance()->GetWKey().IsTrigger()) {
				--m_PauseSelect;
				SoundPool::Instance()->Play(DX_PLAYTYPE_BACK, TRUE, SoundType::SE, "data/Audio/sel.wav");
			}
			if (m_PauseSelect > 3 - 1) { m_PauseSelect = 0; }
			if (m_PauseSelect < 0) { m_PauseSelect = 3 - 1; }
			if (InputControl::Instance()->GetMenuEnter().IsTrigger() || OnMouse) {
				SoundPool::Instance()->Play(DX_PLAYTYPE_BACK, TRUE, SoundType::SE, "data/Audio/decide.wav");
				switch (m_PauseSelect) {
				case 0:
					BaseScene::SetSceneEnd();
					break;
				case 1:
					m_OptionSelect = 0;
					m_PauseOpen = 1;
					break;
				case 2:
					FrameWork::Instance()->SetPauseActive(false);
					break;
				default:
					break;
				}
			}
		}
		break;
		case 1:
		{
			if (InputControl::Instance()->GetSKey().IsTrigger()) {
				++m_OptionSelect;
				SoundPool::Instance()->Play(DX_PLAYTYPE_BACK, TRUE, SoundType::SE, "data/Audio/sel.wav");
			}
			if (InputControl::Instance()->GetWKey().IsTrigger()) {
				--m_OptionSelect;
				SoundPool::Instance()->Play(DX_PLAYTYPE_BACK, TRUE, SoundType::SE, "data/Audio/sel.wav");
			}
			if (m_OptionSelect > 4 - 1) { m_OptionSelect = 0; }
			if (m_OptionSelect < 0) { m_OptionSelect = 4 - 1; }
			if (InputControl::Instance()->GetMenuEnter().IsTrigger() || OnMouse) {
				SoundPool::Instance()->Play(DX_PLAYTYPE_BACK, TRUE, SoundType::SE, "data/Audio/decide.wav");
				switch (m_OptionSelect) {
				case 0:
				{
					switch (SaveData::Instance()->GetWindowSetting()) {
					case WindowSetting::WindowMode:
						SaveData::Instance()->SetWindowSetting(WindowSetting::BorderLess);
						break;
					case WindowSetting::BorderLess:
						SaveData::Instance()->SetWindowSetting(WindowSetting::FullScreen);
						break;
					case WindowSetting::FullScreen:
						SaveData::Instance()->SetWindowSetting(WindowSetting::WindowMode);
						break;
					default:
						break;
					}
					FrameWork::Instance()->UpdateWindowSetting();
				}
				break;
				case 1:
					SaveData::Instance()->SetIsMuteBGMVol(SaveData::Instance()->GetIsMuteBGMVol() ^ 1);
					SoundPool::Instance()->SetVol(SoundType::BGM, SaveData::Instance()->GetIsMuteBGMVol() ? 0 : SaveData::Instance()->GetBGMVol());
					break;
				case 2:
					SaveData::Instance()->SetIsMuteSEVol(SaveData::Instance()->GetIsMuteSEVol() ^ 1);
					SoundPool::Instance()->SetVol(SoundType::SE, SaveData::Instance()->GetIsMuteSEVol() ? 0 : SaveData::Instance()->GetSEVol());
					break;
				case 3:
					m_PauseOpen = 0;
					SoundPool::Instance()->Play(DX_PLAYTYPE_BACK, TRUE, SoundType::SE, "data/Audio/decide.wav");
					break;
				default:
					break;
				}
				SaveData::Instance()->Save();
			}

			if (InputControl::Instance()->GetDKey().IsTrigger()) {
				SoundPool::Instance()->Play(DX_PLAYTYPE_BACK, TRUE, SoundType::SE, "data/Audio/sel.wav");
				switch (m_OptionSelect) {
				case 0:
				{
					switch (SaveData::Instance()->GetWindowSetting()) {
					case WindowSetting::WindowMode:
						SaveData::Instance()->SetWindowSetting(WindowSetting::BorderLess);
						break;
					case WindowSetting::BorderLess:
						SaveData::Instance()->SetWindowSetting(WindowSetting::FullScreen);
						break;
					case WindowSetting::FullScreen:
						SaveData::Instance()->SetWindowSetting(WindowSetting::WindowMode);
						break;
					default:
						break;
					}
					FrameWork::Instance()->UpdateWindowSetting();
					SaveData::Instance()->Save();
				}
				break;
				case 1:
					SaveData::Instance()->SetIsMuteBGMVol(false);
					SaveData::Instance()->SetBGMVol(SaveData::Instance()->GetBGMVol() + 10);
					SoundPool::Instance()->SetVol(SoundType::BGM, SaveData::Instance()->GetIsMuteBGMVol() ? 0 : SaveData::Instance()->GetBGMVol());
					break;
				case 2:
					SaveData::Instance()->SetIsMuteSEVol(false);
					SaveData::Instance()->SetSEVol(SaveData::Instance()->GetSEVol() + 10);
					SoundPool::Instance()->SetVol(SoundType::SE, SaveData::Instance()->GetIsMuteSEVol() ? 0 : SaveData::Instance()->GetSEVol());
					break;
				case 3:
					break;
				default:
					break;
				}
				SaveData::Instance()->Save();
			}
			if (InputControl::Instance()->GetAKey().IsTrigger()) {
				SoundPool::Instance()->Play(DX_PLAYTYPE_BACK, TRUE, SoundType::SE, "data/Audio/sel.wav");
				switch (m_OptionSelect) {
				case 0:
				{
					switch (SaveData::Instance()->GetWindowSetting()) {
					case WindowSetting::WindowMode:
						SaveData::Instance()->SetWindowSetting(WindowSetting::FullScreen);
						break;
					case WindowSetting::BorderLess:
						SaveData::Instance()->SetWindowSetting(WindowSetting::WindowMode);
						break;
					case WindowSetting::FullScreen:
						SaveData::Instance()->SetWindowSetting(WindowSetting::BorderLess);
						break;
					default:
						break;
					}
					FrameWork::Instance()->UpdateWindowSetting();
					SaveData::Instance()->Save();
				}
				break;
				case 1:
					SaveData::Instance()->SetIsMuteBGMVol(false);
					SaveData::Instance()->SetBGMVol(SaveData::Instance()->GetBGMVol() - 10);
					SoundPool::Instance()->SetVol(SoundType::BGM, SaveData::Instance()->GetIsMuteBGMVol() ? 0 : SaveData::Instance()->GetBGMVol());
					break;
				case 2:
					SaveData::Instance()->SetIsMuteSEVol(false);
					SaveData::Instance()->SetSEVol(SaveData::Instance()->GetSEVol() - 10);
					SoundPool::Instance()->SetVol(SoundType::SE, SaveData::Instance()->GetIsMuteSEVol() ? 0 : SaveData::Instance()->GetSEVol());
					break;
				case 3:
					break;
				default:
					break;
				}
				SaveData::Instance()->Save();
			}
		}
		break;
		default:
			break;
		}
	}
	else {
		m_PauseSelect = 0;
		m_PauseOpen = 0;
	}
}
void MainGame::DrawPauseUI() const {
	int Alpha = static_cast<int>(128 * m_PausePer);
	if (Alpha > 0) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, Alpha);
		DrawBox(0, 0, FrameWork::Instance()->GetScreenWidth(), FrameWork::Instance()->GetScreenHeight(), ColorPalette::Black, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		
	}
	if (FrameWork::Instance()->GetIsPauseActive()){
		switch (m_PauseOpen) {
		case 0:
		{
			if ((static_cast<int>(m_PauseTimer * 10) % 10 < 5)) {
				DrawFormatString2ToHandle(32, 32, ColorPalette::White, ColorPalette::Black, FontPool::Instance()->Get("Agency FB", 24, -1, DX_FONTTYPE_ANTIALIASING_EDGE, DX_CHARSET_DEFAULT, 1)->GetHandle(), "Pause");
			}

			int Now = 0;
			DrawFormatString2ToHandle(32, FrameWork::Instance()->GetScreenHeight() - 32 - 24 - 28 * (2 - Now), (m_PauseSelect == Now) ? ColorPalette::Red : ColorPalette::White, ColorPalette::Black, FontPool::Instance()->Get("Agency FB", 24, -1, DX_FONTTYPE_ANTIALIASING_EDGE, DX_CHARSET_DEFAULT, 1)->GetHandle(), "Return Title"); Now++;
			DrawFormatString2ToHandle(32, FrameWork::Instance()->GetScreenHeight() - 32 - 24 - 28 * (2 - Now), (m_PauseSelect == Now) ? ColorPalette::Red : ColorPalette::White, ColorPalette::Black, FontPool::Instance()->Get("Agency FB", 24, -1, DX_FONTTYPE_ANTIALIASING_EDGE, DX_CHARSET_DEFAULT, 1)->GetHandle(), "Option"); Now++;
			DrawFormatString2ToHandle(32, FrameWork::Instance()->GetScreenHeight() - 32 - 24 - 28 * (2 - Now), (m_PauseSelect == Now) ? ColorPalette::Red : ColorPalette::White, ColorPalette::Black, FontPool::Instance()->Get("Agency FB", 24, -1, DX_FONTTYPE_ANTIALIASING_EDGE, DX_CHARSET_DEFAULT, 1)->GetHandle(), "Return Game"); Now++;

			DrawFormatString2ToHandle(32, FrameWork::Instance()->GetScreenHeight() - 32, ColorPalette::White, ColorPalette::Black, FontPool::Instance()->Get("Agency FB", 12, -1, DX_FONTTYPE_ANTIALIASING_EDGE, DX_CHARSET_DEFAULT, 1)->GetHandle(), "WS : 選択 スペース : 決定");
		}
		break;
		case 1:
		{
			if ((static_cast<int>(m_PauseTimer * 10) % 10 < 5)) {
				DrawFormatString2ToHandle(32, 32, ColorPalette::White, ColorPalette::Black, FontPool::Instance()->Get("Agency FB", 24, -1, DX_FONTTYPE_ANTIALIASING_EDGE, DX_CHARSET_DEFAULT, 1)->GetHandle(), "Option");
			}

			int Now = 0;
			DrawFormatString2ToHandle(32 + 32, FrameWork::Instance()->GetScreenHeight() - 32 - 24 - 28 * (3 - Now), (m_OptionSelect == Now) ? ColorPalette::Red : ColorPalette::White, ColorPalette::Black, FontPool::Instance()->Get("Agency FB", 24, -1, DX_FONTTYPE_ANTIALIASING_EDGE, DX_CHARSET_DEFAULT, 1)->GetHandle(), "Window Mode:%s", SaveData::Instance()->GetWindowSettingStr()); Now++;
			if (SaveData::Instance()->GetIsMuteBGMVol()) {
				DrawFormatString2ToHandle(32 + 32, FrameWork::Instance()->GetScreenHeight() - 32 - 24 - 28 * (3 - Now), (m_OptionSelect == Now) ? ColorPalette::Red : ColorPalette::White, ColorPalette::Black, FontPool::Instance()->Get("Agency FB", 24, -1, DX_FONTTYPE_ANTIALIASING_EDGE, DX_CHARSET_DEFAULT, 1)->GetHandle(), "BGM:%s", "Mute"); Now++;
			}
			else {
				DrawFormatString2ToHandle(32 + 32, FrameWork::Instance()->GetScreenHeight() - 32 - 24 - 28 * (3 - Now), (m_OptionSelect == Now) ? ColorPalette::Red : ColorPalette::White, ColorPalette::Black, FontPool::Instance()->Get("Agency FB", 24, -1, DX_FONTTYPE_ANTIALIASING_EDGE, DX_CHARSET_DEFAULT, 1)->GetHandle(), "BGM:%d", SaveData::Instance()->GetBGMVol()); Now++;
			}
			if (SaveData::Instance()->GetIsMuteSEVol()) {
				DrawFormatString2ToHandle(32 + 32, FrameWork::Instance()->GetScreenHeight() - 32 - 24 - 28 * (3 - Now), (m_OptionSelect == Now) ? ColorPalette::Red : ColorPalette::White, ColorPalette::Black, FontPool::Instance()->Get("Agency FB", 24, -1, DX_FONTTYPE_ANTIALIASING_EDGE, DX_CHARSET_DEFAULT, 1)->GetHandle(), "SE:%s", "Mute"); Now++;
			}
			else {
				DrawFormatString2ToHandle(32 + 32, FrameWork::Instance()->GetScreenHeight() - 32 - 24 - 28 * (3 - Now), (m_OptionSelect == Now) ? ColorPalette::Red : ColorPalette::White, ColorPalette::Black, FontPool::Instance()->Get("Agency FB", 24, -1, DX_FONTTYPE_ANTIALIASING_EDGE, DX_CHARSET_DEFAULT, 1)->GetHandle(), "SE:%d", SaveData::Instance()->GetSEVol()); Now++;
			}
			DrawFormatString2ToHandle(32 + 32, FrameWork::Instance()->GetScreenHeight() - 32 - 24 - 28 * (3 - Now), (m_OptionSelect == Now) ? ColorPalette::Red : ColorPalette::White, ColorPalette::Black, FontPool::Instance()->Get("Agency FB", 24, -1, DX_FONTTYPE_ANTIALIASING_EDGE, DX_CHARSET_DEFAULT, 1)->GetHandle(), "Return"); Now++;

			DrawFormatString2ToHandle(32, FrameWork::Instance()->GetScreenHeight() - 32, ColorPalette::White, ColorPalette::Black, FontPool::Instance()->Get("Agency FB", 12, -1, DX_FONTTYPE_ANTIALIASING_EDGE, DX_CHARSET_DEFAULT, 1)->GetHandle(), "WS : 選択 スペース : 決定");
		}
		break;
		default:
			break;
		}
	}
}

void MainGame::InitResult() {
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
	IsGoingNextScene = false;
}
void MainGame::UpdateResult() {
	if (m_ResultClear == -1) {
		SoundPool::Instance()->Play(DX_PLAYTYPE_LOOP, TRUE, SoundType::SE, "data/Audio/Score.wav");
		m_ResultClear++;
	}
	if (m_ResultClear == 0) {
		Mathf::Easing(&m_Respawn, static_cast<float>(m_RespawnScore), 0.9f);
		if (std::abs(m_Respawn - static_cast<float>(m_RespawnScore)) < 1.f) {
			m_Respawn = static_cast<float>(m_RespawnScore);
			if (m_ResultTimer == 0.f) {
				SoundPool::Instance()->StopAll(SoundType::SE, "data/Audio/Score.wav");
			}
			m_ResultTimer += FrameWork::Instance()->GetDeltaTime();
			if (m_ResultTimer > 0.5f) {
				m_ResultTimer = 0.f;
				SoundPool::Instance()->Play(DX_PLAYTYPE_LOOP, TRUE, SoundType::SE, "data/Audio/Score.wav");
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
				SoundPool::Instance()->StopAll(SoundType::SE, "data/Audio/Score.wav");
			}
			m_ResultTimer += FrameWork::Instance()->GetDeltaTime();
			if (m_ResultTimer > 0.5f) {
				m_ResultTimer = 0.f;
				SoundPool::Instance()->Play(DX_PLAYTYPE_LOOP, TRUE, SoundType::SE, "data/Audio/Score.wav");
				m_ResultClear++;
			}
		}
	}
	if (m_ResultClear == 2) {
		Mathf::Easing(&m_Kill, static_cast<float>(m_KillScore), 0.9f);
		if (std::abs(m_Kill - static_cast<float>(m_KillScore)) < 1.f) {
			m_Kill = static_cast<float>(m_KillScore);
			if (m_ResultTimer == 0.f) {
				SoundPool::Instance()->StopAll(SoundType::SE, "data/Audio/Score.wav");
			}
			m_ResultTimer += FrameWork::Instance()->GetDeltaTime();
			if (m_ResultTimer > 0.5f) {
				m_ResultTimer = 0.f;
				SoundPool::Instance()->Play(DX_PLAYTYPE_LOOP, TRUE, SoundType::SE, "data/Audio/Score.wav");
				m_ResultClear++;
			}
		}
	}
	if (m_ResultClear >= 3) {
		if (m_ResultTimer == 0.f) {
			SoundPool::Instance()->StopAll(SoundType::SE, "data/Audio/Score.wav");
		}
		m_ResultTimer += FrameWork::Instance()->GetDeltaTime();
	}
	if (m_ResultClear == 3) {
		if (m_ResultTimer > 0.5f) {
			//SoundPool::Instance()->Play(DX_PLAYTYPE_LOOP, TRUE, SoundType::SE, "data/Audio/Score.wav");
			m_ResultClear = 4;
		}
	}
	const char* Str = "Space :Return Title";
	int Width = GetDrawStringWidthToHandle(Str, static_cast<int>(strlenDx(Str)), FontPool::Instance()->Get("Agency FB", 24, -1, DX_FONTTYPE_ANTIALIASING_EDGE, DX_CHARSET_DEFAULT, 1)->GetHandle());

	OnMouseResultSkip = IntoMouse(FrameWork::Instance()->GetScreenWidth() / 2 - Width / 2, FrameWork::Instance()->GetScreenHeight() * 3 / 4,
		FrameWork::Instance()->GetScreenWidth() / 2 + Width / 2, FrameWork::Instance()->GetScreenHeight() * 3 / 4 + 24);
	if (!IsGoingNextScene) {
		if (m_ResultClear == 4 && (InputControl::Instance()->GetMenuEnter().IsTrigger() || (OnMouseResultSkip && InputControl::Instance()->GetLMEnter().IsTrigger()))) {
			FadeControl::Instance()->SetFadeIn(ColorPalette::White, 1.f);
			IsGoingNextScene = true;
		}
	}
	else {
		if (!FadeControl::Instance()->IsFading()) {
			BaseScene::SetSceneEnd();
		}
	}


	for (int loop = 0; loop < m_ResultAnim.size(); ++loop) {
		if (m_ResultClear >= loop) {
			Mathf::Easing(&m_ResultAnim.at(loop), 0.f, 0.9f);
		}
	}
}
void MainGame::DrawResult() const {
	DrawBox(0, 0, FrameWork::Instance()->GetScreenWidth(), FrameWork::Instance()->GetScreenHeight(), ColorPalette::Black, TRUE);
	DrawGraph(FrameWork::Instance()->GetScreenWidth() - 128 - 32, FrameWork::Instance()->GetScreenHeight() - 64 - 32, m_FinImage.GetHandle(), FALSE);

	const FontHandle* pFont = FontPool::Instance()->Get("BIZ UDゴシック", 24, -1, DX_FONTTYPE_ANTIALIASING_EDGE, DX_CHARSET_DEFAULT, 1);

	int num = 0;
	if (m_ResultClear >= num) {
		DrawFormatString2ToHandle(64 + static_cast<int>(m_ResultAnim[num]), 64 + num * 32, ColorPalette::White, ColorPalette::Black, pFont->GetHandle(), "Respawn Count : %02d", static_cast<int>(m_Respawn)); num++;
	}
	if (m_ResultClear >= num) {
		DrawFormatString2ToHandle(64 + static_cast<int>(m_ResultAnim[num]), 64 + num * 32, ColorPalette::White, ColorPalette::Black, pFont->GetHandle(), "Hit Ratio : %02d%%", static_cast<int>(m_HitRatio)); num++;
	}
	if (m_ResultClear >= num) {
		DrawFormatString2ToHandle(64 + static_cast<int>(m_ResultAnim[num]), 64 + num * 32, ColorPalette::White, ColorPalette::Black, pFont->GetHandle(), "Kill : %02d", static_cast<int>(m_Kill)); num++;
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
		DrawFormatString2ToHandle(64 + static_cast<int>(m_ResultAnim[num]), 64 + num * 32, ColorPalette::White, ColorPalette::Black, pFont->GetHandle(), "Rank : %s", Result.c_str()); num++;
	}

	if (m_ResultClear == 4 && (m_ResultTimer > 0.5f + 1.f) && (static_cast<int>(m_ResultTimer * 10) % 10 < 5)) {
		const char* Str = "Space :Return Title";
		int Width = GetDrawStringWidthToHandle(Str, static_cast<int>(strlenDx(Str)), FontPool::Instance()->Get("Agency FB", 24, -1, DX_FONTTYPE_ANTIALIASING_EDGE, DX_CHARSET_DEFAULT, 1)->GetHandle());
		DrawFormatString2ToHandle(FrameWork::Instance()->GetScreenWidth() / 2 - Width / 2, FrameWork::Instance()->GetScreenHeight() * 3 / 4, ColorPalette::White, ColorPalette::Black, FontPool::Instance()->Get("Agency FB", 24, -1, DX_FONTTYPE_ANTIALIASING_EDGE, DX_CHARSET_DEFAULT, 1)->GetHandle(), Str);
	}

}
