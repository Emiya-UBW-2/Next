#include "TitleScene.hpp"

void TitleScene::InitSub() {
	m_FontBig.SetHandle(CreateFontToHandle("Agency FB", 24, -1, DX_FONTTYPE_ANTIALIASING_EDGE, DX_CHARSET_DEFAULT, 1));
	m_Title.SetHandle(LoadGraph("data/UI/Title.png"));
	m_TitleImage.SetHandle(LoadGraph("data/UI/titleImage.bmp"));

	BaseScene::SetNextSceneID(static_cast<SceneID>(EnumSceneID::Main));
}

void TitleScene::UpdateSub() {
	if (InputControl::Instance()->GetMenuEnter().IsTrigger()) {
		BaseScene::SetSceneEnd();
	}
	m_Timer += FrameWork::Instance()->GetDeltaTime();

	IsHitTriMono = false;
	if (InputControl::Instance()->GetMouseY() + 16 >= (FrameWork::Instance()->GetScreenHeight() - 32)) {
		IsHitTriMono = true;
		if (InputControl::Instance()->GetLMEnter().IsTrigger()) {
			IsTriMonoMode ^= 1;
		}
	}
}

void TitleScene::DrawSub() {
	int YposBase = FrameWork::Instance()->GetScreenHeight() * 2 / 3;
	DrawBox(0, YposBase, FrameWork::Instance()->GetScreenWidth(), FrameWork::Instance()->GetScreenHeight(), GetColor(255, 0, 0), TRUE);

	for (int loop = 0; loop < 5; loop++) {
		float per = std::sin(Mathf::Deg2Rad(90 * (m_Timer + (float)loop / 5 - (int)(m_Timer + (float)loop / 5))));
		int Ypos = YposBase + static_cast<int>(per * (float)(FrameWork::Instance()->GetScreenHeight() * 3 / 4 + 50));
		DrawBox(0, Ypos - static_cast<int>(50.f * per), FrameWork::Instance()->GetScreenWidth(), Ypos, (IsTriMonoMode) ? GetColor(0, 0, 0) : GetColor(192, 0, 0), TRUE);
	}


	DrawBox(0, 0, FrameWork::Instance()->GetScreenWidth(), YposBase, (IsTriMonoMode) ? GetColor(255, 255, 255) : GetColor(192, 192, 192), TRUE);

	DrawGraph(0, static_cast<int>(50 + 50.f * std::sin(Mathf::Deg2Rad(m_Timer * 0.3f * 180.f))), m_TitleImage.GetHandle(), TRUE);

	SetDrawBlendMode(DX_BLENDMODE_MUL, 255);
	DrawGraph(FrameWork::Instance()->GetScreenWidth() / 2 - 768 / 2, 64, m_Title.GetHandle(), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	if ((static_cast<int>(m_Timer * 10) % 10 < 5)) {
		const char* Str = "Press Space To Start";
		int Width = GetDrawStringWidthToHandle(Str, static_cast<int>(strlenDx(Str)), m_FontBig.GetHandle());
		DrawFormatString2ToHandle(FrameWork::Instance()->GetScreenWidth() / 2 - Width / 2, FrameWork::Instance()->GetScreenHeight() * 3 / 4, GetColor(255, 255, 255), GetColor(0, 0, 0), m_FontBig.GetHandle(), Str);
	}

	DrawFormatString2ToHandle(64, FrameWork::Instance()->GetScreenHeight() - 32, IsHitTriMono ? GetColor(255, 0, 0) : GetColor(255, 255, 255), GetColor(0, 0, 0), m_FontBig.GetHandle(), "3色モード : %s", (IsTriMonoMode) ? "厳密" : "カジュアル");

}

void TitleScene::DisposeSub() {
	m_FontBig.ReleaseFont();
	m_Title.ReleaseGraph();
	m_TitleImage.ReleaseGraph();
}