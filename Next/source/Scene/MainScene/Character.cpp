#include "Character.hpp"

#include "Camera.hpp"
#include "Effect.hpp"
#include "SoundControl.hpp"

bool Character::SetBullet(int ID, const Mathf::Vector3& Pos, const Mathf::Vector3& Vec) {
	if (ShotInterval.at(ID) == 0.f) {
		Mathf::Vector3 CaseOffset(static_cast<float>(GetRand(100) - 50) / 100.f * 0.01f, 0.f, 0.f);
		Mathf::Vector3 CaseSpeed(0.f, 0.f, -10.f);
		for (auto& b : m_Bullet) {
			if (!b.IsActive()) {
				b.Init(Pos, m_Vec, Vec, 10.f, (ID == 0) ? 100 : 34);
				ShotInterval.at(ID) = 0.25f;
				SoundControl::Instance()->SetSE((int)EnumSE::Shot);
				return true;
			}
		}
	}
	return false;
}
void Character::SetDamage(int Damage) {
	if (Damage > 0 || Damage == -MaxHP) {
		m_DamageTime = 1.f;
	}
	m_HitPoint = static_cast<int>(Mathf::Clamp(static_cast<float>(m_HitPoint - Damage), 0.f, static_cast<float>(MaxHP)));
	if (m_HitPoint == 0) {
		m_RespawnTime = 3.f;
	}
}
void Character::Spawn(const Mathf::Vector3& Pos) {
	m_Pos = Pos;
	m_RePos = m_Pos;

	m_Vec.x = 0.f;
	m_Vec.y = 1.f;
	m_Vec.z = 0.f;

	m_Rad = 0.f;

	{
		Mathf::Vector3 Offset = Mathf::Vector3(-0.03f * -std::sin(-m_Rad), -0.03f * std::cos(-m_Rad), 0.f);
		float Per = 0.02f * 1.f / m_Vec.y;
		Mathf::Vector3 Pos0 = m_Pos + Offset + Mathf::Vector3(-Per * std::cos(-m_Rad), -Per * std::sin(-m_Rad), 0.f);
		Mathf::Vector3 Pos1 = m_Pos + Offset + Mathf::Vector3(Per * std::cos(-m_Rad), Per * std::sin(-m_Rad), 0.f);
		SmokeVector.at(0) = Pos0;
		SmokeVector.at(1) = Pos1;
	}
	m_BoostTimer = 1.f;
	m_BoostActive = true;
}
void Character::SetLRVec(bool IsLeft, bool IsRight) {
	Mathf::Vector3 Vec = this->GetVec();
	if (IsRight) {
		Vec.x = Mathf::Clamp(Vec.x + 0.5f * FrameWork::Instance()->GetDeltaTime(), -0.25f, 0.25f);
	}
	if (IsLeft) {
		Vec.x = Mathf::Clamp(Vec.x - 0.5f * FrameWork::Instance()->GetDeltaTime(), -0.25f, 0.25f);
	}
	if (!IsLeft && !IsRight) {
		Mathf::Easing(&Vec.x, 0.f, 0.95f);
	}
	this->SetVec(Vec);
}

void Character::Init(const Mathf::Vector3& Pos, std::string Name, float Ofs) {
	Spawn(Pos);
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
	m_BoostTimer = 1.f;
	m_BoostActive = true;
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
	{
		Mathf::Vector3 Pos0 = m_Pos + Offset + Mathf::Vector3(-Per * std::cos(-m_Rad), -Per * std::sin(-m_Rad), 0.f);
		Mathf::Vector3 Pos1 = m_Pos + Offset + Mathf::Vector3(Per * std::cos(-m_Rad), Per * std::sin(-m_Rad), 0.f);

		if (IsAlive()) {
			EffectControl::Instance()->SetEffect((int)EnumEffect::Smoke, Pos0, SmokeVector.at(0));
			EffectControl::Instance()->SetEffect((int)EnumEffect::Smoke, Pos1, SmokeVector.at(1));
		}

		SmokeVector.at(0) = Pos0;
		SmokeVector.at(1) = Pos1;
	}
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
	//エフェクト
	{
		if (!this->IsAlive()) {
			if (this->GetPosition().z > 1.f) {
				if (this->m_DeathEffectInterval == 0.f) {
					this->m_DeathEffectInterval = 0.1f;
					EffectControl::Instance()->SetEffect((int)EnumEffect::Hit, this->GetPosition() + this->GetVec().Nomalize() * 0.1f, this->GetPosition());
				}
			}
			if (this->GetPosition().z <= 0.f) {
				if (this->m_DeathEffectFlag == false) {
					this->m_DeathEffectFlag = true;
					EffectControl::Instance()->SetEffect((int)EnumEffect::Death, this->GetPosition() + this->GetVec().Nomalize() * 0.1f, this->GetPosition());
					SoundControl::Instance()->SetSE((int)EnumSE::Death);
				}
			}
		}
		else {
			this->m_DeathEffectFlag = false;
		}
		this->m_DeathEffectInterval = Mathf::Max(this->m_DeathEffectInterval - FrameWork::Instance()->GetDeltaTime(), 0.f);
	}
}
void Character::DrawShadow() const {
	if (!IsAlive()) {
		if (m_Pos.z < 0.f) { return; }
	}

	for (auto& b : m_Bullet) {
		if (b.IsActive()) {
			b.DrawShadow();
		}
	}

	if (!IsAlive()) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255.f * 2.f * m_Pos.z / 20.f));
	}


	Mathf::Vector3 P1 = MainCamera::Instance()->GetDisplayPoint(m_Pos.x, m_Pos.y, 0.f);
	SetDrawBright(0, 0, 0);
	DrawRotaGraph(static_cast<int>(P1.x), static_cast<int>(P1.y), 1.0, static_cast<double>(m_Rad * 1.5f), m_GraphHandle.at(m_GraphAnim).GetHandle(), TRUE);
	SetDrawBright(255, 255, 255);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}
void Character::Draw() const {
	for (auto& b : m_Bullet) {
		if (b.IsActive()) {
			b.Draw();
		}
	}

	if (!IsAlive()) {
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255.f * 2.f * m_Pos.z / 20.f));
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
void Character::Dispose() {
	m_GraphHandle.at(0).ReleaseGraph();
	m_GraphHandle.at(1).ReleaseGraph();
	m_GraphHandle.at(2).ReleaseGraph();
	m_GraphHandle.at(3).ReleaseGraph();
	m_SubHandle.ReleaseGraph();
}