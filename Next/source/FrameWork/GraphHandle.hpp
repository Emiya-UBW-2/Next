#pragma once

constexpr int InvalidHandle = -1;
class ResourceHandle {
	int m_handle{ InvalidHandle };
public:
	void SetHandle(int handle) { m_handle = handle; }
	void SetInvalid() { SetHandle(InvalidHandle); }
	const bool IsActive() const { return m_handle != InvalidHandle; }
	const int GetHandle() const { return m_handle; }
public:
	ResourceHandle() {
		SetInvalid();
	}
};

#include "DxLib.h"

class GraphHandle : public ResourceHandle {
	ResourceHandle::ResourceHandle;
public:
	void Create(int SizeX, int SizeY, bool UseAlphaChannel = false) {
		this->SetHandle(MakeScreen(SizeX, SizeY, UseAlphaChannel));
	}
	void LoadGraph(const TCHAR* FileName, bool NotUse3DFlag = false) {
		this->SetHandle(DxLib::LoadGraph(FileName, NotUse3DFlag));
	}
	void ReleaseGraph() {
		DeleteGraph(this->GetHandle());
		this->SetInvalid();
	}
};

class FontHandle : public ResourceHandle {
	ResourceHandle::ResourceHandle;
public:
	void Create(const TCHAR* FontName, int Size, int Thick, int FontType = -1, int CharSet = -1, int EdgeSize = -1, bool Italic = false, int Handle = -1) {
		this->SetHandle(CreateFontToHandle(FontName, Size, Thick, FontType, CharSet, EdgeSize, Italic, Handle));
	}
	void ReleaseFont() {
		DeleteFontToHandle(this->GetHandle());
		this->SetInvalid();
	}
};

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
