#pragma once

#include "../FrameWork/Define.hpp"

class TaskTab {
	int x{}, y{}, w{}, h{}, hAdd{};
	bool OnMouse = false;
	bool IsActiveTab = false;
	std::string m_TabName;
	int m_Column = 0;
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

class MainScene : public BaseScene {
	using BaseScene::BaseScene;
private:
	int m_YListPos = 0;
	int m_Yoffset = 0;
	int m_YMax = 0;
	std::vector<std::unique_ptr<TaskTab>> m_TaskTabs;
	GraphHandle m_ListGraph{};
private:
	void DrawTab() const;
protected:
	void InitSub() override;
	void UpdateSub() override;
	void DrawSub() const override;
	void DisposeSub() override;

	std::unique_ptr<BaseScene> MakeNextScene() override;
};

