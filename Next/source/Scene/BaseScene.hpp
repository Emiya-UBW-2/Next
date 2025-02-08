#pragma once

#include "../FrameWork/Define.hpp"

typedef int SceneID;

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

