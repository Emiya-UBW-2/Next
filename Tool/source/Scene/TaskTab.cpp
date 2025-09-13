#include "TaskTab.hpp"

#include "../FrameWork/Define.hpp"
#include <Windows.h>

//
static cv::Mat Hwnd2Mat(HWND hwnd, int nPosLeft, int nPosTop, int nDestWidth, int nDestHeight)
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
static bool MoveToMousePoint(int x, int y) {
	return SetCursorPos(x, y);
	/*
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

	return SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT)) == ARRAYSIZE(inputs);
	//*/
}
static bool ClickKey(const std::string& CheckKey, TaskMove taskMove) {
	Sleep(1);
	ProcessMessage();
	int type = INPUT_MOUSE;
	WORD VK = VK_RETURN;
	int Down = 0;
	int Up = 0;
	if (CheckKey == "LMOUSE") {
		type = INPUT_MOUSE;
		Down = MOUSEEVENTF_LEFTDOWN;
		Up = MOUSEEVENTF_LEFTUP;
	}
	if (CheckKey == "RMOUSE") {
		type = INPUT_MOUSE;
		Down = MOUSEEVENTF_RIGHTDOWN;
		Up = MOUSEEVENTF_RIGHTUP;
	}
	if (CheckKey == "WMOUSE") {
		type = INPUT_MOUSE;
		Down = MOUSEEVENTF_MIDDLEDOWN;
		Up = MOUSEEVENTF_MIDDLEUP;
	}
	if (CheckKey == "RETURN") {
		type = INPUT_KEYBOARD;
		VK = VK_RETURN;
		Down = 0;
		Up = KEYEVENTF_KEYUP;
	}
	if (CheckKey == "ESCAPE") {
		type = INPUT_KEYBOARD;
		VK = VK_ESCAPE;
		Down = 0;
		Up = KEYEVENTF_KEYUP;
	}
	if (CheckKey == "A") {
		type = INPUT_KEYBOARD;
		VK = 'A';
		Down = 0;
		Up = KEYEVENTF_KEYUP;
	}

	switch (taskMove) {
	case TaskMove::Trigger:
		if (type == INPUT_MOUSE) {
			mouse_event(Down, 0, 0, 0, 0);
			mouse_event(Up, 0, 0, 0, 0);
		}
		if (type == INPUT_KEYBOARD) {
			if (!ClickKey(CheckKey, TaskMove::DragStart)) { return false; }
			if (!ClickKey(CheckKey, TaskMove::DragEnd)) { return false; }
			return true;
		}
		break;
	case TaskMove::DragStart:
		if (type == INPUT_MOUSE) {
			mouse_event(Down, 0, 0, 0, 0);
		}
		if (type == INPUT_KEYBOARD) {
			keybd_event(VK, 0, KEYEVENTF_EXTENDEDKEY | Down, 0);
		}
		break;
	case TaskMove::DragEnd:
		if (type == INPUT_MOUSE) {
			mouse_event(Up, 0, 0, 0, 0);
		}
		if (type == INPUT_KEYBOARD) {
			keybd_event(VK, 0, KEYEVENTF_EXTENDEDKEY | Up, 0);
		}
		break;
	default:
		break;
	}
	return true;
	/*
	INPUT inputs[1] = {};
	ZeroMemory(inputs, sizeof(inputs));
	switch (taskMove) {
	case TaskMove::Trigger:
		if (type == INPUT_MOUSE) {
			inputs[0].mi.dwFlags = Down | Up;
		}
		if (type == INPUT_KEYBOARD) {
			if (!ClickKey(CheckKey, TaskMove::DragStart)) { return false; }
			if (!ClickKey(CheckKey, TaskMove::DragEnd)) { return false; }
			return true;
		}
		break;
	case TaskMove::DragStart:
		if (type == INPUT_MOUSE) {
			inputs[0].mi.dwFlags = Down;
		}
		if (type == INPUT_KEYBOARD) {
			inputs[0].ki.wVk = VK;
			inputs[0].ki.dwFlags = Down;
		}
		break;
	case TaskMove::DragEnd:
		if (type == INPUT_MOUSE) {
			inputs[0].mi.dwFlags = Up;
		}
		if (type == INPUT_KEYBOARD) {
			inputs[0].ki.wVk = VK;
			inputs[0].ki.dwFlags = Up;
		}
		break;
	default:
		break;
	}
	inputs[0].type = type;
	return SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT)) == ARRAYSIZE(inputs);
	//*/
}
//
TaskTab::TaskTab() {
	w = FrameWork::Instance()->GetScreenWidth() - 16 * 2;
	h = 32;
	wAdd = 32;
	wSub = 32;
	OnMouse = false;
	IsPlayTask = false;
	m_IsEnd = false;
	IsActiveTab = false;
	
	m_TabColumn.clear();
	m_Dialog.Init();
	ResetEraseTask();
}
void TaskTab::Update(int xofs, int yofs, int index, bool CanMove) {
	auto& tabData = SaveData::Instance()->m_TabData.at(index);

	m_TabName = tabData.m_TabName;
	m_TaskType = tabData.m_TaskType;
	if (m_CheckFilePath != tabData.m_CheckFilePath) {
		m_CheckImage.ReleaseGraph();
		m_CheckImage.LoadGraph(tabData.m_CheckFilePath.c_str());
	}
	m_CheckFilePath = tabData.m_CheckFilePath;
	m_CheckKey = tabData.m_CheckKey;

	const auto& Font = FontPool::Instance()->Get("Bahnschrift Condensed", 24, -1, DX_FONTTYPE_ANTIALIASING_EDGE, DX_CHARSET_DEFAULT, 1)->GetHandle();
	OnMouseClose = IntoMouse(xofs + x + 1, yofs + y + 1, xofs + x + wAdd, yofs + y + h - 1) && (CanMove);
	if (OnMouseClose && InputControl::Instance()->GetLMEnter().IsTrigger()) {
		m_isEraseTask = true;
	}
	if (CanMove) {
		OnMouseMove = IntoMouse(xofs + x + w - wSub, yofs + y + 1, xofs + x + w - 1, yofs + y + h - 1);
		OnMouseMoveing = OnMouseMove && InputControl::Instance()->GetLMEnter().IsTrigger();
		if (OnMouseMoveing) {
			OnMouseMoveY = InputControl::Instance()->GetMouseY();
		}
	}
	else {
		IsActiveTab = false;
		if (OnMouseMoveing) {
			OnMoveY = y;
			int prevY = y;
			y = Mathf::Clamp(y + InputControl::Instance()->GetMouseY() - OnMouseMoveY, 0, FrameWork::Instance()->GetScreenHeight() - (h + 8) - (32 + 8));
			int Max = SaveData::Instance()->m_TabData.size();
			int Move = Mathf::Clamp(index + (y - prevY) / (h + 8), 0, Max - 1);
			OnMouseMoveIndex = Move - index;
			if (InputControl::Instance()->GetLMEnter().IsPress()) {
				OnMouseMove = true;
			}
			else {
				auto Tmp = SaveData::Instance()->m_TabData.at(index);

				for (int loop = index; loop < Max - 1; ++loop) {
					SaveData::Instance()->m_TabData.at(loop) = SaveData::Instance()->m_TabData.at(loop + 1);
				}

				for (int loop = Max - 1 - 1; loop >= Move; --loop) {
					SaveData::Instance()->m_TabData.at(loop + 1) = SaveData::Instance()->m_TabData.at(loop);
				}
				SaveData::Instance()->m_TabData.at(Move) = Tmp;
				OnMouseMoveing = false;
			}
		}
		else {
			OnMouseMove = false;
		}
	}

	OnMouse = IntoMouse(xofs + x + wAdd, yofs + y, xofs + x + w - wSub, yofs + y + h) && (CanMove);
	if (OnMouse && InputControl::Instance()->GetLMEnter().IsTrigger()) {
		IsActiveTab ^= 1;
	}
	m_MsgBar.Update();
	if ((IsActiveTab && !IsPlayTask) || (m_TabColumn.size() == 0)) {
		m_TabColumn.resize(7);
		int index = 0;
		int column = 0;
		{
			m_TabColumn.at(index).m_TabString = "Name:" + tabData.m_TabName;
			m_TabColumn.at(index).m_OnMouse = IntoMouse(xofs + x + wAdd, yofs + y + h + column, xofs + x + w - wSub, yofs + y + h + column + 24) && (CanMove);
			if (m_TabColumn.at(index).m_OnMouse) {
				m_MsgBar.SetStr(&tabData.m_TabName);
			}
		} column += 24; index++;
		{
			m_TabColumn.at(index).m_TabString = "Type:" + std::string(TaskTypeStr[static_cast<int>(tabData.m_TaskType)]);
			m_TabColumn.at(index).m_OnMouse = IntoMouse(xofs + x + wAdd, yofs + y + h + column, xofs + x + w - wSub, yofs + y + h + column + 24) && (CanMove);
			if (m_TabColumn.at(index).m_OnMouse && InputControl::Instance()->GetLMEnter().IsTrigger()) {
				tabData.m_TaskType = static_cast<TaskType>((static_cast<int>(tabData.m_TaskType) + 1) % static_cast<int>(TaskType::Max));
			}
		} column += 24; index++;
		{
			if (m_TaskType == TaskType::WaitTime) {
				char TimeStr[256] = "";
				sprintfDx(TimeStr, "%02d:%02d:%02d", tabData.GetHour(), tabData.GetMinute(), tabData.GetSecond());
				m_TabColumn.at(index).m_TabString = "Time:" + std::string(TimeStr);
				m_TabColumn.at(index).m_OnMouse = IntoMouse(xofs + x + wAdd, yofs + y + h + column, xofs + x + w - wSub, yofs + y + h + column + 24) && (CanMove);
				if (m_TabColumn.at(index).m_OnMouse && InputControl::Instance()->GetLMEnter().IsTrigger()) {
					int wtmp = GetDrawStringWidthToHandle("Time:", sizeof("Time:"), Font);
					{
						int Width = GetDrawStringWidthToHandle("Time:00", sizeof("Time:00"), Font);
						if (IntoMouse(xofs + x + wAdd + wtmp, yofs + y + h + column, xofs + x + wAdd + Width, yofs + y + h + column + 24) && (CanMove)) {
							tabData.SetTime(tabData.GetHour() + 1, tabData.GetMinute(), tabData.GetSecond());
						}
						wtmp = Width;
					}
					{
						int Width = GetDrawStringWidthToHandle("Time:00:00", sizeof("Time:00:00"), Font);
						if (IntoMouse(xofs + x + wAdd + wtmp, yofs + y + h + column, xofs + x + wAdd + Width, yofs + y + h + column + 24) && (CanMove)) {
							tabData.SetTime(tabData.GetHour(), tabData.GetMinute() + 5, tabData.GetSecond());
						}
						wtmp = Width;
					}
					{
						int Width = GetDrawStringWidthToHandle("Time:00:00:00", sizeof("Time:00:00:00"), Font);
						if (IntoMouse(xofs + x + wAdd + wtmp, yofs + y + h + column, xofs + x + wAdd + Width, yofs + y + h + column + 24) && (CanMove)) {
							tabData.SetTime(tabData.GetHour(), tabData.GetMinute(), tabData.GetSecond() + 5);
						}
						wtmp = Width;
					}
				}
				column += 24; index++;
			}
		} 
		{
			if (m_TaskType == TaskType::ClickPoint) {
				std::string FileName = tabData.m_CheckFilePath;
				if (tabData.m_CheckFilePath.rfind("\\") != std::string::npos) {
					FileName = tabData.m_CheckFilePath.substr(tabData.m_CheckFilePath.rfind("\\") + 1);
				}
				m_TabColumn.at(index).m_TabString = "Icon:[" + FileName + "]";
				m_TabColumn.at(index).m_OnMouse = IntoMouse(xofs + x + wAdd, yofs + y + h + column, xofs + x + w - wSub, yofs + y + h + column + 24) && (CanMove);
				if (m_TabColumn.at(index).m_OnMouse && InputControl::Instance()->GetLMEnter().IsTrigger()) {
					if (m_Dialog.Open()) {
						tabData.m_CheckFilePath = m_Dialog.GetPath();
					}
				}
				column += 24; index++;
			}
		}
		{
			if (m_TaskType != TaskType::WaitTime) {
				m_TabColumn.at(index).m_TabString = "Key:";
				m_TabColumn.at(index).m_OnMouse = IntoMouse(xofs + x + wAdd, yofs + y + h + column, xofs + x + w - wSub, yofs + y + h + column + 24) && (CanMove);
				if (m_TabColumn.at(index).m_OnMouse) {
					for (size_t i = 0; i < static_cast<size_t>(KeyNum); ++i) {
						int ID = KeyGuide::GetOffsettoID(static_cast<int>(i), ControlType::PC);
						if (KeyGuide::GetButtonPress(ControlType::PC, ID)) {
							tabData.m_CheckKey = KeyGuide::GetIDtoStr(ID, ControlType::PC);
							break;
						}
					}
				}
				column += 24; index++;
			}
		}
		{
			if (m_TaskType != TaskType::WaitTime) {
				m_TabColumn.at(index).m_TabString = "Move:" + std::string(TaskMoveStr[static_cast<int>(tabData.m_TaskMove)]);
				m_TabColumn.at(index).m_OnMouse = IntoMouse(xofs + x + wAdd, yofs + y + h + column, xofs + x + w - wSub, yofs + y + h + column + 24) && (CanMove);
				if (m_TabColumn.at(index).m_OnMouse && InputControl::Instance()->GetLMEnter().IsTrigger()) {
					tabData.m_TaskMove = static_cast<TaskMove>((static_cast<int>(tabData.m_TaskMove) + 1) % static_cast<int>(TaskMove::Max));
				}
				column += 24; index++;
			}
		} 
		{
			m_TabColumn.at(index).m_TabString = "Next:" + std::string(TaskNextStr[static_cast<int>(tabData.m_TaskNext)]);
			m_TabColumn.at(index).m_OnMouse = IntoMouse(xofs + x + wAdd, yofs + y + h + column, xofs + x + w - wSub, yofs + y + h + column + 24) && (CanMove);
			if (m_TabColumn.at(index).m_OnMouse && InputControl::Instance()->GetLMEnter().IsTrigger()) {
				tabData.m_TaskNext = static_cast<TaskNext>((static_cast<int>(tabData.m_TaskNext) + 1) % static_cast<int>(TaskNext::Max));
			}
		} column += 24; index++;
		m_TabColumn.resize(index);
	}
	if (IsPlayTask) {
		int Time = ((GetNowHiPerformanceCount() - m_StartTime) / 1000 / 1000);
		switch (tabData.m_TaskType) {
		case TaskType::ClickPoint:
			if (!m_MatchTask) {
				m_MatchTask = std::make_unique<MatchTask>(tabData.m_CheckFilePath.c_str());
			}
			if (1 <= Time) {
				if (m_MatchTask) {
					//ウィンドウ画面キャプチャ
					m_MatchTask->Update();
					if (m_MatchTask->IsMatch()) {
						while (!MoveToMousePoint(m_MatchTask->MatchPosX(), m_MatchTask->MatchPosY())) {
							Sleep(1);
						}
						while (!ClickKey(tabData.m_CheckKey, tabData.m_TaskMove)) {
							Sleep(1);
						}
						IsPlayTask = false;
						m_IsEnd = true;
					}
				}
			}
			break;
		case TaskType::KeyType:
			if (1 <= Time) {
				IsPlayTask = false;
				m_IsEnd = true;
				while (!ClickKey(tabData.m_CheckKey, tabData.m_TaskMove)) {
					Sleep(1);
				}
			}
			break;
		case TaskType::WaitTime:
			if (1 <= Time) {
				if (tabData.m_Time <= Time) {
					IsPlayTask = false;
					m_IsEnd = true;
				}
			}
			break;
		default:
			break;
		}
	}
	else {
		m_MatchTask.release();
	}
}
void TaskTab::Draw(bool CanMove) const {
	if (OnMouseMoveing && OnMouseMoveIndex != 0) {
		int tmpY = OnMoveY + ((OnMouseMoveIndex > 0) ? OnMouseMoveIndex + 1 : OnMouseMoveIndex) * (h + 8);
		DrawTriangle(x - 8, tmpY + 3, x - 8, tmpY - 3, x - 2, tmpY, ColorPalette::Black, TRUE);
	}

	int hAdd = IsActiveTab ? (24 * static_cast<int>(m_TabColumn.size()) + 2) : 0;
	const auto& Font = FontPool::Instance()->Get("Bahnschrift Condensed", 24, -1, DX_FONTTYPE_ANTIALIASING_EDGE, DX_CHARSET_DEFAULT, 1)->GetHandle();
	if (OnMouseMoveing) {
		int Add = 2;
		DrawBox(x + Add, y + Add, x + Add + w, y + Add + h, ColorPalette::Black, TRUE);
		DrawBox(x + Add + wAdd, y + Add, x + Add + w - wSub, y + Add + h + hAdd, ColorPalette::Black, TRUE);
	}
	DrawBox(x, y, x + w, y + h, ColorPalette::Black, TRUE);
	DrawBox(x + wAdd, y, x + w - wSub, y + h + hAdd, ColorPalette::Black, TRUE);
	if (hAdd > 0) {
		DrawBox(x + wAdd + 1, y + h, x + w - wSub - 1, y + h + hAdd - 1, ColorPalette::Gray010, TRUE);
	}
	{
		DrawBox(x + 1, y + 1, x + wAdd, y + h - 1, OnMouseClose ? ColorPalette::Red075 : ColorPalette::Red, TRUE);
		std::string Xname = "X";
		int Width = GetDrawStringWidthToHandle(Xname.c_str(), static_cast<int>(Xname.length()), Font);
		DrawFormatString2ToHandle(x + wAdd / 2 - Width / 2, y + h / 2 - 24 / 2, ColorPalette::White, ColorPalette::Gray085, Font, Xname.c_str());
	}
	//Tab
	{

		DrawBox(x + wAdd + 1, y + 1, x + w - wSub - 1, y + h - 1, OnMouse ? ColorPalette::Gray050 : ColorPalette::White, TRUE);
		if (!IsActiveTab) {
			int Width = GetDrawStringWidthToHandle(m_TabName.c_str(), static_cast<int>(m_TabName.length()), Font);
			DrawFormatString2ToHandle(x + wAdd + (w - wSub - wAdd) / 2 - Width / 2, y + h / 2 - 24 / 2, ColorPalette::White, ColorPalette::Gray085, Font, m_TabName.c_str());
		}
	}
	//Run
	{
		std::string Xname = "Ξ";
		DrawBox(x + w - wSub, y + 1, x + w - 1, y + h - 1, OnMouseMove ? ColorPalette::Gray050 : ColorPalette::White, TRUE);
		int Width = GetDrawStringWidthToHandle(Xname.c_str(), static_cast<int>(Xname.length()), Font);
		DrawFormatString2ToHandle(x + w - wSub + wSub / 2 - Width / 2, y + h / 2 - 24 / 2, ColorPalette::White, ColorPalette::Gray085, Font, Xname.c_str());
	}
	if (IsPlayTask)
	{
		DrawBox(x - 1, y + 0, x + w + 1, y + h - 0, ColorPalette::Green, FALSE);
		DrawBox(x - 2, y + 1, x + w + 2, y + h - 1, ColorPalette::Green, FALSE);
	}
	//
	if (IsActiveTab) {
		if (m_TaskType == TaskType::ClickPoint) {
			int X, Y;
			GetGraphSize(m_CheckImage.GetHandle(), &X, &Y);
			DrawExtendGraph(x + wAdd, y + h, x + wAdd + X, y + h + Y, m_CheckImage.GetHandle(), FALSE);
		}
		int column = 0;
		for (auto& s : m_TabColumn) {
			int index = static_cast<int>(&s - &m_TabColumn.front());
			DrawFormatString2ToHandle(x + wAdd, y + h + column, (!IsPlayTask && s.m_OnMouse) ? ColorPalette::Red : ColorPalette::White, ColorPalette::Gray085, Font, s.m_TabString.c_str());
			if (m_TaskType == TaskType::ClickPoint) {
				if (index == 3) {//TODO
					if (m_CheckKey != "") {
						KeyGuide::Instance()->DrawButton(x + wAdd + 64, y + h + column, KeyGuide::GetIDtoOffset(KeyGuide::GetStrtoID(m_CheckKey.c_str(), ControlType::PC), ControlType::PC));
					}
				}
			}
			if (m_TaskType == TaskType::KeyType) {
				if (index == 2) {//TODO
					if (m_CheckKey != "") {
						KeyGuide::Instance()->DrawButton(x + wAdd + 64, y + h + column, KeyGuide::GetIDtoOffset(KeyGuide::GetStrtoID(m_CheckKey.c_str(), ControlType::PC), ControlType::PC));
					}
				}
			}
			column += 24;
		}
	}

	if (!CanMove) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 64);
		DrawBox(x, y, x + w, y + h, ColorPalette::Black, TRUE);
		DrawBox(x + wAdd, y, x + w - wSub, y + h + hAdd, ColorPalette::Black, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
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
		FrameWork::Instance()->GetScreenWidth() - 8 - 2, 0 - (Height - H) * (m_Yoffset_R) / static_cast<int>(Mathf::Max(static_cast<float>(m_YMax), 1.f)),
		FrameWork::Instance()->GetScreenWidth() - 8 + 2, 0 - (Height - H) * (m_Yoffset_R) / static_cast<int>(Mathf::Max(static_cast<float>(m_YMax), 1.f)) + H,
		ColorPalette::White, TRUE);
}
TaskDraw::TaskDraw() {
	m_TaskTabs.clear();
	for (auto& a : SaveData::Instance()->m_TabData) {
		m_TaskTabs.emplace_back(std::make_unique<TaskTab>());
	}

	m_Yoffset = 0;
	m_Yoffset_R = 0;
	m_YListPos = 32+8;
	int Height = (FrameWork::Instance()->GetScreenHeight() - m_YListPos - 8);
	m_ListGraph.Create(FrameWork::Instance()->GetScreenWidth(), Height, true);

	OnMouse = false;
	IsTaskStart = false;
}
void TaskDraw::Update() {
	Mathf::Easing(&m_Yoffset_R, static_cast<float>(m_Yoffset), 0.5f);

	int yposEdge = 8;
	m_YMax = 0;
	int YMax = 0;

	bool IsMoving = false;
	for (auto& t : m_TaskTabs) {
		IsMoving |= t->IsMoveing();
	}
	for (auto& t : m_TaskTabs) {
		int index = static_cast<int>(&t - &m_TaskTabs.front());
		t->SetPos(0, m_Yoffset_R + m_YMax);
		YMax += t->GetHeight() + yposEdge;
		if (index != (m_TaskTabs.size() - 1)) {
			m_YMax = YMax;
		}
		t->Update(0, m_YListPos, index, !IsMoving);
		if (t->IsEnd()){
			t->EndTask();
			auto& tabData = SaveData::Instance()->m_TabData.at(index);
			switch (tabData.m_TaskNext) {
			case TaskNext::GoNextTask:
				if (index != (m_TaskTabs.size() - 1)) {
					m_TaskTabs.at(static_cast<size_t>(index + 1))->Start();
				}
				else {
					IsTaskStart = false;
				}
				break;
			default:
				break;
			}
		}
	}

	for (auto& t : m_TaskTabs) {
		int index = static_cast<int>(&t - &m_TaskTabs.front());
		if (t->IsEraseTask()) {
			t->ResetEraseTask();
			SaveData::Instance()->m_TabData.erase(SaveData::Instance()->m_TabData.begin() + index);
			m_TaskTabs.pop_back();
			break;
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
			if (!t->IsMoveing()) {
				t->Draw(!IsMoving);
			}
		}
		if (!IsTaskStart) {
			const auto& Font = FontPool::Instance()->Get("Bahnschrift Condensed", 24, -1, DX_FONTTYPE_ANTIALIASING_EDGE, DX_CHARSET_DEFAULT, 1)->GetHandle();
			int x, y, w, h;
			x = 16 + 0;
			y = m_Yoffset_R + YMax;
			w = FrameWork::Instance()->GetScreenWidth() - 16 * 2;
			h = 32;


			OnMouse = IntoMouse(x, m_YListPos + y, x + w, m_YListPos + y + h) && !IsTaskStart && (!IsMoving);
			if (OnMouse && InputControl::Instance()->GetLMEnter().IsTrigger()) {
				SaveData::Instance()->m_TabData.emplace_back();
				SaveData::Instance()->m_TabData.back().m_TabName = "New Task " + std::to_string(SaveData::Instance()->m_TabData.size());
				SaveData::Instance()->m_TabData.back().m_TaskType = TaskType::ClickPoint;
				SaveData::Instance()->m_TabData.back().m_CheckFilePath = "data\\Test.png";
				SaveData::Instance()->m_TabData.back().m_CheckKey = "LMOUSE";
				SaveData::Instance()->m_TabData.back().m_TaskMove = TaskMove::Trigger;
				SaveData::Instance()->m_TabData.back().m_TaskNext = TaskNext::GoNextTask;
				SaveData::Instance()->m_TabData.back().SetTime(10, 0, 0);
				//1個追加
				m_TaskTabs.emplace_back(std::make_unique<TaskTab>());

				m_Yoffset = static_cast<int>(Mathf::Clamp(
					static_cast<float>(m_Yoffset + -1 * (32 + 8)),
					static_cast<float>(-m_YMax),
					static_cast<float>(0)
				));
			}

			{
				DrawBox(x, y, x + w, y + h, ColorPalette::Black, TRUE);
				DrawBox(x + 1, y + 1, x + w - 1, y + h - 1, OnMouse ? ColorPalette::Gray050 : ColorPalette::White, TRUE);
				std::string AddBackText = "Add Task...";
				int Width = GetDrawStringWidthToHandle(AddBackText.c_str(), static_cast<int>(AddBackText.length()), Font);
				DrawFormatString2ToHandle(x + w / 2 - Width / 2, y + h / 2 - 24 / 2, ColorPalette::White, ColorPalette::Gray085, Font, AddBackText.c_str());
				if (!!IsMoving) {
					SetDrawBlendMode(DX_BLENDMODE_ALPHA, 64);
					DrawBox(x, y, x + w, y + h, ColorPalette::Black, TRUE);
					SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
				}
			}
		}
		for (auto& t : m_TaskTabs) {
			if (t->IsMoveing()) {
				t->Draw(true);
			}
		}
	}

	OnMouse = IntoMouse(16, 4, FrameWork::Instance()->GetScreenWidth() - 16, m_YListPos - 4) && !IsTaskStart && (SaveData::Instance()->m_TabData.size() > 0) && (!IsMoving);
	if (OnMouse && InputControl::Instance()->GetLMEnter().IsTrigger()) {
		IsTaskStart = true;
		m_TaskTabs.at(0)->Start();
	}
}
void TaskDraw::Draw() const {
	bool IsMoving = false;
	for (auto& t : m_TaskTabs) {
		IsMoving |= t->IsMoveing();
	}

	DrawBox(16, 4, FrameWork::Instance()->GetScreenWidth() - 16, m_YListPos - 4, ColorPalette::Black, TRUE);
	DrawBox(16+1, 4+1, FrameWork::Instance()->GetScreenWidth() - 16-1, m_YListPos - 4-1, OnMouse ? ColorPalette::Gray050 : ColorPalette::White, TRUE);
	const auto& Font = FontPool::Instance()->Get("Bahnschrift Condensed", 24, -1, DX_FONTTYPE_ANTIALIASING_EDGE, DX_CHARSET_DEFAULT, 1)->GetHandle();
	std::string StartButton = IsTaskStart ? "Run" : "Start Task";
	int Width = GetDrawStringWidthToHandle(StartButton.c_str(), static_cast<int>(StartButton.length()), Font);
	DrawFormatString2ToHandle(FrameWork::Instance()->GetScreenWidth() / 2 - Width / 2, 4 + (m_YListPos - 4 - 4) / 2 - 24 / 2, ColorPalette::White, ColorPalette::Gray085, Font, StartButton.c_str());
	if (!!IsMoving) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 64);
		DrawBox(16, 4, FrameWork::Instance()->GetScreenWidth() - 16, m_YListPos - 4, ColorPalette::Black, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}


	DrawGraph(0, m_YListPos, m_ListGraph.GetHandle(), TRUE);
}
