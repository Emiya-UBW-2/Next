#pragma once

#include "../Define.hpp"
#include <memory>

typedef int SceneID;

class BaseScene {
	bool m_IsEnd = false;
public:
	BaseScene() {}
	~BaseScene() {}
protected:
	virtual void InitSub() = 0;
	virtual void UpdateSub() = 0;
	virtual void DrawSub() const = 0;
	virtual void DisposeSub() = 0;
protected:
	void SetSceneEnd() { m_IsEnd = true; }
public:
	void Init() {
		m_IsEnd = false;
		InitSub();
	}
	void Update() {
		UpdateSub();
	}
	void Draw() const {
		DrawSub();
	}
	void Dispose() {
		DisposeSub();
	}
	bool IsEnd()const { return m_IsEnd; }
	virtual std::unique_ptr<BaseScene> MakeNextScene() = 0;
};

class SceneController : public SingletonBase<SceneController, "SceneController"> {
private:
	friend class SingletonBase<SceneController, "SceneController">;
private:
	SceneController() {}
	virtual ~SceneController() {
		Dispose();
	}
private:
	std::unique_ptr<BaseScene> m_pScene = nullptr;
private:
	void DisActive() {
		if (m_pScene) {
			m_pScene->Dispose();
			m_pScene.release();
			m_pScene = nullptr;
		}
	}
public:
	void Active(std::unique_ptr<BaseScene>& pScene) {
		if (!m_pScene) {
			m_pScene = std::move(pScene);
			m_pScene->Init();
		}
	}
	void Update() {
		if (m_pScene) {
			m_pScene->Update();
			if (m_pScene->IsEnd()) {
				std::unique_ptr<BaseScene> NextSceneID = m_pScene->MakeNextScene();
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