#define NOMINMAX

#include "Algorithm.hpp"

#include "Define.hpp"

#include <algorithm>

float Mathf::Min(float value, float min) { return std::min(value, min); }

float Mathf::Max(float value, float max) { return std::max(value, max); }

float Mathf::Clamp(float value, float min, float max) { return std::clamp(value, min, max); }

float Mathf::Lerp(float valueA, float valueB, float per) { return std::lerp(valueA, valueB, per); }

float Mathf::GetEasingRatio(float ratio) noexcept {
	return (1.f - std::powf(ratio, 60.f * FrameWork::Instance()->GetDeltaTime()));
}
