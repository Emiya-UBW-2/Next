#include "Algorithm.hpp"

#include "FrameWork.hpp"

float Mathf::GetEasingRatio(float ratio) noexcept {
	return (1.f - std::powf(ratio, 60.f * FrameWork::Instance()->GetDeltaTime()));
}
