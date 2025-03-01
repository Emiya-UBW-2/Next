#pragma once

#include "Define.hpp"
static const char* ConfigStr[] = {
	"WindowSetting",
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
	WindowSetting m_WindowSetting = WindowSetting::Default;
	char m_SavePath[260] = {};
public:
	WindowSetting GetWindowSetting() const { return m_WindowSetting; }
	const char* GetWindowSettingStr() const { return WindowSettingStr[(int)m_WindowSetting]; }


	void SetWindowSetting(WindowSetting value) {
		m_WindowSetting = value;
	}
public:
	void SetPath();
	void Load();
	void Save();
};

