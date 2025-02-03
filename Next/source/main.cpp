#include "Define.hpp"

#include "MainGame.hpp"

#include<WinUser.h>

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	FrameWork::Create();

	TitleScene* m_TitleScene = nullptr;
	MainGame *m_MainGame = nullptr;

	int BackScreen = 0;

	BackScreen = MakeScreen(FrameWork::Instance()->GetScreenWidth(), FrameWork::Instance()->GetScreenHeight(), FALSE);

	SetTransColor(0, 255, 0);

	HCURSOR Cursor = LoadCursorFromFile("data/Cursor.cur");
	SetClassLongA(GetMainWindowHandle(), -12, (LONG)Cursor);

	//���C�����[�v�J�n
	MenuEnter = 0;

	//�J�n
	if (!m_TitleScene) {
		m_TitleScene = new TitleScene();
		m_TitleScene->Init();
	}
	while (true) {
		SetCursor(Cursor);
		MenuEnter = static_cast<int>(Mathf::Clamp(static_cast<float>(MenuEnter + (CheckHitKey(KEY_INPUT_SPACE) ? 1 : -2)), 0.f, 2.f));
		//
		if (m_TitleScene) {
			m_TitleScene->Update();
		}
		if (m_MainGame) {
			m_MainGame->Update();
		}
		//
		SetDrawScreen(BackScreen);
		ClearDrawScreen();
		{
			if (m_TitleScene) {
				m_TitleScene->Draw();
			}
			if (m_MainGame) {
				m_MainGame->Draw();
			}
		}
		//�`��
		SetDrawScreen(DX_SCREEN_BACK);
		ClearDrawScreen();
		{
			DrawBox(0, 0, FrameWork::Instance()->GetWindowWidth(), FrameWork::Instance()->GetWindowHeight(), GetColor(32, 32, 32), TRUE);

			int Width = FrameWork::Instance()->GetWindowHeight() * FrameWork::Instance()->GetScreenWidth() / FrameWork::Instance()->GetScreenHeight();
			DrawExtendGraph(FrameWork::Instance()->GetWindowWidth() / 2 - Width / 2, 0, FrameWork::Instance()->GetWindowWidth() / 2 + Width / 2, FrameWork::Instance()->GetWindowHeight(), BackScreen, FALSE);
			//�f�o�b�O�\��
			//clsDx();
			//printfDx("FPS:%4.1f\n", GetFPS());
			//printfDx("%d\n", MenuEnter);
			//printfDx("�f���^�^�C��:%5.2fs\n", FrameWork::Instance()->GetDeltaTime());
		}
		if (!FrameWork::Instance()->Flip()) {
			break;
		}
		//���V�[���ւ̑J��
		if (m_MainGame && m_MainGame->IsEnd()) {
			m_MainGame->Dispose();
			delete m_MainGame;
			m_MainGame = nullptr;

			//�J�n
			if (!m_TitleScene) {
				m_TitleScene = new TitleScene();
				m_TitleScene->Init();
			}
		}
		if (m_TitleScene && m_TitleScene->IsEnd()) {
			m_TitleScene->Dispose();
			delete m_TitleScene;
			m_TitleScene = nullptr;

			//�J�n
			if (!m_MainGame) {
				m_MainGame = new MainGame();
				m_MainGame->Init();
			}
		}
	}

	FrameWork::Release();
	return 0;// �\�t�g�̏I�� 
}
