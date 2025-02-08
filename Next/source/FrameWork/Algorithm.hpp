#pragma once

#include "DxLib.h"

#include <cmath>

namespace Mathf {
	class Vector3 {
	public:
		float x;
		float y;
		float z;
	public:
		Vector3(float x, float y, float z) {
			this->x = x;
			this->y = y;
			this->z = z;
		}
		Vector3() {
			Vector3(0, 0, 0);
		}

		Vector3 operator+(const Vector3& target) const {
			Vector3 Ret = *this;
			Ret.x += target.x;
			Ret.y += target.y;
			Ret.z += target.z;
			return Ret;
		}
		void operator+=(const Vector3& target) {
			this->x += target.x;
			this->y += target.y;
			this->z += target.z;
		}
		Vector3 operator*(float target) const {
			Vector3 Ret = *this;
			Ret.x *= target;
			Ret.y *= target;
			Ret.z *= target;
			return Ret;
		}
		void operator*=(float target) {
			this->x *= target;
			this->y *= target;
			this->z *= target;
		}

		Vector3 operator-(const Vector3& target) const {
			Vector3 Ret = *this;
			Ret.x -= target.x;
			Ret.y -= target.y;
			Ret.z -= target.z;
			return Ret;
		}
		void operator-=(const Vector3& target) {
			this->x -= target.x;
			this->y -= target.y;
			this->z -= target.z;
		}
		Vector3 operator/(float target) const {
			Vector3 Ret = *this;
			Ret.x /= target;
			Ret.y /= target;
			Ret.z /= target;
			return Ret;
		}
		void operator/=(float target) {
			this->x /= target;
			this->y /= target;
			this->z /= target;
		}

		Vector3 Nomalize() const {
			return (*this) / this->Length();
		}
		
		float Length() const {
			return std::hypotf(std::hypotf(this->x, this->y), this->z);
		}
		inline static Vector3		Cross(const Vector3& A, const Vector3& B) noexcept { return Vector3(A.y * B.z - A.z * B.y, A.z * B.x - A.x * B.z, A.x * B.y - A.y * B.x); }
		inline static float			Dot(const Vector3& A, const Vector3& B) noexcept { return A.x * B.x + A.y * B.y + A.z * B.z; }
	};
	//角度を度からラジアンに変換
	static float Deg2Rad(float value) { return value * DX_PI_F / 180.f; }
	//大きい方、小さい方に制限する
	extern float Min(float value, float min);
	extern float Max(float value, float max);
	//範囲内に収める
	extern float Clamp(float value, float min, float max);
	//線形補完
	extern float Lerp(float valueA, float valueB, float per);
	//イージング
	extern float GetEasingRatio(float ratio) noexcept;
	static void Easing(float* A, const float& B, float ratio) noexcept { *A = Lerp(*A, B, GetEasingRatio(ratio)); }
	static void Easing(Vector3* A, const Vector3& B, float ratio) noexcept {
		A->x = Lerp(A->x, B.x, GetEasingRatio(ratio));
		A->y = Lerp(A->y, B.y, GetEasingRatio(ratio));
		A->z = Lerp(A->z, B.z, GetEasingRatio(ratio));
	}
	// 線分同士の交差判定
	static bool GetSegmenttoSegment(const Vector3& SegmentAPos1, const Vector3& SegmentAPos2, const Vector3& SegmentBPos1, const Vector3& SegmentBPos2, SEGMENT_SEGMENT_RESULT* Result) noexcept {
		VECTOR Pos1t = VGet(SegmentAPos1.x, SegmentAPos1.y, SegmentAPos1.z);
		VECTOR Pos2t = VGet(SegmentAPos2.x, SegmentAPos2.y, SegmentAPos2.z);
		VECTOR PosAt = VGet(SegmentBPos1.x, SegmentBPos1.y, SegmentBPos1.z);
		VECTOR PosBt = VGet(SegmentBPos2.x, SegmentBPos2.y, SegmentBPos2.z);
		Segment_Segment_Analyse(&Pos1t, &Pos2t, &PosAt, &PosBt, Result);
		float len = 0.001f;
		return (Result->SegA_SegB_MinDist_Square <= (len * len));
	}
}
