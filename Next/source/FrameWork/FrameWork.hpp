#pragma once

#include "Define.hpp"

class SceneController;

class FrameWork : public SingletonBase<FrameWork, "FrameWork"> {
private:
	friend class SingletonBase<FrameWork, "FrameWork">;
private:
	FrameWork() {
		Init();
	}
	virtual ~FrameWork() {
		Dispose();
	}
private:
	FrameWork(const FrameWork&) = delete;
	FrameWork& operator=(const FrameWork&) = delete;
	FrameWork(FrameWork&&) = delete;
	FrameWork& operator=(FrameWork&&) = delete;
private:
	int BackScreen = 0;

	int m_WindowWidth = 1280;			//ウィンドウサイズX
	int m_WindowHeight = 720;			//ウィンドウサイズY

	int m_ScreenWidth = 960;			//スクリーンサイズX
	int m_ScreenHeight = 720;			//スクリーンサイズY

	LONGLONG MicroSecondOnLoopStartFrame = 0;
	LONGLONG MicroSecondDeltaTime = 1000 * 1000 / 60;

	SceneController* _SceneController{ nullptr };

	bool m_IsPauseEnable{ true };
	bool m_IsPauseActive{ false };
private:
	bool isVsync = true;
public:
	const int GetWindowWidth() const { return m_WindowWidth; }
	const int GetWindowHeight() const { return m_WindowHeight; }
	const int GetScreenWidth() const { return m_ScreenWidth; }
	const int GetScreenHeight() const { return m_ScreenHeight; }

	const int GetBackScreen() const { return BackScreen; }

	//ループ開始からの経過秒数をマイクロ秒で取る
	const LONGLONG GetNowTimeStart() const { return GetNowHiPerformanceCount() - MicroSecondOnLoopStartFrame; }
	//デルタタイムを秒で取る
	const float GetDeltaTime() const { return (float)(MicroSecondDeltaTime) / 1000.f / 1000.f; }

	void SetPauseEnable(bool value) { m_IsPauseEnable = value; }
public:
	void Init();
	bool Update();
	void Dispose();
};

