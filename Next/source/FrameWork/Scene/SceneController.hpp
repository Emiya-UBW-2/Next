#pragma once

typedef int SceneID;

enum class EnumSceneID {
	Title,
	Main,
};


class BaseScene {
	bool m_IsEnd = false;
	SceneID m_NextSceneID = -1;
public:
	BaseScene() {}
	~BaseScene() {}
protected:
	virtual void InitSub() = 0;
	virtual void UpdateSub() = 0;
	virtual void DrawSub() = 0;
	virtual void DisposeSub() = 0;
protected:
	void SetSceneEnd() { m_IsEnd = true; }
	void SetNextSceneID(SceneID value) { m_NextSceneID = value; }
public:
	void Init() {
		m_IsEnd = false;
		InitSub();
	}
	void Update() {
		UpdateSub();
	}
	void Draw() {
		DrawSub();
	}
	void Dispose() {
		DisposeSub();
	}
	bool IsEnd()const { return m_IsEnd; }
	SceneID GetNextSceneID() const { return m_NextSceneID; }
};


class SceneController {
	BaseScene* m_pScene = nullptr;
private:
	void Active(SceneID ID);
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