#include "TaskTab.hpp"

#include "../FrameWork/Define.hpp"
#include <Windows.h>

//
cv::Mat Hwnd2Mat(HWND hwnd, int nPosLeft, int nPosTop, int nDestWidth, int nDestHeight)
{
	HDC hwindowDC = GetDC(hwnd);
	HDC hwindowCompatibleDC = CreateCompatibleDC(hwindowDC);

	HBITMAP           hbwindow = NULL;
	cv::Mat           src{};
	BITMAPINFOHEADER  bi{};

	SetStretchBltMode(hwindowCompatibleDC, COLORONCOLOR);

	RECT windowsize;
	GetClientRect(hwnd, &windowsize);

	int srcheight = windowsize.bottom;
	int srcwidth = windowsize.right;

	src.create(nDestHeight, nDestWidth, CV_8UC3);

	hbwindow = CreateCompatibleBitmap(hwindowDC, nDestWidth, nDestHeight);
	if (hbwindow == NULL)
	{
		src.data = NULL;
		return src;
	}

	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = nDestWidth;
	bi.biHeight = -nDestHeight;
	bi.biPlanes = 1;
	bi.biBitCount = 24;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;

	SelectObject(hwindowCompatibleDC, hbwindow);

	StretchBlt(hwindowCompatibleDC, 0, 0, nDestWidth, nDestHeight, hwindowDC, nPosLeft, nPosTop, nDestWidth, nDestHeight, SRCCOPY);
	GetDIBits(hwindowCompatibleDC, hbwindow, 0, nDestHeight, src.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);

	if (hbwindow != NULL)
	{
		DeleteObject(hbwindow);
		hbwindow = NULL;
	}

	if (hwindowCompatibleDC != NULL)
	{
		DeleteDC(hwindowCompatibleDC);
		hwindowCompatibleDC = NULL;
	}

	if (hwindowDC != NULL)
	{
		ReleaseDC(hwnd, hwindowDC);
		hwindowDC = NULL;
	}

	return src;
}
void MoveToMousePoint(int x, int y) {
	// DPIを反映するデスクトップサイズ
	int DispXSize = static_cast<int>(GetSystemMetrics(SM_CXSCREEN));
	int DispYSize = static_cast<int>(GetSystemMetrics(SM_CYSCREEN));
	LONG absX = (x * 65536 + DispXSize - 1) / DispXSize;
	LONG absY = (y * 65536 + DispYSize - 1) / DispYSize;

	INPUT inputs[1] = {};
	ZeroMemory(inputs, sizeof(inputs));

	inputs[0].type = INPUT_MOUSE;
	inputs[0].mi.dx = absX;
	inputs[0].mi.dy = absY;
	inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;

	SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
}
void MouseClick() {
	INPUT inputs[1] = {};
	ZeroMemory(inputs, sizeof(inputs));

	inputs[0].type = INPUT_MOUSE;
	inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP;

	SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
}
//
TaskTab::TaskTab(const char* TabName) {
	w = FrameWork::Instance()->GetScreenWidth() - 16 * 2;
	h = 32;
	wSub = 64;
	hAdd = 0;
	OnMouse = false;
	IsPlayTask = false;
	m_IsEnd = false;
	IsActiveTab = false;
	m_TabName = TabName;

	SetTime(10, 0, 0);
	m_TabColumn.clear();

	m_CheckFilePath = "data\\Test.png";

	m_Dialog.Init();
}
void TaskTab::Update(int xofs, int yofs) {
	const auto& Font = FontPool::Instance()->Get("Agency FB", 24, -1, DX_FONTTYPE_ANTIALIASING_EDGE, DX_CHARSET_DEFAULT, 1)->GetHandle();
	OnMouse = IntoMouse(xofs + x, yofs + y, xofs + x + w - wSub, yofs + y + h);
	if (OnMouse && InputControl::Instance()->GetLMEnter().IsTrigger()) {
		IsActiveTab ^= 1;
	}
	if ((IsActiveTab && !IsPlayTask) || (m_TabColumn.size() == 0)) {
		m_TabColumn.resize(5);
		int column = 0;
		{
			m_TabColumn.at(0).m_TabString = "Type:" + std::string(TaskTypeStr[static_cast<int>(m_TaskType)]);
			m_TabColumn.at(0).m_OnMouse = IntoMouse(xofs + x, yofs + y + h + column, xofs + x + w - wSub, yofs + y + h + column + 24);
			if (m_TabColumn.at(0).m_OnMouse && InputControl::Instance()->GetLMEnter().IsTrigger()) {
				m_TaskType = static_cast<TaskType>((static_cast<int>(m_TaskType) + 1) % static_cast<int>(TaskType::Max));
			}
		} column += 24;
		{
			char TimeStr[256] = "";
			sprintfDx(TimeStr, "%02d:%02d:%02d", (m_Type / 60 / 60) % 24, (m_Type / 60) % 60, m_Type % 60);
			m_TabColumn.at(1).m_TabString = "Time:" + std::string(TimeStr);
			m_TabColumn.at(1).m_OnMouse = IntoMouse(xofs + x, yofs + y + h + column, xofs + x + w - wSub, yofs + y + h + column + 24);
			if (m_TabColumn.at(1).m_OnMouse && InputControl::Instance()->GetLMEnter().IsTrigger()) {
				int wtmp = GetDrawStringWidthToHandle("Time:", sizeof("Time:"), Font);
				{
					int Width = GetDrawStringWidthToHandle("Time:00", sizeof("Time:00"), Font);
					if (IntoMouse(xofs + x + wtmp, yofs + y + h + column, xofs + x + Width, yofs + y + h + column + 24)) {
						SetTime(GetHour() + 1, GetMinute(), GetSecond());
					}
					wtmp = Width;
				}
				{
					int Width = GetDrawStringWidthToHandle("Time:00:00", sizeof("Time:00:00"), Font);
					if (IntoMouse(xofs + x + wtmp, yofs + y + h + column, xofs + x + Width, yofs + y + h + column + 24)) {
						SetTime(GetHour(), GetMinute() + 5, GetSecond());
					}
					wtmp = Width;
				}
				{
					int Width = GetDrawStringWidthToHandle("Time:00:00:00", sizeof("Time:00:00:00"), Font);
					if (IntoMouse(xofs + x + wtmp, yofs + y + h + column, xofs + x + Width, yofs + y + h + column + 24)) {
						SetTime(GetHour(), GetMinute(), GetSecond() + 5);
					}
					wtmp = Width;
				}
			}
		} column += 24;
		{
			m_TabColumn.at(2).m_TabString = "Icon:[" + m_CheckFilePath + "]";
			m_TabColumn.at(2).m_OnMouse = IntoMouse(xofs + x, yofs + y + h + column, xofs + x + w - wSub, yofs + y + h + column + 24);
			if (m_TabColumn.at(2).m_OnMouse && InputControl::Instance()->GetLMEnter().IsTrigger()) {
				if (m_Dialog.Open()) {
					m_CheckFilePath = m_Dialog.GetPath();
				}
			}
		} column += 24;
		{
			m_TabColumn.at(3).m_TabString = "Move:" + std::string(TaskMoveStr[static_cast<int>(m_TaskMove)]);
			m_TabColumn.at(3).m_OnMouse = IntoMouse(xofs + x, yofs + y + h + column, xofs + x + w - wSub, yofs + y + h + column + 24);
			if (m_TabColumn.at(3).m_OnMouse && InputControl::Instance()->GetLMEnter().IsTrigger()) {
				m_TaskMove = static_cast<TaskMove>((static_cast<int>(m_TaskMove) + 1) % static_cast<int>(TaskMove::Max));
			}
		} column += 24;
		{
			m_TabColumn.at(4).m_TabString = "Next:" + std::string(TaskNextStr[static_cast<int>(m_TaskNext)]);
			m_TabColumn.at(4).m_OnMouse = IntoMouse(xofs + x, yofs + y + h + column, xofs + x + w - wSub, yofs + y + h + column + 24);
			if (m_TabColumn.at(4).m_OnMouse && InputControl::Instance()->GetLMEnter().IsTrigger()) {
				m_TaskNext = static_cast<TaskNext>((static_cast<int>(m_TaskNext) + 1) % static_cast<int>(TaskNext::Max));
			}
		} column += 24;
	}
	hAdd = IsActiveTab ? (24 * static_cast<int>(m_TabColumn.size()) + 2) : 0;

	if (IsPlayTask) {
		switch (m_TaskType) {
		case TaskType::ClickPoint:
		{
			if (!m_MatchTask) {
				m_MatchTask = std::make_unique<MatchTask>(m_CheckFilePath.c_str());
			}
			else {
				//ウィンドウ画面キャプチャ
				m_MatchTask->Update();
				if (m_MatchTask->IsMatch()) {
					MoveToMousePoint(m_MatchTask->MatchPosX(), m_MatchTask->MatchPosY());
					IsPlayTask = false;
					m_IsEnd = true;
					{
						INPUT inputs[1] = {};
						ZeroMemory(inputs, sizeof(inputs));

						inputs[0].type = INPUT_MOUSE;

						switch (m_TaskMove) {
						case TaskMove::LeftMouseTrigger:
							inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP;
							break;
						case TaskMove::LeftMouseDragStart:
							inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
							break;
						case TaskMove::LeftMouseDragEnd:
							inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTUP;
							break;
						default:
							break;
						}
						SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
					}
				}
			}
		}
			break;
		case TaskType::KeyType:
			break;
		default:
			break;
		}
	}
	else {
		m_MatchTask.release();
	}
}
void TaskTab::Draw() const {
	const auto& Font = FontPool::Instance()->Get("Agency FB", 24, -1, DX_FONTTYPE_ANTIALIASING_EDGE, DX_CHARSET_DEFAULT, 1)->GetHandle();
	DrawBox(x, y, x + w, y + h, ColorPalette::Black, TRUE);
	DrawBox(x, y, x + w - wSub, y + h + hAdd, ColorPalette::Black, TRUE);
	if (hAdd > 0) {
		DrawBox(x + 1, y + h, x + w - wSub - 1, y + h + hAdd - 1, ColorPalette::Gray010, TRUE);
	}
	{
		DrawBox(x + 1, y + 1, x + w - wSub - 1, y + h - 1, OnMouse ? ColorPalette::Gray050 : ColorPalette::White, TRUE);
		int Width = GetDrawStringWidthToHandle(m_TabName.c_str(), static_cast<int>(m_TabName.length()), Font);
		DrawFormatString2ToHandle(x + (w - wSub) / 2 - Width / 2, y + h / 2 - 24 / 2, ColorPalette::White, ColorPalette::Gray085, Font, m_TabName.c_str());
	}
	//走るボタン
	{
		std::string Xname = IsPlayTask ? "Run" : "Stop";
		DrawBox(x + w - wSub, y + 1, x + w - 1, y + h - 1, IsPlayTask ? ColorPalette::Red : ColorPalette::Green, TRUE);
		int Width = GetDrawStringWidthToHandle(Xname.c_str(), static_cast<int>(Xname.length()), Font);
		DrawFormatString2ToHandle(x + w - wSub + wSub / 2 - Width / 2, y + h / 2 - 24 / 2, ColorPalette::White, ColorPalette::Gray085, Font, Xname.c_str());
	}
	//
	if (IsActiveTab) {
		int column = 0;
		for (auto& s : m_TabColumn) {
			DrawFormatString2ToHandle(x, y + h + column, (!IsPlayTask && s.m_OnMouse) ? ColorPalette::Red : ColorPalette::White, ColorPalette::Gray085, Font, s.m_TabString.c_str()); column += 24;
		}
	}
}
//
MatchTask::MatchTask(const char* Path) {
	checkimage = cv::imread(Path, cv::IMREAD_UNCHANGED);
	matchWidth = checkimage.cols;
	matchHeight = checkimage.rows;
	m_IsMatchThreadEnd = true;
	matchPointX = -1;
	matchPointY = -1;
	isMatch = false;
}
MatchTask::~MatchTask() {
	checkimage.release();
	if (m_MatchThread.joinable()) {
		m_MatchThread.join();
	}
}
void MatchTask::Update() {
	//ウィンドウ画面キャプチャ
	if (m_IsMatchThreadEnd) {
		m_IsMatchThreadEnd = false;
		if (m_MatchThread.joinable()) {
			m_MatchThread.join();
		}
		std::thread Thread([&]() {
			int DispXSize = static_cast<int>(GetSystemMetrics(SM_CXSCREEN));
			int DispYSize = static_cast<int>(GetSystemMetrics(SM_CYSCREEN));
			monitor_img = Hwnd2Mat(GetDesktopWindow(), 0, 0, DispXSize, DispYSize);
			//マッチ検出
			cv::Point max_pt{};
			double maxVal{};
			cv::matchTemplate(monitor_img, checkimage, resultimage, cv::TM_CCOEFF_NORMED);
			cv::minMaxLoc(resultimage, NULL, &maxVal, NULL, &max_pt);
			isMatch = maxVal > 0.9f;
			matchPointX = isMatch ? max_pt.x : -1;
			matchPointY = isMatch ? max_pt.y : -1;
			m_IsMatchThreadEnd = true;
			});
		m_MatchThread.swap(Thread);
	}
}

