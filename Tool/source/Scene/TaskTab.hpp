#pragma once

#include "../FrameWork/Define.hpp"

#include <opencv2/opencv.hpp>
#include <thread>

class MatchTask {
	//
	cv::Mat monitor_img{};
	cv::Mat checkimage{};
	cv::Mat resultimage{};

	int matchWidth{}, matchHeight{};
	int matchPointX{}, matchPointY{};
	bool isMatch{};

	std::thread m_MatchThread{};
	bool m_IsMatchThreadEnd{};
public:
	const bool IsMatch() const { return isMatch; }
	const int MatchPosX() const { return matchPointX + matchWidth / 2; }
	const int MatchPosY() const { return matchPointY + matchHeight / 2; }
public:
	MatchTask(const char* Path);
	~MatchTask();
public:
	void Update();
};


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
	LeftMouseTrigger,
	LeftMouseDragStart,
	LeftMouseDragEnd,
	Max,
};
static const char* TaskMoveStr[] = {
	"LeftMouseTrigger",
	"LeftMouseDragStart",
	"LeftMouseDragEnd",
};

enum class TaskNext {
	GoNextTask,
	Max,
};
static const char* TaskNextStr[] = {
	"GoNextTask",
};


struct TabColumn {
	std::string m_TabString;
	bool m_OnMouse{};
};

class TaskTab {
	int x{}, y{}, w{}, wSub{}, h{}, hAdd{};
	bool OnMouse = false;
	bool IsActiveTab = false;
	bool IsPlayTask = false;
	bool m_IsEnd = false;
	std::string m_TabName;
	DialogManager m_Dialog;
	std::string m_CheckFilePath;
	std::vector<TabColumn> m_TabColumn{};
	TaskType m_TaskType = TaskType::ClickPoint;
	TaskMove m_TaskMove = TaskMove::LeftMouseTrigger;
	TaskNext m_TaskNext = TaskNext::GoNextTask;
	int m_Type{};
	std::unique_ptr<MatchTask> m_MatchTask;
public:
	void Start() {
		IsPlayTask = true;
		m_IsEnd = false;
	}
	void EndTask() {
		IsPlayTask = false;
		m_IsEnd = false;
	}

	bool IsEnd() const { return m_IsEnd; }
public:
	void SetTime(int Hour, int Minute, int Second) {
		m_Type = 60 * 60 * (Hour % 24) + 60 * (Minute % 60) + (Second % 60);
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
public:
	void SetPos(int XP, int YP) {
		x = 16 + XP;
		y = YP;
	}
	const int GetHeight() const { return h + hAdd; }
public:
	TaskTab(const char* TabName);
	~TaskTab() {}
public:
	void Update(int xofs, int yofs);
	void Draw() const;
};

class TaskDraw {
	int m_YListPos = 0;
	int m_Yoffset = 0;
	int m_YMax = 0;
	std::vector<std::unique_ptr<TaskTab>> m_TaskTabs;
	GraphHandle m_ListGraph{};
	bool OnMouse{};

	bool IsTaskStart{};
private:
	void DrawTab() const;
public:
	TaskDraw();
	void Update();
	void Draw() const;
};