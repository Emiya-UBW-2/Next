#include "FrameWork/Define.hpp"

#include "Scene/SceneController.hpp"

#include<WinUser.h>

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	FrameWork::Create();
	InputControl::Create();
	{
		SceneController _SceneController;
		SetTransColor(0, 255, 0);

		HCURSOR Cursor = LoadCursorFromFile("data/Cursor.cur");
		SetClassLongA(GetMainWindowHandle(), -12, (LONG)Cursor);

		//メインループ開始
		IsTriMonoMode = false;

		_SceneController.Init();
		while (true) {
			SetCursor(Cursor);
			InputControl::Instance()->Update();
			//
			_SceneController.Update();
			//描画
			SetDrawScreen(FrameWork::Instance()->GetBackScreen());
			ClearDrawScreen();
			{
				_SceneController.Draw();
			}
			if (!FrameWork::Instance()->Flip()) {
				break;
			}
		}
		_SceneController.Dispose();
	}
	InputControl::Release();
	FrameWork::Release();
	return 0;// ソフトの終了 
}
