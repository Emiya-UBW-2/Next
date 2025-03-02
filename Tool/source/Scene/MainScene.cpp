#include "MainScene.hpp"



TaskTab::TaskTab(const char* TabName) {
	w = FrameWork::Instance()->GetScreenWidth() - 16 * 2;
	h = 32;
	hAdd = 0;
	OnMouse = false;
	IsActiveTab = false;
	m_TabName = TabName;
}

void TaskTab::Update(int xofs, int yofs) {
	OnMouse = IntoMouse(xofs + x, yofs + y, xofs + x + w, yofs + y + h);
	if (OnMouse && InputControl::Instance()->GetLMEnter().IsTrigger()) {
		IsActiveTab ^= 1;
	}
	if (IsActiveTab) {
		hAdd = 32*5;
	}
	else {
		hAdd = 0;
	}
}
void TaskTab::Draw() const {
	DrawBox(x, y, x + w, y + h + hAdd, ColorPalette::Black, TRUE);
	if (hAdd > 0) {
		DrawBox(x + 1, y + h, x + w - 1, y + h + hAdd - 1, ColorPalette::Gray010, TRUE);
	}
	DrawBox(x + 1, y + 1, x + w - 1, y + h - 1, OnMouse ? ColorPalette::Gray050 : ColorPalette::White, TRUE);
	int Width = GetDrawStringWidthToHandle(m_TabName.c_str(), static_cast<int>(m_TabName.length()), FontPool::Instance()->Get("Agency FB", 24, -1, DX_FONTTYPE_ANTIALIASING_EDGE, DX_CHARSET_DEFAULT, 1)->GetHandle());
	DrawFormatString2ToHandle(x + w / 2 - Width / 2, y + h / 2 - 24 / 2, ColorPalette::White, ColorPalette::Gray085, FontPool::Instance()->Get("Agency FB", 24, -1, DX_FONTTYPE_ANTIALIASING_EDGE, DX_CHARSET_DEFAULT, 1)->GetHandle(), m_TabName.c_str());
}



void MainScene::DrawTab() const {
	int Height = (FrameWork::Instance()->GetScreenHeight() - m_YListPos - 8);
	DrawBox(
		FrameWork::Instance()->GetScreenWidth() - 8 - 2, 0,
		FrameWork::Instance()->GetScreenWidth() - 8 + 2, 0 + Height,
		ColorPalette::Black, TRUE);

	int H = static_cast<int>(Mathf::Clamp(
		static_cast<float>(Height - m_YMax),
		static_cast<float>(40),
		static_cast<float>(Height)
	));
	DrawBox(
		FrameWork::Instance()->GetScreenWidth() - 8 - 2, 0 - (Height - H) * m_Yoffset / static_cast<int>(Mathf::Max(static_cast<float>(m_YMax), 1.f)),
		FrameWork::Instance()->GetScreenWidth() - 8 + 2, 0 - (Height - H) * m_Yoffset / static_cast<int>(Mathf::Max(static_cast<float>(m_YMax), 1.f)) + H,
		ColorPalette::White, TRUE);
}

void MainScene::InitSub() {
	FadeControl::Instance()->SetFadeOut(ColorPalette::Black, 0.1f);
	m_TaskTabs.clear();
	m_TaskTabs.emplace_back(std::make_unique<TaskTab>("Tab1"));
	m_TaskTabs.emplace_back(std::make_unique<TaskTab>("Tab1"));
	m_TaskTabs.emplace_back(std::make_unique<TaskTab>("Tab1"));
	m_TaskTabs.emplace_back(std::make_unique<TaskTab>("Tab1"));
	m_TaskTabs.emplace_back(std::make_unique<TaskTab>("Tab1"));
	m_TaskTabs.emplace_back(std::make_unique<TaskTab>("Tab1"));
	m_TaskTabs.emplace_back(std::make_unique<TaskTab>("Tab1"));

	m_Yoffset = 0;
	m_YListPos = 32;
	int Height = (FrameWork::Instance()->GetScreenHeight() - m_YListPos - 8);
	m_ListGraph.Create(FrameWork::Instance()->GetScreenWidth(), Height, true);
}

void MainScene::UpdateSub() {
	FrameWork::Instance()->SetPauseEnable(false);

	int yposEdge = 8;
	m_YMax = 0;
	for (auto& t : m_TaskTabs) {
		t->SetPos(0, m_Yoffset + m_YMax);
		if (&t - &m_TaskTabs.front() != (m_TaskTabs.size() - 1)) {
			m_YMax += t->GetHeight() + yposEdge;
		}
		t->Update(0, m_YListPos);
	}

	m_Yoffset = static_cast<int>(Mathf::Clamp(
		static_cast<float>(m_Yoffset + InputControl::Instance()->GetMouseWheel() * (32 + 8)),
		static_cast<float>(-m_YMax),
		static_cast<float>(0)
	));
	SetDrawScreen(m_ListGraph.GetHandle());
	ClearDrawScreen();
	{
		DrawTab();
		for (auto& t : m_TaskTabs) {
			t->Draw();
		}
	}
}

void MainScene::DrawSub() const {
	//”wŒi•`‰æ
	DrawBox(0, 0, FrameWork::Instance()->GetScreenWidth(), FrameWork::Instance()->GetScreenHeight(), ColorPalette::Gray025, TRUE);
	//
	DrawGraph(0, m_YListPos, m_ListGraph.GetHandle(), TRUE);
}

void MainScene::DisposeSub() {
}

std::unique_ptr<BaseScene> MainScene::MakeNextScene()
{
	return std::make_unique<MainScene>();
}