void TaskDraw::DrawTab() const {
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
TaskDraw::TaskDraw() {
	m_TaskTabs.clear();
	m_TaskTabs.emplace_back(std::make_unique<TaskTab>(("Tab" + std::to_string(m_TaskTabs.size())).c_str()));
	m_TaskTabs.emplace_back(std::make_unique<TaskTab>(("Tab" + std::to_string(m_TaskTabs.size())).c_str()));
	m_TaskTabs.emplace_back(std::make_unique<TaskTab>(("Tab" + std::to_string(m_TaskTabs.size())).c_str()));

	m_Yoffset = 0;
	m_YListPos = 32+8;
	int Height = (FrameWork::Instance()->GetScreenHeight() - m_YListPos - 8);
	m_ListGraph.Create(FrameWork::Instance()->GetScreenWidth(), Height, true);

	OnMouse = false;
	IsTaskStart = false;
}
void TaskDraw::Update() {
	int yposEdge = 8;
	m_YMax = 0;
	for (auto& t : m_TaskTabs) {
		int index = static_cast<int>(&t - &m_TaskTabs.front());
		t->SetPos(0, m_Yoffset + m_YMax);
		if (index != (m_TaskTabs.size() - 1)) {
			m_YMax += t->GetHeight() + yposEdge;
		}
		t->Update(0, m_YListPos);
		if (t->IsEnd()){
			t->EndTask();
			if (index != (m_TaskTabs.size() - 1)) {
				m_TaskTabs.at(static_cast<size_t>(index + 1))->Start();
			}
			else {
				IsTaskStart = false;
			}
		}
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

	OnMouse = IntoMouse(16, 4, FrameWork::Instance()->GetScreenWidth() - 16, m_YListPos - 4) && !IsTaskStart;
	if (OnMouse && InputControl::Instance()->GetLMEnter().IsTrigger()) {
		IsTaskStart = true;
		m_TaskTabs.at(0)->Start();
	}
}
void TaskDraw::Draw() const {
	DrawBox(16, 4, FrameWork::Instance()->GetScreenWidth() - 16, m_YListPos - 4, ColorPalette::Black, TRUE);
	DrawBox(16+1, 4+1, FrameWork::Instance()->GetScreenWidth() - 16-1, m_YListPos - 4-1, OnMouse ? ColorPalette::Gray050 : ColorPalette::White, TRUE);
	const auto& Font = FontPool::Instance()->Get("Agency FB", 24, -1, DX_FONTTYPE_ANTIALIASING_EDGE, DX_CHARSET_DEFAULT, 1)->GetHandle();
	std::string StartButton = IsTaskStart ? "Run" : "Start Task";
	int Width = GetDrawStringWidthToHandle(StartButton.c_str(), static_cast<int>(StartButton.length()), Font);
	DrawFormatString2ToHandle(FrameWork::Instance()->GetScreenWidth() / 2 - Width / 2, 4 + (m_YListPos - 4 - 4) / 2 - 24 / 2, ColorPalette::White, ColorPalette::Gray085, Font, StartButton.c_str());

	DrawGraph(0, m_YListPos, m_ListGraph.GetHandle(), TRUE);
}
