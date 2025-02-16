#pragma once

#include "../../FrameWork/Define.hpp"

class MainCamera {
private:
	static const MainCamera* m_Singleton;
public:
	static void Create(void) noexcept {
		m_Singleton = new MainCamera();
	}
	static void Release(void) noexcept {
		delete m_Singleton;
	}
	static MainCamera* Instance(void) noexcept {
		if (m_Singleton == nullptr) {
			MessageBox(NULL, "Failed MainCamera Instance Create", "", MB_OK);
			exit(-1);
		}
		// if (m_Singleton == nullptr) { m_Singleton = new MainCamera(); }
		return (MainCamera*)m_Singleton;
	}
private:
	MainCamera() {
	}
	~MainCamera() {
	}
private:
	MainCamera(const MainCamera&) = delete;
	MainCamera& operator=(const MainCamera&) = delete;
	MainCamera(MainCamera&&) = delete;
	MainCamera& operator=(MainCamera&&) = delete;
private:
	Mathf::Vector3 CamPos;
public:
	Mathf::Vector3 GetDisplayPoint(float X, float Y, float Z) const {
		X = X - CamPos.x;
		Y = Y - CamPos.y;
		Z = Z - CamPos.z;
		float Rad1 = Mathf::Deg2Rad(-30.f + 90.f);
		float Rad2 = Mathf::Deg2Rad(-30.f);

		Mathf::Vector3 Ret;

		float Xrate = Mathf::Lerp(0.25f, 1.f, X);

		Ret.x = -200.f + std::cos(Rad1) * (1200.f * X) + std::cos(Rad2) * (Y * Xrate * 1000.f);
		Ret.y = -200.f + std::sin(Rad1) * (1200.f * X) + std::sin(Rad2) * (Y * Xrate * 1000.f) - Z * 10.f;
		Ret.z = 0.f;

		return Ret;
	}
	Mathf::Vector3 GetDisplayPoint(const Mathf::Vector3& Pos) const { return GetDisplayPoint(Pos.x, Pos.y, Pos.z); }
public:
	const Mathf::Vector3& GetCamPos() const { return CamPos; }
	void SetCamPos(const Mathf::Vector3& Pos) { CamPos = Pos; }
};
