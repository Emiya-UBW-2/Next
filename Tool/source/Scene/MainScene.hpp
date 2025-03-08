#pragma once

#include "../FrameWork/Define.hpp"

#include "TaskTab.hpp"

class MainScene : public BaseScene {
	using BaseScene::BaseScene;
private:
	std::unique_ptr<TaskDraw> m_TaskDraw;
protected:
	void InitSub() override;
	void UpdateSub() override;
	void DrawSub() const override;
	void DisposeSub() override;

	std::unique_ptr<BaseScene> MakeNextScene() override;
};

