#include "SceneController.hpp"

#include "../../Scene/TitleScene.hpp"
#include "../../Scene/MainScene.hpp"

void SceneController::Active(SceneID ID) {
	if (!m_pScene) {
		switch (static_cast<EnumSceneID>(ID))
		{
		case EnumSceneID::Title:
			m_pScene = new TitleScene();
			break;
		case EnumSceneID::Main:
			m_pScene = new MainGame();
			break;
		default:
			break;
		}
		m_pScene->Init();
	}
}