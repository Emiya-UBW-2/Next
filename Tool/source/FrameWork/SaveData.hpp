#pragma once

#include "Define.hpp"

#include <string>
#include <vector>

enum class TaskType {
	ClickPoint,
	KeyType,
	Max,
};
static const char* TaskTypeStr[] = {
	"ClickPoint",
	"KeyType",
};

enum class TaskMove {
	Trigger,
	DragStart,
	DragEnd,
	Max,
};
static const char* TaskMoveStr[] = {
	"Trigger",
	"DragStart",
	"DragEnd",
};

enum class TaskNext {
	GoNextTask,
	Max,
};
static const char* TaskNextStr[] = {
	"GoNextTask",
};

static const char* ConfigStr[] = {
	"WindowSetting",
	"BGM",
	"SE",
	"MuteBGM",
	"MuteSE",
};

static const char* TaskStr[] = {
	"Name",
	"Type",
	"Hour",
	"Min",
	"Sec",
	"Icon",
	"Key",
	"Move",
	"Next",
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

struct TabData {
	std::string m_TabName = "New Task";
	std::string m_CheckFilePath = "data\\Test.png";
	std::string m_CheckKey = "LMOUSE";
	TaskType m_TaskType = TaskType::ClickPoint;
	TaskMove m_TaskMove = TaskMove::Trigger;
	TaskNext m_TaskNext = TaskNext::GoNextTask;
	int m_Type = GetTime(10, 0, 0);
public:
	int GetTime(int Hour, int Minute, int Second) {
		return 60 * 60 * (Hour % 24) + 60 * (Minute % 60) + (Second % 60);
	}
	void SetTime(int Hour, int Minute, int Second) {
		m_Type = GetTime(Hour, Minute, Second);
	}
	int GetHour() const {
		return (m_Type / 60 / 60) % 24;
	}
	int GetMinute() const {
		return (m_Type / 60) % 60;
	}
	int GetSecond() const {
		return (m_Type) % 60;
	}
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
	std::vector<TabData> m_TabData;
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

