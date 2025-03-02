#pragma once

#include "Define.hpp"

#include <string>
#include <vector>

class SoundHandle : public ResourceHandle {
	ResourceHandle::ResourceHandle;
public:
	void Create(const TCHAR* FileName, int SoundType = DX_SOUNDDATATYPE_MEMNOPRESS, int BufferNum = 3, int UnionHandle = -1) {
		SetCreateSoundDataType(SoundType);
		this->SetHandle(LoadSoundMem(FileName));
		SetCreateSoundDataType(DX_SOUNDDATATYPE_MEMNOPRESS);
	}
	void ReleaseSound() {
		DeleteSoundMem(this->GetHandle());
		this->SetInvalid();
	}
};

enum class SoundType {
	BGM,
	SE,
};

class SoundPool : public SingletonBase<SoundPool, "SoundPool"> {
private:
	friend class SingletonBase<SoundPool, "SoundPool">;
private:
	SoundPool() {}
	virtual ~SoundPool() {
		Dispose();
	}
private:
	SoundPool(const SoundPool&) = delete;
	SoundPool& operator=(const SoundPool&) = delete;
	SoundPool(SoundPool&&) = delete;
	SoundPool& operator=(SoundPool&&) = delete;
private:
	struct SoundOnce {
		SoundType m_Type{};
		std::vector<SoundHandle> m_Handle{};
		int m_Now{};
		std::string m_FileName{};
		int m_SoundLoad{};
		int m_BufferNum{};
		int m_UnionHandle{};
	public:
		bool IsSameSound(SoundType Type, const TCHAR* FileName, int SoundLoad = DX_SOUNDDATATYPE_MEMNOPRESS, int BufferNum = 3, int UnionHandle = -1) const {
			return (m_Type == Type) && (m_FileName == FileName) && (m_SoundLoad == SoundLoad) && (m_BufferNum == BufferNum) && (m_UnionHandle == UnionHandle);
		}
	};
private:
	std::vector<SoundOnce> m_Once;
private:
	SoundOnce* Get(SoundType Type, const TCHAR* FileName, int SoundLoad = DX_SOUNDDATATYPE_MEMNOPRESS, int BufferNum = 3, int UnionHandle = -1) {
		for (auto& o : m_Once) {
			if (o.IsSameSound(Type, FileName, SoundLoad, BufferNum, UnionHandle)) {
				return &o;
			}
		}
		return nullptr;
	}
	void Dispose() {
		for (auto& o : m_Once) {
			for (auto& s : o.m_Handle) {
				s.ReleaseSound();
			}
			o.m_Handle.clear();
		}
		m_Once.clear();
	}
public:
	void Add(int SoundNum, SoundType Type, const TCHAR* FileName, int SoundLoad = DX_SOUNDDATATYPE_MEMNOPRESS, int BufferNum = 3, int UnionHandle = -1);
	void Play(int PlayType, int TopPositionFlag, SoundType Type, const TCHAR* FileName, int SoundLoad = DX_SOUNDDATATYPE_MEMNOPRESS, int BufferNum = 3, int UnionHandle = -1) {
		SoundOnce* Ptr = Get(Type, FileName, SoundLoad, BufferNum, UnionHandle);
		if (Ptr) {
			PlaySoundMem(Ptr->m_Handle.at(Ptr->m_Now).GetHandle(), PlayType, TopPositionFlag);
			++Ptr->m_Now %= static_cast<int>(Ptr->m_Handle.size());
		}
	}
	void StopAll(SoundType Type, const TCHAR* FileName, int SoundLoad = DX_SOUNDDATATYPE_MEMNOPRESS, int BufferNum = 3, int UnionHandle = -1) {
		SoundOnce* Ptr = Get(Type, FileName, SoundLoad, BufferNum, UnionHandle);
		if (Ptr) {
			for (auto& h : Ptr->m_Handle) {
				StopSoundMem(h.GetHandle());
			}
		}
	}
	void SetVol(SoundType Type, int Per) {
		for (auto& o : m_Once) {
			if (o.m_Type == Type) {
				for (auto& h : o.m_Handle) {
					ChangeVolumeSoundMem(Per * 255 / 100, h.GetHandle());
				}
			}
		}
	}
	void Del(SoundType Type, const TCHAR* FileName, int SoundLoad = DX_SOUNDDATATYPE_MEMNOPRESS, int BufferNum = 3, int UnionHandle = -1) {
		for (auto& o : m_Once) {
			if (o.IsSameSound(Type, FileName, SoundLoad, BufferNum, UnionHandle)) {
				for (auto& h : o.m_Handle) {
					StopSoundMem(h.GetHandle());
					h.ReleaseSound();
				}
				o.m_Handle.clear();
				m_Once.erase(m_Once.begin() + (&o-&m_Once.front()));
				break;
			}
		}
	}
};