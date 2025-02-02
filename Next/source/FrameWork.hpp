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
	int m_WindowWidth = 1280;			//ウィンドウサイズX
	int m_WindowHeight = 720;			//ウィンドウサイズY

	int m_ScreenWidth = 960;			//スクリーンサイズX
	int m_ScreenHeight = 720;			//スクリーンサイズY

	LONGLONG MicroSecondOnLoopStartFrame = 0;
	LONGLONG MicroSecondDeltaTime = 1000 * 1000 / 60;
private:
	bool isVsync = true;
public:
	const int GetWindowWidth() const { return m_WindowWidth; }
	const int GetWindowHeight() const { return m_WindowHeight; }
	const int GetScreenWidth() const { return m_ScreenWidth; }
	const int GetScreenHeight() const { return m_ScreenHeight; }

	//ループ開始からの経過秒数をマイクロ秒で取る
	const LONGLONG GetNowTimeStart() const { return GetNowHiPerformanceCount() - MicroSecondOnLoopStartFrame; }
	//デルタタイムを秒で取る
	const float GetDeltaTime() const { return (float)(MicroSecondDeltaTime) / 1000.f / 1000.f; }
public:
	void Init();
	bool Flip();
	void Dispose();
};
