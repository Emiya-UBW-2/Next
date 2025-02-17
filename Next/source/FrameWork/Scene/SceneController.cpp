#include "SceneController.hpp"

#include "../../Scene/TitleScene.hpp"
#include "../../Scene/MainScene.hpp"

void SceneController::Active(BaseScene* pScene) {
	if (!m_pScene) {
		m_pScene = pScene;
		m_pScene->Init();
	}
}
void SceneController::DisActive() {
	if (m_pScene) {
		m_pScene->Dispose();
		delete m_pScene;
		m_pScene = nullptr;
	}
}

void SceneController::Init() {
	Active(new TitleScene());
}

void SceneController::Update() {
	if (m_pScene) {
		m_pScene->Update();
		if (m_pScene->IsEnd()) {
			SceneID NextSceneID = m_pScene->GetNextSceneID();
			DisActive();
			switch (static_cast<EnumSceneID>(NextSceneID)) {
			case EnumSceneID::Title:
				Active(new TitleScene());
				break;
			case EnumSceneID::Main:
				Active(new MainGame());
				break;
			default:
				break;
			}
		}
	}
}