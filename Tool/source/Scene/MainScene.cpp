#include "MainScene.hpp"
//
void MainScene::InitSub() {
	FadeControl::Instance()->SetFadeOut(ColorPalette::Black, 0.1f);
	m_TaskDraw = std::make_unique<TaskDraw>();
}
void MainScene::UpdateSub() {
	FrameWork::Instance()->SetPauseEnable(false);

	m_TaskDraw->Update();
}
void MainScene::DrawSub() const {
	//”wŒi•`‰æ
	DrawBox(0, 0, FrameWork::Instance()->GetScreenWidth(), FrameWork::Instance()->GetScreenHeight(), ColorPalette::Gray025, TRUE);
	//
	m_TaskDraw->Draw();
}
void MainScene::DisposeSub() {
	m_TaskDraw.release();
}
std::unique_ptr<BaseScene> MainScene::MakeNextScene() { return std::make_unique<MainScene>(); }
