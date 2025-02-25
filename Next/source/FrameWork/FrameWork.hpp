#pragma once

#include "Define.hpp"

#include <optional>

class SceneController;

enum class WindowSetting {
	Default,
	WindowMode = Default,
	BorderLess,
	FullScreen,
};

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

	WindowSetting m_WindowSetting = WindowSetting::Default;
	int m_WindowWidth = 1280;			//�E�B���h�E�T�C�YX
	int m_WindowHeight = 720;			//�E�B���h�E�T�C�YY

	int m_WindowModeWidth = 1280;			//�E�B���h�E�T�C�YX
	int m_WindowModeHeight = 720;			//�E�B���h�E�T�C�YY
	int m_FullScreenModeWidth = 1280;			//�E�B���h�E�T�C�YX
	int m_FullScreenModeHeight = 720;			//�E�B���h�E�T�C�YY

	int m_ScreenWidth = 960;			//�X�N���[���T�C�YX
	int m_ScreenHeight = 720;			//�X�N���[���T�C�YY

	LONGLONG MicroSecondOnLoopStartFrame = 0;
	LONGLONG MicroSecondDeltaTime = 1000 * 1000 / 60;
	float		m_TimeScale = 1.f;
	std::optional<float>		m_TimeScalePrev{};
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

	const bool GetIsPauseActive() const { return m_IsPauseActive; }

	//���[�v�J�n����̌o�ߕb�����}�C�N���b�Ŏ��
	const LONGLONG GetNowTimeStart() const { return GetNowHiPerformanceCount() - MicroSecondOnLoopStartFrame; }
	//�f���^�^�C����b�Ŏ��
	const float GetFixedDeltaTime() const { return (float)(MicroSecondDeltaTime) / 1000.f / 1000.f; }//�^�C���X�P�[���Ɋ֌W�Ȃ�
	const float GetDeltaTime() const { return GetFixedDeltaTime() * m_TimeScale; }//�^�C���X�P�[���Ɋ֌W����

	void SetPauseEnable(bool value) { m_IsPauseEnable = value; }
	void SetPauseActive(bool value) {
		m_IsPauseActive = value;
		if (m_IsPauseActive) {
			m_TimeScalePrev = m_TimeScale;
			m_TimeScale = 0.0f;
		}
		else {
			if (m_TimeScalePrev.has_value()) {
				m_TimeScale = m_TimeScalePrev.value();
				m_TimeScalePrev.reset();
			}
			else {
				m_TimeScale = 1.f;//�Ƃ肠����1�{�ŊJ�n
			}
		}
	}
	void SetTimeScale(float value) {
		if (m_TimeScalePrev.has_value()) {
			//�|�[�Y��ʒ��͕ύX������_���I
		}
		m_TimeScale = value;
	}
	void SetWindowSetting(WindowSetting value) {
		m_WindowSetting = value;

		// DPI�ݒ�
		int DPI = 96;
		GetMonitorDpi(NULL, &DPI);
		if (DPI == 0) {
			DPI = 96;
		}
		// DPI�𔽉f����f�X�N�g�b�v�T�C�Y
		int DispXSize = static_cast<int>(GetSystemMetrics(SM_CXSCREEN)) * 96 / DPI;
		int DispYSize = static_cast<int>(GetSystemMetrics(SM_CYSCREEN)) * 96 / DPI;

		switch (m_WindowSetting) {
		case WindowSetting::WindowMode:
			m_WindowWidth = m_WindowModeWidth;
			m_WindowHeight = m_WindowModeHeight;
			SetWindowSize(m_WindowWidth, m_WindowHeight);
			SetWindowStyleMode(0);
			SetWindowPosition((DispXSize - m_WindowWidth) / 2, (DispYSize - m_WindowHeight) / 2);
			SetWindowSizeExtendRate(1.0, 1.0);
			ChangeWindowMode(TRUE);
			break;
		case WindowSetting::BorderLess:
			m_WindowWidth = m_FullScreenModeWidth;
			m_WindowHeight = m_FullScreenModeHeight;
			SetWindowStyleMode(2);
			SetWindowPosition(0, 0);
			SetWindowSizeExtendRate(
				static_cast<double>(DispXSize) / static_cast<double>(m_WindowWidth),
				static_cast<double>(DispYSize) / static_cast<double>(m_WindowHeight)
			);
			ChangeWindowMode(TRUE);
			break;
		case WindowSetting::FullScreen:
			m_WindowWidth = m_FullScreenModeWidth;
			m_WindowHeight = m_FullScreenModeHeight;
			SetWindowStyleMode(2);
			SetWindowPosition(0, 0);
			SetWindowSizeExtendRate(1.0, 1.0);
			SetFullScreenResolutionMode(DX_FSRESOLUTIONMODE_NATIVE);
			SetFullScreenScalingMode(DX_FSSCALINGMODE_NEAREST);
			ChangeWindowMode(FALSE);
			break;
		default:
			break;
		}
	}
public:
	void Init();
	bool Update();
	void Dispose();
};

