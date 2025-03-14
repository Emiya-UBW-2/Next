#pragma once

#include "../../FrameWork/Define.hpp"

class MainCamera : public SingletonBase<MainCamera, "MainCamera"> {
private:
	friend class SingletonBase<MainCamera, "MainCamera">;
private:
	MainCamera() {}
	virtual ~MainCamera() {}
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
		float Rate = static_cast<float>(FrameWork::Instance()->GetScreenWidth()) / 960.f;
		Ret.x = -200.f * Rate + std::cos(Rad1) * (1200.f * X * Rate) + std::cos(Rad2) * (Y * Xrate * 1000.f * Rate);
		Ret.y = -200.f * Rate + std::sin(Rad1) * (1200.f * X * Rate) + std::sin(Rad2) * (Y * Xrate * 1000.f * Rate) - Z * 10.f * Rate;
		Ret.z = 0.f;

		return Ret;
	}
	Mathf::Vector3 GetDisplayPoint(const Mathf::Vector3& Pos) const { return GetDisplayPoint(Pos.x, Pos.y, Pos.z); }
public:
	const Mathf::Vector3& GetCamPos() const { return CamPos; }
	void SetCamPos(const Mathf::Vector3& Pos) { CamPos = Pos; }
};
