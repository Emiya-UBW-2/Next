#pragma once
#include "ResourceHandle.hpp"
#include "DxLib.h"
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

class FontPool {
private:
	static const FontPool* m_Singleton;
public:
	static void Create(void) noexcept {
		m_Singleton = new FontPool();
	}
	static void Release(void) noexcept {
		delete m_Singleton;
	}
	static FontPool* Instance(void) noexcept {
		if (m_Singleton == nullptr) {
			MessageBox(NULL, "Failed FontPool Instance Create", "", MB_OK);
			exit(-1);
		}
		// if (m_Singleton == nullptr) { m_Singleton = new FontPool(); }
		return (FontPool*)m_Singleton;
	}
private:
	FontPool() {
	}
	~FontPool() {
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
		int m_Size;
		int m_Thick;
		int m_FontType = -1;
		int m_CharSet = -1;
		int m_EdgeSize = -1;
		bool m_Italic = false;
		int m_Handle = -1;
	public:
		bool IsSameFont(const TCHAR* FontName, int Size, int Thick, int FontType = -1, int CharSet = -1, int EdgeSize = -1, bool Italic = false, int Handle = -1) {
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