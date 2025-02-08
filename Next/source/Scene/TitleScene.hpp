#pragma once

#include "../FrameWork/Define.hpp"

#include "BaseScene.hpp"

class TitleScene : public BaseScene {
	using BaseScene::BaseScene;
private:
	int m_Title = 0;
	int m_TitleImage = 0;
	int m_FontBig = 0;
	float m_Timer = 0.f;
	bool IsHitTriMono = false;
protected:
	void InitSub() override;
	void UpdateSub() override {
		if (InputControl::Instance()->GetMenuEnter().IsTrigger()) {
			BaseScene::SetSceneEnd();
		}
		m_Timer += FrameWork::Instance()->GetDeltaTime();

		//マウス座標を取得しておく
		int MouseX;
		int MouseY;
		//マウス座標を取得しておく
		GetMousePoint(&MouseX, &MouseY);

		MouseX = MouseX * FrameWork::Instance()->GetScreenWidth() / FrameWork::Instance()->GetWindowWidth() + 16;
		MouseY = MouseY * FrameWork::Instance()->GetScreenHeight() / FrameWork::Instance()->GetWindowHeight() + 16;
		IsHitTriMono = false;
		if (MouseY >= (FrameWork::Instance()->GetScreenHeight() - 32)) {
			IsHitTriMono = true;
			if (InputControl::Instance()->GetLMEnter().IsTrigger()) {
				IsTriMonoMode ^= 1;
			}
		}
	}
	void DrawSub() override {
		int YposBase = FrameWork::Instance()->GetScreenHeight() *2 / 3;
		DrawBox(0, YposBase, FrameWork::Instance()->GetScreenWidth(), FrameWork::Instance()->GetScreenHeight(), GetColor(255, 0, 0), TRUE);

		for (int loop = 0; loop < 5; loop++) {
			float per = std::sin(Mathf::Deg2Rad(90 * (m_Timer + (float)loop / 5 - (int)(m_Timer + (float)loop / 5))));
			int Ypos = YposBase + static_cast<int>(per * (float)(FrameWork::Instance()->GetScreenHeight() * 3/ 4 + 50));
			DrawBox(0, Ypos - static_cast<int>(50.f * per), FrameWork::Instance()->GetScreenWidth(), Ypos, (IsTriMonoMode) ? GetColor(0, 0, 0) : GetColor(192, 0, 0), TRUE);
		}


		DrawBox(0, 0, FrameWork::Instance()->GetScreenWidth(), YposBase, (IsTriMonoMode) ? GetColor(255, 255, 255) : GetColor(192, 192, 192), TRUE);

		DrawGraph(0, static_cast<int>(50 + 50.f * std::sin(Mathf::Deg2Rad(m_Timer * 0.3f * 180.f))), m_TitleImage, TRUE);

		SetDrawBlendMode(DX_BLENDMODE_MUL, 255);
		DrawGraph(FrameWork::Instance()->GetScreenWidth() / 2 - 768 / 2, 64, m_Title, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

		if ((static_cast<int>(m_Timer * 10) % 10 < 5)) {
			const char* Str = "Press Space To Start";
			int Width = GetDrawStringWidthToHandle(Str, static_cast<int>(strlenDx(Str)), m_FontBig);
			DrawFormatString2ToHandle(FrameWork::Instance()->GetScreenWidth() / 2 - Width / 2, FrameWork::Instance()->GetScreenHeight() * 3 / 4, GetColor(255, 255, 255), GetColor(0, 0, 0), m_FontBig, Str);
		}

		DrawFormatString2ToHandle(64, FrameWork::Instance()->GetScreenHeight() - 32, IsHitTriMono ? GetColor(255, 0, 0) : GetColor(255, 255, 255), GetColor(0, 0, 0), m_FontBig, "3色モード : %s", (IsTriMonoMode) ? "厳密" : "カジュアル");
		
	}
	void DisposeSub() override {
		DeleteFontToHandle(m_FontBig);
		DeleteGraph(m_Title);
	}
};

