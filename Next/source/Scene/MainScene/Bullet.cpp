#include "Bullet.hpp"

#include "Camera.hpp"

void Bullet::Update() {
	m_RePos = m_Pos;
	m_Pos += (m_BaseVec + m_AddVec) * FrameWork::Instance()->GetDeltaTime();
	m_Time += FrameWork::Instance()->GetDeltaTime();
	if (m_Pos.z < 0.f) {
		DisActive();
	}
}
void Bullet::DrawShadow() const {
	if (!IsActive()) { return; }
	Mathf::Vector3 PP = m_Pos - m_AddVec.Nomalize() * 0.003f * m_Size;
	Mathf::Vector3 P1 = MainCamera::Instance()->GetDisplayPoint(m_Pos.x, m_Pos.y, 0.f);
	Mathf::Vector3 P2 = MainCamera::Instance()->GetDisplayPoint(PP.x, PP.y, 0.f);
	DrawLine(static_cast<int>(P1.x), static_cast<int>(P1.y), static_cast<int>(P2.x), static_cast<int>(P2.y), ColorPalette::Black, static_cast<int>(m_Size));
	DrawCircle(static_cast<int>(P1.x), static_cast<int>(P1.y), static_cast<int>(m_Size / 2 - 1), ColorPalette::Black, TRUE);
	DrawCircle(static_cast<int>(P2.x), static_cast<int>(P2.y), static_cast<int>(m_Size / 2 - 1), ColorPalette::Black, TRUE);
}
void Bullet::Draw() const {
	if (!IsActive()) { return; }
	Mathf::Vector3 PP = m_Pos - m_AddVec.Nomalize() * 0.003f * m_Size;
	Mathf::Vector3 P1 = MainCamera::Instance()->GetDisplayPoint(m_Pos);
	Mathf::Vector3 P2 = MainCamera::Instance()->GetDisplayPoint(PP);
	DrawLine(static_cast<int>(P1.x), static_cast<int>(P1.y), static_cast<int>(P2.x), static_cast<int>(P2.y), ColorPalette::White, static_cast<int>(m_Size));
	DrawCircle(static_cast<int>(P1.x), static_cast<int>(P1.y), static_cast<int>(m_Size / 2 - 1), ColorPalette::White, TRUE);
	DrawCircle(static_cast<int>(P2.x), static_cast<int>(P2.y), static_cast<int>(m_Size / 2 - 1), ColorPalette::White, TRUE);

}

const BulletControl* SingletonBase<BulletControl, "BulletControl">::m_Singleton = nullptr;

void BulletControl::Set(int ShooterID, const Mathf::Vector3& Pos, const Mathf::Vector3& BaseVec, const Mathf::Vector3& AddVec, float Size, int Damage) {
	m_Pool.emplace_back(std::make_unique<Bullet>());
	m_Pool.back()->Init(ShooterID, Pos, BaseVec, AddVec, Size, Damage);
}
