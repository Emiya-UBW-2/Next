#include "TitleScene.hpp"

void TitleScene::InitSub() {
	FontPool::Create();
	FontPool::Instance()->Add("Agency FB", 12, -1, DX_FONTTYPE_ANTIALIASING_EDGE, DX_CHARSET_DEFAULT, 1);
	FontPool::Instance()->Add("Agency FB", 24, -1, DX_FONTTYPE_ANTIALIASING_EDGE, DX_CHARSET_DEFAULT, 1);
	FontPool::Instance()->Add("BIZ UDÉSÉVÉbÉN", 24, -1, DX_FONTTYPE_ANTIALIASING_EDGE, DX_CHARSET_DEFAULT, 1);
	
	m_Title.LoadGraph("data/UI/Title.png");
	m_TitleImage.LoadGraph("data/UI/titleImage.bmp");

	BaseScene::SetNextSceneID(static_cast<SceneID>(EnumSceneID::Main));
}

void TitleScene::UpdateSub() {
	if (InputControl::Instance()->GetMenuEnter().IsTrigger()) {
		BaseScene::SetSceneEnd();
	}
	m_Timer += FrameWork::Instance()->GetDeltaTime();
}

void TitleScene::DrawSub() {
	int YposBase = FrameWork::Instance()->GetScreenHeight() * 2 / 3;
	DrawBox(0, YposBase, FrameWork::Instance()->GetScreenWidth(), FrameWork::Instance()->GetScreenHeight(), ColorPalette::Red, TRUE);

	for (int loop = 0; loop < 5; loop++) {
		float per = std::sin(Mathf::Deg2Rad(90 * (m_Timer + (float)loop / 5 - (int)(m_Timer + (float)loop / 5))));
		int Ypos = YposBase + static_cast<int>(per * (float)(FrameWork::Instance()->GetScreenHeight() * 3 / 4 + 50));
		DrawBox(0, Ypos - static_cast<int>(50.f * per), FrameWork::Instance()->GetScreenWidth(), Ypos, ColorPalette::Red075, TRUE);
	}


	DrawBox(0, 0, FrameWork::Instance()->GetScreenWidth(), YposBase, ColorPalette::Gray025, TRUE);

	DrawGraph(0, static_cast<int>(50 + 50.f * std::sin(Mathf::Deg2Rad(m_Timer * 0.3f * 180.f))), m_TitleImage.GetHandle(), TRUE);

	SetDrawBlendMode(DX_BLENDMODE_MUL, 255);
	DrawGraph(FrameWork::Instance()->GetScreenWidth() / 2 - 768 / 2, 64, m_Title.GetHandle(), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	if ((static_cast<int>(m_Timer * 10) % 10 < 5)) {
		const char* Str = "Press Space To Start";
		int Width = GetDrawStringWidthToHandle(Str, static_cast<int>(strlenDx(Str)), FontPool::Instance()->Get("Agency FB", 24, -1, DX_FONTTYPE_ANTIALIASING_EDGE, DX_CHARSET_DEFAULT, 1)->GetHandle());
		DrawFormatString2ToHandle(FrameWork::Instance()->GetScreenWidth() / 2 - Width / 2, FrameWork::Instance()->GetScreenHeight() * 3 / 4, ColorPalette::White, ColorPalette::Black, FontPool::Instance()->Get("Agency FB", 24, -1, DX_FONTTYPE_ANTIALIASING_EDGE, DX_CHARSET_DEFAULT, 1)->GetHandle(), Str);
	}
}

void TitleScene::DisposeSub() {
	m_Title.ReleaseGraph();
	m_TitleImage.ReleaseGraph();
}