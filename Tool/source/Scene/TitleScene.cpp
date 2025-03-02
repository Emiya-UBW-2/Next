#include "TitleScene.hpp"

void TitleScene::InitSub() {
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
	FrameWork::Instance()->SetPauseEnable(false);
	m_Timer += FrameWork::Instance()->GetDeltaTime();
	if (!IsGoingNextScene) {
		if (!FadeControl::Instance()->IsFading()) {
			if (InputControl::Instance()->GetMenuEnter().IsTrigger() || (OnMouse && InputControl::Instance()->GetLMEnter().IsTrigger())) {
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
	DrawBox(0, 0, FrameWork::Instance()->GetScreenWidth(), FrameWork::Instance()->GetScreenHeight(), ColorPalette::Red, TRUE);
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
}

std::unique_ptr<BaseScene> TitleScene::MakeNextScene()
{
	return std::make_unique<TitleScene>();
}
