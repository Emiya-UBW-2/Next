#pragma once

#include "../FrameWork/Define.hpp"

class TitleScene : public BaseScene {
	using BaseScene::BaseScene;
private:
	float m_Timer = 0.f;
	bool IsGoingNextScene = false;
	bool OnMouse = false;
protected:
	void InitSub() override;
	void UpdateSub() override;
	void DrawSub() const override;
	void DisposeSub() override;

	std::unique_ptr<BaseScene> MakeNextScene() override;
};

