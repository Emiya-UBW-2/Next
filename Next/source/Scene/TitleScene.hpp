#pragma once

#include "../FrameWork/Define.hpp"

class TitleScene : public BaseScene {
	using BaseScene::BaseScene;
private:
	GraphHandle m_Title;
	GraphHandle m_TitleImage;
	float m_Timer = 0.f;
	bool IsGoingNextScene = false;
protected:
	void InitSub() override;
	void UpdateSub() override;
	void DrawSub() override;
	void DisposeSub() override;

	std::unique_ptr<BaseScene> MakeNextScene() override;
};

