#pragma once
#include "ResourceHandle.hpp"
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
