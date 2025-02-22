#pragma once
#include "ResourceHandle.hpp"
#include "Algorithm.hpp"
#include "DxLib.h"
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

class SoundPool {
private:
	static const SoundPool* m_Singleton;
public:
	static void Create(void) noexcept {
		m_Singleton = new SoundPool();
	}
	static void Release(void) noexcept {
		delete m_Singleton;
	}
	static SoundPool* Instance(void) noexcept {
		if (m_Singleton == nullptr) {
			MessageBox(NULL, "Failed SoundPool Instance Create", "", MB_OK);
			exit(-1);
		}
		// if (m_Singleton == nullptr) { m_Singleton = new SoundPool(); }
		return (SoundPool*)m_Singleton;
	}
private:
	SoundPool() {
	}
	~SoundPool() {
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
		int m_SoundType{};
		int m_BufferNum{};
		int m_UnionHandle{};
	public:
		bool IsSameSound(SoundType Type, const TCHAR* FileName, int SoundType = DX_SOUNDDATATYPE_MEMNOPRESS, int BufferNum = 3, int UnionHandle = -1) const {
			return (m_Type == Type) && (m_FileName == FileName) && (m_SoundType == SoundType) && (m_BufferNum == BufferNum) && (m_UnionHandle == UnionHandle);
		}
	};
private:
	std::vector<SoundOnce> m_Once;
private:
	SoundOnce* Get(SoundType Type, const TCHAR* FileName, int SoundType = DX_SOUNDDATATYPE_MEMNOPRESS, int BufferNum = 3, int UnionHandle = -1) {
		for (auto& o : m_Once) {
			if (o.IsSameSound(Type, FileName, SoundType, BufferNum, UnionHandle)) {
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
	void Add(int SoundNum, SoundType Type, const TCHAR* FileName, int SoundType = DX_SOUNDDATATYPE_MEMNOPRESS, int BufferNum = 3, int UnionHandle = -1) {
		if (Get(Type, FileName, SoundType, BufferNum, UnionHandle)) {
			return;
		}
		m_Once.emplace_back();
		m_Once.back().m_Handle.resize(static_cast<int>(Mathf::Max(1.f, static_cast<float>(SoundNum))));
		m_Once.back().m_Handle.at(0).Create(FileName, SoundType, BufferNum, UnionHandle);
		for (int loop = 1; loop < m_Once.back().m_Handle.size(); ++loop) {
			m_Once.back().m_Handle.at(loop).SetHandle(DuplicateSoundMem(m_Once.back().m_Handle.at(0).GetHandle(), BufferNum));
		}
		m_Once.back().m_FileName = FileName;
		m_Once.back().m_SoundType = SoundType;
		m_Once.back().m_BufferNum = BufferNum;
		m_Once.back().m_UnionHandle = UnionHandle;
	}
	void Play(int PlayType, int TopPositionFlag, SoundType Type, const TCHAR* FileName, int SoundType = DX_SOUNDDATATYPE_MEMNOPRESS, int BufferNum = 3, int UnionHandle = -1) {
		SoundOnce* Ptr = Get(Type, FileName, SoundType, BufferNum, UnionHandle);
		if (Ptr) {
			PlaySoundMem(Ptr->m_Handle.at(Ptr->m_Now).GetHandle(), PlayType, TopPositionFlag);
			++Ptr->m_Now %= static_cast<int>(Ptr->m_Handle.size());
		}
	}
	void StopAll(SoundType Type, const TCHAR* FileName, int SoundType = DX_SOUNDDATATYPE_MEMNOPRESS, int BufferNum = 3, int UnionHandle = -1) {
		SoundOnce* Ptr = Get(Type, FileName, SoundType, BufferNum, UnionHandle);
		if (Ptr) {
			for (auto& h : Ptr->m_Handle) {
				StopSoundMem(h.GetHandle());
			}
		}
	}
	void Del(SoundType Type, const TCHAR* FileName, int SoundType = DX_SOUNDDATATYPE_MEMNOPRESS, int BufferNum = 3, int UnionHandle = -1) {
		for (auto& o : m_Once) {
			if (o.IsSameSound(Type, FileName, SoundType, BufferNum, UnionHandle)) {
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