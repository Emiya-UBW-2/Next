#include "MainGame.hpp"

Mathf::Vector3 CamPos;

void MainGame::Update() {
	{
		Mathf::Vector3 Vec = m_Character.GetVec();

		bool UpKey = CheckHitKey(KEY_INPUT_W);
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
			m_Character.SetBullet(0, m_Character.GetPosition(), m_Character.GetVec().Nomalize() * 2.f);
		}

		//マウス座標を取得しておく
		int MouseX;
		int MouseY;
		//マウス座標を取得しておく
		GetMousePoint(&MouseX, &MouseY);

		MouseX = MouseX * FrameWork::Instance()->GetScreenWidth() / FrameWork::Instance()->GetWindowWidth();
		MouseY = MouseY * FrameWork::Instance()->GetScreenHeight() / FrameWork::Instance()->GetWindowHeight();

		Mathf::Vector3 MP1(MouseX, MouseY, 0.f);

		Mathf::Vector3 PG = m_Character.GetGunPos();
		Mathf::Vector3 P1 = GetDisplayPoint(PG.x - CamPos.x, PG.y - CamPos.y, PG.z - CamPos.z);
		Mathf::Vector3 P1X = GetDisplayPoint(PG.x + 1.f - CamPos.x, PG.y + 0.f - CamPos.y, PG.z - CamPos.z);
		Mathf::Vector3 P1Z = GetDisplayPoint(PG.x + 0.f - CamPos.x, PG.y + 1.f - CamPos.y, PG.z - CamPos.z);

		Mathf::Vector3 Vector;
		Vector.x = Mathf::Vector3::Dot(Mathf::Vector3::Cross((MP1 - P1).Nomalize(), (P1Z - P1).Nomalize()), Mathf::Vector3::Cross((P1X - P1).Nomalize(), (P1Z - P1).Nomalize()));//Sin
		Vector.y = Mathf::Vector3::Dot((MP1 - P1).Nomalize(), (P1Z - P1).Nomalize());//Cos
		Vector.z = 0.f;

		m_Character.SetGunRad(std::atan2f(Vector.x, Vector.y));
		if (ShotSubKey) {
			m_Character.SetBullet(1, PG, Vector.Nomalize() * 2.f);
		}

		m_Character.SetVec(Vec);

		if (UpKey) {
			if (prevUpKey != UpKey) {
				m_SonicTimer = 0.f;
			}
			else {
				m_SonicTimer += FrameWork::Instance()->GetDeltaTime();
			}
			if (m_SonicTimer > 0.025f) {
				m_SonicTimer -= 0.025f;
				m_Sonic.SetSonic(m_Character.GetPosition());
			}
		}
		m_Sonic.Update();

		prevUpKey = UpKey;
	}
	m_Character.Update();

	for (auto& e : m_Enemy) {
		Mathf::Vector3 Vec = e.GetVec();
		//Vec.x = 0.f;
		if (e.GetPosition().x < m_Character.GetPosition().x) {
			Vec.x = Mathf::Clamp(Vec.x + 0.5f * FrameWork::Instance()->GetDeltaTime(), -0.25f, 0.25f);
		}
		else {
			Vec.x = Mathf::Clamp(Vec.x - 0.5f * FrameWork::Instance()->GetDeltaTime(), -0.25f, 0.25f);
		}

		if (e.GetPosition().y < m_Character.GetPosition().y) {
			Mathf::Easing(&Vec.y, 1.5f, 0.95f);
		}
		else {
			Mathf::Easing(&Vec.y, 0.65f, 0.95f);
		}
		e.SetVec(Vec);
	}
	for (auto& b : m_Character.GetBulletList()) {
		if (b.IsActive()) {
			SEGMENT_SEGMENT_RESULT Ret;
			for (auto& e : m_Enemy) {
				Mathf::GetSegmenttoSegment(b.GetRePos(), b.GetPosition(), e.GetRePos(), e.GetPosition(),&Ret);
				float len = 0.025f;
				if (Ret.SegA_SegB_MinDist_Square <= (len * len)) {
					Mathf::Vector3 Pos(Ret.SegA_MinDist_Pos.x, Ret.SegA_MinDist_Pos.y, Ret.SegA_MinDist_Pos.z);

					b.DisActive();
					m_HitEffect.SetHitEffect(Pos+e.GetVec().Nomalize()*0.1f);
					break;
				}
			}
		}
	}
	m_HitEffect.Update();

	for (auto& e : m_Enemy) {
		e.Update();
	}
	{
		Mathf::Vector3 Pos = m_Character.GetPosition();
		Pos.x -= 0.75f;
		Pos.y -= 0.15f;

		Mathf::Easing(&CamPos, Pos, 0.9f);
		CamPos.z = 10.f;
	}
}
