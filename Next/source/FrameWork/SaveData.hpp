#pragma once

#include "Define.hpp"

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
public:
	WindowSetting GetWindowSetting() const { return m_WindowSetting; }
	const char* GetWindowSettingStr() const { return WindowSettingStr[(int)m_WindowSetting]; }


	void SetWindowSetting(WindowSetting value) {
		m_WindowSetting = value;
	}
public:
	void Load();
	void Save();
};

