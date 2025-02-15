#include "FrameWork/Define.hpp"

#include<WinUser.h>

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	SetTransColor(0, 255, 0);
	FrameWork::Create();

	HCURSOR Cursor = LoadCursorFromFile("data/Cursor.cur");
	SetClassLongA(GetMainWindowHandle(), -12, (LONG)Cursor);

	//メインループ開始
	IsTriMonoMode = false;

	while (true) {
		SetCursor(Cursor);
		if (!FrameWork::Instance()->Update()) {
			break;
		}
	}
	FrameWork::Release();
	return 0;// ソフトの終了 
}
