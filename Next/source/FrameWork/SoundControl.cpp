#include "SoundControl.hpp"

const SoundPool* SingletonBase<SoundPool, "SoundPool">::m_Singleton = nullptr;

void SoundPool::Add(int SoundNum, SoundType Type, const TCHAR* FileName, int SoundLoad, int BufferNum, int UnionHandle) {
	if (Get(Type, FileName, SoundLoad, BufferNum, UnionHandle)) {
		return;
	}
	m_Once.emplace_back();
	m_Once.back().m_Handle.resize(static_cast<int>(Mathf::Max(1.f, static_cast<float>(SoundNum))));
	m_Once.back().m_Handle.at(0).Create(FileName, SoundLoad, BufferNum, UnionHandle);
	for (int loop = 1; loop < m_Once.back().m_Handle.size(); ++loop) {
		m_Once.back().m_Handle.at(loop).SetHandle(DuplicateSoundMem(m_Once.back().m_Handle.at(0).GetHandle(), BufferNum));
	}
	switch (Type) {
	case SoundType::BGM:
		SetVol(Type, SaveData::Instance()->GetIsMuteBGMVol() ? 0 : SaveData::Instance()->GetBGMVol());
		break;
	case SoundType::SE:
		SetVol(Type, SaveData::Instance()->GetIsMuteSEVol() ? 0 : SaveData::Instance()->GetSEVol());
		break;
	default:
		break;
	}
	m_Once.back().m_Type = Type;
	m_Once.back().m_FileName = FileName;
	m_Once.back().m_SoundLoad = SoundLoad;
	m_Once.back().m_BufferNum = BufferNum;
	m_Once.back().m_UnionHandle = UnionHandle;
}