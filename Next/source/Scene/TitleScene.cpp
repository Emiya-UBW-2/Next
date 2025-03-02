#include "TitleScene.hpp"
#include "MainScene.hpp"

void TitleScene::InitSub() {
	SoundPool::Instance()->Add(1, SoundType::SE, "data/Audio/sel.wav");
	SoundPool::Instance()->Add(1, SoundType::SE, "data/Audio/decide.wav");

	m_Title.LoadGraph("data/UI/Title.png");
	m_TitleImage.LoadGraph("data/UI/titleImage.bmp");

	FadeControl::Instance()->SetFadeOut(ColorPalette::Black, 1.f);
	IsGoingNextScene = false;
	OnMouse = false;
}

void TitleScene::UpdateSub() {
	const char* Str = "Press Space To Start";
	int Width = GetDrawStringWidthToHandle(Str, static_cast<int>(strlenDx(Str)), FontPool::Instance()->Get("Agency FB", 24, -1, DX_FONTTYPE_ANTIALIASING_EDGE, DX_CHARSET_DEFAULT, 1)->GetHandle());

	bool prev = OnMouse;
	OnMouse = IntoMouse(FrameWork::Instance()->GetScreenWidth() / 2 - Width / 2, FrameWork::Instance()->GetScreenHeight() * 3 / 4,
		FrameWork::Instance()->GetScreenWidth() / 2 + Width / 2, FrameWork::Instance()->GetScreenHeight() * 3 / 4 + 24);
	if (OnMouse && prev != OnMouse) {
		SoundPool::Instance()->Play(DX_PLAYTYPE_BACK, TRUE, SoundType::SE, "data/Audio/sel.wav");
	}
	FrameWork::Instance()->SetPauseEnable(false);
	m_Timer += FrameWork::Instance()->GetDeltaTime();
	if (!IsGoingNextScene) {
		if (!FadeControl::Instance()->IsFading()) {
			if (InputControl::Instance()->GetMenuEnter().IsTrigger() || (OnMouse && InputControl::Instance()->GetLMEnter().IsTrigger())) {
				SoundPool::Instance()->Play(DX_PLAYTYPE_BACK, TRUE, SoundType::SE, "data/Audio/decide.wav");
				FadeControl::Instance()->SetFadeIn(ColorPalette::Black, 1.f);
				IsGoingNextScene = true;
			}
		}
	}
	else {
		if (!FadeControl::Instance()->IsFading()) {
			BaseScene::SetSceneEnd();
		}
	}
}

void TitleScene::DrawSub() const {
	//”wŒi•`‰æ
	int YposBase = FrameWork::Instance()->GetScreenHeight() * 2 / 3;
	DrawBox(0, YposBase, FrameWork::Instance()->GetScreenWidth(), FrameWork::Instance()->GetScreenHeight(), ColorPalette::Red, TRUE);
	for (int loop = 0; loop < 5; loop++) {
		float per = std::sin(Mathf::Deg2Rad(90 * (m_Timer + (float)loop / 5 - (int)(m_Timer + (float)loop / 5))));
		int Ypos = YposBase + static_cast<int>(per * (float)(FrameWork::Instance()->GetScreenHeight() * 3 / 4 + 50));
		DrawBox(0, Ypos - static_cast<int>(50.f * per), FrameWork::Instance()->GetScreenWidth(), Ypos, ColorPalette::Red075, TRUE);
	}
	DrawBox(0, 0, FrameWork::Instance()->GetScreenWidth(), YposBase, ColorPalette::Gray025, TRUE);
	
	double Rate = static_cast<double>(static_cast<float>(FrameWork::Instance()->GetScreenWidth()) / 960.f);

	//”òs‹@‰æ‘œ•`‰æ

	DrawRotaGraph(
		FrameWork::Instance()->GetScreenWidth()/2, FrameWork::Instance()->GetScreenHeight()/2 + static_cast<int>(50 + 50.f * std::sin(Mathf::Deg2Rad(m_Timer * 0.3f * 180.f))),
		Rate,0.0,m_TitleImage.GetHandle(), TRUE);
	//ƒ^ƒCƒgƒ‹•¶Žš
	SetDrawBlendMode(DX_BLENDMODE_MUL, 255);
	DrawRotaGraph(FrameWork::Instance()->GetScreenWidth() / 2, static_cast<int>((64 + 48) * Rate), Rate, 0.0, m_Title.GetHandle(), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	//Press Space To Start

	bool isDraw = !FadeControl::Instance()->IsFading();
	if (!OnMouse) {
		isDraw &= ((static_cast<int>(m_Timer * 10) % 10 < 5));
	}
	if (isDraw) {
		const char* Str = "Press Space To Start";
		int Width = GetDrawStringWidthToHandle(Str, static_cast<int>(strlenDx(Str)), FontPool::Instance()->Get("Agency FB", 24, -1, DX_FONTTYPE_ANTIALIASING_EDGE, DX_CHARSET_DEFAULT, 1)->GetHandle());
		DrawFormatString2ToHandle(FrameWork::Instance()->GetScreenWidth() / 2 - Width / 2, FrameWork::Instance()->GetScreenHeight() * 3 / 4, OnMouse ? ColorPalette::Red : ColorPalette::White, ColorPalette::Black, FontPool::Instance()->Get("Agency FB", 24, -1, DX_FONTTYPE_ANTIALIASING_EDGE, DX_CHARSET_DEFAULT, 1)->GetHandle(), Str);
	}
}

void TitleScene::DisposeSub() {
	m_Title.ReleaseGraph();
	m_TitleImage.ReleaseGraph();
}

std::unique_ptr<BaseScene> TitleScene::MakeNextScene()
{
	return std::make_unique<MainGame>();
}
