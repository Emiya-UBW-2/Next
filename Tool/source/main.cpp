#include "FrameWork/Define.hpp"
//#include<WinUser.h>

#include "Scene/TitleScene.hpp"

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	SetTransColor(0, 255, 0);
	FrameWork::Create();
	//HCURSOR Cursor = LoadCursorFromFile("data/Cursor.cur");
	//SetClassLongA(GetMainWindowHandle(), -12, (LONG)Cursor);
	//メインループ開始
	FontPool::Create();

	FontPool::Instance()->Add("Agency FB", 24, -1, DX_FONTTYPE_ANTIALIASING_EDGE, DX_CHARSET_DEFAULT, 1);
	//初期シーンを設定
	std::unique_ptr<BaseScene> First = std::make_unique<TitleScene>();
	SceneController::Instance()->Active(First);

	while (true) {
		//SetCursor(Cursor);
		if (!FrameWork::Instance()->Update()) {
			break;
		}
	}

	FontPool::Release();
	FrameWork::Release();
	return 0;// ソフトの終了 
}
