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
	void ReleaseGraph() {
		DeleteGraph(this->GetHandle());
		this->SetInvalid();
	}
};

class FontHandle : public ResourceHandle {
	ResourceHandle::ResourceHandle;
public:
	void ReleaseFont() {
		DeleteFontToHandle(this->GetHandle());
		this->SetInvalid();
	}
};

class SoundHandle : public ResourceHandle {
	ResourceHandle::ResourceHandle;
public:
	void ReleaseSound() {
		DeleteSoundMem(this->GetHandle());
		this->SetInvalid();
	}
};
