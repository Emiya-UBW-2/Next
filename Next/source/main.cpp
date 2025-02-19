#include "FrameWork/Define.hpp"

#include<WinUser.h>

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	SetTransColor(0, 255, 0);
	FrameWork::Create();
	//HCURSOR Cursor = LoadCursorFromFile("data/Cursor.cur");
	//SetClassLongA(GetMainWindowHandle(), -12, (LONG)Cursor);
	//メインループ開始

	FontPool::Create();

	FontPool::Instance()->Add("Agency FB", 12, -1, DX_FONTTYPE_ANTIALIASING_EDGE, DX_CHARSET_DEFAULT, 1);
	FontPool::Instance()->Add("Agency FB", 24, -1, DX_FONTTYPE_ANTIALIASING_EDGE, DX_CHARSET_DEFAULT, 1);
	FontPool::Instance()->Add("BIZ UDゴシック", 24, -1, DX_FONTTYPE_ANTIALIASING_EDGE, DX_CHARSET_DEFAULT, 1);

	SoundPool::Create();

	while (true) {
		//SetCursor(Cursor);
		if (!FrameWork::Instance()->Update()) {
			break;
		}
	}

	FontPool::Release();
	SoundPool::Release();

	FrameWork::Release();
	return 0;// ソフトの終了 
}
