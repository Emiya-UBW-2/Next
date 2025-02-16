#include "SoundControl.hpp"

const SoundControl* SoundControl::m_Singleton = nullptr;

//
void SoundControl::SetSE(int Type) {
	switch ((EnumSE)Type) {
	case EnumSE::Shot:
	{
		PlaySoundMem(m_ShotSE.at(m_ShotSENow).GetHandle(), DX_PLAYTYPE_BACK);
		++m_ShotSENow %= static_cast<int>(m_ShotSE.size());
	}
	break;
	case EnumSE::Damage:
	{
		PlaySoundMem(m_DamageSE.at(m_DamageSENow).GetHandle(), DX_PLAYTYPE_BACK);
		++m_DamageSENow %= static_cast<int>(m_DamageSE.size());
	}
	break;
	case EnumSE::Death:
	{
		PlaySoundMem(m_DeathSE.at(m_DeathSENow).GetHandle(), DX_PLAYTYPE_BACK);
		++m_DeathSENow %= static_cast<int>(m_DeathSE.size());
	}
	break;
	default:
		break;
	}
}
