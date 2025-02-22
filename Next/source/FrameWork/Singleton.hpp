#pragma once

#include "DxLib.h"

//qiita.com/TwilightUncle/items/aadc9f60f857e1ab7031���@��������e���v���[�g�ɓ����
// "Ce"��constexpr�̂���
template <int Size>
struct CeString
{
	// �����񃊃e������萄�_���s�����߂̃R���X�g���N�^
	constexpr CeString(const char(&s_literal)[Size])
	{
		for (int i = 0; i < Size; i++) buf[i] = s_literal[i];
		// �����񃊃e�����ȊO���n����Ă�������Ƃ��Ă̑̍ق�ۂ��߁A�ꉞ�ŏI�v�f�ɂ͏I�[���������Ă���
		buf[Size - 1] = '\0';
	}

	// ������i�[�̈�
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
// �q�̃T���v��
/*
class A : public SingletonBase<A> {
private:
	friend class SingletonBase<A>;
}
//*/
