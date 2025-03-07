#pragma once

#include "../FrameWork/Define.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <thread>

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
	LeftMouse,
	Max,
};
static const char* TaskMoveStr[] = {
	"LeftMouse",
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
	std::string m_TabName;
	std::vector<TabColumn> m_TabColumn{};
	TaskType m_TaskType = TaskType::ClickPoint;
	TaskMove m_TaskMove = TaskMove::LeftMouse;
	TaskNext m_TaskNext = TaskNext::GoNextTask;
	int m_Type{};
public:
	void SetTime(int Hour,int Minute,int Second) {
		m_Type = 60 * 60 * (Hour % 24) + 60 * (Minute % 60) + (Second % 60);
	}
	int GetHour() const {
		return (m_Type / 60 / 60) % 24;
	}
	int GetMinute() const {
		return (m_Type/ 60) % 60;
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


static void MoveToMousePoint(int x, int y);

class MainScene : public BaseScene {
	using BaseScene::BaseScene;
private:
	int m_YListPos = 0;
	int m_Yoffset = 0;
	int m_YMax = 0;
	std::vector<std::unique_ptr<TaskTab>> m_TaskTabs;
	GraphHandle m_ListGraph{};
	int DispXSize{ 1 };
	int DispYSize{ 1 };

	//
	cv::Mat monitor_img{};
	cv::Mat checkimage{};
	cv::Mat resultimage{};

	int matchPointX{}, matchPointY{};
	bool isMatch{};

	std::thread m_MatchThread{};
	bool m_IsMatchThreadEnd{};
private:
	void DrawTab() const;
protected:
	void InitSub() override;
	void UpdateSub() override;
	void DrawSub() const override;
	void DisposeSub() override;

	std::unique_ptr<BaseScene> MakeNextScene() override;
};

