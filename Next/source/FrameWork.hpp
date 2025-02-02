#pragma once

#include "Define.hpp"

class FrameWork {
private:
	static const FrameWork* m_Singleton;
public:
	static void Create(void) noexcept {
		m_Singleton = new FrameWork();
	}
	static void Release(void) noexcept {
		delete m_Singleton;
	}
	static FrameWork* Instance(void) noexcept {
		if (m_Singleton == nullptr) {
			MessageBox(NULL, "Failed FrameWork Instance Create", "", MB_OK);
			exit(-1);
		}
		// if (m_Singleton == nullptr) { m_Singleton = new FrameWork(); }
		return (FrameWork*)m_Singleton;
	}
private:
	FrameWork() {
		Init();
	}
	~FrameWork() {
		Dispose();
	}
private:
	FrameWork(const FrameWork&) = delete;
	FrameWork& operator=(const FrameWork&) = delete;
	FrameWork(FrameWork&&) = delete;
	FrameWork& operator=(FrameWork&&) = delete;
private:
	int m_WindowWidth = 1280;			//�E�B���h�E�T�C�YX
	int m_WindowHeight = 720;			//�E�B���h�E�T�C�YY

	int m_ScreenWidth = 960;			//�X�N���[���T�C�YX
	int m_ScreenHeight = 720;			//�X�N���[���T�C�YY

	LONGLONG MicroSecondOnLoopStartFrame = 0;
	LONGLONG MicroSecondDeltaTime = 1000 * 1000 / 60;
private:
	bool isVsync = true;
public:
	const int GetWindowWidth() const { return m_WindowWidth; }
	const int GetWindowHeight() const { return m_WindowHeight; }
	const int GetScreenWidth() const { return m_ScreenWidth; }
	const int GetScreenHeight() const { return m_ScreenHeight; }

	//���[�v�J�n����̌o�ߕb�����}�C�N���b�Ŏ��
	const LONGLONG GetNowTimeStart() const { return GetNowHiPerformanceCount() - MicroSecondOnLoopStartFrame; }
	//�f���^�^�C����b�Ŏ��
	const float GetDeltaTime() const { return (float)(MicroSecondDeltaTime) / 1000.f / 1000.f; }
public:
	void Init();
	bool Flip();
	void Dispose();
};
