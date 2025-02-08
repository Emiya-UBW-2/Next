#pragma once

#include "../FrameWork/Define.hpp"

#include "BaseScene.hpp"
#include "TitleScene.hpp"
#include "MainScene.hpp"

enum class EnumSceneID {
	Title,
	Main,
};

class SceneController {
	BaseScene* m_pScene = nullptr;
private:
	void Active(SceneID ID) {
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
	void DisActive() {
		if (m_pScene) {
			m_pScene->Dispose();
			delete m_pScene;
			m_pScene = nullptr;
		}
	}
public:
	void Init() {
		Active(static_cast<SceneID>(EnumSceneID::Title));
	}
	void Update() {
		if (m_pScene) {
			m_pScene->Update();
			if (m_pScene->IsEnd()) {
				SceneID NextSceneID = m_pScene->GetNextSceneID();
				DisActive();
				Active(NextSceneID);
			}
		}
	}
	void Draw() {
		if (m_pScene) {
			m_pScene->Draw();
		}
	}
	void Dispose() {
		DisActive();
	}
};