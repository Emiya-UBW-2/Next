#include "TitleScene.hpp"

#include "SceneController.hpp"

void TitleScene::InitSub() {
	m_FontBig = CreateFontToHandle("Agency FB", 24, -1, DX_FONTTYPE_ANTIALIASING_EDGE, DX_CHARSET_DEFAULT, 1);
	m_Title = LoadGraph("data/UI/Title.png");
	m_TitleImage = LoadGraph("data/UI/titleImage.bmp");

	BaseScene::SetNextSceneID(static_cast<SceneID>(EnumSceneID::Main));
}