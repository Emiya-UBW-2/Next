#pragma once

#include "Define.hpp"

#include <optional>

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

	int m_WindowWidth = 1280;			//�E�B���h�E�T�C�YX
	int m_WindowHeight = 720;			//�E�B���h�E�T�C�YY

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
public:
	void Init();
	bool Update();
	void Dispose();
};

