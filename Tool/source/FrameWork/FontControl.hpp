#pragma once

#include "Define.hpp"

#include <string>
#include <vector>

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

class FontPool : public SingletonBase<FontPool, "FontPool"> {
private:
	friend class SingletonBase<FontPool, "FontPool">;
private:
	FontPool() {}
	virtual ~FontPool() {
		Dispose();
	}
private:
	FontPool(const FontPool&) = delete;
	FontPool& operator=(const FontPool&) = delete;
	FontPool(FontPool&&) = delete;
	FontPool& operator=(FontPool&&) = delete;
private:
	struct FontOnce {
		FontHandle m_FontHandle{};

		std::string m_FontName{};
		int m_Size = 0;
		int m_Thick = 0;
		int m_FontType = -1;
		int m_CharSet = -1;
		int m_EdgeSize = -1;
		bool m_Italic = false;
		int m_Handle = -1;
	public:
		bool IsSameFont(const TCHAR* FontName, int Size, int Thick, int FontType = -1, int CharSet = -1, int EdgeSize = -1, bool Italic = false, int Handle = -1) const {
			return (m_FontName == FontName) &&
			(m_Size == Size) &&
			(m_Thick == Thick) &&
			(m_FontType == FontType) &&
			(m_CharSet == CharSet) &&
			(m_EdgeSize == EdgeSize) &&
			(m_Italic == Italic) &&
			(m_Handle == Handle);
		}
	};
private:
	std::vector<FontOnce> m_Once;
private:
	void Dispose() {
		for (auto& o : m_Once) {
			o.m_FontHandle.ReleaseFont();
		}
		m_Once.clear();
	}
public:
	void Add(const TCHAR* FontName, int Size, int Thick, int FontType = -1, int CharSet = -1, int EdgeSize = -1, bool Italic = false, int Handle = -1) {
		if (Get(FontName, Size, Thick, FontType, CharSet, EdgeSize, Italic, Handle)) {
			return;
		}
		m_Once.emplace_back();
		m_Once.back().m_FontHandle.Create(FontName, Size, Thick, FontType, CharSet, EdgeSize, Italic, Handle);
		m_Once.back().m_FontName = FontName;
		m_Once.back().m_Size = Size;
		m_Once.back().m_Thick = Thick;
		m_Once.back().m_FontType = FontType;
		m_Once.back().m_CharSet = CharSet;
		m_Once.back().m_EdgeSize = EdgeSize;
		m_Once.back().m_Italic = Italic;
		m_Once.back().m_Handle = Handle;
	}
	FontHandle* Get(const TCHAR* FontName, int Size, int Thick, int FontType = -1, int CharSet = -1, int EdgeSize = -1, bool Italic = false, int Handle = -1) {
		for (auto& o : m_Once) {
			if (o.IsSameFont(FontName, Size, Thick, FontType, CharSet, EdgeSize, Italic, Handle)) {
				return &o.m_FontHandle;
			}
		}
		return nullptr;
	}
};