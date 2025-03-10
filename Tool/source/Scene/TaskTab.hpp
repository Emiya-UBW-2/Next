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




struct TabColumn {
	std::string m_TabString;
	bool m_OnMouse{};
};

class TaskTab {
	int x{}, y{}, w{}, wAdd{}, wSub{}, h{}, hAdd{};
	bool OnMouseClose = false;
	bool OnMouseMove = false;
	bool OnMouse = false;
	bool IsActiveTab = false;
	bool IsPlayTask = false;
	bool m_IsEnd = false;
	DialogManager m_Dialog;
	GraphHandle m_CheckImage;
	std::vector<TabColumn> m_TabColumn{};
	std::unique_ptr<MatchTask> m_MatchTask;

	std::string m_TabName;
	TaskType m_TaskType;
	std::string m_CheckFilePath;
	std::string m_CheckKey;
	bool m_isEraseTask = false;
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
	bool IsEraseTask() const { return m_isEraseTask; }
	void ResetEraseTask() { m_isEraseTask = false; }
public:
	void SetPos(int XP, int YP) {
		x = 16 + XP;
		y = YP;
	}
	const int GetHeight() const { return h + hAdd; }
public:
	TaskTab();
	~TaskTab() {}
public:
	void Update(int xofs, int yofs, int index);
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