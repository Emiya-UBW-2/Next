#include "Define.hpp"

#include "MainGame.hpp"



int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	FrameWork::Create();

	MainGame m_MainGame;

	int BackScreen = 0;

	BackScreen = MakeScreen(FrameWork::Instance()->GetScreenWidth(), FrameWork::Instance()->GetScreenHeight(), FALSE);

	SetTransColor(0, 255, 0);

	m_MainGame.Init();
	//メインループ開始
	while (true) {
		//
		m_MainGame.Update();
		//
		SetDrawScreen(BackScreen);
		ClearDrawScreen();
		{
			m_MainGame.Draw();
		}
		//描画
		SetDrawScreen(DX_SCREEN_BACK);
		ClearDrawScreen();
		{
			int Width = FrameWork::Instance()->GetWindowHeight() * FrameWork::Instance()->GetScreenWidth() / FrameWork::Instance()->GetScreenHeight();
			DrawExtendGraph(FrameWork::Instance()->GetWindowWidth() / 2 - Width / 2, 0, FrameWork::Instance()->GetWindowWidth() / 2 + Width / 2, FrameWork::Instance()->GetWindowHeight(), BackScreen, FALSE);
			//デバッグ表示
			clsDx();
			printfDx("FPS:%4.1f\n", GetFPS());
			//printfDx("デルタタイム:%5.2fs\n", FrameWork::Instance()->GetDeltaTime());
		}
		if (!FrameWork::Instance()->Flip()) {
			break;
		}
	}
	FrameWork::Release();
	return 0;// ソフトの終了 
}
