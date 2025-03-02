#pragma once

#include "Define.hpp"
static const char* ConfigStr[] = {
	"WindowSetting",
	"BGM",
	"SE",
	"MuteBGM",
	"MuteSE",
};

enum class WindowSetting {
	Default,
	WindowMode = Default,
	BorderLess,
	FullScreen,
};

static const char* WindowSettingStr[] = {
	"Window",
	"BorderLess",
	"FullScreen",
};

class SaveData : public SingletonBase<SaveData, "SaveData"> {
private:
	friend class SingletonBase<SaveData, "SaveData">;
private:
	SaveData() {
		SetPath();
		Load();
	}
	virtual ~SaveData() {
		Save();
	}
private:
	SaveData(const SaveData&) = delete;
	SaveData& operator=(const SaveData&) = delete;
	SaveData(SaveData&&) = delete;
	SaveData& operator=(SaveData&&) = delete;
private:
	char m_SavePath[260] = {};
	WindowSetting m_WindowSetting = WindowSetting::Default;
	int m_BGMVol = 50;
	int m_SEVol = 50;
	bool m_IsMuteBGMVol = false;
	bool m_IsMuteSEVol = false;
public:
	WindowSetting GetWindowSetting() const { return m_WindowSetting; }
	const char* GetWindowSettingStr() const { return WindowSettingStr[(int)m_WindowSetting]; }
	int GetBGMVol() const { return m_BGMVol; }
	int GetSEVol() const { return m_SEVol; }
	bool GetIsMuteBGMVol() const { return m_IsMuteBGMVol; }
	bool GetIsMuteSEVol() const { return m_IsMuteSEVol; }

	void SetWindowSetting(WindowSetting value) { m_WindowSetting = value; }
	void SetBGMVol(int value) { m_BGMVol = value; }
	void SetSEVol(int value) { m_SEVol = value; }
	void SetIsMuteBGMVol(bool value) { m_IsMuteBGMVol = value; }
	void SetIsMuteSEVol(bool value) { m_IsMuteSEVol = value; }
public:
	void SetPath();
	void Load();
	void Save();
};

