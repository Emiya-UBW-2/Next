#pragma once

#include "DxLib.h"

//qiita.com/TwilightUncle/items/aadc9f60f857e1ab7031より　文字列をテンプレートに入れる
// "Ce"はconstexprのつもり
template <int Size>
struct CeString
{
	// 文字列リテラルより推論を行うためのコンストラクタ
	constexpr CeString(const char(&s_literal)[Size])
	{
		for (int i = 0; i < Size; i++) buf[i] = s_literal[i];
		// 文字列リテラル以外が渡されても文字列としての体裁を保つため、一応最終要素には終端文字を入れておく
		buf[Size - 1] = '\0';
	}

	// 文字列格納領域
	char buf[Size];
};

template <class T, CeString MES>
class SingletonBase {
private:
	static const T* m_Singleton;
public:
	static void Create(void) noexcept {
		m_Singleton = new T();
	}
	static void Release(void) noexcept {
		delete m_Singleton;
	}
	static T* Instance(void) noexcept {
		if (m_Singleton == nullptr) {
			char Message[256]{};
			sprintfDx(Message, "Failed Instance Create[%s]", MES.buf);
			MessageBox(NULL, Message, "", MB_OK);
			exit(-1);
		}
		// if (m_Singleton == nullptr) { m_Singleton = new T(); }
		return (T*)m_Singleton;
	}
protected:
	SingletonBase(void) noexcept {}
	virtual ~SingletonBase(void) noexcept {}
private:
	SingletonBase(const SingletonBase&) = delete;
	SingletonBase& operator=(const SingletonBase&) = delete;
	SingletonBase(SingletonBase&&) = delete;
	SingletonBase& operator=(SingletonBase&&) = delete;
};
// 子のサンプル
/*
class A : public SingletonBase<A> {
private:
	friend class SingletonBase<A>;
}
//*/
