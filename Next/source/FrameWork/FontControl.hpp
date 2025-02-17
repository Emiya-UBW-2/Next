#pragma once
#include "ResourceHandle.hpp"
#include "DxLib.h"

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
